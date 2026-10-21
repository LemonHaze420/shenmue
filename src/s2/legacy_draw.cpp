#include "s2/legacy_draw.hpp"

#include "s2/frame_pipeline.hpp"
#include "s2/game_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "d3t/threads.hpp"

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/hiki.hpp"
#include "s2/hlib.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/lighting.hpp"
#include "s2/screen_film.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/texture_source.hpp"
#include "s2/pak.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <immintrin.h>
#endif

    namespace shenmue::s2 {


std::int32_t g_legacyScissorEnabled = 0;

namespace {
void RetireLegacyFlagDebugCompatibilityResources();

constexpr std::uint32_t xb01Signature = 0x31304258U;




alignas(16) std::array<std::byte, 0xf50> shaderParameterStorage{};
auto &shaderParameters = *reinterpret_cast<LegacyShaderParameters *>(shaderParameterStorage.data());


std::array<std::uint32_t, 172> initialEngineSettings{};
std::array<std::uint32_t, 172> currentEngineSettings{};
static_assert(sizeof(initialEngineSettings) == 0x2b0);
static_assert(sizeof(currentEngineSettings) == 0x2b0);
std::uint64_t engineFlags = 0x0000000007fff817ULL;
std::uint32_t stencilReference = 0;
std::uint32_t renderingMode = 9;
std::uint64_t blendFlags = 0x00000000f0000290ULL;
std::array<std::int32_t, 2> stripPacketBlendModes{};
std::array<std::array<std::uint32_t, 4>, 4> pixelSamplerKeys{{
    {{0x999bc2aaU, 0x0002fd99U, 0, 0}},
    {{0x0003c02aU, 0, 0, 0}},
    {{0x0003c02aU, 0, 0, 0}},
    {{0x0003c02aU, 0, 0, 0}},
}};
std::uint32_t savedAlphaOperation = std::bit_cast<std::uint32_t>(-0.9f);
std::array<std::uint32_t, 20> legacyFvfLayoutAliases{};
std::array<std::uint32_t, 10> currentLegacyVertexShaderMap{};
std::int32_t currentVertexColourLayoutType = 1;
std::uint32_t currentVertexColourShader = 0;
std::array<LegacyVertexColourStateRecord, 16> legacyVertexColourStates{};
std::array<LightRuntimeEntry, 16> selectedLights{};
Vector3 globalLightColour{};
std::uint32_t packedGlobalLightColour{};
Vector4 legacyModelColour = LEGACY_MODEL_COLOUR_DEFAULT;
Vector3 particleLightColour{1.0f, 1.0f, 1.0f};
bool legacyVertexColourStateActive = false;





float temporaryModelLight = 0.30000001f;
Vector3 modelLightVector{temporaryModelLight, temporaryModelLight, temporaryModelLight};
} // namespace
namespace legacy_draw_detail {
float temporaryModelScale = 1.0f;
} // namespace legacy_draw_detail
namespace {
using legacy_draw_detail::temporaryModelScale;

Vector3 modelReferenceDirection{1.0f, 0.0f, 0.0f};
Vector3 temporaryModelPosition{};


Vector4 lambertFvfReference{};
float legacyAlphaMultiplier = 1.0f;
bool legacyAlphaPassEnabled = false;
std::uint32_t perspectiveProjection = 1;
std::uint32_t legacySceneFogEnabled = 1;
std::uint32_t legacyModelFogEnabled = 1;
std::uint32_t legacyFogAlphaModes{};
std::int32_t legacyQueuedDepthFromGeometry = 1;
float legacyQueuedFallbackDepth{};
float legacyDepthFadeNear = 1.0f;
float legacyDepthFadeFar = 10000.0f;
float legacyDepthFadeIntensity = 1.0f;
std::array<float, 2> legacyTextureUVWidths{std::bit_cast<float>(0xffffffffU), std::bit_cast<float>(0xffffffffU)};
Vector3 legacyWeatherSpawnDrift{std::bit_cast<float>(0xffffffffU), std::bit_cast<float>(0xffffffffU), std::bit_cast<float>(0xffffffffU)};
std::uint32_t legacyWeatherRenderPass = 0xffffffffU;
std::array<std::uint32_t, 2> legacyWeatherShaderTypes{0xffffffffU, 0xffffffffU};
std::uint32_t legacyWeatherVertexStride = 0xffffffffU;
std::int32_t legacyWeatherQuadCount = -1;
std::uint32_t legacyWeatherLightingMode = 0xffffffffU;

// InitShenmueRendering's saved settings are not a snapshot of the current
// XB01 state. AssignEmulatedShadowRenderFlags restores these values even if
// a preceding material changed the packed engine/blend/rasterizer owners.
struct SavedProjectedShadowRenderSettings {
    std::uint32_t depthFunction{513};
    std::uint32_t blendEnabled{};
    std::uint32_t colourWriteMask{15};
    std::uint32_t depthWriteEnabled{1};
    std::uint32_t stencilEnabled{};
    std::uint32_t stencilPass{7680};
    std::uint32_t stencilDepthFail{7680};
    std::uint32_t cullMode{2304};
    std::uint32_t stencilFunction{519};
    std::uint32_t stencilReadMask{0xffffffffU};
    std::uint32_t stencilReference{};
};
SavedProjectedShadowRenderSettings savedProjectedShadowRenderSettings{};
Vector3 projectedShadowDirection{};
Vector4 projectedShadowModelDirection{};
std::uint32_t projectedShadowStencilPartition = 0;
bool projectedShadowDebugEnabled = false;
bool characterShadowFrameActive{};
bool characterShadowSavedBusy{};

struct LegacyPacketAlphaState {
    std::uint32_t hasAlpha{};
    std::uint32_t alphaOp8{};
    std::uint32_t alphaOp9{};
    std::uint32_t reserved{};
};


struct LegacyRenderPacketState {
    Matrix4x4 worldMatrix;
    std::array<LegacyVertexColourStateRecord, 16> savedVertexColourState;
    float alphaScale;
    Vector3 modelLight;
    std::uint32_t modelLightPad;
    Vector3 modelRef;
    std::uint32_t modelRefPad;
    Vector3 lightDir;
    std::int32_t fvfLayoutType;
    Vector3 lambertRefboard;
    std::uint32_t lambertFvfLayout[3];
    Vector3 depthFade;
    std::uint32_t reservedC18;
    std::uint32_t flags;
    Matrix4x4 alphaPassMatrix;
    LegacyPacketAlphaState alphaState;
    float modelScale;
    std::uint32_t reservedC74;
};

static_assert(sizeof(LegacyRenderPacketState) == 0xc78);
static_assert(offsetof(LegacyRenderPacketState, alphaScale) == 0xbc0);
static_assert(offsetof(LegacyRenderPacketState, depthFade) == 0xc0c);
static_assert(offsetof(LegacyRenderPacketState, flags) == 0xc1c);
static_assert(offsetof(LegacyRenderPacketState, alphaPassMatrix) == 0xc20);
static_assert(offsetof(LegacyRenderPacketState, modelScale) == 0xc70);




struct LegacyQueuedDraw {
    const LegacyRenderPacketState *packet;
    std::uint32_t type;
    std::uint32_t xb01Offset;
    union {
        const std::byte *command;
        LegacyUpdateCallback callback;
    };
    union {
        const LegacyDrawResources *resources;
        void *callbackData;
        std::uint64_t savedMaterialState0;
    };
    union {
        std::span<const std::byte> modelBytes;
        struct {
            std::uint64_t savedMaterialState1;
            std::uint32_t savedMaterialCount;
            std::uint32_t padding;
        } materialReplay;
    };
};

static_assert(sizeof(LegacyQueuedDraw) == 0x30);
static_assert(offsetof(LegacyQueuedDraw, type) == 0x08);
static_assert(offsetof(LegacyQueuedDraw, command) == 0x10);
static_assert(offsetof(LegacyQueuedDraw, callbackData) == 0x18);

struct LegacyQueuedShaderEntry {
    const LegacyRenderPacketState *packet;
    const std::byte *model;
};
static_assert(sizeof(LegacyQueuedShaderEntry) == 0x10);
static_assert(offsetof(LegacyQueuedShaderEntry, model) == 8);

LegacyQueuedShaderEntry *legacyQueuedShaderEntries{};
float *legacyQueuedShaderSortKeys{};
std::uint32_t legacyQueuedShaderCount{};
std::uint32_t legacyQueuedDrawCommandCount{};
std::uint32_t legacyQueuedCallbackCount{};
std::uint32_t legacyModelQueryIndex{};
std::uint8_t legacyModelQueryBatchReady{};
std::uint32_t unboundLegacyShaderModels{};
std::uint32_t unboundLegacyQueryModels{};

LegacyRenderPacketState *legacyRenderPackets = nullptr;
LegacyQueuedDraw *legacyQueuedDraws = nullptr;
float *legacyQueuedDrawSortKeys = nullptr;
std::uint32_t *legacySortedQueueIndices = nullptr;
std::uint32_t *legacySortScratch = nullptr;
std::uint32_t legacyRenderPacketCapacity = 0;
std::uint32_t legacyQueuedDrawCapacity = 0;
std::uint32_t legacyRenderPacketCount = 0;
std::uint32_t legacyQueuedDrawCount = 0;
bool legacyRenderQueueEnabled = true;
Matrix4x4 legacyAlphaPassMatrix{};
LegacyPacketAlphaState legacyPacketAlphaState{};
bool legacyBusyPreparingDraw = false;
std::uint32_t legacyMirroredDrawMode = 0;
std::uint32_t legacyNavigationRenderFlags{};
std::uint32_t legacyNavigationRenderDirty{};

struct SmokeGpuVertex {
    Vector4 position;
    std::uint32_t colour;
    float u;
    float v;
};

static_assert(sizeof(SmokeGpuVertex) == 0x1c);

struct FireGpuVertex {
    Vector3 position;
    float u;
    float v;
};

static_assert(sizeof(FireGpuVertex) == 0x14);

struct SmokeDrawState {
    SmokeGpuVertex *mapped{};
    std::uint32_t capacity{};
    std::uint32_t count{};
    std::uint32_t texture{};
    std::uint32_t renderPass{};
    bool compact{};
    LegacyEffectGpuFlags gpuFlags{};
    Matrix4x4 projection{};
};

Matrix4x4 smokeProjection{};
SmokeDrawState smokeDraw{};
std::array<LegacyFireQuad, 512> fireQuads{};
std::uint32_t fireQuadCount = 0;
std::int32_t fireVertexBuffer = 0;

bool AllocateLegacyRenderQueue() {
    RetireLegacyFlagDebugCompatibilityResources();





    constexpr std::uint32_t packetCapacity = 1000;
    constexpr std::uint32_t drawCapacity = 1000;
    legacyRenderPacketCapacity = 0;
    legacyQueuedDrawCapacity = 0;
    legacyRenderPacketCount = 0;
    legacyQueuedDrawCount = 0;
    legacyQueuedShaderEntries = nullptr;
    legacyQueuedShaderSortKeys = nullptr;
    legacyQueuedShaderCount = 0;
    legacyQueuedDrawCommandCount = 0;
    legacyQueuedCallbackCount = 0;
    legacyRenderPackets = static_cast<LegacyRenderPacketState *>(::operator new(sizeof(LegacyRenderPacketState) * packetCapacity));
    legacySortedQueueIndices = static_cast<std::uint32_t *>(::operator new(sizeof(std::uint32_t) * drawCapacity));
    legacySortScratch = static_cast<std::uint32_t *>(::operator new(sizeof(std::uint32_t) * drawCapacity));
    if (legacyRenderPackets && legacySortedQueueIndices && legacySortScratch) {
        // The zero-capacity shader entry/key pair is not allocated.
        legacyQueuedDraws = static_cast<LegacyQueuedDraw *>(::operator new(sizeof(LegacyQueuedDraw) * drawCapacity));
        legacyQueuedDrawSortKeys = static_cast<float *>(::operator new(sizeof(float) * drawCapacity));
        if (legacyQueuedDraws && legacyQueuedDrawSortKeys) {
            legacyRenderPacketCapacity = packetCapacity;
            legacyQueuedDrawCapacity = drawCapacity;
            return true;
        }
    }



    if (legacyRenderPackets) {
        ::operator delete(legacyRenderPackets);
        legacyRenderPackets = nullptr;
    }
    if (legacySortedQueueIndices) {
        ::operator delete(legacySortedQueueIndices);
        legacySortedQueueIndices = nullptr;
    }
    if (legacySortScratch) {
        ::operator delete(legacySortScratch);
        legacySortScratch = nullptr;
    }
    if (legacyQueuedDraws) {
        ::operator delete(legacyQueuedDraws);
        legacyQueuedDraws = nullptr;
    }
    if (legacyQueuedDrawSortKeys) {
        ::operator delete(legacyQueuedDrawSortKeys);
        legacyQueuedDrawSortKeys = nullptr;
    }
    return false;
}



void UpdateLegacyDepthFadeConstants() {
    const float difference = legacyDepthFadeFar - legacyDepthFadeNear;
    if (std::abs(difference) >= std::numeric_limits<float>::epsilon()) {
        shaderParameters.constants[22] = {-(legacyDepthFadeIntensity / difference),
            ((legacyDepthFadeIntensity * legacyDepthFadeFar) / difference - legacyDepthFadeIntensity) + 1.0f, 1.0f - legacyDepthFadeIntensity, 0.0f};
    } else {

        shaderParameters.constants[22] = {std::bit_cast<float>(0xb8d1bc76U), std::bit_cast<float>(0x3f800347U), 0.0f, 0.0f};
    }
}

Vector3 TransformModelDirection(const Vector3 &value) {
    return {workTransformMatrix.rows[0].x * value.x + workTransformMatrix.rows[0].y * value.y + workTransformMatrix.rows[0].z * value.z,
        workTransformMatrix.rows[1].x * value.x + workTransformMatrix.rows[1].y * value.y + workTransformMatrix.rows[1].z * value.z,
        workTransformMatrix.rows[2].x * value.x + workTransformMatrix.rows[2].y * value.y + workTransformMatrix.rows[2].z * value.z};
}

void PrepareLegacyModelLambertState() {
    legacyVertexColourStateActive = false;
    temporaryModelPosition = TransformModelDirection(modelReferenceDirection);
    if (temporaryModelScale != 1.0f) {
        temporaryModelPosition.x /= temporaryModelScale;
        temporaryModelPosition.y /= temporaryModelScale;
        temporaryModelPosition.z /= temporaryModelScale;
    }
    modelLightVector = {temporaryModelLight, temporaryModelLight, temporaryModelLight};


    shaderParameters.constants[27] = {temporaryModelPosition.x, temporaryModelPosition.y, temporaryModelPosition.z, 0.0f};

    if ((std::bit_cast<std::uint32_t>(lambertFvfReference.x) & 1U) != 0) {
        temporaryModelPosition = TransformModelDirection({lambertFvfReference.x, lambertFvfReference.y, lambertFvfReference.z});
        if (temporaryModelScale != 1.0f) {
            temporaryModelPosition.x /= temporaryModelScale;
            temporaryModelPosition.y /= temporaryModelScale;
            temporaryModelPosition.z /= temporaryModelScale;
        }
    } else {
        temporaryModelPosition = {};
    }

    shaderParameters.constants[18] = {temporaryModelPosition.x, temporaryModelPosition.y, temporaryModelPosition.z, 0.0f};
    currentLegacyVertexShaderMap = g_legacyLambertVertexShaderMap;
}

Vector3 TransformLightVector(const Vector3 &value) {
    return {workTransformMatrix.rows[1].x * value.y + workTransformMatrix.rows[0].x * value.x + workTransformMatrix.rows[2].x * value.z,
        workTransformMatrix.rows[1].y * value.y + workTransformMatrix.rows[0].y * value.x + workTransformMatrix.rows[2].y * value.z,
        workTransformMatrix.rows[1].z * value.y + workTransformMatrix.rows[0].z * value.x + workTransformMatrix.rows[2].z * value.z};
}

Vector3 TransformLightPosition(const Vector3 &value) {
    Vector3 result = TransformLightVector(value);
    result.x += workTransformMatrix.rows[3].x;
    result.y += workTransformMatrix.rows[3].y;
    result.z += workTransformMatrix.rows[3].z;
    return result;
}

void PrepareLegacyModelVertexColourState() {
    legacyVertexColourStateActive = true;
    modelLightVector = globalLightColour;
    std::array<Vector4, 57> constants{};
    constants[0] = {globalLightColour.x, globalLightColour.y, globalLightColour.z, 0.0f};
    std::uint32_t directionalCount = 0;
    std::uint32_t pointCount = 0;
    std::uint32_t spotCount = 0;

    const Matrix4x4 savedTransform = workTransformMatrix;
    Matrix4x4::NormalizeAndSetTransformMatrix();
    for (LegacyVertexColourStateRecord &light : legacyVertexColourStates) {
        if (!light.enabled)
            continue;
        Vector4 *output = nullptr;
        if (light.kind == 2 && directionalCount < 2) {
            output = &constants[1 + 2 * directionalCount++];
        } else if (light.kind == 3 && pointCount < 14) {
            output = &constants[5 + 3 * pointCount++];
        } else if (light.kind == 4 && spotCount < 2) {
            output = &constants[47 + 5 * spotCount++];
        } else {
            continue;
        }
        if (light.kind != 3) {
            light.modelDirection = TransformLightVector(light.transformedDirection);
            if (temporaryModelScale != 1.0f) {
                light.modelDirection.x /= temporaryModelScale;
                light.modelDirection.y /= temporaryModelScale;
                light.modelDirection.z /= temporaryModelScale;
            }
        }
        if (light.kind != 2) {
            light.modelPosition = TransformLightPosition(light.transformedPosition);
            output[0] = {light.modelPosition.x, light.modelPosition.y, light.modelPosition.z, 0.0f};
        } else {
            output[0] = {light.modelDirection.x, light.modelDirection.y, light.modelDirection.z, 0.0f};
        }
        const Vector4 colour{light.colour.y, light.colour.z, light.colour.w, light.colour.x};
        if (light.kind == 4) {
            output[1] = {light.modelDirection.x, light.modelDirection.y, light.modelDirection.z, 0.0f};
            output[2] = colour;
            output[3] = {light.attenuation.x, light.attenuation.y, light.attenuation.z, 8.0f};
            output[4] = {light.coneCosine, light.inverseRange, 0.0f, 0.0f};
        } else {
            output[1] = colour;
            if (light.kind == 3) {
                output[2] = {light.attenuation.x, light.attenuation.y, light.attenuation.z, 8.0f};
            }
        }
    }
    workTransformMatrix = savedTransform;


    std::copy_n(constants.begin(), 5, shaderParameters.constants + 26);
    std::copy_n(constants.begin() + 5, 42, shaderParameters.constants + 39);
    std::copy_n(constants.begin() + 47, 10, shaderParameters.constants + 82);
    shaderParameters.constants[112].x = static_cast<float>(directionalCount);
    shaderParameters.constants[112].y = static_cast<float>(pointCount);
    shaderParameters.constants[112].z = static_cast<float>(spotCount);
    currentLegacyVertexShaderMap = g_legacyLambertVertexShaderMap;
    currentLegacyVertexShaderMap[0] = g_lightingVertexShader;
    currentLegacyVertexShaderMap[1] = g_lightingVertexShader;
}

std::uint32_t ConvertDepthSetting(std::uint32_t setting) {
    switch (setting) {
    case 512:
        return 1;
    case 513:
        return 5;
    case 514:
        return 3;
    case 515:
        return 7;
    case 516:
        return 2;
    case 517:
        return 6;
    case 518:
        return 4;
    default:
        return 0;
    }
}

std::uint32_t ConvertBlendSetting(std::uint32_t setting) {
    if (setting >= 768 && setting <= 776)
        return setting - 766;
    if (setting == 1)
        return 1;
    if (setting >= 12 && setting <= 17)
        return setting - 1;
    return 0;
}

std::uint32_t ConvertStencilSetting(std::uint32_t setting) {
    switch (setting) {
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

} // namespace

static void ApplyEngineFlag(std::uint32_t index, std::uint32_t setting) {
    switch (index) {
    case 1:
        stencilReference = setting;
        break;
    case 2:
        engineFlags = (engineFlags & 0xfffffffffff807ffULL) | (std::uint64_t(setting & 0xffU) << 11U);
        break;
    case 3:
        engineFlags = (engineFlags & 0xfffffffff807ffffULL) | (std::uint64_t(setting & 0xffU) << 19U);
        break;
    case 4:
        shaderParameters.base.textureFactor = {float((setting >> 16U) & 0xffU) / 255.0f, float((setting >> 8U) & 0xffU) / 255.0f,
            float(setting & 0xffU) / 255.0f, float((setting >> 24U) & 0xffU) / 255.0f};
        break;
    case 7:
        engineFlags = (engineFlags & ~1ULL) | (setting != 0);
        break;
    case 0x22:
        shaderParameters.base.fogColour = {float((setting >> 16U) & 0xffU) / 255.0f, float((setting >> 8U) & 0xffU) / 255.0f, float(setting & 0xffU) / 255.0f};
        break;
    case 0x34:
        engineFlags = (engineFlags & ~0x40ULL) | (std::uint64_t(setting != 0) << 6U);
        break;
    case 0x35:
        engineFlags = (engineFlags & ~0x780000000ULL) | (std::uint64_t(ConvertStencilSetting(setting)) << 31U);
        break;
    case 0x36:
        engineFlags = (engineFlags & ~0x78000000ULL) | (std::uint64_t(ConvertStencilSetting(setting)) << 27U);
        break;
    case 0x37:
        engineFlags = (engineFlags & ~0x7800000000ULL) | (std::uint64_t(ConvertStencilSetting(setting)) << 35U);
        break;
    case 0x38:
        engineFlags = (engineFlags & ~0x780ULL) | (std::uint64_t(ConvertDepthSetting(setting)) << 7U);
        break;
    case 0x39:
        engineFlags = (engineFlags & ~0x3cULL) | (std::uint64_t(ConvertDepthSetting(setting)) << 2U);
        break;
    case 0x3a:
        shaderParameters.base.alphaFunction = setting - 512U;
        break;
    case 0x3b:
        blendFlags = (blendFlags & ~1ULL) | (setting != 0);
        break;
    case 0x3c:
        shaderParameters.base.alphaTestEnabled = setting;
        break;
    case 0x3d:
        shaderParameters.base.alphaReference = float(setting) / 255.0f;
        break;
    case 0x3e:
        blendFlags = (blendFlags & ~0x7cULL) | (std::uint64_t(ConvertBlendSetting(setting)) << 2U);
        break;
    case 0x3f:
        blendFlags = (blendFlags & ~0xf80ULL) | (std::uint64_t(ConvertBlendSetting(setting)) << 7U);
        break;
    case 0x40:
        engineFlags = (engineFlags & ~2ULL) | (std::uint64_t(setting != 0) << 1U);
        break;
    case 0x43:
        blendFlags = (blendFlags & 0x0fffffffULL) | (std::uint64_t(setting) << 28U);
        break;
    case 0x5c:
        shaderParameters.base.fogEnabled = setting;
        break;
    case 0x67:
        shaderParameters.base.specularEnabled = setting;
        break;
    case 0x93: {
        const std::uint32_t mode = setting == 2304 ? 1U : setting == 2305 ? 2U : 0U;
        renderingMode = (renderingMode & ~0x18U) | (mode << 3U);
        break;
    }
    case 0x95: {
        std::uint32_t encoded = 0;
        if (static_cast<std::int32_t>(setting) > 0)
            encoded = (setting & 0xffffU) | 0x80U;
        else
            encoded = (0U - setting) & 0xffffU;
        renderingMode = (renderingMode & ~0x1fe0U) | ((encoded << 5U) & 0x1fe0U);
        shaderParameters.base.depthBias =
            (setting & 0x80U) ? -float(setting & 0x7fU) * 0.00000075000003f : float(static_cast<std::int32_t>(setting)) * 0.00000075000003f;
        break;
    }
    case 0xab: {
        const std::uint32_t value = setting >= 2 && setting <= 5 ? setting - 1U : 0U;
        blendFlags = (blendFlags & ~0x7000ULL) | (std::uint64_t(value) << 12U);
        break;
    }
    case 0xae:
        renderingMode = (renderingMode & ~4U) | ((setting != 0) ? 4U : 0U);
        break;
    case 0xcf:
        blendFlags = (blendFlags & ~0xf8000ULL) | (std::uint64_t(ConvertBlendSetting(setting)) << 15U);
        break;
    case 0xd0:
        blendFlags = (blendFlags & ~0x1f00000ULL) | (std::uint64_t(ConvertBlendSetting(setting)) << 20U);
        break;
    case 0xd1: {
        const std::uint32_t value = setting >= 2 && setting <= 5 ? setting - 1U : 0U;
        blendFlags = (blendFlags & ~0xe000000ULL) | (std::uint64_t(value) << 25U);
        break;
    }
    default:
        break;
    }
}

// @verified - p: SetEngineFlag.
void SetEngineFlag(std::uint32_t index, std::uint32_t setting) {
    ApplyEngineFlag(index, setting);
    currentEngineSettings[index] = setting;
}

// @verified - p: RestoreEngineFlag.
void RestoreEngineFlag(std::uint32_t index) {
    ApplyEngineFlag(index, initialEngineSettings[index]);
    currentEngineSettings[index] = initialEngineSettings[index];
}

// @verified - p
void shenSetCharaDrawFlags(std::uint32_t mode) {
    if (mode - 1U > 6U)
        return;
    SetEngineFlag(0x34, 1);
    SetEngineFlag(1, mode << 5U);
    SetEngineFlag(0x37, 7681);
}

// @verified - p
void shenSetRenderFlags() {
    RestoreEngineFlag(0x34);
    RestoreEngineFlag(1);
    RestoreEngineFlag(0x37);
}

// @verified - p: InitShaderStates.
void InitShaderStates() {
    constexpr std::array<std::uint32_t, 69> orderedSettings{
        7,8,66,64,60,62,63,147,149,57,61,58,26,59,92,103,34,35,36,
        37,38,48,52,53,54,55,56,1,2,3,4,128,129,130,131,102,139,
        141,142,143,145,146,5,148,151,154,155,156,157,158,159,160,
        161,162,163,166,67,171,16,132,133,134,135,136,140,152,165,167,170};
    for (const auto index : orderedSettings)
        RestoreEngineFlag(index);
    for (std::uint32_t index = 0; index < initialEngineSettings.size(); ++index)
        RestoreEngineFlag(index);
}

// @verified - p: InitShenmueRendering.
void InitShenmueRendering() {
    blendFlags = 0xf0000000U;
    renderingMode = 1;
    engineFlags = 0;
    initialEngineSettings[166] = std::bit_cast<std::uint32_t>(64.0f);
    initialEngineSettings[7] = 1;
    initialEngineSettings[64] = 1;
    initialEngineSettings[60] = initialEngineSettings[61] = 0;
    initialEngineSettings[62] = 770;
    initialEngineSettings[63] = 771;
    initialEngineSettings[147] = 2304;
    initialEngineSettings[57] = 513;
    initialEngineSettings[58] = 516;
    initialEngineSettings[26] = initialEngineSettings[92] = 0;
    initialEngineSettings[103] = 1;
    initialEngineSettings[34] = 0;
    initialEngineSettings[36] = initialEngineSettings[37] = 0;
    initialEngineSettings[38] = std::bit_cast<std::uint32_t>(1.0f);
    initialEngineSettings[149] = initialEngineSettings[48] = initialEngineSettings[52] = 0;
    initialEngineSettings[53] = initialEngineSettings[54] = initialEngineSettings[55] = 7680;
    initialEngineSettings[56] = 519;
    initialEngineSettings[1] = initialEngineSettings[4] = 0;
    initialEngineSettings[2] = initialEngineSettings[3] = 0xffffffffU;
    initialEngineSettings[128] = initialEngineSettings[129] = 0;
    initialEngineSettings[130] = initialEngineSettings[131] = 0;
    initialEngineSettings[102] = 1;
    initialEngineSettings[139] = 0;
    initialEngineSettings[141] = initialEngineSettings[142] = initialEngineSettings[143] = 1;
    initialEngineSettings[154] = std::bit_cast<std::uint32_t>(1.0f);
    initialEngineSettings[156] = initialEngineSettings[157] = 0;
    initialEngineSettings[158] = initialEngineSettings[159] = 0;
    initialEngineSettings[160] = std::bit_cast<std::uint32_t>(1.0f);
    initialEngineSettings[161] = 1;
    initialEngineSettings[162] = 0xffffffffU;
    initialEngineSettings[171] = initialEngineSettings[16] = 1;
    initialEngineSettings[132] = initialEngineSettings[133] = 0;
    initialEngineSettings[134] = initialEngineSettings[135] = 0;
    initialEngineSettings[136] = 1;
    initialEngineSettings[152] = initialEngineSettings[167] = 0;
    initialEngineSettings[170] = std::bit_cast<std::uint32_t>(1.0f);
    initialEngineSettings[67] = 15;
    InitShaderStates();
}

void SetAlphaFlags(std::uint32_t stage, std::uint32_t setting, std::uint32_t value) {
    if (stage >= pixelSamplerKeys.size())
        return;
    std::uint64_t key = std::uint64_t(pixelSamplerKeys[stage][0]) | (std::uint64_t(pixelSamplerKeys[stage][1]) << 32U);
    switch (setting) {
    case 0: {
        const std::uint64_t bits = value == 1 ? 0 : value == 2 ? 64 : 128;
        key = (key & ~0xc0ULL) | bits;
        break;
    }
    case 1: {
        const std::uint64_t bits = value == 1 ? 0 : value == 2 ? 256 : 512;
        key = (key & ~0x300ULL) | bits;
        break;
    }
    case 3:
        key = (key & ~1ULL) | 0x3c002ULL;
        break;
    case 4:
        key = (key & ~4ULL) | 0x3c008ULL;
        break;
    case 5:
        key = (key & ~0x10ULL) | 0x3c020ULL;
        break;
    case 6:
        key = (key & 0xfffc00000003ffffULL) | (std::uint64_t(value) << 18U);
        break;
    case 16:
        shaderParameters.base.alphaOperation = value;
        return;
    case 17:
        shaderParameters.base.alphaArgument0 = value;
        return;
    case 18:
        shaderParameters.base.alphaArgument1 = value;
        return;
    case 19:
        shaderParameters.base.alphaArgument2 = value;
        return;
    default:
        return;
    }
    pixelSamplerKeys[stage][0] = static_cast<std::uint32_t>(key);
    pixelSamplerKeys[stage][1] = static_cast<std::uint32_t>(key >> 32U);
}

namespace {

void CommitGeometryState(d3t::D3TRenderState &state) {
    std::array<std::byte, 16> depth{};
    std::memcpy(depth.data(), &engineFlags, sizeof(engineFlags));
    if (state.depthStencilState != depth || state.stencilReference != stencilReference) {
        state.depthStencilState = depth;
        state.stencilReference = stencilReference;
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
    }
    if (state.rasterizerState != renderingMode) {
        state.rasterizerState = renderingMode;
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
    }
    if (std::memcmp(state.blendState.data(), &blendFlags, sizeof(blendFlags)) != 0) {
        std::memcpy(state.blendState.data(), &blendFlags, sizeof(blendFlags));
        state.pipelineStateFlags |= d3t::PipelineBlend;
    }

    auto *const bytes = reinterpret_cast<std::byte *>(&state);
    constexpr std::size_t pixelSamplerOffset = 0x534;
    for (std::uint32_t sampler = 0; sampler < 4; ++sampler) {
        const std::size_t offset = pixelSamplerOffset + sampler * 16U;
        if (std::memcmp(bytes + offset, pixelSamplerKeys[sampler].data(), 16U) != 0) {
            std::memcpy(bytes + offset, pixelSamplerKeys[sampler].data(), 16U);
            state.samplerDirtyMasks[1] |= std::uint16_t(1U << sampler);
        }
    }
}

bool ReadU32(std::span<const std::byte> bytes, std::size_t offset, std::uint32_t &value) {
    if (offset > bytes.size() || sizeof(value) > bytes.size() - offset) {
        return false;
    }
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

bool IsNativeOpcode(std::uint8_t opcode) {
    return opcode <= 22;
}

bool Contains(std::span<const std::byte> bytes, std::size_t offset, std::size_t size) {
    return offset <= bytes.size() && size <= bytes.size() - offset;
}

template <typename Value> Value ReadValue(std::span<const std::byte> bytes, std::size_t offset) {
    Value value{};
    if (Contains(bytes, offset, sizeof(value))) {
        std::memcpy(&value, bytes.data() + offset, sizeof(value));
    }
    return value;
}

std::int32_t ResolveVertexBuffer(const LegacyDrawResources &resources, std::uint32_t localIndex) {
    return localIndex < resources.vertexBuffers.size() ? resources.vertexBuffers[localIndex].handle : 0;
}

const LegacyIndexDraw *FindIndexDraw(const LegacyDrawResources &resources, std::uint32_t commandOffset) {
    const auto found = std::lower_bound(resources.indexDraws.begin(), resources.indexDraws.end(), commandOffset,
        [](const LegacyIndexDraw &draw, std::uint32_t offset) { return draw.commandOffset < offset; });
    return found != resources.indexDraws.end() && found->commandOffset == commandOffset ? &*found : nullptr;
}

std::uint32_t ResolveTexture(const LegacyDrawResources &resources, std::uint32_t commandOffset, std::uint32_t recordIndex) {
    for (const LegacyTextureBinding &binding : resources.textureBindings) {
        if (binding.commandOffset == commandOffset && binding.recordIndex == recordIndex) {
            return binding.textureHandle;
        }
    }
    return 0;
}

void SelectShader(d3t::D3TRenderState &state, std::uint32_t primitiveType) {
    std::uint32_t vertexShader = primitiveType < currentLegacyVertexShaderMap.size() ? currentLegacyVertexShaderMap[primitiveType] : 0;
    std::uint32_t inputLayout = legacyFvfLayoutAliases[0];
    if (primitiveType == 3) {
        vertexShader = currentVertexColourShader;
        const std::int32_t layoutIndex = std::clamp(currentVertexColourLayoutType, 0, 2) + 10;
        inputLayout = g_fvfLayouts[static_cast<std::size_t>(layoutIndex)];
    } else {
        const std::size_t alias = primitiveType + (legacyVertexColourStateActive ? 10U : 0U);
        inputLayout = alias < legacyFvfLayoutAliases.size() ? legacyFvfLayoutAliases[alias] : 0;
    }
    if (state.vertexShader != vertexShader) {
        state.vertexShader = vertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.inputLayout != inputLayout) {
        state.inputLayout = inputLayout;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (primitiveType != 3) {
        shaderParameters.base.useEnvironmentMap = primitiveType >= 4 && primitiveType <= 6;
    }
}

void BindPixelTexture0(d3t::D3TRenderState &state, std::uint32_t texture) {
    // D3T PS resource table and view-flags table, verified in both games.
    auto *const bytes = reinterpret_cast<std::byte *>(&state);
    constexpr std::size_t textureOffset = 0x1b4;
    constexpr std::size_t viewFlagsOffset = 0x334;
    std::uint32_t oldTexture{};
    std::uint32_t oldFlags{};
    std::memcpy(&oldTexture, bytes + textureOffset, 4);
    std::memcpy(&oldFlags, bytes + viewFlagsOffset, 4);
    if (oldTexture != texture || oldFlags != 0) {
        state.shaderResourceDirtyMasks[1] |= 1U;
        std::memcpy(bytes + textureOffset, &texture, 4);
        const std::uint32_t zero = 0;
        std::memcpy(bytes + viewFlagsOffset, &zero, 4);
    }
}



// @verified - p
void ClearPixelShaderEffectSRVSlot(d3t::D3TRenderState &state, std::uint32_t slot) {
    auto *const bytes = reinterpret_cast<std::byte *>(&state);
    const std::size_t textureOffset = 0x1b4 + 4 * slot;
    const std::size_t viewFlagsOffset = 0x334 + 4 * slot;
    std::uint32_t oldTexture{};
    std::uint32_t oldFlags{};
    std::memcpy(&oldTexture, bytes + textureOffset, 4);
    std::memcpy(&oldFlags, bytes + viewFlagsOffset, 4);
    if (oldTexture != g_legacyDefaultEffectTexture || oldFlags != 0) {
        state.shaderResourceDirtyMasks[1] |= 1U << slot;
        std::memcpy(bytes + textureOffset, &g_legacyDefaultEffectTexture, 4);
        const std::uint32_t zero = 0;
        std::memcpy(bytes + viewFlagsOffset, &zero, 4);
    }
}

void ResetLegacyMaterialState(d3t::D3TRenderState &state) {
    // DXEffectClear_SetRenderFlags restores the engine's initialized
    // material defaults after every FVF packet, not after the whole model.
    ClearPixelShaderEffectSRVSlot(state, 0);
    SetEngineFlag(0x39, 513);
    SetEngineFlag(0x40, 1);
    SetEngineFlag(0x3e, 770);
    SetEngineFlag(0x3f, 771);
    SetEngineFlag(0x3c, 0);
    SetEngineFlag(0x3a, 516);
    SetEngineFlag(0x3d, 0);
    SetEngineFlag(0x95, 0);
    SetEngineFlag(0x5c, 0);
    SetEngineFlag(0x93, 2304);
    SetAlphaFlags(0, 16, 4);
    SetAlphaFlags(0, 18, 2);
    SetAlphaFlags(0, 19, 0);
    SetAlphaFlags(0, 0, 3);
    SetAlphaFlags(0, 1, 3);
    SetAlphaFlags(0, 3, 2);
    SetAlphaFlags(0, 4, 2);
    SetAlphaFlags(0, 5, 2);
    SetAlphaFlags(0, 6, 0);
}

void CommitFrameForRaster() {
    Matrix4x4 *const sourceStack = g_matrixStack.top.stack;
    *sourceStack = workTransformMatrix;

    SetActiveStack(MatrixStackType::Transparent);
    Matrix4x4_Push();
    Matrix4x4::MultiplyWithPrePassMatrix(sourceStack);
    Matrix4x4::AffineProjectionSwap();
    Matrix4x4 *const transformed = g_matrixStack.top.stack;
    *transformed = workTransformMatrix;
    shaderParameters.constants[1] = transformed->rows[0];
    shaderParameters.constants[2] = transformed->rows[1];
    shaderParameters.constants[3] = transformed->rows[2];
    shaderParameters.constants[4] = transformed->rows[3];
    Matrix4x4::Pop(1);

    SetActiveStack(MatrixStackType::DepthPrepass);
    Matrix4x4::AffineProjectionSwap();
    const Vector4 projectionRow0 = workTransformMatrix.rows[0];
    const Vector4 projectionRow1 = workTransformMatrix.rows[1];
    const Vector4 projectionRow2 = workTransformMatrix.rows[2];
    Matrix4x4::AffineProjectionSwap();
    shaderParameters.constants[19] = projectionRow0;
    shaderParameters.constants[20] = projectionRow1;
    shaderParameters.constants[21] = projectionRow2;
}

} // namespace

void UpdateShaderParams() {
#if defined(_WIN32)
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return;
    void *const mapped = d3t::MapTempConstantBufferForWriteDiscard(context, 3, 1, static_cast<std::int32_t>(shaderParameterStorage.size()));
    if (mapped) {
        std::memcpy(mapped, shaderParameterStorage.data(), shaderParameterStorage.size());
    }
    CommitGeometryState(d3t::GetRenderState(*context));
#endif
}

namespace {

#if defined(_WIN32)
#endif

void AssignPixelShaderForShenmue(std::span<const std::byte> bytes, std::size_t command, d3t::D3TRenderState &state) {
    if (!Contains(bytes, command, 0x3c))
        return;
    const Vector4 vertexColour = ReadValue<Vector4>(bytes, command + 0x24);
    Vector4 material = ReadValue<Vector4>(bytes, command + 0x14);
    const Vector4 lightScale = ReadValue<Vector4>(bytes, command + 0x04);
    const std::uint64_t materialExtra = ReadValue<std::uint64_t>(bytes, command + 0x34);
    material.w = ReadValue<float>(bytes, command + 0x38);

    // AssignPixelShaderForShenmue writes aliases inside the contiguous
    // GShaderParams upload: spotLights[0].pos, g_pTexture2D_0,
    // stru_...9A48 and dword_...9A58 are c[5], c[6], c[7], c[8].
    shaderParameters.constants[5] = {
        lightScale.x * modelLightVector.x * material.x, lightScale.y * modelLightVector.y * material.y, lightScale.z * modelLightVector.z * material.z, 1.0f};
    if (legacyAlphaPassEnabled) {
        material.w = material.w > 0.0f ? legacyAlphaMultiplier * material.w : legacyAlphaMultiplier;
    }
    if (perspectiveProjection == 1) {
        material.x = 1.0f;
        material.y = 0.0f;
        material.z = 0.0f;
    }
    shaderParameters.constants[6] = material;
    shaderParameters.constants[7] = vertexColour;
    shaderParameters.constants[8] = {};
    std::memcpy(&shaderParameters.constants[8], &materialExtra, sizeof(materialExtra));
    const std::uint32_t pixelShader = g_simplePixelShader;
    if (state.pixelShader != pixelShader) {
        state.pixelShader = pixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
}

std::array<std::uint32_t, 6> postFxSelectorParameters{};
std::array<float, 21> postFxColourParameters{};
std::array<float, 5> postFxBlurWeights{};
std::array<std::uint32_t, 62> postFxCrossParameters{};
std::array<std::byte, 0x74> postFxModeDefaults{};
std::array<std::byte, 0x74> postFxModeCurrent{};
bool postFxModeActive = false;
float postFxModeScale = 1.0f;
std::uint32_t postFxScratchTexture = 0;
std::uint32_t postFxRetainedTexture = 0;

template <typename Value> void WritePostFxModeValue(std::array<std::byte, 0x74> &bank, std::size_t offset, const Value &value) {
    std::memcpy(bank.data() + offset, &value, sizeof(value));
}

void InitializePostFxShaderConstants(const std::array<float, 7> &parameters) {
    const float scale = 256.0f / ((128.0f - parameters[1]) + (128.0f - parameters[1]));
    const float luminance = std::clamp((parameters[0] - 128.0f) * scale + 128.0f, -255.0f, 255.0f);
    const float halfScale = scale * 0.5f;
    const float halfInverse = (1.0f - parameters[2]) * 0.5f;

    shaderParameters.base.constants[0] = {halfScale, halfScale, halfScale, 0.0f};
    shaderParameters.base.constants[1] = {parameters[2], halfInverse, halfInverse, 0.0f};
    shaderParameters.base.constants[2] = {halfInverse, halfInverse, parameters[2], 0.0f};
    shaderParameters.constant0[0] = {parameters[4], 0.0f, 0.0f, 0.0f};
    shaderParameters.constant0[1] = {0.0f, parameters[5], 0.0f, 0.0f};
    shaderParameters.constant0[2] = {0.0f, 0.0f, parameters[6], 0.0f};
    shaderParameters.constant1[0] = {(luminance / 255.0f) * 0.5f, (luminance / 255.0f) * 0.5f, (luminance / 255.0f) * 0.5f, 0.0f};
    shaderParameters.constant1[1] = {halfInverse, parameters[2], halfInverse, 0.0f};
}

void InitializePostFxModeDefaults() {
    postFxModeDefaults.fill(std::byte{});
    postFxModeDefaults[2] = std::byte{1};
    constexpr std::array<float, 4> scaleValues{1.0f, 1.0f, 1.1f, 1.15f};
    std::memcpy(postFxModeDefaults.data() + 4, scaleValues.data(), sizeof(scaleValues));
    WritePostFxModeValue(postFxModeDefaults, 0x18, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x1c, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x20, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x24, 1.0f);
    postFxModeDefaults[0x28] = std::byte{1};
    WritePostFxModeValue(postFxModeDefaults, 0x2c, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x3c, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x40, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x44, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x4c, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x50, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x54, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x58, 1.0f);
    constexpr float fullScreenExtent = 6500.0f;
    WritePostFxModeValue(postFxModeDefaults, 0x60, fullScreenExtent);
    WritePostFxModeValue(postFxModeDefaults, 0x64, 1.0f);
    WritePostFxModeValue(postFxModeDefaults, 0x68, fullScreenExtent);
    WritePostFxModeValue(postFxModeDefaults, 0x6c, 1.0f);
    postFxModeCurrent = postFxModeDefaults;
    postFxModeActive = false;
    postFxModeScale = 1.0f;
}

void ApplyPostProcessingMode(std::int32_t mode) {
    if (mode == 0) {
        if (!postFxModeActive)
            return;
        postFxModeActive = false;
        postFxModeCurrent = postFxModeDefaults;
        return;
    }
    if (mode != 10 && mode != 22 && mode != 38 && mode != 59)
        return;

    postFxModeActive = true;
    postFxModeCurrent = postFxModeDefaults;
    if (mode == 10) {
        constexpr std::uint16_t enabledPair = 0x0101;
        WritePostFxModeValue(postFxModeCurrent, 0, enabledPair);
        return;
    }
    if (mode == 22) {
        postFxModeCurrent[0] = std::byte{1};
        postFxModeCurrent[2] = std::byte{0};
        postFxModeCurrent[0x14] = std::byte{1};
        WritePostFxModeValue(postFxModeCurrent, 4, 0.5f);
        postFxModeScale = 1.1f;
        return;
    }
    if (mode == 38) {
        constexpr std::uint16_t enabledPair = 0x0101;
        WritePostFxModeValue(postFxModeCurrent, 0x48, enabledPair);
        WritePostFxModeValue(postFxModeCurrent, 0x58, 1.0f);
        return;
    }

    postFxModeCurrent[0x14] = std::byte{1};
    postFxModeCurrent[0x28] = std::byte{1};
    postFxModeCurrent[0x48] = std::byte{1};
    WritePostFxModeValue(postFxModeCurrent, 0x2c, 0.75f);
    WritePostFxModeValue(postFxModeCurrent, 0x4c, 2.0f);
    WritePostFxModeValue(postFxModeCurrent, 0x50, 2.0f);
    WritePostFxModeValue(postFxModeCurrent, 0x54, 1.0f);
    WritePostFxModeValue(postFxModeCurrent, 0x58, 0.80000001f);
    postFxModeScale = 1.05f;
}

std::uint32_t CurrentPostProcessingRenderTarget() {
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return 0;
    std::uint32_t texture{};
    std::memcpy(&texture, d3t::GetRenderState(*context).outputTargets.data(), sizeof(texture));
    return texture;
}

bool EnsurePostProcessingScratch(std::uint32_t sceneTexture) {
    d3t::Texture2DDescription scene{};
    if (!sceneTexture || !d3t::GetTexture2DDescription(sceneTexture, &scene)) {
        return false;
    }
    d3t::Texture2DDescription scratch{};
    if (postFxScratchTexture && d3t::GetTexture2DDescription(postFxScratchTexture, &scratch) && scratch.width == scene.width &&
        scratch.height == scene.height && scratch.arraySize == scene.arraySize && scratch.mipLevels == scene.mipLevels && scratch.format == scene.format &&
        scratch.sampleCount == scene.sampleCount && scratch.sampleQuality == scene.sampleQuality) {
        return true;
    }
    if (postFxScratchTexture) {
        d3t::InitTexture2DResource(postFxScratchTexture);
        postFxScratchTexture = 0;
    }
    scene.initialData = nullptr;
    postFxScratchTexture = d3t::CreateTexture2D(&scene);
    return postFxScratchTexture != 0;
}

bool RecreatePostFxRetainedTexture(std::uint32_t sceneTexture) {
    if (postFxRetainedTexture) {
        d3t::InitTexture2DResource(postFxRetainedTexture);
        postFxRetainedTexture = 0;
    }
    d3t::Texture2DDescription description{};
    if (!sceneTexture || !d3t::GetTexture2DDescription(sceneTexture, &description)) {
        description = {d3t::g_originalScreenWidth, d3t::g_originalScreenHeight, 1, 1, 4, 4, 0, 1, 0, 0, nullptr};
    }
    if (!description.width || !description.height)
        return false;
    description.initialData = nullptr;
    postFxRetainedTexture = d3t::CreateTexture2D(&description);
    return postFxRetainedTexture != 0;
}

bool PostFxRetainedTextureMatches(std::uint32_t sceneTexture) {
    d3t::Texture2DDescription scene{};
    d3t::Texture2DDescription retained{};
    return sceneTexture && postFxRetainedTexture && d3t::GetTexture2DDescription(sceneTexture, &scene) &&
        d3t::GetTexture2DDescription(postFxRetainedTexture, &retained) && scene.width == retained.width && scene.height == retained.height;
}

void BindFullscreenOutput(d3t::D3TRenderState &state, std::uint32_t texture, float width, float height) {
    std::array<std::byte, 0x84> targets{};
    std::memcpy(targets.data(), &texture, sizeof(texture));
    if (state.outputTargets != targets) {
        state.outputTargets = targets;
        state.bufferStateFlags |= d3t::BufferOutputTargets;
    }
    const std::array<float, 6> viewport{0.0f, 0.0f, width, height, 0.0f, 1.0f};
    if (state.viewport != viewport) {
        state.viewport = viewport;
        state.viewportDirtyMask |= 1;
    }
}

bool RenderLegacyFullscreenQuad(std::uint32_t outputTexture, std::uint32_t inputTexture, std::uint32_t pixelShader, const float *blurStrength) {
#if defined(_WIN32)
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    std::uint32_t width{};
    std::uint32_t height{};
    if (!context || !context->deviceContext || !outputTexture || !inputTexture || outputTexture == inputTexture || !g_fullscreenQuadVertexShader ||
        !pixelShader || !d3t::GetTexture2DDimensions(outputTexture, &width, &height) || !width || !height) {
        return false;
    }

    auto &state = d3t::GetRenderState(*context);
    const std::array<float, 6> savedViewport = state.viewport;
    BindFullscreenOutput(state, outputTexture, static_cast<float>(width), static_cast<float>(height));
    if (state.vertexShader != g_fullscreenQuadVertexShader) {
        state.vertexShader = g_fullscreenQuadVertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != pixelShader) {
        state.pixelShader = pixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[20]) {
        state.inputLayout = g_fvfLayouts[20];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }

    const std::array<Vector4, 12> vertices{{
        {-1.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {},
        {-1.0f, -1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {},
        {1.0f, 1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 0.0f},
        {},
        {1.0f, -1.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.0f, 0.0f},
        {},
    }};
    if (void *const mapped = d3t::GetMappedConstantBuffer(context, 0, static_cast<std::uint32_t>(sizeof(vertices)))) {
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    } else {
        return false;
    }

    const std::uint64_t savedEngineFlags = engineFlags;
    const std::uint64_t savedBlendFlags = blendFlags;
    const std::uint32_t savedAlphaTest = shaderParameters.base.alphaTestEnabled;
    const std::uint32_t savedFog = shaderParameters.base.fogEnabled;
    SetEngineFlag(7, 0);
    SetEngineFlag(0x5c, 0);
    if (blurStrength) {
        SetEngineFlag(0x3b, 1);
        SetEngineFlag(0x3c, 0);
        shaderParameters.base.constants[0] = {*blurStrength, *blurStrength, *blurStrength, *blurStrength};
    }


    BindPixelTexture0(state, inputTexture);
    for (std::uint32_t slot = 1; slot < 4; ++slot)
        ClearPixelShaderEffectSRVSlot(state, slot);
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    ClearPixelShaderEffectSRVSlot(state, 0);
    engineFlags = savedEngineFlags;
    blendFlags = savedBlendFlags;
    shaderParameters.base.alphaTestEnabled = savedAlphaTest;
    shaderParameters.base.fogEnabled = savedFog;
    d3t::BindSceneFrameRenderTarget();
    if (state.viewport != savedViewport) {
        state.viewport = savedViewport;
        state.viewportDirtyMask |= 1;
    }
    return true;
#else
    static_cast<void>(outputTexture);
    static_cast<void>(inputTexture);
    static_cast<void>(pixelShader);
    static_cast<void>(blurStrength);
    return false;
#endif
}

bool RenderLegacyFullscreenQuadAlpha(std::uint32_t outputTexture, std::uint32_t inputTexture, float alpha) {
    const Vector4 savedTextureFactor = shaderParameters.base.textureFactor;
    const std::uint32_t savedAlphaOperation = shaderParameters.base.alphaOperation;
    const std::uint32_t savedAlphaArgument2 = shaderParameters.base.alphaArgument2;
    const std::uint64_t savedBlendFlags = blendFlags;
    const auto savedSampler = pixelSamplerKeys[0];

    const auto alphaByte = static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha * 255.0f));
    SetEngineFlag(4, (alphaByte << 24U) | 0x00ffffffU);
    SetEngineFlag(0x3b, 1);
    SetAlphaFlags(0, 0x10, 3);
    SetAlphaFlags(0, 0x13, 3);
    SetAlphaFlags(0, 4, 1);
    const bool rendered = RenderLegacyFullscreenQuad(outputTexture, inputTexture, g_defaultPixelShader, nullptr);

    shaderParameters.base.textureFactor = savedTextureFactor;
    shaderParameters.base.alphaOperation = savedAlphaOperation;
    shaderParameters.base.alphaArgument2 = savedAlphaArgument2;
    blendFlags = savedBlendFlags;
    pixelSamplerKeys[0] = savedSampler;
    return rendered;
}

float ReadPostEffectFloat(const PostEffectShaderPacket &packet, std::size_t index) {
    float value{};
    std::memcpy(&value, packet.parameters.data() + index * sizeof(value), sizeof(value));
    return value;
}

struct StripType3GpuVertex {
    Vector4 position;
    std::uint32_t diffuse;
    std::uint32_t specular;
    float u;
    float v;
};
static_assert(sizeof(StripType3GpuVertex) == 0x20);

std::int32_t stripType3VertexBuffer{};
std::uint32_t stripType3VertexCapacity{};
std::uint32_t stripType3WriteOffset{};
Matrix4x4 stripProjection{};
float stripNearClip{};
std::int32_t stripType0VertexBuffer{};
std::uint32_t stripBufferStateFlags{};
LegacyStripType3Batch *stripType3Batches{};
LegacyStripType3Batch **stripType3SortPointers{};
std::uint32_t stripType3BatchCapacity{}, stripType3BatchCount{};

} // namespace



void SetLegacySavedAlphaOperation(std::uint32_t operation) {


    savedAlphaOperation = operation;
    SetAlphaFlags(0, 6, operation);
}

float GetTexUVW(std::uint32_t index) {
    return index > 1 ? 0.0f : legacyTextureUVWidths[index];
}

std::int64_t SetTexUVW(std::uint32_t index, float width) {
    if (index > 1)
        return 0;
    legacyTextureUVWidths[index] = width;
    return 1;
}

void PublishLegacyWeatherSpawnDrift(const Vector3 &drift) {
    std::memcpy(&legacyWeatherSpawnDrift, &drift, sizeof(drift));
}

void SetLegacyWeatherRenderPass(bool translucent) {
    legacyWeatherRenderPass = translucent ? 1U : 0U;
    legacyWeatherShaderTypes = {28U, 12U};
    legacyWeatherVertexStride = 28U;
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.vertexShader != g_fvfPtdTVertexShader) {
        state.vertexShader = g_fvfPtdTVertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != g_fvfPtdTPixelShader) {
        state.pixelShader = g_fvfPtdTPixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[28]) {
        state.inputLayout = g_fvfLayouts[28];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
#endif
}

void RenderLegacyWeatherQuads(std::int32_t count) {
    legacyWeatherQuadCount = count;
    SetEngineFlag(0x64, 0);
    legacyWeatherLightingMode = 0;
    if (count <= 0)
        return;
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (legacyWeatherRenderPass == 1U) {
        SetEngineFlag(0x40, 0);
        SetEngineFlag(0x3b, 1);
    }
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleList)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(6U * static_cast<std::uint32_t>(count), 0);
    if (legacyWeatherRenderPass == 1U) {
        SetEngineFlag(0x3b, savedProjectedShadowRenderSettings.blendEnabled);
        SetEngineFlag(0x40, savedProjectedShadowRenderSettings.depthWriteEnabled);
    }
#endif
}

void SetLegacyFogEnabled(std::uint32_t enabled) {
    legacySceneFogEnabled = enabled;
}

void SetLegacyFogMode(std::int32_t mode) {


    legacyFogAlphaModes = (static_cast<std::uint32_t>(mode) << 22U) | (legacyFogAlphaModes & 0xff3fffffU);
    legacySceneFogEnabled = mode != 2;
    legacyModelFogEnabled = mode != 2;
}

bool GetLegacyRenderQueueEnabled() {
    return legacyRenderQueueEnabled;
}

void SetLegacyRenderQueueEnabled(bool enabled) {
    legacyRenderQueueEnabled = enabled;
}

void SetLegacyFogColour(std::uint32_t colour) {
    SetEngineFlag(0x22U, colour);
}

void SetLegacyDepthFadeRange(float nearDistance, float farDistance) {
    legacyDepthFadeNear = nearDistance;
    legacyDepthFadeFar = farDistance;
    UpdateLegacyDepthFadeConstants();
}

void SetLegacyDepthFadeBlue(float intensity) {
    legacyDepthFadeIntensity = intensity;
    UpdateLegacyDepthFadeConstants();
}

// @verified - p
void SetLegacyQueuedDrawDepthMode(std::int32_t fromGeometry, float fallbackDepth) {
    legacyQueuedFallbackDepth = fallbackDepth;
    legacyQueuedDepthFromGeometry = fromGeometry;
}

bool InitSystemMatrices() {
    legacyPacketAlphaState = {0, 0, 0xff7f7f7fU, 0};
    legacyBusyPreparingDraw = false;
    legacyQueuedDepthFromGeometry = 1;
    legacyQueuedFallbackDepth = 0.0f;
    legacyAlphaPassMatrix = {{
        {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
    const bool allocated = AllocateLegacyRenderQueue();
    legacyRenderQueueEnabled = true;
    return allocated;
}

bool GetLegacyRenderMask() {
    return perspectiveProjection - 1U <= 2U;
}

std::uint32_t GetLegacyPerspectiveProjection() {
    return perspectiveProjection;
}

// @verified - p
void SetProjectionOff() {
    perspectiveProjection = 0;
}

bool GetLegacyBusyPreparingDraw() {
    return legacyBusyPreparingDraw;
}

bool SetLegacyBusyPreparingDraw(bool busy) {
    const bool previous = legacyBusyPreparingDraw;
    legacyBusyPreparingDraw = busy;
    return previous;
}

void SetLegacyStencilReference(std::uint32_t reference) {
    SetEngineFlag(1, reference);
}
void SetLegacyDisableReflections(bool disabled) {
    shaderParameters.base.disableReflections = disabled;
}

// @verified - p
void SetLegacyMirroredDrawMode(std::uint32_t enabled) {
    legacyMirroredDrawMode = enabled;
    legacyNavigationRenderFlags = (legacyNavigationRenderFlags & 0x7fffffffU) | (enabled ? 0x80000000U : 0U);
}

// @verified - p
std::uint32_t SetLegacyNavigationRenderMode(std::int32_t mode) {
    legacyNavigationRenderFlags = (static_cast<std::uint32_t>(mode) << 16U) | (legacyNavigationRenderFlags & 0xfffcffffU);
    if (!legacyNavigationRenderDirty)
        legacyNavigationRenderDirty = 1;
    return legacyNavigationRenderDirty;
}

void SetLegacyColourWriteMask(std::uint32_t mask) {
    SetEngineFlag(0x43, mask);
}

void BeginLegacyCharacterShadowFrame() {
    SetEngineFlag(0x34, 1);
    SetEngineFlag(1, 0);
    SetEngineFlag(0x36, 7680);
    SetEngineFlag(0x35, 7680);
    const bool sun = GetPreviousActiveLight() == 1;
    SetEngineFlag(0x38, sun ? 514 : 513);
    SetEngineFlag(0x37, sun ? 7682 : 7681);
    legacyRenderQueueEnabled = false;
    if (legacyBusyPreparingDraw) {
        legacyBusyPreparingDraw = false;
        characterShadowSavedBusy = true;
    }
    characterShadowFrameActive = true;
}

bool SuspendLegacyCharacterShadowFrame() {
    if (!characterShadowFrameActive)
        return false;
    const auto &saved = savedProjectedShadowRenderSettings;
    SetEngineFlag(0x34, saved.stencilEnabled);
    SetEngineFlag(1, saved.stencilReference);
    SetEngineFlag(0x38, saved.stencilFunction);
    SetEngineFlag(0x36, saved.stencilDepthFail);
    SetEngineFlag(0x35, 7680);
    SetEngineFlag(0x37, saved.stencilPass);
    legacyRenderQueueEnabled = true;
    legacyBusyPreparingDraw = characterShadowSavedBusy;
    characterShadowSavedBusy = false;
    return true;
}

void ResumeLegacyCharacterShadowFrame(bool suspended) {
    if (suspended)
        BeginLegacyCharacterShadowFrame();
}

void EndLegacyCharacterShadowFrame() {
    const SavedProjectedShadowRenderSettings &saved = savedProjectedShadowRenderSettings;
    SetEngineFlag(0x34, saved.stencilEnabled);
    SetEngineFlag(1, saved.stencilReference);
    SetEngineFlag(0x38, saved.stencilFunction);
    SetEngineFlag(0x36, saved.stencilDepthFail);
    SetEngineFlag(0x35, 7680);
    SetEngineFlag(0x37, saved.stencilPass);
    legacyRenderQueueEnabled = true;
    legacyBusyPreparingDraw = characterShadowSavedBusy;
    characterShadowSavedBusy = false;
    characterShadowFrameActive = false;
}

void PerformCommonShaderPass(std::uint32_t colour) {
#if defined(_WIN32)
    auto *context = d3t::DX11::g_ppD3DDeviceContext;
    if (perspectiveProjection == 1 || !context || !context->deviceContext || !g_fvfPdTVertexShader || !g_fvfPdTPixelShader || !g_fvfLayouts[29])
        return;
    auto &state = d3t::GetRenderState(*context);
    const auto viewport = state.viewport;
    float width{}, height{};
    if (!d3t::GetSceneFrameTargetSize(&width, &height))
        return;
    const float inset = 1.0f - static_cast<float>(static_cast<std::int32_t>(viewport[2])) / width;
    struct Vertex {
        float x, y, z, w;
        std::uint32_t colour;
    };
    const std::array<Vertex, 4> vertices{{{inset - 1.0f, 1.0f, 0.0f, 1.0f, colour}, {inset - 1.0f, -1.0f, 0.0f, 1.0f, colour},
        {1.0f - inset, 1.0f, 0.0f, 1.0f, colour}, {1.0f - inset, -1.0f, 0.0f, 1.0f, colour}}};
    state.viewport = {0.0f, 0.0f, width, height, 1.0f, 1.0f};
    state.viewportDirtyMask = 1;
    SetEngineFlag(0x3b, 1);
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x5c, 0);
    SetEngineFlag(0x34, 1);
    SetEngineFlag(0x38, 517);
    SetEngineFlag(0x37, 0);
    SetEngineFlag(2, 31);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    if (state.vertexShader != g_fvfPdTVertexShader) {
        state.vertexShader = g_fvfPdTVertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != g_fvfPdTPixelShader) {
        state.pixelShader = g_fvfPdTPixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[29]) {
        state.inputLayout = g_fvfLayouts[29];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (void *mapped = d3t::GetMappedConstantBuffer(context, 0, sizeof(vertices)))
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    state.viewport = viewport;
    state.viewportDirtyMask = 1;
    const auto &saved = savedProjectedShadowRenderSettings;
    SetEngineFlag(0x3b, saved.blendEnabled);
    SetEngineFlag(0x34, saved.stencilEnabled);
    SetEngineFlag(0x37, saved.stencilPass);
    SetEngineFlag(0x38, saved.stencilFunction);
    SetEngineFlag(0x40, saved.depthWriteEnabled);
    SetEngineFlag(0x5c, legacySceneFogEnabled);
    SetEngineFlag(2, saved.stencilReadMask);
#else
    static_cast<void>(colour);
#endif
}

void InitializeLegacyDrawState() {
    shaderParameterStorage.fill(std::byte{});
    InitShenmueRendering();


    shaderParameters.base.fogColour = {1.0f, 1.0f, 1.0f};
    shaderParameters.base.alphaFunction = 4;
    shaderParameters.base.alphaOperation = 4;
    shaderParameters.base.alphaArgument0 = 1;
    shaderParameters.base.alphaArgument1 = 2;
    shaderParameters.base.specularEnabled = 1;
    shaderParameters.base.constants[0] = {0.5f, 0.5f, 0.5f, 0.0f};
    shaderParameters.base.constants[1] = {0.0f, 0.5f, 0.5f, 0.0f};
    shaderParameters.base.constants[2] = {0.5f, 0.5f, 0.0f, 0.0f};
    shaderParameters.constant0[0] = {1.0f, 0.0f, 0.0f, 0.0f};
    shaderParameters.constant0[1] = {0.0f, 1.0f, 0.0f, 0.0f};
    shaderParameters.constant0[2] = {0.0f, 0.0f, 1.0f, 0.0f};
    shaderParameters.constant1[0] = {};
    shaderParameters.constant1[1] = {0.5f, 0.0f, 0.5f, 0.0f};

    // Initialization calls SetMiscFlags(1, 1, 1, 1, 1) immediately after
    // InitSystemMatrices.  These are its exact writes into GShaderParams.c.
    shaderParameters.constants[0] = {0.0f, 0.0f, 1.0f, 0.5f};
    shaderParameters.constants[10] = {0.0f, 0.0f, -1.0f, 1.0f};
    shaderParameters.constants[12] = {1.0f, 1.0f, 1.0f, 1.0f};
    shaderParameters.constants[13] = {1.0f, 1.0f, 1.0f, 1.0f};

    temporaryModelLight = 0.30000001f;
    temporaryModelScale = 1.0f;
    temporaryModelPosition = {};
    lambertFvfReference = {};
    legacyAlphaMultiplier = 1.0f;
    legacyAlphaPassEnabled = false;
    perspectiveProjection = 0;
    legacySceneFogEnabled = 1;
    legacyModelFogEnabled = 1;
    legacyDepthFadeNear = 1.0f;
    legacyDepthFadeFar = 10000.0f;
    legacyDepthFadeIntensity = 1.0f;
    legacyMirroredDrawMode = 0;
    savedProjectedShadowRenderSettings = {};
    projectedShadowDirection = {};
    projectedShadowModelDirection = {};
    projectedShadowStencilPartition = 0;
    projectedShadowDebugEnabled = false;
    characterShadowFrameActive = false;
    characterShadowSavedBusy = false;

    smokeDraw = {};
    smokeProjection = {};
    fireQuadCount = 0;
    shaderParameters.constants[24] = {0.0f, 0.0f, 0.0f, 0.49803922f};
    currentVertexColourLayoutType = 1;
    legacyVertexColourStates.fill({});
    selectedLights.fill({});
    globalLightColour = {};
    particleLightColour = {1.0f, 1.0f, 1.0f};
    modelReferenceDirection = {1.0f, 0.0f, 0.0f};
    legacyVertexColourStateActive = false;
    currentVertexColourShader = g_originalVertexColour1Shader;



    legacyFvfLayoutAliases.fill(0);
    legacyFvfLayoutAliases[0] = g_fvfLayouts[0];
    legacyFvfLayoutAliases[1] = g_fvfLayouts[1];
    legacyFvfLayoutAliases[2] = g_fvfLayouts[2];
    legacyFvfLayoutAliases[4] = g_fvfLayouts[4];
    legacyFvfLayoutAliases[5] = g_fvfLayouts[5];
    legacyFvfLayoutAliases[6] = g_fvfLayouts[6];
    legacyFvfLayoutAliases[9] = g_fvfLayouts[0];
    legacyFvfLayoutAliases[10] = g_fvfLayouts[0];
    legacyFvfLayoutAliases[11] = g_fvfLayouts[0];
    legacyFvfLayoutAliases[12] = g_fvfLayouts[2];
    legacyFvfLayoutAliases[14] = g_fvfLayouts[16];
    legacyFvfLayoutAliases[15] = g_fvfLayouts[16];
    legacyFvfLayoutAliases[16] = g_fvfLayouts[6];
    legacyFvfLayoutAliases[19] = g_fvfLayouts[0];
    PrepareLegacyModelLambertState();
}

void InitializeLegacyPostProcessingParameters() {

    InitializePostFxShaderConstants(
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f});


    postFxSelectorParameters.fill(0);
    postFxSelectorParameters[0] = 5;
    postFxSelectorParameters[5] = std::bit_cast<std::uint32_t>(0.25f);



    postFxColourParameters.fill(0.0f);
    postFxColourParameters[0] = 1.0f;
    postFxColourParameters[5] = 0.29899999f;
    postFxColourParameters[6] = 0.58700001f;
    postFxColourParameters[7] = 0.114f;
    postFxColourParameters[8] = 1.0f;
    postFxColourParameters[12] = 1.0f;
    postFxColourParameters[13] = 0.5f;
    postFxColourParameters[14] = 0.5f;
    postFxColourParameters[15] = 0.5f;


    postFxBlurWeights.fill(0.5f);



    InitializePostFxShaderConstants(
        {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f});


    postFxCrossParameters.fill(0);
    postFxCrossParameters[1] = std::bit_cast<std::uint32_t>(0.30000001f);
    postFxCrossParameters[2] = std::bit_cast<std::uint32_t>(0.30000001f);
    postFxCrossParameters[3] = std::bit_cast<std::uint32_t>(0.30000001f);
    postFxCrossParameters[4] = 20;
    postFxCrossParameters[5] = 4;
    postFxCrossParameters[8] = std::bit_cast<std::uint32_t>(-1.0f);
    postFxCrossParameters[9] = std::bit_cast<std::uint32_t>(-1.0f);
    postFxCrossParameters[10] = std::bit_cast<std::uint32_t>(0.2f);
    postFxCrossParameters[22] = std::bit_cast<std::uint32_t>(-1.0f);
    postFxCrossParameters[23] = std::bit_cast<std::uint32_t>(1.0f);
    postFxCrossParameters[24] = std::bit_cast<std::uint32_t>(0.2f);
    postFxCrossParameters[36] = std::bit_cast<std::uint32_t>(1.0f);
    postFxCrossParameters[37] = std::bit_cast<std::uint32_t>(1.0f);
    postFxCrossParameters[38] = std::bit_cast<std::uint32_t>(0.2f);
    postFxCrossParameters[50] = std::bit_cast<std::uint32_t>(1.0f);
    postFxCrossParameters[51] = std::bit_cast<std::uint32_t>(-1.0f);
    postFxCrossParameters[52] = std::bit_cast<std::uint32_t>(0.2f);
    InitializePostFxModeDefaults();
}

void InitializeLegacyPostFxTransitionTexture() {
    RecreatePostFxRetainedTexture(CurrentPostProcessingRenderTarget());
}

void RenderLegacyPostFxTransition(float alpha) {
    if (GetLegacyPerspectiveProjection() == 1)
        return;

    const std::uint32_t sceneTexture = CurrentPostProcessingRenderTarget();
    if (!EnsurePostProcessingScratch(sceneTexture))
        return;
    if (!PostFxRetainedTextureMatches(sceneTexture) && !RecreatePostFxRetainedTexture(sceneTexture)) {
        return;
    }

    // RenderLegacyPostFxTransition preserves the current scene in scratch, restores the
    // separately retained texture to scene, then alpha scratch over scene.
    if (!RenderLegacyFullscreenQuad(postFxScratchTexture, sceneTexture, g_defaultPixelShader, nullptr)) {
        return;
    }
    if (!RenderLegacyFullscreenQuad(sceneTexture, postFxRetainedTexture, g_defaultPixelShader, nullptr)) {
        return;
    }
    RenderLegacyFullscreenQuadAlpha(sceneTexture, postFxScratchTexture, alpha);
}

void ReleaseLegacyPostProcessingTargets() {
    RetireLegacyFlagDebugCompatibilityResources();
    if (postFxRetainedTexture) {
        d3t::InitTexture2DResource(postFxRetainedTexture);
        postFxRetainedTexture = 0;
    }
    if (postFxScratchTexture) {
        d3t::InitTexture2DResource(postFxScratchTexture);
        postFxScratchTexture = 0;
    }
#if defined(_WIN32)
    if (smokeDraw.mapped && d3t::DX11::g_ppD3DDeviceContext) {
        d3t::DX11_ResetMappedResources(d3t::DX11::g_ppD3DDeviceContext);
        d3t::DX11_UnmapAndSyncShaderBindings(d3t::DX11::g_ppD3DDeviceContext);
    }
#endif
    smokeDraw = {};
    if (fireVertexBuffer)
        d3t::ValidateAndCommitBuffer(fireVertexBuffer);
    fireVertexBuffer = 0;
    fireQuadCount = 0;
}

void RenderPostProcessing(std::int32_t pass) {
    const PostProcessingState &postProcessing = GetPostProcessingState();
    if (pass < 0 || static_cast<std::size_t>(pass) >= postProcessing.enabled.size() || postProcessing.enabled[static_cast<std::size_t>(pass)] != 1) {
        return;
    }

    std::int32_t lastMode = 0;
    if (pass == 2)
        return;

    const std::uint32_t sceneTexture = CurrentPostProcessingRenderTarget();
    if (!EnsurePostProcessingScratch(sceneTexture))
        return;

    for (std::size_t slot = 0; slot < 2; ++slot) {
        const PostEffectShaderPacket &packet = postProcessing.packets[slot + 2U * static_cast<std::size_t>(pass)];
        const std::uint32_t outputTexture = slot != 0 ? sceneTexture : postFxScratchTexture;
        const std::uint32_t inputTexture = slot != 0 ? postFxScratchTexture : sceneTexture;
        switch (packet.kind) {
        case 2: {
            const float mode = std::abs(ReadPostEffectFloat(packet, 2)) * 1000.0f + ReadPostEffectFloat(packet, 0) + ReadPostEffectFloat(packet, 1) * 100.0f;
            lastMode = static_cast<std::int32_t>(mode);
            ApplyPostProcessingMode(lastMode);
            break;
        }
        case 3:



            break;
        case 4: {
            const float strength = ReadPostEffectFloat(packet, 0);
            postFxBlurWeights.fill(strength);
            RenderLegacyFullscreenQuad(outputTexture, inputTexture, g_blurPixelShader, &postFxBlurWeights[0]);
            break;
        }
        case 6: {


            const float mode = ReadPostEffectFloat(packet, 2) + ReadPostEffectFloat(packet, 0) + ReadPostEffectFloat(packet, 1);
            lastMode = static_cast<std::int32_t>(mode);
            ApplyPostProcessingMode(lastMode);
            break;
        }
        case 7:
            if (slot == 0 || postProcessing.enabled[0] != 1) {
                RenderLegacyFullscreenQuad(outputTexture, inputTexture, g_defaultPixelShader, nullptr);
            }
            break;
        default:

            break;
        }
    }

    if (pass == 1)
        ApplyPostProcessingMode(lastMode);
}

void SetLegacyVertexColourType(std::int32_t type) {
    if (type < 0 || type > 2)
        return;
    currentVertexColourLayoutType = type;
    // SetShaders initializes original_vcol0_vs_0 as an alias of vcol0.
    currentVertexColourShader = type == 1 ? g_originalVertexColour1Shader : g_originalVertexColour0Shader;
}

std::int32_t GetLegacyVertexColourType() {
    return currentVertexColourLayoutType;
}
bool GetLegacyVertexColourStateEnabled(std::uint32_t slot) {
    return slot < legacyVertexColourStates.size() && legacyVertexColourStates[slot].enabled != 0;
}

void SetLegacyVertexColourStateEnabled(std::uint32_t slot, bool enabled) {
    if (slot < legacyVertexColourStates.size()) {
        legacyVertexColourStates[slot].enabled = enabled;
    }
}

// @verified - p
void ExchangeLegacyModelColour(const Vector4 *colour, Vector4 *previous) {
    if (previous)
        *previous = legacyModelColour;
    legacyModelColour = *colour;
}

void SetLegacyGlobalLighting(const Vector4 &colour) {
    globalLightColour = {colour.y * 1.4f, colour.z * 1.4f, colour.w * 1.4f};
    temporaryModelLight = globalLightColour.x * 0.30000001f + globalLightColour.y * 0.58999997f + globalLightColour.z * 0.11f;
    const float components[]{temporaryModelLight, globalLightColour.x, globalLightColour.y, globalLightColour.z};
    std::uint32_t packed = 0;
    for (float component : components) {
        const float value = component * 255.0f;
        const std::int64_t integer =
            value >= -9223372036854775808.0f && value < 9223372036854775808.0f ? static_cast<std::int64_t>(value) : std::numeric_limits<std::int64_t>::min();
        packed = (packed << 8U) | static_cast<std::uint32_t>(integer);
    }
    SetEngineFlag(139, packed);
    packedGlobalLightColour = packed;
}
// @verified - p
Vector3 GetLegacyParticleLightColour() {
    return particleLightColour;
}
// @verified - p
Vector3 GetLegacyParticleLightDirection() {
    return modelReferenceDirection;
}
// @verified - p
float GetLegacyParticleLightIntensity() {
    return temporaryModelLight;
}

void SetLegacyLight(std::uint32_t slot, LightRuntimeEntry &source) {
    if (slot >= legacyVertexColourStates.size())
        return;
    selectedLights[slot] = source;
    LegacyVertexColourStateRecord &light = legacyVertexColourStates[slot];
    light.enabled = source.indexes[0];
    light.kind = source.indexes[1] & 0xf7U;
    if (source.vectors[0].y < 0.00001f)
        source.vectors[0].y = 0.0f;
    if (source.vectors[0].z < 0.00001f)
        source.vectors[0].z = 0.0f;
    if (source.vectors[1].x < 0.00001f)
        source.vectors[1].x = 0.0f;
    if (std::abs(source.vectors[1].y) < 0.0001f)
        source.vectors[1].y = 0.0f;
    const float intensity = source.vectors[1].y;
    light.colour = {source.vectors[0].x, intensity * source.vectors[0].y, intensity * source.vectors[0].z, intensity * source.vectors[1].x};
    light.range = source.vectors[1].z;
    light.inverseRange = 1.0f / light.range;
    light.rangeSquared = light.range * light.range;
    const float falloff = source.value1c * light.range;
    const float square = falloff * falloff;
    const float fourth = square * square;
    light.attenuation = {fourth * fourth, 0.0f, 0.0f};
    light.position = source.vector20;
    if (light.kind == 3) {
        Vector3 root;
        GetLightRootPosition(&root);
        light.position.x += root.x;
        light.position.y += root.y;
        light.position.z += root.z;
    }
    light.direction = {-source.values2c[0], -source.values2c[1], -source.value34};
    const float length = std::sqrt(light.direction.x * light.direction.x + light.direction.y * light.direction.y + light.direction.z * light.direction.z);
    if (length != 0.0f) {
        light.direction.x /= length;
        light.direction.y /= length;
        light.direction.z /= length;
    }
    const std::uint16_t angle = static_cast<std::uint16_t>(source.unknown02);
    light.coneCosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    light.coneSine = sin_fixed16(static_cast<std::int16_t>(angle));
    light.inverseConeWidth = light.coneCosine >= 1.0f ? 0.0f : 1.0f / (1.0f - light.coneCosine);
}

void SetLegacySelectedLightKind(std::uint32_t slot, std::int32_t kind) {
    if (slot >= selectedLights.size() || kind >= 5)
        return;
    selectedLights[slot].indexes[1] = static_cast<std::uint8_t>(kind);
    legacyVertexColourStates[slot].kind = static_cast<std::uint16_t>(kind);
}

// @verified - p
std::uint64_t GetLegacySelectedLightKind(std::uint32_t slot) {
    if (slot >= selectedLights.size())
        return 6;
    return static_cast<std::uint32_t>(static_cast<std::int8_t>(selectedLights[slot].indexes[1])) & 0xfffffff7U;
}

void SetLegacySelectedLightEnabled(std::uint32_t slot, bool enabled) {
    if (slot >= selectedLights.size())
        return;
    selectedLights[slot].indexes[0] = enabled ? 1 : 0;
    legacyVertexColourStates[slot].enabled = enabled ? 1 : 0;
}

void SetLegacySelectedLightDirection(std::uint32_t slot, const Vector3 &direction) {
    if (slot >= selectedLights.size())
        return;
    selectedLights[slot].values2c = {direction.x, direction.y};
    selectedLights[slot].value34 = direction.z;
    Vector3 &transformed = legacyVertexColourStates[slot].direction;
    transformed = {-direction.x, -direction.y, -direction.z};
    const float length = std::sqrt(transformed.x * transformed.x + transformed.y * transformed.y + transformed.z * transformed.z);
    if (length != 0.0f) {
        const float reciprocal = 1.0f / length;
        transformed.x *= reciprocal;
        transformed.y *= reciprocal;
        transformed.z *= reciprocal;
    }
}

void SetLegacySelectedLightColour(std::uint32_t slot, const Vector4 &colour) {
    if (slot >= selectedLights.size())
        return;
    selectedLights[slot].vectors[0] = {colour.x, colour.y, colour.z};
    selectedLights[slot].vectors[1].x = colour.w;
}

// @verified - p
std::uint32_t GetAvailableSelectedLightSlot() {
    const std::int32_t mode = GetCurrentActiveLight();
    if (mode == 1)
        return selectedLights[0].indexes[0] == 0 ? 0 : 17;
    if (mode != 2 && mode != 3)
        return 17;
    for (std::uint32_t slot = 0; slot < selectedLights.size(); ++slot)
        if (selectedLights[slot].indexes[0] == 0)
            return slot;
    return 17;
}

// @verified - p
void SetSelectedLightIntensity(std::uint32_t slot, float intensity) {
    if (std::abs(intensity) < 0.0001f)
        intensity = 0.0f;
    if (slot < selectedLights.size())
        selectedLights[slot].vectors[1].y = intensity;
}

// @verified - p
void shenSetLightPosition(std::uint32_t slot, const Vector3 *position) {
    if (slot >= selectedLights.size())
        return;
    LightRuntimeEntry &light = selectedLights[slot];
    if ((light.indexes[1] & 0xf7U) == 3) {
        Vector3 root;
        GetLightRootPosition(&root);
        light.vector20 = {.x = position->x - root.x, .y = position->y - root.y, .z = position->z - root.z};
    } else {
        light.vector20 = *position;
    }
    legacyVertexColourStates[slot].position = *position;
}

// @verified - p
void shenSetLightScale2(std::uint32_t slot, float range) {
    if (slot >= selectedLights.size())
        return;
    range = std::fmax(0.01f, range);
    LightRuntimeEntry &light = selectedLights[slot];
    LegacyVertexColourStateRecord &backend = legacyVertexColourStates[slot];
    light.vectors[1].z = range;
    backend.range = range;
    backend.inverseRange = 1.0f / range;
    backend.rangeSquared = range * range;
    const float falloff = range * light.value1c;
    const float square = falloff * falloff;
    backend.attenuation = {.x = (square * square) * (square * square), .y = 0.0f, .z = 0.0f};
}

// @verified - p
void shenSetLightScale(std::uint32_t slot, float falloff) {
    if (slot >= selectedLights.size())
        return;
    LightRuntimeEntry &light = selectedLights[slot];
    light.value1c = std::fmax(0.01f, falloff);
    const float radius = light.value1c * light.vectors[1].z;
    const float square = radius * radius;
    legacyVertexColourStates[slot].attenuation = {.x = (square * square) * (square * square), .y = 0.0f, .z = 0.0f};
}

void ResetLegacySelectedLight(std::uint32_t slot, std::int32_t activeMode, const LightRuntimeEntry &defaults) {
    if (slot >= selectedLights.size())
        return;
    LightRuntimeEntry &selected = selectedLights[slot];
    LegacyVertexColourStateRecord &backend = legacyVertexColourStates[slot];
    SetLegacySelectedLightEnabled(slot, slot == 0 && activeMode == 1);
    SetLegacySelectedLightKind(slot, slot == 0 && activeMode == 1 ? 2 : 0);
    SetLegacySelectedLightColour(slot, {defaults.vectors[0].x, defaults.vectors[0].y, defaults.vectors[0].z, defaults.vectors[1].x});
    selected.vectors[1].y = 1.0f;
    selected.vectors[1].z = 2.0f;
    backend.range = 2.0f;
    backend.inverseRange = 0.5f;
    backend.rangeSquared = 4.0f;
    selected.value1c = 0.60000002f;
    const float falloff = selected.vectors[1].z * selected.value1c;
    const float square = falloff * falloff;
    const float fourth = square * square;
    backend.attenuation = {fourth * fourth, 0.0f, 0.0f};
    selected.vector20 = defaults.vector20;
    backend.position = defaults.vector20;
    SetLegacySelectedLightDirection(slot, {defaults.values2c[0], defaults.values2c[1], defaults.value34});
    selected.unknown02 = 1820;
    backend.coneCosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(1820)));
    backend.coneSine = sin_fixed16(1820);
    backend.inverseConeWidth = backend.coneCosine >= 1.0f ? 0.0f : 1.0f / (1.0f - backend.coneCosine);
}

Vector3 GetLegacyLightDirection(std::uint32_t slot) {
    if (slot >= selectedLights.size())
        return {};
    const LightRuntimeEntry &light = selectedLights[slot];
    return {light.values2c[0], light.values2c[1], light.value34};
}

bool AccumulateProjectedModelMinimumHeight(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, float &minimumHeight) {
    if (!Contains(modelBytes, xb01Offset, 32))
        return false;
    std::uint64_t command = static_cast<std::uint64_t>(xb01Offset) + 28ULL + 4ULL * ReadValue<std::uint32_t>(modelBytes, xb01Offset + 28);
    while (Contains(modelBytes, command, 4)) {
        const std::uint32_t header = ReadValue<std::uint32_t>(modelBytes, command);
        if (header == 0)
            return true;
        const std::uint64_t commandSize = 4ULL * (header >> 8U);
        if (commandSize < 4 || !Contains(modelBytes, command, commandSize)) {
            return false;
        }
        if ((header & 0xffU) == 1) {
            if (commandSize < 8)
                return false;
            const std::int32_t count = ReadValue<std::int32_t>(modelBytes, command + 4);
            std::uint64_t buffer = command + 8;
            for (std::int32_t index = 0; index < count; ++index) {
                if (buffer > command + commandSize || command + commandSize - buffer < 8)
                    return false;
                const std::uint16_t stride = ReadValue<std::uint16_t>(modelBytes, buffer);
                const std::uint16_t flags = ReadValue<std::uint16_t>(modelBytes, buffer + 2);
                const std::uint32_t size = ReadValue<std::uint32_t>(modelBytes, buffer + 4);
                const std::uint64_t vertices = buffer + 8;
                if (size > command + commandSize - vertices)
                    return false;
                if ((flags & 2U) != 0) {
                    if (stride == 0)
                        return false;
                    const auto vertexCount = static_cast<std::int32_t>(size / stride);
                    std::uint64_t vertex = vertices;
                    for (std::int32_t item = 0; item < vertexCount; ++item) {
                        if (!Contains(modelBytes, vertex, sizeof(Vector3))) {
                            return false;
                        }
                        const Vector3 position = ReadValue<Vector3>(modelBytes, vertex);
                        const float y = ((workTransformMatrix.rows[1].y * position.y + workTransformMatrix.rows[0].y * position.x) +
                                            workTransformMatrix.rows[2].y * position.z) +
                            workTransformMatrix.rows[3].y;
                        if (minimumHeight > y)
                            minimumHeight = y;
                        // AccumulateProjectedModelMinimumHeight advances by stride/4 DWORDs.
                        vertex += stride >> 2U;
                    }
                }
                buffer = vertices + size;
            }
        }
        command += commandSize;
    }
    return false;
}

void UpdateLegacyLightTransforms() {
    for (LegacyVertexColourStateRecord &light : legacyVertexColourStates) {
        if (!light.enabled)
            continue;
        light.transformedDirection = TransformLightVector(light.direction);
        light.transformedPosition = TransformLightPosition(light.position);
    }
    LegacyVertexColourStateRecord &primary = legacyVertexColourStates[0];
    shaderParameters.constants[13] = {0.0f, 0.0f, 0.0f, 1.0f};
    if (primary.enabled == 1) {
        const LightRuntimeEntry &source = selectedLights[0];
        primary.colour = {source.vectors[0].x, source.vectors[0].y, source.vectors[0].z, source.vectors[1].x};
        primary.transformedDirection.x *= source.vectors[1].y;
        primary.transformedDirection.y *= source.vectors[1].y;
        primary.transformedDirection.z *= source.vectors[1].y;
        modelReferenceDirection = primary.transformedDirection;
        for (std::size_t slot = 1; slot < legacyVertexColourStates.size(); ++slot) {
            legacyVertexColourStates[slot].colour.y = 0.0f;
            legacyVertexColourStates[slot].colour.z = 0.0f;
            legacyVertexColourStates[slot].colour.w = 0.0f;
        }
        if (legacyVertexColourStates[15].enabled == 1) {
            const LightRuntimeEntry &source15 = selectedLights[15];
            legacyVertexColourStates[15].colour = {source15.vectors[0].x, source15.vectors[0].y * source15.vectors[1].y,
                source15.vectors[0].z * source15.vectors[1].y, source15.vectors[1].x * source15.vectors[1].y};
        }
    } else {
        primary.colour = {1.0f, 1.0f, 1.0f, 1.0f};
        primary.transformedDirection = {};
        modelReferenceDirection = {};

        Vector4 value = primary.colour;
        for (std::size_t slot = 1; slot < legacyVertexColourStates.size(); ++slot) {
            LegacyVertexColourStateRecord &light = legacyVertexColourStates[slot];
            if (light.enabled == 1) {
                const LightRuntimeEntry &source = selectedLights[slot];
                const float intensity = source.vectors[1].y;
                value = {source.vectors[0].x, source.vectors[0].y * intensity, source.vectors[0].z * intensity, source.vectors[1].x * intensity};
            } else {
                value.y = 0.0f;
                value.z = 0.0f;
                value.w = 0.0f;
            }
            light.colour = value;
        }
    }
    modelLightVector = {primary.colour.y, primary.colour.z, primary.colour.w};
    particleLightColour = {primary.colour.y, primary.colour.z, primary.colour.w};
    shaderParameters.constants[12] = {primary.colour.y, primary.colour.z, primary.colour.w, 1.0f};
}

LegacyModelShaderPath GetLegacyCharacterShaderPath() {
    return legacyVertexColourStates[0].enabled == 1 && legacyVertexColourStates[15].enabled != 1 ? LegacyModelShaderPath::Lambert :
                                                                                                   LegacyModelShaderPath::VertexColour;
}

void SetLegacyLambertFvfReference(const Vector3 *direction) {
    if (!direction) {
        lambertFvfReference.x = 0.0f;
        return;
    }



    const float inputX = -direction->x;
    const float inputY = -direction->y;
    const float inputZ = -direction->z;
    const float transformedX = workTransformMatrix.rows[0].x * inputX + workTransformMatrix.rows[0].y * inputY + workTransformMatrix.rows[0].z * inputZ;
    const float transformedY = workTransformMatrix.rows[1].x * inputX + workTransformMatrix.rows[1].y * inputY + workTransformMatrix.rows[1].z * inputZ;
    const float transformedZ = workTransformMatrix.rows[2].x * inputX + workTransformMatrix.rows[2].y * inputY + workTransformMatrix.rows[2].z * inputZ;
    const std::uint32_t marker = std::bit_cast<std::uint32_t>(transformedX) | 1U;
    lambertFvfReference.x = std::bit_cast<float>(marker);
    lambertFvfReference.y = transformedY;
    lambertFvfReference.z = transformedZ;
    lambertFvfReference.w = 0.0f;
}

bool InspectLegacyDrawCommandStream(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, LegacyDrawCommandStreamStats &stats) {
    stats = {};
    std::uint32_t signature{};
    std::uint32_t firstPacketOffset{};
    if (!ReadU32(modelBytes, xb01Offset, signature) || signature != xb01Signature || !ReadU32(modelBytes, xb01Offset + 24, firstPacketOffset)) {
        return false;
    }

    std::size_t packet = static_cast<std::size_t>(xb01Offset) + static_cast<std::size_t>(firstPacketOffset + 6U) * 4U;
    for (std::size_t packetGuard = 0; packetGuard < 65536; ++packetGuard) {
        std::uint32_t packetHeader{};
        if (!ReadU32(modelBytes, packet, packetHeader)) {
            return false;
        }
        if (!packetHeader) {
            return true;
        }
        const std::uint32_t packetDwords = packetHeader >> 8U;
        if (!packetDwords || packetDwords > (modelBytes.size() - packet) / 4U) {
            return false;
        }
        const std::size_t packetEnd = packet + static_cast<std::size_t>(packetDwords) * 4U;
        std::size_t command = packet + 20U;
        ++stats.packetCount;
        while (command < packetEnd) {
            std::uint32_t commandHeader{};
            if (!ReadU32(modelBytes, command, commandHeader)) {
                return false;
            }
            if (!(commandHeader & 0xffU)) {
                break;
            }
            const std::uint8_t opcode = static_cast<std::uint8_t>(commandHeader);
            const std::uint32_t commandDwords = commandHeader >> 8U;
            if (!commandDwords || commandDwords > (packetEnd - command) / 4U) {
                return false;
            }
            ++stats.commandCount;
            if (!IsNativeOpcode(opcode)) {
                ++stats.unsupportedCommandCount;
            }
            if (opcode == static_cast<std::uint8_t>(LegacyDrawCommandOpcode::DrawIndexed)) {
                ++stats.drawCount;
            } else if (opcode == static_cast<std::uint8_t>(LegacyDrawCommandOpcode::BindTextures)) {
                ++stats.textureBindCount;
            } else if (opcode == static_cast<std::uint8_t>(LegacyDrawCommandOpcode::BindVertexStream) ||
                opcode == static_cast<std::uint8_t>(LegacyDrawCommandOpcode::BindVertexStreams)) {
                ++stats.vertexStreamBindCount;
            }
            command += static_cast<std::size_t>(commandDwords) * 4U;
        }
        packet = packetEnd;
    }
    return false;
}

bool PrepareLegacyDrawResources(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, LegacyDrawResources &resources) {
    ReleaseLegacyDrawResources(resources);
#if defined(_WIN32)


    if (!d3t::DX11::g_ppD3DDeviceContext)
        d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
#endif

    std::uint32_t signature{};
    std::uint32_t firstPacketOffset{};
    std::uint32_t commandSizeWords{};
    if (!ReadU32(modelBytes, xb01Offset, signature) || signature != xb01Signature || !ReadU32(modelBytes, xb01Offset + 24, firstPacketOffset) ||
        !ReadU32(modelBytes, xb01Offset + 28, commandSizeWords)) {
        return false;
    }

    // PrepareLegacyDrawResources starts at xb01 + 28 + commandSizeWords * 4.
    std::size_t resourceCommand = static_cast<std::size_t>(xb01Offset) + 28U + static_cast<std::size_t>(commandSizeWords) * 4U;
    for (std::size_t guard = 0; guard < 1024; ++guard) {
        std::uint32_t header{};
        if (!ReadU32(modelBytes, resourceCommand, header))
            return false;
        if (!header)
            break;
        const std::uint8_t opcode = static_cast<std::uint8_t>(header);
        const std::uint32_t lengthDwords = header >> 8U;
        if (!lengthDwords || lengthDwords > (modelBytes.size() - resourceCommand) / 4U) {
            return false;
        }
        if (opcode == 1) {
            std::uint32_t count{};
            if (!ReadU32(modelBytes, resourceCommand + 4, count))
                return false;
            std::size_t record = resourceCommand + 8;
            for (std::uint32_t index = 0; index < count; ++index) {
                std::uint32_t metadata{};
                std::uint32_t byteSize{};
                if (!ReadU32(modelBytes, record, metadata) || !ReadU32(modelBytes, record + 4, byteSize) || !Contains(modelBytes, record + 8, byteSize)) {
                    ReleaseLegacyDrawResources(resources);
                    return false;
                }
                std::int32_t handle{};
                if (!d3t::CreateBuffer_Wrapper(static_cast<std::int32_t>(byteSize), &handle, modelBytes.data() + record + 8)) {
                    ReleaseLegacyDrawResources(resources);
                    return false;
                }
                resources.vertexBuffers.push_back({metadata, handle});
                record += 8U + byteSize;
            }
        }
        resourceCommand += static_cast<std::size_t>(lengthDwords) * 4U;
    }

    std::vector<std::uint16_t> indices;
    std::size_t packet = static_cast<std::size_t>(xb01Offset) + static_cast<std::size_t>(firstPacketOffset + 6U) * 4U;
    for (std::size_t packetGuard = 0; packetGuard < 65536; ++packetGuard) {
        std::uint32_t packetHeader{};
        if (!ReadU32(modelBytes, packet, packetHeader))
            goto fail;
        if (!packetHeader)
            break;
        const std::uint32_t packetDwords = packetHeader >> 8U;
        if (!packetDwords || packetDwords > (modelBytes.size() - packet) / 4U)
            goto fail;
        const std::size_t packetEnd = packet + std::size_t(packetDwords) * 4U;
        for (std::size_t command = packet + 20U; command < packetEnd;) {
            std::uint32_t commandHeader{};
            if (!ReadU32(modelBytes, command, commandHeader))
                goto fail;
            if (!(commandHeader & 0xffU))
                break;
            const std::uint8_t opcode = static_cast<std::uint8_t>(commandHeader);
            const std::uint32_t lengthDwords = commandHeader >> 8U;
            if (!lengthDwords || lengthDwords > (packetEnd - command) / 4U)
                goto fail;
            if (opcode == 16 || opcode == 18) {
                std::uint32_t primitive{};
                std::uint32_t count{};
                if (!ReadU32(modelBytes, command + 4, primitive) || !ReadU32(modelBytes, command + 8, count) ||
                    !Contains(modelBytes, command + 12, std::size_t(count) * sizeof(std::uint16_t))) {
                    goto fail;
                }
                const std::uint32_t start = static_cast<std::uint32_t>(indices.size());
                const auto *first = reinterpret_cast<const std::uint16_t *>(modelBytes.data() + command + 12);
                indices.insert(indices.end(), first, first + count);
                resources.indexDraws.push_back({static_cast<std::uint32_t>(command - xb01Offset), primitive, count, start});
            } else if (opcode == 11) {
                std::uint32_t count{};
                if (!ReadU32(modelBytes, command + 4, count) || !Contains(modelBytes, command + 8, std::size_t(count) * 8U)) {
                    goto fail;
                }
                for (std::uint32_t index = 0; index < count; ++index) {







                    const std::int16_t textureIndex = ReadValue<std::int16_t>(modelBytes, command + 8U + index * 8U);
                    resources.textureBindings.push_back({static_cast<std::uint32_t>(command - xb01Offset), index, static_cast<std::int32_t>(textureIndex), 0});
                }
            }
            command += std::size_t(lengthDwords) * 4U;
        }
        packet = packetEnd;
    }

    if (!indices.empty()) {
        if (!d3t::CreateSceneIndexBuffer(static_cast<std::int32_t>(indices.size()), &resources.indexBuffer) || !resources.indexBuffer)
            goto fail;
        void *const destination = d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer, 1, 0);
        if (!destination)
            goto fail;
        std::memcpy(destination, indices.data(), indices.size() * sizeof(std::uint16_t));
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer);
    }
    resources.prepared = true;
    return true;

fail:
    ReleaseLegacyDrawResources(resources);
    return false;
}

bool RefreshLegacyDrawResources(
    std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &source, LegacyDrawResources &resources) {
    if (!source.prepared || !resources.prepared || source.vertexBuffers.size() != resources.vertexBuffers.size())
        return false;
    std::uint32_t signature{};
    if (!ReadU32(modelBytes, xb01Offset, signature) || signature != xb01Signature)
        return false;
    std::uint32_t indexCount = 0;
    for (const LegacyIndexDraw &draw : source.indexDraws) {
        if (!Contains(
                modelBytes, static_cast<std::size_t>(xb01Offset) + draw.commandOffset + 12, static_cast<std::size_t>(draw.indexCount) * sizeof(std::uint16_t)))
            return false;
        indexCount += draw.indexCount;
    }
    const std::size_t byteSize = static_cast<std::size_t>(indexCount) * sizeof(std::uint16_t);
    if (byteSize > static_cast<std::size_t>((std::numeric_limits<std::int32_t>::max)()))
        return false;
    if (resources.indexBuffer && d3t::GetSizeForModelBuffer(resources.indexBuffer) != static_cast<std::int32_t>(byteSize)) {
        d3t::ReleaseSceneIndexBuffer(resources.indexBuffer);
        resources.indexBuffer = 0;
    }
    if (byteSize && !resources.indexBuffer) {
        if (!d3t::CreateSceneIndexBuffer(static_cast<std::int32_t>(indexCount), &resources.indexBuffer) || !resources.indexBuffer)
            return false;
    }
    if (byteSize) {
        std::byte *const destination = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer, 1, 0));
        if (!destination)
            return false;
        std::size_t offset = 0;
        for (const LegacyIndexDraw &draw : source.indexDraws) {
            const std::size_t size = static_cast<std::size_t>(draw.indexCount) * sizeof(std::uint16_t);
            std::memcpy(destination + offset, modelBytes.data() + xb01Offset + draw.commandOffset + 12, size);
            offset += size;
        }
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, resources.indexBuffer);
    }
    resources.indexDraws = source.indexDraws;
    resources.textureBindings = source.textureBindings;
    for (std::size_t i = 0; i < resources.vertexBuffers.size(); ++i) {
        resources.vertexBuffers[i].metadata = source.vertexBuffers[i].metadata;
    }
    return true;
}

bool UploadLegacyVertexBuffer(LegacyDrawResources &resources, std::uint32_t index, std::span<const std::byte> bytes) {
    if (index >= resources.vertexBuffers.size())
        return false;
    const std::int32_t handle = resources.vertexBuffers[index].handle;
    if (d3t::GetSizeForModelBuffer(handle) < 0 || bytes.size() > static_cast<std::size_t>(d3t::GetSizeForModelBuffer(handle)))
        return false;
    void *const destination = d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, handle, 2, 0);
    if (!destination)
        return false;
    std::memcpy(destination, bytes.data(), bytes.size());
    d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, handle);
    return true;
}

namespace {
// @verified - p
bool MutateLegacyCpuWaveStrips(
    std::span<const std::byte> bytes, std::uint32_t meshOffset, std::span<const LegacyWaveVertex> vertices, float scrollU, float scrollV) {
    std::uint32_t flags;
    if (!ReadU32(bytes, meshOffset + 4, flags))
        return false;
    if (flags & 0x200U)
        scrollU += flags & 0x100U ? 1.0f : -1.0f;
    if (flags & 0x2000U)
        scrollV += flags & 0x1000U ? 1.0f : -1.0f;
    flags &= ~0xff00U;
    std::byte *const source = const_cast<std::byte *>(bytes.data());
    std::memcpy(source + meshOffset + 4, &flags, 4);
    std::uint32_t uRegions = 0, vRegions = 0;
    const auto updateUV = [&](std::size_t offset) {
        float u, v;
        std::memcpy(&u, source + offset + 24, 4);
        std::memcpy(&v, source + offset + 28, 4);
        u += scrollU;
        v += scrollV;
        uRegions |= u >= 0.0f ? (u >= 1.0f ? 1U : 0x10U) : 0x100U;
        vRegions |= v >= 0.0f ? (v >= 1.0f ? 1U : 0x10U) : 0x100U;
        std::memcpy(source + offset + 24, &u, 4);
        const std::uint32_t vBits = std::bit_cast<std::uint32_t>(v) | 1U;
        std::memcpy(source + offset + 28, &vBits, 4);
    };
    if (!vertices.empty()) {
        std::uint32_t state;
        if (!ReadU32(bytes, meshOffset + 32, state))
            return false;
        state &= 0xfffe7fffU;
        std::memcpy(source + meshOffset + 32, &state, 4);
        std::size_t cursor = static_cast<std::size_t>(meshOffset) + 112;
        bool valid = true;
        const auto writeVertex = [&](std::size_t gridIndex, std::size_t stride) {
            if (!Contains(bytes, cursor, 32)) {
                valid = false;
                return;
            }
            const LegacyWaveVertex &vertex = vertices[gridIndex];
            std::memcpy(source + cursor + 4, &vertex.height, 4);
            std::memcpy(source + cursor + 12, &vertex.normal, 4);
            std::memcpy(source + cursor + 16, &vertex.diffuse, 4);
            std::memcpy(source + cursor + 20, &vertex.specular, 4);
            updateUV(cursor);
            cursor += stride;
        };
        for (std::int32_t column = 16; column >= 0; --column) {
            writeVertex(7 * 17 + column, 32);
            writeVertex(8 * 17 + column, 32);
        }
        for (std::int32_t row = 1; row <= 6; ++row) {
            cursor += 8;
            for (std::int32_t column = 0; column <= 16; ++column)
                writeVertex((8 + row) * 17 + column, 40);
            cursor += 8;
            for (std::int32_t column = 16; column >= 0; --column)
                writeVertex((7 - row) * 17 + column, 40);
        }
        for (const std::int32_t row : {15, 16, 0}) {
            cursor += row == 0 ? 16 : 8;
            for (std::int32_t column = 0; column <= 16; ++column)
                writeVertex(row * 17 + column, 40);
        }
        if (!valid)
            return false;
    } else {
        std::size_t cursor = static_cast<std::size_t>(meshOffset) + 24;
        for (;;) {
            std::uint32_t header, count;
            if (!ReadU32(bytes, cursor, header))
                return false;
            if (!header)
                break;
            if (static_cast<std::int32_t>(header) < 0) {
                std::uint32_t state;
                if (!ReadU32(bytes, cursor + 8, state))
                    return false;
                state &= 0xfffe7fffU;
                std::memcpy(source + cursor + 8, &state, 4);
                cursor += 80;
                if (!ReadU32(bytes, cursor, header))
                    return false;
            }
            if (!ReadU32(bytes, cursor + 4, count))
                return false;
            if (header & 8U)
                count *= 3U;
            cursor += 8;
            while (count--) {
                std::uint32_t vertex;
                if (!ReadU32(bytes, cursor, vertex))
                    return false;
                if (vertex & 1U) {
                    if (!Contains(bytes, cursor, 32))
                        return false;
                    updateUV(cursor);
                    cursor += 32;
                } else
                    cursor += 8;
            }
        }
    }
    if (uRegions == 0x100U)
        flags |= 0x300U;
    else if (uRegions == 1U)
        flags |= 0x200U;
    if (vRegions == 0x100U)
        flags |= 0x3000U;
    else if (vRegions == 1U)
        flags |= 0x2000U;
    std::memcpy(source + meshOffset + 4, &flags, 4);
    return true;
}

bool MutateLegacyWaveBuffers(std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources,
    std::span<const LegacyWaveVertex> vertices, float scrollU, float scrollV, std::uint32_t diffuse, std::uint32_t specular, bool uvOnly = false) {
    std::uint32_t signature{}, firstPacket{}, vertexOffset{}, flags{};
    if (!ReadU32(bytes, xb01Offset, signature))
        return false;
    if (signature != xb01Signature)
        return MutateLegacyCpuWaveStrips(bytes, xb01Offset, vertices, scrollU, scrollV);
    if (!resources.prepared || !ReadU32(bytes, xb01Offset + 24, firstPacket) || !ReadU32(bytes, xb01Offset + 28, vertexOffset) ||
        !ReadU32(bytes, xb01Offset + 4, flags))
        return false;
    std::byte *const source = const_cast<std::byte *>(bytes.data());
    const bool geometry = !vertices.empty();
    if (geometry || uvOnly) {
        if (flags & 0x200U)
            scrollU += flags & 0x100U ? 1.0f : -1.0f;
        if (flags & 0x2000U)
            scrollV += flags & 0x1000U ? 1.0f : -1.0f;
        flags &= ~0xff00U;
        std::memcpy(source + xb01Offset + 4, &flags, 4);
    }
    std::uint32_t uRegions = 0, vRegions = 0;
    const auto region = [](float value) { return value >= 0.0f ? (value >= 1.0f ? 1U : 0x10U) : 0x100U; };
    std::size_t packet = std::size_t(xb01Offset) + (std::size_t(firstPacket) + 6U) * 4U;
    std::uint32_t bufferIndex = 0;
    bool selectedBuffer = uvOnly;
    for (;;) {
        std::uint32_t packetHeader{};
        if (!ReadU32(bytes, packet, packetHeader))
            return false;
        if (!packetHeader)
            break;
        const std::size_t packetSize = std::size_t(packetHeader >> 8U) * 4U;
        if (packetSize < 24 || !Contains(bytes, packet, packetSize))
            return false;
        const std::size_t end = packet + packetSize;
        std::size_t command = packet + 20;
        for (;;) {
            std::uint32_t header{};
            if (command >= end || !ReadU32(bytes, command, header))
                return false;
            if (!(header & 0xffU))
                break;
            const std::size_t size = std::size_t(header >> 8U) * 4U;
            if (size < 4 || size > end - command)
                return false;
            if ((header & 0xffU) == 1) {
                if (size < 12 || !ReadU32(bytes, command + 8, bufferIndex))
                    return false;
                bufferIndex &= 0xffU;
                selectedBuffer = true;
            } else if ((header & 0xffU) == 13) {
                std::uint32_t count{};
                if (size < 8 || !ReadU32(bytes, command + 4, count) || std::size_t(count) > (size - 8) / 4)
                    return false;
                for (std::uint32_t slot = 0; slot < count; ++slot) {
                    std::byte *const binding = source + command + 8 + 4U * slot;
                    if ((binding[1] == std::byte{0} || binding[1] == std::byte{1}) && binding[2] == std::byte{3})
                        binding[2] = std::byte{1};
                }
            }
            command += size;
        }
        if (!selectedBuffer || bufferIndex >= resources.vertexBuffers.size())
            return false;
        const std::size_t streamHeader = std::size_t(xb01Offset) + (std::size_t(vertexOffset) + 9U) * 4U + 12U * bufferIndex;
        std::uint32_t byteCount{};
        if (!ReadU32(bytes, streamHeader + 4, byteCount))
            return false;
        const std::size_t streamOffset = streamHeader + 8;
        const std::size_t copiedBytes = byteCount & ~31U;
        if (!Contains(bytes, streamOffset, copiedBytes) || (geometry && copiedBytes < 289U * 32U))
            return false;
        const std::int32_t handle = resources.vertexBuffers[bufferIndex].handle;
        const std::int32_t size = d3t::GetSizeForModelBuffer(handle);
        if (size < 0 || copiedBytes > static_cast<std::size_t>(size))
            return false;
        if (uvOnly && copiedBytes == 0) {
            packet = end;
            continue;
        }
        std::byte *const mapped = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, handle, 1, 0));
        if (!mapped)
            return false;
        std::byte *const data = source + streamOffset;
        std::size_t cursor = 0;
        const auto updateUV = [&](std::size_t offset) {
            float u{}, v{};
            std::memcpy(&u, data + offset + 24, 4);
            std::memcpy(&v, data + offset + 28, 4);
            u += scrollU;
            v += scrollV;
            uRegions |= region(u);
            vRegions |= region(v);
            std::memcpy(data + offset + 24, &u, 4);
            const std::uint32_t vBits = std::bit_cast<std::uint32_t>(v) | 1U;
            std::memcpy(data + offset + 28, &vBits, 4);
        };
        const auto writeVertex = [&](std::size_t gridIndex) {
            const LegacyWaveVertex &vertex = vertices[gridIndex];
            std::memcpy(data + cursor + 4, &vertex.height, 4);
            std::memcpy(data + cursor + 12, &vertex.normal, 4);
            std::memcpy(data + cursor + 16, &vertex.diffuse, 4);
            std::memcpy(data + cursor + 20, &vertex.specular, 4);
            updateUV(cursor);
            cursor += 32;
        };
        if (geometry) {
            for (int column = 16; column >= 0; --column) {
                writeVertex(7 * 17 + column);
                writeVertex(8 * 17 + column);
            }
            for (int row = 1; row <= 6; ++row) {
                for (int column = 0; column <= 16; ++column)
                    writeVertex((8 + row) * 17 + column);
                for (int column = 16; column >= 0; --column)
                    writeVertex((7 - row) * 17 + column);
            }
            for (const int row : {15, 16, 0})
                for (int column = 0; column <= 16; ++column)
                    writeVertex(row * 17 + column);
            std::memcpy(mapped, data, copiedBytes);
        } else if (uvOnly) {
            for (; cursor < copiedBytes; cursor += 32) {
                updateUV(cursor);
                std::memcpy(mapped + cursor + 24, data + cursor + 24, 8);
            }
        } else {
            for (; cursor < copiedBytes; cursor += 32) {
                std::memcpy(data + cursor + 16, &diffuse, 4);
                std::memcpy(data + cursor + 20, &specular, 4);
                std::memcpy(mapped + cursor + 16, data + cursor + 16, 8);
            }
        }
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, handle);
        resources.waveVertexColours = true;
        packet = end;
    }
    if (geometry || uvOnly) {
        if (uRegions == 0x100U)
            flags |= 0x300U;
        else if (uRegions == 1U)
            flags |= 0x200U;
        if (vRegions == 0x100U)
            flags |= 0x3000U;
        else if (vRegions == 1U)
            flags |= 0x2000U;
        std::memcpy(source + xb01Offset + 4, &flags, 4);
    }
    return true;
}
} // namespace

bool UpdateLegacyWaveGeometry(std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources,
    std::span<const LegacyWaveVertex, 289> vertices, float scrollU, float scrollV) {
    return MutateLegacyWaveBuffers(bytes, xb01Offset, resources, vertices, scrollU, scrollV, 0, 0);
}

// @verified - p
bool ReadLegacyWaveGridPositions(
    std::span<const std::byte> bytes, std::uint32_t meshOffset, LegacyDrawResources &resources, std::span<Vector3, 289> positions) {
    std::uint32_t signature;
    if (!ReadU32(bytes, meshOffset, signature))
        return false;
    const bool xb01 = signature == xb01Signature;
    std::uint32_t firstPacket = 0, vertexOffset = 0;
    if (xb01 && (!ReadU32(bytes, meshOffset + 24, firstPacket) || !ReadU32(bytes, meshOffset + 28, vertexOffset)))
        return false;
    std::size_t packet = meshOffset + (static_cast<std::size_t>(firstPacket) + 6U) * 4U;
    std::uint32_t bufferIndex = 0;
    bool selectedBuffer = false;
    do {
        std::uint32_t header = 0;
        std::size_t nextPacket = 0;
        if (xb01) {
            if (!ReadU32(bytes, packet, header))
                return false;
            if (!header)
                break;
            nextPacket = packet + static_cast<std::size_t>(header >> 8U) * 4U;
            std::size_t command = packet + 20;
            for (;;) {
                if (!ReadU32(bytes, command, header))
                    return false;
                if (!(header & 0xffU))
                    break;
                const std::size_t size = static_cast<std::size_t>(header >> 8U) * 4U;
                if (!size || command + size > nextPacket)
                    return false;
                if ((header & 0xffU) == 1) {
                    if (!ReadU32(bytes, command + 8, bufferIndex))
                        return false;
                    bufferIndex &= 0xffU;
                    selectedBuffer = true;
                }
                command += size;
            }
            if (!selectedBuffer || bufferIndex >= resources.vertexBuffers.size())
                return false;
            (void)d3t::GetWriteMappedBufferPointer(d3t::DX11::g_ppD3DDeviceContext, resources.vertexBuffers[bufferIndex].handle, 1, 0);
        }
        std::size_t cursor = meshOffset + (xb01 ? (static_cast<std::size_t>(vertexOffset) + 11U) * 4U + 12U * bufferIndex : 112U);
        bool valid = true;
        const std::size_t stride = xb01 ? 32U : 40U;
        const auto readPosition = [&](std::size_t index, std::size_t advance) {
            if (!Contains(bytes, cursor, 12)) {
                valid = false;
                return;
            }
            std::memcpy(&positions[index].x, bytes.data() + cursor, 4);
            std::memcpy(&positions[index].z, bytes.data() + cursor + 8, 4);
            cursor += advance;
        };
        for (std::int32_t column = 16; column >= 0; --column) {
            readPosition(7 * 17 + column, 32);
            readPosition(8 * 17 + column, 32);
        }
        for (std::int32_t row = 1; row <= 6; ++row) {
            if (!xb01)
                cursor += 8;
            for (std::int32_t column = 0; column <= 16; ++column)
                readPosition((8 + row) * 17 + column, stride);
            if (!xb01)
                cursor += 8;
            for (std::int32_t column = 16; column >= 0; --column)
                readPosition((7 - row) * 17 + column, stride);
        }
        for (std::int32_t row : {15, 16, 0}) {
            if (!xb01)
                cursor += row == 0 ? 16 : 8;
            for (std::int32_t column = 0; column <= 16; ++column)
                readPosition(row * 17 + column, stride);
        }
        if (!valid)
            return false;
        if (!xb01)
            return true;
        d3t::DX11_CopySceneNodeToGPU(d3t::DX11::g_ppD3DDeviceContext, resources.vertexBuffers[bufferIndex].handle);
        packet = nextPacket;
    } while (true);
    return true;
}

bool UpdateLegacyWaveColours(
    std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources, std::uint32_t diffuse, std::uint32_t specular) {
    std::uint32_t signature{};
    if (!ReadU32(bytes, xb01Offset, signature))
        return false;
    if (signature == xb01Signature)
        return MutateLegacyWaveBuffers(bytes, xb01Offset, resources, {}, 0.0f, 0.0f, diffuse, specular);
    std::byte *const source = const_cast<std::byte *>(bytes.data());
    std::size_t packet = std::size_t(xb01Offset) + 24;
    for (;;) {
        std::uint32_t header{}, count{};
        if (!ReadU32(bytes, packet, header))
            return false;
        if (!header)
            return true;
        if (static_cast<std::int32_t>(header) < 0) {
            std::uint32_t state{};
            if (!ReadU32(bytes, packet + 8, state))
                return false;
            state &= 0xfffe7fffU;
            std::memcpy(source + packet + 8, &state, 4);
            packet += 80;
            if (!ReadU32(bytes, packet, header))
                return false;
        }
        if (!ReadU32(bytes, packet + 4, count))
            return false;
        if (header & 8U)
            count *= 3U;
        packet += 8;
        while (count--) {
            std::uint32_t vertex{};
            if (!ReadU32(bytes, packet, vertex))
                return false;
            if (vertex & 1U) {
                if (!Contains(bytes, packet, 32))
                    return false;
                std::memcpy(source + packet + 16, &diffuse, 4);
                std::memcpy(source + packet + 20, &specular, 4);
                packet += 32;
            } else
                packet += 8;
        }
    }
}

bool UpdateLegacyTextureUVs(std::span<const std::byte> bytes, std::uint32_t xb01Offset, LegacyDrawResources &resources, float scrollU, float scrollV) {
    return MutateLegacyWaveBuffers(bytes, xb01Offset, resources, {}, scrollU, scrollV, 0, 0, true);
}

void ReleaseLegacyDrawResources(LegacyDrawResources &resources) {



    for (std::uint32_t index = 0; index < legacyQueuedDrawCount; ++index) {
        if (legacyQueuedDraws[index].type != 1 || legacyQueuedDraws[index].resources != &resources)
            continue;
        static_cast<void>(d3t::BindSceneFrameRenderTarget());
        FlushQueuedLegacyDrawPasses();
        break;
    }
    for (const LegacyVertexBufferBinding &buffer : resources.vertexBuffers) {
        if (buffer.handle)
            d3t::ReleaseBufferByHandle(&buffer.handle);
    }
    if (resources.indexBuffer) {
        d3t::ReleaseSceneIndexBuffer(resources.indexBuffer);
    }
    resources = {};
}

namespace {

void BindLegacyVertexStream(d3t::D3TRenderState &state, const LegacyDrawResources &resources, std::uint32_t encoded, std::uint32_t localIndex) {
    const std::uint32_t slot = (encoded >> 8U) & 0xffU;
    if (slot >= 32)
        return;
    const std::uint32_t handle = static_cast<std::uint32_t>(ResolveVertexBuffer(resources, localIndex));
#if defined(_WIN32)
    // UnmapAll/SetVertexStreamHandleFromCmd release this stream's mapped
    // temporary buffer before replacing its handle; a later draw must not
    // republish the old resource-cache binding.
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (slot < context->resourceCache.size() / 3) {
        auto *const cache = context->resourceCache.data() + slot * 3;
        if (cache[0]) {
            static_cast<ID3D11DeviceContext *>(context->deviceContext)->Unmap(reinterpret_cast<ID3D11Resource *>(cache[0]), 0);
            cache[0] = 0;
        }
    }
#endif
    if (state.vertexBufferHandles[slot] != handle) {
        state.vertexBufferHandles[slot] = handle;
        state.vertexBufferDirtyMask |= 1U << slot;
    }
}

#if defined(_WIN32)
std::uint32_t DrawLegacyIndexedCommand(const LegacyDrawResources &resources, std::uint32_t commandOffset) {
    const LegacyIndexDraw *const draw = FindIndexDraw(resources, commandOffset);
    if (!draw || !resources.indexBuffer)
        return 0;
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    auto &state = d3t::GetRenderState(*context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    d3t::TopologyType topology = d3t::TopologyType::TriangleStrip;
    if (draw->primitive == 1)
        topology = d3t::TopologyType::LineList;
    else if (draw->primitive == 5)
        topology = d3t::TopologyType::TriangleList;
    if (state.topology != static_cast<std::uint32_t>(topology)) {
        state.topology = static_cast<std::uint32_t>(topology);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    if (state.indexBufferHandle != static_cast<std::uint32_t>(resources.indexBuffer) || state.indexFormat != 11U) {
        state.indexBufferHandle = static_cast<std::uint32_t>(resources.indexBuffer);
        state.indexFormat = 11U;
        state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
    }
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    native->DrawIndexed(draw->indexCount, draw->startIndex, 0);
    return 1;
}
#endif

LegacyRenderPacketState *CaptureLegacyRenderPacketState(std::uint32_t captureArgument = 0) {
    if (legacyRenderPacketCount >= legacyRenderPacketCapacity)
        return nullptr;
    LegacyRenderPacketState &packet = legacyRenderPackets[legacyRenderPacketCount++];
    packet.worldMatrix = workTransformMatrix;
    if (legacyVertexColourStateActive) {
        packet.savedVertexColourState = legacyVertexColourStates;
    } else {
        packet.lambertRefboard = modelReferenceDirection;
        packet.lambertFvfLayout[0] = std::bit_cast<std::uint32_t>(lambertFvfReference.x);
        packet.lambertFvfLayout[1] = std::bit_cast<std::uint32_t>(lambertFvfReference.y);
        packet.lambertFvfLayout[2] = std::bit_cast<std::uint32_t>(lambertFvfReference.z);
    }
    packet.alphaScale = legacyAlphaMultiplier;
    packet.modelLight = particleLightColour;
    const Vector4 &modelRef = shaderParameters.constants[13];
    packet.modelRef = {modelRef.x, modelRef.y, modelRef.z};
    packet.lightDir = modelLightVector;
    packet.fvfLayoutType = currentVertexColourLayoutType;
    packet.depthFade = {legacyDepthFadeNear, legacyDepthFadeFar, legacyDepthFadeIntensity};


    packet.flags = std::uint32_t(legacyAlphaPassEnabled) | (std::uint32_t(captureArgument != 0) << 1U) | (std::uint32_t(legacyVertexColourStateActive) << 2U) |
        (std::uint32_t(legacyBusyPreparingDraw) << 3U) | (std::uint32_t(legacyModelFogEnabled != 0) << 4U) | (std::uint32_t(legacyMirroredDrawMode != 0) << 5U);
    packet.alphaPassMatrix = legacyAlphaPassMatrix;
    packet.alphaState = legacyPacketAlphaState;
    packet.modelScale = temporaryModelScale;
    return &packet;
}

void QueueLegacyDrawPacket(const LegacyRenderPacketState *packet, std::span<const std::byte> modelBytes, std::uint32_t xb01Offset,
    const LegacyDrawResources &resources, std::uint32_t commandOffset, std::uint32_t header) {
    if (!packet || legacyQueuedDrawCount >= legacyQueuedDrawCapacity)
        return;
    const Vector3 centre = ReadValue<Vector3>(modelBytes, commandOffset + 4U);
    const Matrix4x4 &world = packet->worldMatrix;
    legacyQueuedDrawSortKeys[legacyQueuedDrawCount] = !legacyQueuedDepthFromGeometry ? legacyQueuedFallbackDepth :
        (header & 8U)                                                                ? centre.z :
                        ((world.rows[1].z * centre.y + world.rows[0].z * centre.x) + world.rows[2].z * centre.z) + world.rows[3].z;
    legacyQueuedDraws[legacyQueuedDrawCount++] = {packet, 1, xb01Offset, modelBytes.data() + commandOffset, &resources, modelBytes};
    ++legacyQueuedDrawCommandCount;
}

std::span<const std::uint32_t> SortLegacyQueuedDraws() {
    return {RadixSortIndicesBy4ByteKey(legacyQueuedDrawSortKeys, legacyQueuedDrawCount, legacySortedQueueIndices, legacySortScratch), legacyQueuedDrawCount};
}

bool HasLegacyAlphaPassMatrix() {
    return legacyPacketAlphaState.hasAlpha || legacyPacketAlphaState.alphaOp8;
}

void PrepareLegacyAlphaPassMatrix() {
    SetAlphaFlags(1, 0, 4);
    SetAlphaFlags(1, 1, 4);
    const Matrix4x4 world = workTransformMatrix;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&legacyAlphaPassMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(&world);
    Matrix4x4::AffineProjectionSwap();
    for (unsigned row = 0; row < 4; ++row) {
        shaderParameters.constants[14 + row] = workTransformMatrix.rows[row];
    }
    Matrix4x4::Pop(1);
}

void RestoreLegacyAlphaPassSamplers() {
    SetAlphaFlags(1, 0, 3);
    SetAlphaFlags(1, 1, 3);
}

void RestoreLegacyRenderPacketState(const LegacyRenderPacketState &packet) {
    legacyAlphaPassMatrix = packet.alphaPassMatrix;
    legacyPacketAlphaState = packet.alphaState;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&packet.worldMatrix);
    CommitFrameForRaster();
    temporaryModelScale = packet.modelScale;
    if (packet.flags & 4U) {
        legacyVertexColourStates = packet.savedVertexColourState;
        PrepareLegacyModelVertexColourState();
    } else {
        modelReferenceDirection = packet.lambertRefboard;
        lambertFvfReference.x = std::bit_cast<float>(packet.lambertFvfLayout[0]);
        lambertFvfReference.y = std::bit_cast<float>(packet.lambertFvfLayout[1]);
        lambertFvfReference.z = std::bit_cast<float>(packet.lambertFvfLayout[2]);
        PrepareLegacyModelLambertState();
    }
    modelLightVector = packet.lightDir;
    particleLightColour = packet.modelLight;
    shaderParameters.constants[12] = {packet.modelLight.x, packet.modelLight.y, packet.modelLight.z, 1.0f};
    shaderParameters.constants[13] = {packet.modelRef.x, packet.modelRef.y, packet.modelRef.z, 1.0f};
    SetLegacyVertexColourType(packet.fvfLayoutType);
    legacyAlphaMultiplier = packet.alphaScale;
    legacyDepthFadeNear = packet.depthFade.x;
    legacyDepthFadeFar = packet.depthFade.y;
    legacyDepthFadeIntensity = packet.depthFade.z;
    legacyAlphaPassEnabled = (packet.flags & 1U) != 0;
    legacyModelFogEnabled = (packet.flags >> 4U) & 1U;
    UpdateLegacyDepthFadeConstants();
    legacyMirroredDrawMode = (packet.flags >> 5U) & 1U;
}

std::uint32_t ExecuteLegacyDrawPacket(
    std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, std::size_t packet) {
#if !defined(_WIN32)
    static_cast<void>(modelBytes);
    static_cast<void>(xb01Offset);
    static_cast<void>(resources);
    static_cast<void>(packet);
    return 0;
#else
    if (!d3t::DX11::g_ppD3DDeviceContext) {
        return 0;
    }
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    auto &state = d3t::GetRenderState(*context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (!native) {
        return 0;
    }

    std::uint32_t draws = 0;
    std::uint32_t packetHeader{};
    if (!ReadU32(modelBytes, packet, packetHeader) || !packetHeader)
        return 0;
    const std::uint32_t packetDwords = packetHeader >> 8U;
    if (!packetDwords || packetDwords > (modelBytes.size() - packet) / 4U)
        return 0;
    const std::size_t packetEnd = packet + static_cast<std::size_t>(packetDwords) * 4U;
    if ((packetHeader & 4U) != 0)
        SetEngineFlag(0x93, 0);
    else if (legacyMirroredDrawMode)
        SetEngineFlag(0x93, 2305);
    for (std::size_t command = packet + 20U; command < packetEnd;) {
        std::uint32_t header{};
        if (!ReadU32(modelBytes, command, header) || !(header & 0xffU)) {
            break;
        }
        const std::uint8_t opcode = static_cast<std::uint8_t>(header);
        const std::uint32_t lengthDwords = header >> 8U;
        if (!lengthDwords || lengthDwords > (packetEnd - command) / 4U)
            break;
        const std::uint32_t commandOffset = static_cast<std::uint32_t>(command - xb01Offset);

        switch (opcode) {
        case 1: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 8U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 8U;
                BindLegacyVertexStream(state, resources, ReadValue<std::uint32_t>(modelBytes, record), ReadValue<std::uint32_t>(modelBytes, record + 4));
            }
            break;
        }
        case 2:
            SelectShader(state, ReadValue<std::uint32_t>(modelBytes, command + 4));
            break;
        case 4:
            AssignPixelShaderForShenmue(modelBytes, command, state);
            break;
        case 8: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 2U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 2U;
                SetEngineFlag(ReadValue<std::uint8_t>(modelBytes, record), ReadValue<std::uint8_t>(modelBytes, record + 1U));
            }
            break;
        }
        case 9: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 4U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 4U;
                SetEngineFlag(ReadValue<std::uint16_t>(modelBytes, record), ReadValue<std::uint16_t>(modelBytes, record + 2U));
            }
            break;
        }
        case 10: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 8U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 8U;
                SetEngineFlag(ReadValue<std::uint32_t>(modelBytes, record), ReadValue<std::uint32_t>(modelBytes, record + 4U));
            }
            break;
        }
        case 11: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::uint32_t texture = ResolveTexture(resources, commandOffset, index);
                BindPixelTexture0(state, texture ? texture : d3t::g_defaultWhiteTexture);
            }
            break;
        }
        case 13: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 4U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 4U;
                const auto stage = ReadValue<std::uint8_t>(modelBytes, record);
                const auto setting = ReadValue<std::uint8_t>(modelBytes, record + 1U);
                auto value = ReadValue<std::uint16_t>(modelBytes, record + 2U);
                if (resources.waveVertexColours && setting <= 1 && (value & 0xffU) == 3)
                    value = static_cast<std::uint16_t>((value & 0xff00U) | 1U);
                SetAlphaFlags(stage, setting, value);
            }
            break;
        }
        case 14: {
            const std::uint32_t count = ReadValue<std::uint32_t>(modelBytes, command + 4);
            if (!Contains(modelBytes, command + 8, std::size_t(count) * 8U))
                return draws;
            for (std::uint32_t index = 0; index < count; ++index) {
                const std::size_t record = command + 8U + index * 8U;
                const std::uint32_t stage = ReadValue<std::uint8_t>(modelBytes, record);
                const std::uint32_t setting = ReadValue<std::uint8_t>(modelBytes, record + 1U);
                const std::uint32_t value = ReadValue<std::uint32_t>(modelBytes, record + 4U);
                SetAlphaFlags(stage, setting, value);
                if (setting == 6U && value == 0U) {
                    SetAlphaFlags(stage, setting, savedAlphaOperation);
                }
            }
            break;
        }
        case 16:
            draws += DrawLegacyIndexedCommand(resources, commandOffset);
            break;
        case 18:
            break;
        case 19:
            BindLegacyVertexStream(state, resources, ReadValue<std::uint32_t>(modelBytes, command + 4), ReadValue<std::uint32_t>(modelBytes, command + 8));
            break;
        case 21:
            SetEngineFlag(0x93U, ReadValue<std::uint32_t>(modelBytes, command + 4U) ^ legacyMirroredDrawMode);
            break;
        case 22:
            SetEngineFlag(0x5cU, legacyModelFogEnabled & legacySceneFogEnabled & ReadValue<std::uint32_t>(modelBytes, command + 4U));
            break;
        default:
            break;
        }
        command += static_cast<std::size_t>(lengthDwords) * 4U;
    }
    ResetLegacyMaterialState(state);
    return draws;
#endif
}

} // namespace
namespace {
bool EnsureFireVertexBuffer() {
#if defined(_WIN32)
    if (fireVertexBuffer)
        return true;
    constexpr std::array<FireGpuVertex, 4> vertices{{
        {{-0.5f, 1.0f, 0.0f}, 0.0f, 0.94999999f},
        {{-0.5f, 0.0f, 0.0f}, 0.0f, 0.0f},
        {{0.5f, 1.0f, 0.0f}, 1.0f, 0.94999999f},
        {{0.5f, 0.0f, 0.0f}, 1.0f, 0.0f},
    }};
    constexpr std::size_t vertexCount = 4U * 512U;
    constexpr std::size_t byteSize = vertexCount * sizeof(FireGpuVertex);
    const d3t::BufferDescription description{
        .flags = 1, .size = static_cast<std::int32_t>(byteSize), .count = 0, .flags3 = 3, .buffer = nullptr, .name = "Fire quads"};
    fireVertexBuffer = d3t::CreateBuffer(&description);
    if (!fireVertexBuffer)
        return false;
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    auto *const mapped = static_cast<FireGpuVertex *>(d3t::GetWriteMappedBufferPointer(context, fireVertexBuffer, 1, 0));
    if (!mapped) {
        d3t::ValidateAndCommitBuffer(fireVertexBuffer);
        fireVertexBuffer = 0;
        return false;
    }
    for (std::size_t slot = 0; slot < fireQuads.size(); ++slot)
        std::memcpy(mapped + 4U * slot, vertices.data(), sizeof(vertices));
    d3t::DX11_CopySceneNodeToGPU(context, fireVertexBuffer);
    return true;
#else
    return false;
#endif
}

Vector4 TransformEffectPoint(const Matrix4x4 &matrix, const Vector4 &point) {
    return {point.x * matrix.rows[0].x + point.y * matrix.rows[1].x + point.z * matrix.rows[2].x + point.w * matrix.rows[3].x,
        point.x * matrix.rows[0].y + point.y * matrix.rows[1].y + point.z * matrix.rows[2].y + point.w * matrix.rows[3].y,
        point.x * matrix.rows[0].z + point.y * matrix.rows[1].z + point.z * matrix.rows[2].z + point.w * matrix.rows[3].z,
        point.x * matrix.rows[0].w + point.y * matrix.rows[1].w + point.z * matrix.rows[2].w + point.w * matrix.rows[3].w};
}
Matrix4x4 MultiplyEffectMatrices(const Matrix4x4 &lhs, const Matrix4x4 &rhs) {
    Matrix4x4 product{};
    for (std::size_t row = 0; row < 4; ++row) {
        const float *const left = &lhs.rows[row].x;
        float *const output = &product.rows[row].x;
        for (std::size_t column = 0; column < 4; ++column) {
            output[column] = left[0] * (&rhs.rows[0].x)[column] + left[1] * (&rhs.rows[1].x)[column] + left[2] * (&rhs.rows[2].x)[column] +
                left[3] * (&rhs.rows[3].x)[column];
        }
    }
    return product;
}

void DrawQueuedFireQuad(void *data) {
#if defined(_WIN32)
    const auto *const quad = static_cast<const LegacyFireQuad *>(data);
    if (!quad)
        return;
    const std::size_t textureIndex = (quad->frameTick & 1U) ? 0U : 1U;
    const std::uint32_t texture = quad->textures[textureIndex];
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext || !EnsureFireVertexBuffer() || !g_fireVertexShader || !g_fvfPtdPixelShader || !g_fvfLayouts[22]) {
        return;
    }
    auto &state = d3t::GetRenderState(*context);
    if (state.vertexShader != g_fireVertexShader) {
        state.vertexShader = g_fireVertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != g_fvfPtdPixelShader) {
        state.pixelShader = g_fvfPtdPixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[22]) {
        state.inputLayout = g_fvfLayouts[22];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(fireVertexBuffer)) {
        state.vertexBufferHandles[0] = static_cast<std::uint32_t>(fireVertexBuffer);
        state.vertexBufferDirtyMask |= 1;
    }
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    shaderParameters.constants[25] = {1.0f, 1.0f, 1.0f, std::clamp(quad->alpha, 0.0f, 1.0f)};
    BindPixelTexture0(state, texture);
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    const auto slot = static_cast<std::uint32_t>(quad - fireQuads.data());
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 4U * slot);
    ClearPixelShaderEffectSRVSlot(state, 0);
#else
    static_cast<void>(data);
#endif
}
} // namespace


static std::uint32_t ExecuteLegacyDrawCommandStreamInternal(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset,
    const LegacyDrawResources &resources, LegacyModelShaderPath shaderPath, float alpha, bool manageModelState) {
#if !defined(_WIN32)
    static_cast<void>(modelBytes);
    static_cast<void>(xb01Offset);
    static_cast<void>(resources);
    static_cast<void>(shaderPath);
    static_cast<void>(alpha);
    static_cast<void>(manageModelState);
    return 0;
#else
    if (!resources.prepared) {
        return 0;
    }
    if (!d3t::DX11::g_ppD3DDeviceContext || !d3t::DX11::g_ppD3DDeviceContext->deviceContext) {
        return 0;
    }
    std::uint32_t firstPacketOffset{};
    if (!ReadU32(modelBytes, xb01Offset + 24, firstPacketOffset))
        return 0;
    if (manageModelState) {
        legacyAlphaPassEnabled = alpha != 1.0f;
        legacyAlphaMultiplier = alpha;
        if (legacyAlphaPassEnabled) {
            SetEngineFlag(0x40, 0);
            SetEngineFlag(0x3b, 1);
        }
        if (shaderPath == LegacyModelShaderPath::VertexColour)
            PrepareLegacyModelVertexColourState();
        else
            PrepareLegacyModelLambertState();
    }
    shaderParameters.constants[25] = {1.0f, 1.0f, 1.0f, alpha};
    if (manageModelState) {
        CommitFrameForRaster();
        if (HasLegacyAlphaPassMatrix())
            PrepareLegacyAlphaPassMatrix();
    }

    // InitSystemMatrices selects ShaderFunctions[2] for opaque model
    // dispatch and [3] for instance alpha. Only header bit 1 adds an opaque
    // material to the deferred queue; bit 0x40 has no deferred second pass.
    LegacyRenderPacketState *captured = legacyRenderQueueEnabled && legacyAlphaPassEnabled ? CaptureLegacyRenderPacketState() : nullptr;
    std::uint32_t draws = 0;
    std::size_t packet = static_cast<std::size_t>(xb01Offset) + static_cast<std::size_t>(firstPacketOffset + 6U) * 4U;
    for (std::size_t packetGuard = 0; packetGuard < 65536; ++packetGuard) {
        std::uint32_t header{};
        if (!ReadU32(modelBytes, packet, header) || !header)
            break;
        const std::uint32_t packetDwords = header >> 8U;
        if (!packetDwords || packetDwords > (modelBytes.size() - packet) / 4U)
            break;
        if (!legacyRenderQueueEnabled) {


            if (legacyAlphaPassEnabled) {
                SetEngineFlag(0x3b, 1);
                SetEngineFlag(0x40, legacyBusyPreparingDraw ? 1 : 0);
            } else if (header & 1U) {
                // DrawLegacyCommandListImmediate: mask 0x01
                // takes precedence over 0x40 and only enables blending.
                SetEngineFlag(0x3b, 1);
            } else if (header & 0x40U) {
                SetEngineFlag(0x3c, 1);
                SetEngineFlag(0x3d, 255);
                SetEngineFlag(0x3a, 514);
            }
            draws += ExecuteLegacyDrawPacket(modelBytes, xb01Offset, resources, packet);
            if (legacyAlphaPassEnabled) {
                RestoreEngineFlag(0x40);
            } else if (header & 1U) {
                RestoreEngineFlag(0x3b);
            } else if (header & 0x40U) {
                RestoreEngineFlag(0x3c);
                RestoreEngineFlag(0x3d);
                RestoreEngineFlag(0x3a);
            }
        } else if (legacyAlphaPassEnabled) {
            QueueLegacyDrawPacket(captured, modelBytes, xb01Offset, resources, static_cast<std::uint32_t>(packet), header);
        } else if (header & 0x41U) {
            SetEngineFlag(0x3c, 1);
            SetEngineFlag(0x3d, 255);
            SetEngineFlag(0x3a, 514);
            draws += ExecuteLegacyDrawPacket(modelBytes, xb01Offset, resources, packet);
            RestoreEngineFlag(0x3c);
            RestoreEngineFlag(0x3d);
            RestoreEngineFlag(0x3a);
            if (header & 1U) {
                if (!captured)
                    captured = CaptureLegacyRenderPacketState();
                QueueLegacyDrawPacket(captured, modelBytes, xb01Offset, resources, static_cast<std::uint32_t>(packet), header);
            }
        } else {
            draws += ExecuteLegacyDrawPacket(modelBytes, xb01Offset, resources, packet);
        }
        packet += static_cast<std::size_t>(packetDwords) * 4U;
    }
    if (!legacyRenderQueueEnabled && legacyAlphaPassEnabled)
        RestoreEngineFlag(0x3b);
    if (manageModelState) {
        if (HasLegacyAlphaPassMatrix())
            RestoreLegacyAlphaPassSamplers();
        if (legacyAlphaPassEnabled) {
            RestoreEngineFlag(0x3b);
            RestoreEngineFlag(0x40);
        }
        legacyAlphaPassEnabled = false;
        legacyVertexColourStateActive = false;
        temporaryModelScale = 1.0f;
    }
    return draws;
#endif
}

std::uint32_t ExecuteLegacyDrawCommandStream(
    std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, LegacyModelShaderPath shaderPath, float alpha) {
    return ExecuteLegacyDrawCommandStreamInternal(modelBytes, xb01Offset, resources, shaderPath, alpha, true);
}

namespace {
Vector4 legacyModelReferenceBoard{};

bool IsLegacyGameDisplayXB01(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset) {
    std::uint32_t signature{};
    return ReadU32(modelBytes, xb01Offset, signature) && signature == xb01Signature;
}

bool AdmitLegacyGameDisplayModel(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset) {
    std::uint32_t signature{};
    if (!ReadU32(modelBytes, xb01Offset, signature))
        return false;
    if (signature != xb01Signature)
        return true;
    if (!Contains(modelBytes, std::size_t(xb01Offset) + 8, sizeof(Vector3) + sizeof(float)))
        return false;
    Vector3 centre;
    float radius;
    std::memcpy(&centre, modelBytes.data() + xb01Offset + 8, sizeof(centre));
    std::memcpy(&radius, modelBytes.data() + xb01Offset + 20, sizeof(radius));
    return GetModelCheck(&centre, radius);
}

std::array<std::uint32_t, 5> legacyMaterialBucketCounts{};
std::uint32_t legacyMaterialSpecularCount{}, legacyMaterialVertexCount{}, legacyMaterialUnknownHeaderSeen{}, legacyMaterialZeroHeaderSeen{};
const std::byte *legacyMaterialCursor{};
std::int32_t legacyMaterialLayout{}, legacyMaterialPrimitive{}, legacyMaterialVertexMultiplier{1};
std::uint32_t legacyMaterialDeferredMask{};
bool legacyMaterialDeferred{}, legacyMaterialDeferredSpecular{};
float legacyMaterialBlendIntensity{};




void ConvertLegacyMaterialWideVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const std::byte *vertex = source;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 20U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 24U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 28;
        source += 32;
    }
    legacyMaterialBucketCounts[4] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialWideReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const bool inlineVertex = (*reinterpret_cast<const std::uint32_t *>(source) & 1U) != 0;
        const std::byte *vertex = inlineVertex ? source : source + *reinterpret_cast<const std::int32_t *>(source + 4) + 8;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 20U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 24U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 28;
        source += inlineVertex ? 32 : 8;
    }
    legacyMaterialBucketCounts[4] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialPackedVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const std::byte *vertex = source;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 24;
        source += 32;
    }
    legacyMaterialBucketCounts[3] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialPackedReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const bool inlineVertex = (*reinterpret_cast<const std::uint32_t *>(source) & 1U) != 0;
        const std::byte *vertex = inlineVertex ? source : source + *reinterpret_cast<const std::int32_t *>(source + 4) + 8;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 24;
        source += inlineVertex ? 32 : 8;
    }
    legacyMaterialBucketCounts[3] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialColourVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const std::byte *vertex = source;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 20U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 24U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 28;
        source += 32;
    }
    legacyMaterialBucketCounts[2] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialColourReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const bool inlineVertex = (*reinterpret_cast<const std::uint32_t *>(source) & 1U) != 0;
        const std::byte *vertex = inlineVertex ? source : source + *reinterpret_cast<const std::int32_t *>(source + 4) + 8;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 20U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 24U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 28;
        source += inlineVertex ? 32 : 8;
    }
    legacyMaterialBucketCounts[2] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialShortVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const std::byte *vertex = source;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 20;
        source += 32;
    }
    legacyMaterialBucketCounts[1] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialShortReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const bool inlineVertex = (*reinterpret_cast<const std::uint32_t *>(source) & 1U) != 0;
        const std::byte *vertex = inlineVertex ? source : source + *reinterpret_cast<const std::int32_t *>(source + 4) + 8;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 20;
        source += inlineVertex ? 32 : 8;
    }
    legacyMaterialBucketCounts[1] += count;
    legacyMaterialCursor = source;
}

void ConvertLegacyMaterialRawReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    std::uintptr_t destination = 0;
    for (std::uint32_t remaining = count; remaining != 0; --remaining) {
        const bool inlineVertex = (*reinterpret_cast<const std::uint32_t *>(source) & 1U) != 0;
        const std::byte *vertex = inlineVertex ? source : source + *reinterpret_cast<const std::int32_t *>(source + 4) + 8;
        *reinterpret_cast<volatile std::uint32_t *>(destination + 0U) = *reinterpret_cast<const std::uint32_t *>(vertex + 0U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 4U) = *reinterpret_cast<const std::uint32_t *>(vertex + 4U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 8U) = *reinterpret_cast<const std::uint32_t *>(vertex + 8U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 12U) = *reinterpret_cast<const std::uint32_t *>(vertex + 12U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 16U) = *reinterpret_cast<const std::uint32_t *>(vertex + 16U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 20U) = *reinterpret_cast<const std::uint32_t *>(vertex + 20U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 24U) = *reinterpret_cast<const std::uint32_t *>(vertex + 24U);
        *reinterpret_cast<volatile std::uint32_t *>(destination + 28U) = *reinterpret_cast<const std::uint32_t *>(vertex + 28U);
        destination += 32;
        source += inlineVertex ? 32 : 8;
    }
    legacyMaterialBucketCounts[0] += count;
    legacyMaterialCursor = source;
}

void AdvanceLegacyMaterialWideVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 32ULL * count;
    legacyMaterialBucketCounts[4] += count;
}

void AdvanceLegacyMaterialWideReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 32 : 8;
    legacyMaterialCursor = source;
    legacyMaterialBucketCounts[4] += count;
}

void AdvanceLegacyMaterialPackedVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 32ULL * count;
    legacyMaterialBucketCounts[3] += count;
}

void AdvanceLegacyMaterialPackedReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 32 : 8;
    legacyMaterialCursor = source;
    legacyMaterialBucketCounts[3] += count;
}

void AdvanceLegacyMaterialColourVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 32ULL * count;
    legacyMaterialBucketCounts[2] += count;
}

void AdvanceLegacyMaterialColourReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 32 : 8;
    legacyMaterialCursor = source;
    legacyMaterialBucketCounts[2] += count;
}

void AdvanceLegacyMaterialShortVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 32ULL * count;
    legacyMaterialBucketCounts[1] += count;
}

void AdvanceLegacyMaterialShortReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 32 : 8;
    legacyMaterialCursor = source;
    legacyMaterialBucketCounts[1] += count;
}

void AdvanceLegacyMaterialRawVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 32ULL * count;
    legacyMaterialBucketCounts[0] += count;
}

void AdvanceLegacyMaterialRawReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 32 : 8;
    legacyMaterialCursor = source;
    legacyMaterialBucketCounts[0] += count;
}

void SkipLegacyMaterialVertices() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    legacyMaterialCursor += 8 + 56ULL * count;
}

void SkipLegacyMaterialReferences() {
    const std::uint32_t count = std::uint32_t(legacyMaterialVertexMultiplier) * *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor + 4);
    const std::byte *source = legacyMaterialCursor + 8;
    for (std::uint32_t remaining = count; remaining != 0; --remaining)
        source += (*reinterpret_cast<const std::uint32_t *>(source) & 1U) ? 56 : 8;
    legacyMaterialCursor = source;
}

using LegacyMaterialPass = void (*)();
constexpr LegacyMaterialPass legacyMaterialImmediatePasses[14]{ConvertLegacyMaterialWideVertices, ConvertLegacyMaterialWideReferences,
    ConvertLegacyMaterialPackedVertices, ConvertLegacyMaterialPackedReferences, ConvertLegacyMaterialColourVertices, ConvertLegacyMaterialColourReferences,
    SkipLegacyMaterialVertices, SkipLegacyMaterialReferences, ConvertLegacyMaterialShortVertices, ConvertLegacyMaterialShortReferences,
    ConvertLegacyMaterialRawReferences, ConvertLegacyMaterialRawReferences, ConvertLegacyMaterialRawReferences, ConvertLegacyMaterialRawReferences};
constexpr LegacyMaterialPass legacyMaterialReplayPasses[14]{AdvanceLegacyMaterialWideVertices, AdvanceLegacyMaterialWideReferences,
    AdvanceLegacyMaterialPackedVertices, AdvanceLegacyMaterialPackedReferences, AdvanceLegacyMaterialColourVertices, AdvanceLegacyMaterialColourReferences,
    SkipLegacyMaterialVertices, SkipLegacyMaterialReferences, AdvanceLegacyMaterialShortVertices, AdvanceLegacyMaterialShortReferences,
    AdvanceLegacyMaterialRawVertices, AdvanceLegacyMaterialRawReferences, AdvanceLegacyMaterialRawVertices, AdvanceLegacyMaterialRawReferences};

void ProcessLegacyRenderSubCommandsImmediate(const std::byte *end) {
    std::int32_t pass = 0;
    while (legacyMaterialCursor != end) {
        const std::uint32_t opcode = *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor);
        if (!(opcode & 0x80U)) {
            pass = ((opcode & 0x20U) != 0) + 2 * legacyMaterialLayout;
            legacyMaterialPrimitive = LEGACY_MATERIAL_PRIMITIVE_ARGUMENTS[(opcode >> 2U) & 7U];
            legacyMaterialVertexMultiplier = legacyMaterialPrimitive == 5 ? 3 : 1;
            SetEngineFlag(0x93, LEGACY_MATERIAL_CULL_ARGUMENTS[opcode & 3U]);
        }
        legacyMaterialImmediatePasses[pass]();
    }
}

void ProcessLegacyMaterialSubCommands(const std::byte *end) {
    std::int32_t pass = 0;
    while (legacyMaterialCursor != end) {
        const std::uint32_t opcode = *reinterpret_cast<const std::uint32_t *>(legacyMaterialCursor);
        if (!(opcode & 0x80U)) {
            pass = ((opcode & 0x20U) != 0) + 2 * legacyMaterialLayout;
            legacyMaterialPrimitive = LEGACY_MATERIAL_PRIMITIVE_ARGUMENTS[(opcode >> 2U) & 7U];
            legacyMaterialVertexMultiplier = legacyMaterialPrimitive == 5 ? 3 : 1;
            SetEngineFlag(0x93, LEGACY_MATERIAL_CULL_ARGUMENTS[opcode & 3U]);
        }
        legacyMaterialReplayPasses[pass]();
    }
}

void SetLegacyMaterialSpecular(const LegacyMaterialCommand &) {
    ++legacyMaterialSpecularCount;
    SetEngineFlag(0x67, 1);
}
void CountLegacyMaterialWide(const LegacyMaterialCommand &) {
    ++legacyMaterialBucketCounts[4];
    SetEngineFlag(0x67, 0);
}
void CountLegacyMaterialPacked(const LegacyMaterialCommand &) {
    ++legacyMaterialBucketCounts[3];
    SetEngineFlag(0x67, 0);
}
void LegacyMaterialNullsub(const LegacyMaterialCommand &) {
}
void SetLegacyMaterialColour(const LegacyMaterialCommand &material) {
    ++legacyMaterialBucketCounts[2];
    shaderParameters.constants[6] = {
        material.colourScale * material.colour.x, material.colourScale * material.colour.y, material.colourScale * material.colour.z, material.colourAlpha};
    SetEngineFlag(0x67, 0);
}
using LegacyMaterialHandler = void (*)(const LegacyMaterialCommand &);
constexpr LegacyMaterialHandler legacyMaterialHandlers[5]{
    SetLegacyMaterialSpecular, CountLegacyMaterialWide, CountLegacyMaterialPacked, LegacyMaterialNullsub, SetLegacyMaterialColour};

void SetLegacyMaterialLayout(const LegacyMaterialCommand &material) {
    legacyMaterialLayout = material.alphaMode > 0 ? 6 : material.alphaMode + 5;
    if (material.alphaMode == -3)
        legacyMaterialLayout -= currentVertexColourLayoutType;
}

void SetLegacyMaterialAlphaAndBias(const LegacyMaterialCommand &material) {
    SetAlphaFlags(0, 0, LEGACY_MATERIAL_ALPHA_ARGUMENTS[(material.blendFlags >> 16U) & 5U]);
    SetAlphaFlags(0, 1, LEGACY_MATERIAL_ALPHA_ARGUMENTS[(material.blendFlags >> 15U) & 5U]);
    SetLegacyMaterialLayout(material);
    SetEngineFlag(0x95, std::uint32_t(material.depthBias - 1) <= 15U ? material.depthBias : 0U);
    const float alpha = material.alphaSlot6 < -9.9899998f || material.alphaSlot6 > 9.9899998f ? 0.0f : material.alphaSlot6;
    SetAlphaFlags(0, 6, std::bit_cast<std::uint32_t>(alpha));
}

void QueueLegacyRenderPacket_Type0_MaterialCmd(const LegacyRenderPacketState *packet, const LegacyMaterialCommand *material) {
    if (!material || legacyQueuedDrawCount >= legacyQueuedDrawCapacity)
        return;
    const Vector3 &centre = material->sortPosition;
    float depth;
    if (packet) {
        const Matrix4x4 &world = packet->worldMatrix;
        depth = ((world.rows[1].z * centre.y + world.rows[0].z * centre.x) + world.rows[2].z * centre.z) + world.rows[3].z;
    } else
        depth = legacyQueuedDepthFromGeometry ? centre.z : legacyQueuedFallbackDepth;
    legacyQueuedDrawSortKeys[legacyQueuedDrawCount] = depth;
    LegacyQueuedDraw &entry = legacyQueuedDraws[legacyQueuedDrawCount++];
    entry.packet = packet;
    entry.type = 0;
    entry.command = reinterpret_cast<const std::byte *>(material);
    entry.savedMaterialState0 = std::uint64_t(legacyMaterialBucketCounts[0]) | (std::uint64_t(legacyMaterialBucketCounts[1]) << 32U);
    entry.materialReplay.savedMaterialState1 = std::uint64_t(legacyMaterialBucketCounts[2]) | (std::uint64_t(legacyMaterialBucketCounts[3]) << 32U);
    entry.materialReplay.savedMaterialCount = legacyMaterialBucketCounts[4];
}

void shenProcessMaterialCommands(const LegacyMaterialCommand &material) {
    SetEngineFlag(0x39, LEGACY_MATERIAL_DEPTH_ARGUMENTS[material.renderFlags >> 29U]);
    SetEngineFlag(0x3e, 770);
    SetEngineFlag(0x3f, 771);
    SetLegacyMaterialAlphaAndBias(material);
    legacyMaterialHandlers[legacyMaterialLayout](material);
    legacyMaterialCursor = reinterpret_cast<const std::byte *>(&material) + sizeof(material);
    ProcessLegacyMaterialSubCommands(legacyMaterialCursor + material.subCommandBytes);
}

std::uint32_t ExecuteModelRenderCommandStream(const std::byte *commands) {
    const LegacyMaterialCommand *material = reinterpret_cast<const LegacyMaterialCommand *>(commands);
    LegacyRenderPacketState *captured = nullptr;
    while (material->header) {
        SetEngineFlag(0x5c, (material->blendFlags & 0xc00000U) == 0x800000U ? 0U : legacySceneFogEnabled & legacyModelFogEnabled);
        RestoreEngineFlag(0x3b);
        const std::uint32_t renderMode = (material->header >> 24U) & 3U;
        if ((renderMode == 2U || legacyAlphaPassEnabled) && legacyRenderQueueEnabled) {
            if (!captured)
                captured = CaptureLegacyRenderPacketState(1);
            QueueLegacyRenderPacket_Type0_MaterialCmd(captured, material);
            legacyMaterialDeferredMask = 0x01010101U;
            legacyMaterialDeferred = legacyMaterialDeferredSpecular = true;
            SetLegacyMaterialLayout(*material);
        } else {
            if (renderMode == 2U || legacyAlphaPassEnabled)
                SetEngineFlag(0x3b, 1);
            SetEngineFlag(0x39, LEGACY_MATERIAL_DEPTH_ARGUMENTS[material->renderFlags >> 29U]);
            if (!legacyAlphaPassEnabled) {
                SetEngineFlag(0x3b, LEGACY_MATERIAL_BLEND_ARGUMENTS[(material->blendFlags >> 19U) & 3U]);
                legacyMaterialBlendIntensity = (material->blendFlags & 0x100000U) == 0 ? 1.0f : 0.0f;
                SetEngineFlag(0x3e, LEGACY_MATERIAL_SOURCE_BLEND_ARGUMENTS[material->blendFlags >> 29U]);
                SetEngineFlag(0x3f, LEGACY_MATERIAL_DESTINATION_BLEND_ARGUMENTS[(material->blendFlags >> 26U) & 7U]);
            }
            SetLegacyMaterialAlphaAndBias(*material);
            legacyMaterialHandlers[legacyMaterialLayout](*material);
            legacyMaterialDeferredMask = 0;
            legacyMaterialDeferred = legacyMaterialDeferredSpecular = false;
        }
        legacyMaterialCursor = reinterpret_cast<const std::byte *>(material) + sizeof(*material);
        const std::byte *end = legacyMaterialCursor + material->subCommandBytes;
        ProcessLegacyRenderSubCommandsImmediate(end);
        material = reinterpret_cast<const LegacyMaterialCommand *>(end);
    }
    RestoreEngineFlag(0x3b);
    return material->renderFlags;
}

void RestoreLegacyCpuMaterialFlags() {
    if (d3t::DX11::g_ppD3DDeviceContext)
        ClearPixelShaderEffectSRVSlot(d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext), 0);
    for (std::uint32_t setting : {0x93U, 0x67U, 0x39U, 0x3eU, 0x3fU, 0x3cU, 0x3aU, 0x42U, 0x95U, 0x5cU})
        RestoreEngineFlag(setting);
    SetAlphaFlags(0, 0x0c, 0xffffffffU);
    SetAlphaFlags(0, 0x0e, 0xffffffffU);
    SetAlphaFlags(0, 0x0f, 0xffffffffU);
    SetAlphaFlags(0, 0x10, 4);
    SetAlphaFlags(0, 0x12, 2);
    SetAlphaFlags(0, 0x13, 0);
    SetAlphaFlags(0, 0, 3);
    SetAlphaFlags(0, 1, 3);
    SetAlphaFlags(0, 3, 2);
    SetAlphaFlags(0, 4, 2);
    SetAlphaFlags(0, 5, 2);
    SetAlphaFlags(0, 6, savedAlphaOperation);
}

std::uint32_t CommitLegacyModelMaterialCommands(const std::byte *model) {
    const std::int32_t signature = *reinterpret_cast<const std::int32_t *>(model);
    if (signature == -1 && !legacyMaterialUnknownHeaderSeen)
        legacyMaterialUnknownHeaderSeen = 1;
    else if (signature == 0 && !legacyMaterialZeroHeaderSeen)
        legacyMaterialZeroHeaderSeen = 1;
    legacyMaterialBucketCounts.fill(0);
    legacyMaterialSpecularCount = 0;
    const std::uint32_t vertices = ExecuteModelRenderCommandStream(model + 24);
    legacyMaterialVertexCount += vertices;
    RestoreLegacyCpuMaterialFlags();
    return vertices;
}

}

std::uint32_t ModelFlagDispatch_SetModelFlagDataOpaque(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources) {
    if (!AdmitLegacyGameDisplayModel(modelBytes, xb01Offset)) {
        temporaryModelScale = 1.0f;
        return 0;
    }
    legacyAlphaPassEnabled = false;
    legacyAlphaMultiplier = 1.0f;
    PrepareLegacyModelLambertState();
    const bool xb01 = IsLegacyGameDisplayXB01(modelBytes, xb01Offset);
    if (xb01)
        legacyModelReferenceBoard = legacyModelColour;
    CommitFrameForRaster();
    if (xb01 && HasLegacyAlphaPassMatrix())
        PrepareLegacyAlphaPassMatrix();
    const std::uint32_t draws = xb01 ? ExecuteLegacyDrawCommandStreamInternal(modelBytes, xb01Offset, resources, LegacyModelShaderPath::Lambert, 1.0f, false) :
                                       CommitLegacyModelMaterialCommands(modelBytes.data() + xb01Offset);
    if (xb01 && HasLegacyAlphaPassMatrix())
        RestoreLegacyAlphaPassSamplers();
    legacyAlphaPassEnabled = false;
    legacyVertexColourStateActive = false;
    temporaryModelScale = 1.0f;
    return draws;
}

// @verified - p
std::uint32_t ModelFlagDispatch_ReadModelDataOpaque(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources) {
    if (!AdmitLegacyGameDisplayModel(modelBytes, xb01Offset)) {
        temporaryModelScale = 1.0f;
        return 0;
    }
    legacyAlphaPassEnabled = false;
    legacyAlphaMultiplier = 1.0f;
    PrepareLegacyModelVertexColourState();
    const bool xb01 = IsLegacyGameDisplayXB01(modelBytes, xb01Offset);
    if (xb01)
        legacyModelReferenceBoard = legacyModelColour;
    CommitFrameForRaster();
    if (xb01 && HasLegacyAlphaPassMatrix())
        PrepareLegacyAlphaPassMatrix();
    const std::uint32_t draws = xb01 ?
        ExecuteLegacyDrawCommandStreamInternal(modelBytes, xb01Offset, resources, LegacyModelShaderPath::VertexColour, 1.0f, false) :
        CommitLegacyModelMaterialCommands(modelBytes.data() + xb01Offset);
    if (xb01 && HasLegacyAlphaPassMatrix())
        RestoreLegacyAlphaPassSamplers();
    legacyAlphaPassEnabled = false;
    legacyVertexColourStateActive = false;
    temporaryModelScale = 1.0f;
    return draws;
}

// @verified - p
std::uint32_t ReadModelDataWithAlpha(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, float alpha) {
    Vector4 referenceBoard = legacyModelColour;
    if (alpha == 1.0f)
        return ModelFlagDispatch_ReadModelDataOpaque(modelBytes, xb01Offset, resources);
    if (!AdmitLegacyGameDisplayModel(modelBytes, xb01Offset)) {
        temporaryModelScale = 1.0f;
        return 0;
    }
    legacyAlphaMultiplier = alpha;
    legacyAlphaPassEnabled = true;
    PrepareLegacyModelVertexColourState();
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x3b, 1);
    const bool xb01 = IsLegacyGameDisplayXB01(modelBytes, xb01Offset);
    if (xb01) {
        referenceBoard.w = alpha;
        legacyModelReferenceBoard = referenceBoard;
    }
    CommitFrameForRaster();
    if (xb01 && HasLegacyAlphaPassMatrix())
        PrepareLegacyAlphaPassMatrix();
    const std::uint32_t draws = xb01 ?
        ExecuteLegacyDrawCommandStreamInternal(modelBytes, xb01Offset, resources, LegacyModelShaderPath::VertexColour, alpha, false) :
        CommitLegacyModelMaterialCommands(modelBytes.data() + xb01Offset);
    if (xb01 && HasLegacyAlphaPassMatrix())
        RestoreLegacyAlphaPassSamplers();
    RestoreEngineFlag(0x3b);
    RestoreEngineFlag(0x40);
    legacyAlphaPassEnabled = false;
    legacyVertexColourStateActive = false;
    temporaryModelScale = 1.0f;
    return draws;
}

std::uint32_t SetModelFlagDataWithAlpha(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, float alpha) {
    Vector4 referenceBoard = legacyModelColour;
    if (alpha == 1.0f)
        return ModelFlagDispatch_SetModelFlagDataOpaque(modelBytes, xb01Offset, resources);
    if (!AdmitLegacyGameDisplayModel(modelBytes, xb01Offset)) {
        temporaryModelScale = 1.0f;
        return 0;
    }
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x3b, 1);
    legacyAlphaMultiplier = alpha;
    legacyAlphaPassEnabled = true;
    PrepareLegacyModelLambertState();
    const bool xb01 = IsLegacyGameDisplayXB01(modelBytes, xb01Offset);
    if (xb01) {
        referenceBoard.w = alpha;
        legacyModelReferenceBoard = referenceBoard;
    }
    CommitFrameForRaster();
    if (xb01 && HasLegacyAlphaPassMatrix())
        PrepareLegacyAlphaPassMatrix();
    const std::uint32_t draws = xb01 ? ExecuteLegacyDrawCommandStreamInternal(modelBytes, xb01Offset, resources, LegacyModelShaderPath::Lambert, alpha, false) :
                                       CommitLegacyModelMaterialCommands(modelBytes.data() + xb01Offset);
    if (xb01 && HasLegacyAlphaPassMatrix())
        RestoreLegacyAlphaPassSamplers();
    RestoreEngineFlag(0x3b);
    RestoreEngineFlag(0x40);
    legacyAlphaPassEnabled = false;
    legacyVertexColourStateActive = false;
    temporaryModelScale = 1.0f;
    return draws;
}

void SetLegacyModelScale(float scale) {
    temporaryModelScale = scale;
}

float GetLegacyModelScale() {
    return temporaryModelScale;
}


std::uint32_t g_xb01Flag4MeshSeen{};
void NoteXb01Flag4Mesh() {
    if (g_xb01Flag4MeshSeen == 0)
        g_xb01Flag4MeshSeen = 1;
}

bool SphereVisCheck(const Vector3 &center, float radius, float clipRange) {
    const d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return false;
    const d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::int32_t left = TruncateToInt32(state.viewport[0]);
    const std::int32_t top = TruncateToInt32(state.viewport[1]);
    const std::uint32_t width = static_cast<std::uint32_t>(TruncateToInt32(state.viewport[2]));
    const std::uint32_t height = static_cast<std::uint32_t>(TruncateToInt32(state.viewport[3]));
    const float modelScale = GetLegacyModelScale();
    if (std::fabs(modelScale) < 0.000001f)
        return false;
    const float scale = modelScale * radius;
    Vector3 world{};
    Matrix4x4::TransformPoint(center, world);
    if (-world.z - scale > clipRange + 10.0f || scale - world.z < -32.0f)
        return false;
    Matrix4x4_Push(&transformMatrix_prePass);
    shenPushScreenOutputMatrixAndMultiply();
    const Matrix4x4 &matrix = workTransformMatrix;
    const Vector4 point{(matrix.rows[1].x * world.y + matrix.rows[0].x * world.x) + matrix.rows[2].x * world.z + matrix.rows[3].x,
        (matrix.rows[1].y * world.y + matrix.rows[0].y * world.x) + matrix.rows[2].y * world.z + matrix.rows[3].y,
        (matrix.rows[1].z * world.y + matrix.rows[0].z * world.x) + matrix.rows[2].z * world.z + matrix.rows[3].z,
        (matrix.rows[1].w * world.y + matrix.rows[0].w * world.x) + matrix.rows[2].w * world.z + matrix.rows[3].w};
    const float w = point.w < 0.0000001f ? point.w + 0.0000001f : point.w;
    const float reciprocal = 1.0f / w;
    const Vector3 projected{point.x * reciprocal, point.y * reciprocal, reciprocal};
    Matrix4x4::Pop(1);
    if (projected.z < 0.0f)
        return true;
    const float projectedRadius = scale * projected.z;
    return projected.x + projectedRadius > static_cast<float>(left) - 96.0f ||
        static_cast<float>(std::bit_cast<std::int32_t>(width + static_cast<std::uint32_t>(left))) + 96.0f > projected.x - projectedRadius ||
        projected.y + projectedRadius > static_cast<float>(top) - 96.0f ||
        static_cast<float>(std::bit_cast<std::int32_t>(height + static_cast<std::uint32_t>(top))) + 96.0f > projected.y - projectedRadius;
}

void SetLegacyProjectedShadowDirectionAndScale(const Vector3 &direction, float scale) {
    projectedShadowDirection = direction;
    projectedShadowModelDirection.w = scale;
}

std::uint32_t SetLegacyProjectedShadowStencilPartition(std::uint32_t partition) {
    if (partition <= 15)
        projectedShadowStencilPartition = partition;
    return projectedShadowStencilPartition;
}

bool BeginLegacyProjectedShadowPass() {
    if (perspectiveProjection == 1 || !d3t::DX11::g_ppD3DDeviceContext || !d3t::DX11::g_ppD3DDeviceContext->deviceContext || !g_projectedShadowVertexShader ||
        !g_fvfLayouts[13]) {
        return false;
    }
    auto &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    if (state.vertexShader != g_projectedShadowVertexShader) {
        state.vertexShader = g_projectedShadowVertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != 0) {
        state.pixelShader = 0;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[13]) {
        state.inputLayout = g_fvfLayouts[13];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (projectedShadowDebugEnabled)
        SetEngineFlag(0x3b, 1);
    else
        SetEngineFlag(0x43, 0);
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x34, 1);
    SetEngineFlag(0x39, 515);
    if (projectedShadowStencilPartition != 0) {
        SetEngineFlag(0x38, 517);
        SetEngineFlag(2, 224);
        SetEngineFlag(1, 32 * projectedShadowStencilPartition);
    }
    return true;
}

void EndLegacyProjectedShadowPass() {
    if (perspectiveProjection == 1)
        return;
    const auto &saved = savedProjectedShadowRenderSettings;
    SetEngineFlag(0x39, saved.depthFunction);
    SetEngineFlag(0x3b, saved.blendEnabled);
    SetEngineFlag(0x43, saved.colourWriteMask);
    SetEngineFlag(0x40, saved.depthWriteEnabled);
    SetEngineFlag(0x34, saved.stencilEnabled);
    SetEngineFlag(0x37, saved.stencilPass);
    SetEngineFlag(0x36, saved.stencilDepthFail);
    SetEngineFlag(0x93, saved.cullMode);
    if (projectedShadowStencilPartition != 0) {
        SetEngineFlag(0x38, saved.stencilFunction);
        SetEngineFlag(2, saved.stencilReadMask);
        SetEngineFlag(1, saved.stencilReference);
    }

}

std::uint32_t ExecuteProjectedShadowCommandStream(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources) {
#if !defined(_WIN32)
    static_cast<void>(modelBytes);
    static_cast<void>(xb01Offset);
    static_cast<void>(resources);
    return 0;
#else
    if (perspectiveProjection == 1 || !resources.prepared || !d3t::DX11::g_ppD3DDeviceContext || !d3t::DX11::g_ppD3DDeviceContext->deviceContext) {
        return 0;
    }
    std::uint32_t firstPacketOffset{};
    if (!ReadU32(modelBytes, xb01Offset + 24, firstPacketOffset))
        return 0;
    Matrix4x4::AffineProjectionSwap();
    projectedShadowModelDirection.x =
        (workTransformMatrix.rows[1].x * projectedShadowDirection.y + workTransformMatrix.rows[0].x * projectedShadowDirection.x) +
        workTransformMatrix.rows[2].x * projectedShadowDirection.z;
    projectedShadowModelDirection.y =
        (workTransformMatrix.rows[1].y * projectedShadowDirection.y + workTransformMatrix.rows[0].y * projectedShadowDirection.x) +
        workTransformMatrix.rows[2].y * projectedShadowDirection.z;
    projectedShadowModelDirection.z =
        (workTransformMatrix.rows[1].z * projectedShadowDirection.y + workTransformMatrix.rows[0].z * projectedShadowDirection.x) +
        workTransformMatrix.rows[2].z * projectedShadowDirection.z;
    Matrix4x4::AffineProjectionSwap();
    shaderParameters.constants[23] = projectedShadowModelDirection;
    CommitFrameForRaster();

    auto &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    std::uint32_t draws = 0;
    std::size_t packet = static_cast<std::size_t>(xb01Offset) + (static_cast<std::size_t>(firstPacketOffset) + 6U) * 4U;
    while (Contains(modelBytes, packet, 4)) {
        const std::uint32_t packetHeader = ReadValue<std::uint32_t>(modelBytes, packet);
        if (!packetHeader)
            break;
        const std::uint32_t packetDwords = packetHeader >> 8U;
        if (packetDwords < 5 || packetDwords > (modelBytes.size() - packet) / 4U)
            break;
        const std::size_t packetEnd = packet + static_cast<std::size_t>(packetDwords) * 4U;
        for (std::size_t command = packet + 20; command < packetEnd;) {
            const std::uint32_t header = ReadValue<std::uint32_t>(modelBytes, command);
            const auto opcode = static_cast<std::uint8_t>(header);
            if (!opcode)
                break;
            // Bind opcodes return the pointer after their records, whereas
            // opcode 15 always skips sixteen bytes. Only the other opcodes
            // advance by their encoded DWORD length.
            std::uint32_t lengthDwords = opcode == 15 ? 4 : opcode == 19 ? 3 : header >> 8U;
            std::uint32_t streamCount = 0;
            if (opcode == 1) {
                if (packetEnd - command < 8)
                    return draws;
                streamCount = ReadValue<std::uint32_t>(modelBytes, command + 4);
                if (streamCount > (packetEnd - command - 8) / 8U)
                    return draws;
                lengthDwords = 2 + 2 * streamCount;
            }
            if (!lengthDwords || lengthDwords > (packetEnd - command) / 4U)
                return draws;
            if (opcode == 1) {
                for (std::uint32_t index = 0; index < streamCount; ++index) {
                    const std::size_t record = command + 8 + 8ULL * index;
                    BindLegacyVertexStream(state, resources, ReadValue<std::uint32_t>(modelBytes, record), ReadValue<std::uint32_t>(modelBytes, record + 4));
                }
            } else if (opcode == 16) {
                const auto offset = static_cast<std::uint32_t>(command - xb01Offset);
                SetEngineFlag(0x36, 7682);
                SetEngineFlag(0x93, 2305);
                draws += DrawLegacyIndexedCommand(resources, offset);
                SetEngineFlag(0x36, 7683);
                SetEngineFlag(0x93, 2304);
                draws += DrawLegacyIndexedCommand(resources, offset);
            } else if (opcode == 19) {
                BindLegacyVertexStream(state, resources, ReadValue<std::uint32_t>(modelBytes, command + 4), ReadValue<std::uint32_t>(modelBytes, command + 8));
            }
            command += static_cast<std::size_t>(lengthDwords) * 4U;
        }
        packet = packetEnd;
    }
    return draws;
#endif
}

#if defined(_WIN32)
void ApplyLegacyEffectRenderModes(d3t::D3TRenderState &state, const LegacyEffectGpuFlags &gpuFlags, std::uint32_t texture);
#endif

bool BeginSmokeDraw(std::uint32_t count, std::uint32_t emitterFlags, std::uint32_t texture, const LegacyEffectGpuFlags &gpuFlags) {
#if defined(_WIN32)
    if (smokeDraw.mapped || !count || count > std::numeric_limits<std::uint32_t>::max() / 6U) {
        return false;
    }
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    const std::uint32_t vertexCount = 6U * count;
    if (!context || vertexCount > std::numeric_limits<std::uint32_t>::max() / sizeof(SmokeGpuVertex)) {
        return false;
    }
    if (!GetLegacyRenderMask()) {
        const Matrix4x4 *const cameraProjection = g_matrixStack.entries[2].stack;
        if (!cameraProjection)
            return false;
        Matrix4x4 screenOutput{};
        screenOutput.rows[0].x = 320.0f;
        screenOutput.rows[1].y = -240.0f;
        screenOutput.rows[2].z = 1.0f;
        screenOutput.rows[3] = {320.0f, 240.0f, 0.0f, 1.0f};
        smokeProjection = MultiplyEffectMatrices(*cameraProjection, screenOutput);
    }
    const bool compact = (emitterFlags & 0x20000000U) != 0;
    const std::uint32_t vertexShader = compact ? g_fvfPdVertexShader : g_fvfPtdTVertexShader;
    const std::uint32_t pixelShader = compact ? g_fvfPdPixelShader : g_fvfPtdTPixelShader;
    const std::uint32_t inputLayout = g_fvfLayouts[compact ? 26U : 28U];
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.vertexShader != vertexShader) {
        state.vertexShader = vertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != pixelShader) {
        state.pixelShader = pixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != inputLayout) {
        state.inputLayout = inputLayout;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    ApplyLegacyEffectRenderModes(state, gpuFlags, texture);
    auto *const mapped = static_cast<SmokeGpuVertex *>(d3t::GetMappedConstantBuffer(context, 0, vertexCount * sizeof(SmokeGpuVertex)));
    if (!mapped)
        return false;
    // ParticleSystem's SetRenderPass argument: emitter bit 30 selects pass one.
    smokeDraw = {.mapped = mapped,
        .capacity = count,
        .count = 0,
        .texture = texture,
        .renderPass = (emitterFlags & 0x40000000U) ? 1U : 0U,
        .compact = (emitterFlags & 0x20000000U) != 0,
        .gpuFlags = gpuFlags,
        .projection = smokeProjection};
    return true;
#else
    static_cast<void>(count);
    static_cast<void>(emitterFlags);
    static_cast<void>(texture);
    static_cast<void>(gpuFlags);
    return false;
#endif
}

bool AppendSmokeBillboard(const Matrix4x4 &worldTransform, const Vector3 &position, std::uint16_t angle, float width, float height, std::uint32_t colour) {
    if (!smokeDraw.mapped || smokeDraw.count >= smokeDraw.capacity) {
        return false;
    }

    const Vector4 centre = TransformEffectPoint(worldTransform, {position.x, position.y, position.z, 1.0f});
    constexpr float fixedAngleToRadians = 6.28318530717958647692f / 65536.0f;
    const float radians = static_cast<float>(angle) * fixedAngleToRadians;
    const float sine = std::sin(radians);
    const float cosine = std::cos(radians);

    // ParticleSystem's selected billboard renderer first moves the particle into
    // incoming camera space, then discards the incoming XYZ basis so the quad faces the camera.
    Matrix4x4 billboard = transformMatrix_prePass;
    billboard.rows[0].w = worldTransform.rows[0].w;
    billboard.rows[1].w = worldTransform.rows[1].w;
    billboard.rows[2].w = worldTransform.rows[2].w;
    billboard.rows[3] = centre;
    const Matrix4x4 billboardBase = billboard;
    billboard.rows[0] = TransformEffectPoint(billboardBase, {cosine, sine, 0.0f, billboard.rows[0].w});
    billboard.rows[1] = TransformEffectPoint(billboardBase, {-sine, cosine, 0.0f, billboard.rows[1].w});
    // ParticleSystem's selected billboard renderer publishes transformMatrix_prePass_1
    // as camera stack entry two followed by the fixed 640x480 screen-output matrix.
    billboard = MultiplyEffectMatrices(billboard, smokeDraw.projection);
    const auto corner = [&](float horizontal, float vertical) { return TransformEffectPoint(billboard, {horizontal, vertical, 0.0f, 1.0f}); };
    Vector4 topLeft = corner(-width, height);
    Vector4 bottomLeft = corner(-width, -height);
    Vector4 topRight = corner(width, height);
    Vector4 bottomRight = corner(width, -height);
    if (topLeft.w <= 0.0f || bottomLeft.w <= 0.0f || topRight.w <= 0.0f || bottomRight.w <= 0.0f) {
        return false;
    }
    const auto perspectiveDivide = [](Vector4 &point) {
        const float reciprocalW = 1.0f / point.w;
        point = {point.x * reciprocalW, point.y * reciprocalW, point.z * reciprocalW, reciprocalW};
    };
    perspectiveDivide(topLeft);
    perspectiveDivide(bottomLeft);
    perspectiveDivide(topRight);
    perspectiveDivide(bottomRight);
    SmokeGpuVertex *const vertices = smokeDraw.mapped + 6U * smokeDraw.count;
    vertices[0] = {topLeft, colour, 0.0f, 0.0f};
    vertices[1] = {topRight, colour, 1.0f, 0.0f};
    vertices[2] = {bottomRight, colour, 1.0f, 1.0f};
    vertices[3] = {topLeft, colour, 0.0f, 0.0f};
    vertices[4] = {bottomRight, colour, 1.0f, 1.0f};
    vertices[5] = {bottomLeft, colour, 0.0f, 1.0f};
    ++smokeDraw.count;
    return true;
}

#if defined(_WIN32)
void ApplyLegacyEffectRenderModes(d3t::D3TRenderState &state, const LegacyEffectGpuFlags &gpuFlags, std::uint32_t texture) {
    SetEngineFlag(0x67, (gpuFlags.textureFlags >> 2U) & 1U);
    SetEngineFlag(0x93, (gpuFlags.samplerControl & 0x18000000U) ? 2305U : 0U);
    constexpr std::array<std::uint32_t, 6> alphaArguments{1U, 3U, 0U, 0U, 2U, 3U};
    const std::uint32_t packed = gpuFlags.gpuFlags;
    SetEngineFlag(0x40, (gpuFlags.samplerControl & 0x04000000U) == 0);
    constexpr std::array<std::uint32_t, 8> sourceBlendArguments{
        1U, 0U, 774U, 775U, 770U, 771U, 772U, 773U};
    constexpr std::array<std::uint32_t, 8> destinationBlendArguments{
        1U, 0U, 768U, 769U, 770U, 771U, 772U, 773U};
    SetEngineFlag(0x3e, sourceBlendArguments[packed >> 29U]);
    SetEngineFlag(0x3f, destinationBlendArguments[(packed >> 26U) & 7U]);
    SetAlphaFlags(0, 0, alphaArguments[(packed >> 16U) & 5U]);
    SetAlphaFlags(0, 1, alphaArguments[(packed >> 15U) & 5U]);
    switch ((packed >> 13U) & 3U) {
    case 0:
        SetAlphaFlags(0, 3, 1);
        SetAlphaFlags(0, 4, 1);
        SetAlphaFlags(0, 5, 1);
        break;
    case 2:
        SetAlphaFlags(0, 3, 2);
        SetAlphaFlags(0, 4, 2);
        SetAlphaFlags(0, 5, 1);
        break;
    default:
        SetAlphaFlags(0, 3, 2);
        SetAlphaFlags(0, 4, 2);
        SetAlphaFlags(0, 5, 2);
        break;
    }
    switch ((packed >> 6U) & 3U) {
    case 0:
        SetAlphaFlags(0, 0x10, 3);
        SetAlphaFlags(0, 0x13, 2);
        break;
    case 1:
        SetAlphaFlags(0, 0x10, 2);
        SetAlphaFlags(0, 0x12, 2);
        break;
    case 2:
        SetAlphaFlags(0, 0x10, 3);
        SetAlphaFlags(0, 0x13, 0);
        break;
    case 3:
        SetAlphaFlags(0, 0x10, 4);
        SetAlphaFlags(0, 0x11, 2);
        SetAlphaFlags(0, 0x12, 0);
        break;
    }
    if ((gpuFlags.textureFlags & 8U) && texture)
        BindPixelTexture0(state, ResolveEffectTextureSource(texture, perspectiveProjection));
    else
        ClearPixelShaderEffectSRVSlot(state, 0);
}

void RestoreLegacyEffectRenderFlags(d3t::D3TRenderState &state) {
    // SetParticleRenderFlags restores the InitShenmueRendering/InitShenmueGlobals saved settings.
    ClearPixelShaderEffectSRVSlot(state, 0);
    SetEngineFlag(0x93, 2304);
    SetEngineFlag(0x67, 1);
    SetEngineFlag(0x64, 1);
    SetEngineFlag(0x39, 513);
    SetEngineFlag(0x40, 1);
    SetEngineFlag(0x3e, 770);
    SetEngineFlag(0x3f, 771);
    SetEngineFlag(0x42, 0);
    SetAlphaFlags(0, 0x10, 4);
    SetAlphaFlags(0, 0x12, 2);
    SetAlphaFlags(0, 0x13, 0);
    SetAlphaFlags(0, 0, 3);
    SetAlphaFlags(0, 1, 3);
    SetAlphaFlags(0, 3, 2);
    SetAlphaFlags(0, 4, 2);
    SetAlphaFlags(0, 5, 2);
}
#endif


// @verified - p
void DecodeLegacyEffectStrip(const std::byte *&stream, std::span<LegacyEffectVertex> output, float uvScale) {
    for (LegacyEffectVertex &vertex : output) {
        std::int32_t encoded;
        std::memcpy(&encoded, stream, sizeof(encoded));
        const bool inlineVertex = (encoded & 1) != 0;
        const std::byte *const source = inlineVertex ? stream : stream - 4LL * (encoded / 4);
        std::memcpy(&vertex.position, source, sizeof(vertex.position));
        std::memcpy(&vertex.diffuse, source + 28, sizeof(vertex.diffuse));
        std::memcpy(&vertex.specular, source + 32, sizeof(vertex.specular));
        std::uint16_t u;
        std::uint16_t v;
        std::memcpy(&u, source + 24, sizeof(u));
        std::memcpy(&v, source + 26, sizeof(v));
        vertex.v = static_cast<float>(v) * uvScale;
        vertex.u = static_cast<float>(u) * uvScale;
        stream += inlineVertex ? 36 : 4;
    }
}

namespace {
HikiModelEntity *currentEffectEntity{};
std::int32_t effectEntityScratch{};
float effectUvScale{};

void NullLegacyEffectCommand(const std::byte *) {
}

// @verified - p
void SetLegacyEffectWorldScale(const std::byte *command) {
    std::int32_t divisor;
    std::memcpy(&divisor, command + 4, sizeof(divisor));
    effectUvScale = 1.0f / static_cast<float>(divisor);
}

void SubmitLegacyEffectCommand(const std::byte *command) {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const LegacyEffectGpuFlags &flags = *reinterpret_cast<const LegacyEffectGpuFlags *>(command + 4);
    std::uint32_t texture;
    std::memcpy(&texture, command + 16, sizeof(texture));
    ApplyLegacyEffectRenderModes(state, flags, texture);
    SetEngineFlag(0x40, 0);
    std::int32_t stripCount;
    std::memcpy(&stripCount, command + 20, sizeof(stripCount));
    const std::byte *stream = command + 24;
    while (stripCount != 0) {
        std::int32_t signedCount;
        std::memcpy(&signedCount, stream, sizeof(signedCount));
        stream += 4;
        --stripCount;
        const std::uint32_t vertexCount = static_cast<std::uint32_t>(signedCount < 0 ? -signedCount : signedCount);
        if (state.vertexShader != g_fvfPtdVertexShader) {
            state.vertexShader = g_fvfPtdVertexShader;
            state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
            state.vertexBufferDirtyMask = ~std::uint32_t{0};
        }
        if (state.pixelShader != g_fvfPtdPixelShader) {
            state.pixelShader = g_fvfPtdPixelShader;
            state.pipelineStateFlags |= d3t::PipelinePixelShader;
        }
        if (state.inputLayout != g_fvfLayouts[24]) {
            state.inputLayout = g_fvfLayouts[24];
            state.pipelineStateFlags |= d3t::PipelineInputLayout;
        }
        LegacyEffectVertex *const mapped =
            static_cast<LegacyEffectVertex *>(d3t::GetMappedConstantBuffer(context, 0, vertexCount * sizeof(LegacyEffectVertex)));
        if (!mapped)
            return;
        DecodeLegacyEffectStrip(stream, {mapped, vertexCount}, effectUvScale);
        SetEngineFlag(0x64, 1);
        if (vertexCount) {
            SetEngineFlag(0x3b, 1);
            if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
                state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
                state.bufferStateFlags |= d3t::BufferTopology;
            }
            UpdateShaderParams();
            d3t::DX11_ResetMappedResources(context);
            d3t::DX11_UnmapAndSyncShaderBindings(context);
            d3t::DX11_SetupShadersForDXAndSetViewports(context);
            static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(vertexCount, 0);
            SetEngineFlag(0x3b, 0);
        }
    }
    RestoreLegacyEffectRenderFlags(state);
#else
    static_cast<void>(command);
#endif
}

constexpr std::array<void (*)(const std::byte *), 8> effectDrawFunctions{SubmitLegacyEffectCommand, SubmitLegacyEffectCommand, SubmitLegacyEffectCommand,
    SubmitLegacyEffectCommand, SubmitLegacyEffectCommand, SubmitLegacyEffectCommand, NullLegacyEffectCommand, SetLegacyEffectWorldScale};

void DrawLegacyEffectCommands() {
    Vector3 centre;
    Matrix4x4::TransformPoint(currentEffectEntity->centre, centre);
    temporaryModelScale = 1.0f;
    if (!(g_projectionNearClip <= currentEffectEntity->clipRadius - centre.z && -centre.z - currentEffectEntity->clipRadius <= g_projectionFarClip))
        return;
    Matrix4x4_Push();
    CommitFrameForRaster();
    effectUvScale = 0.00390625f;
    const std::byte *command = currentEffectEntity->chunkOffset ?
        static_cast<const std::byte *>(HLib::GetPointerFromDynamicHeapOffsetBase(currentEffectEntity->chunkOffset)) :
        nullptr;
    std::uint32_t header;
    std::memcpy(&header, command, sizeof(header));
    while ((header & 0xffU) != 0xffU) {
        const std::uint32_t type = header & 0xffU;
        effectDrawFunctions[type](command);
        command += static_cast<std::int32_t>(header) >> 8;
        std::memcpy(&header, command, sizeof(header));
    }
    Matrix4x4::Pop(1);
}
} // namespace

// @verified - p
void DrawLegacyEffectEntity(HikiModelEntity &entity) {
    currentEffectEntity = &entity;
    effectEntityScratch = 0;
    DrawLegacyEffectCommands();
}

// @verified - p
void LegacySpriteNullsub(const LegacySpritePacket &packet, std::uint32_t texture, float depth) {
    static_cast<void>(packet);
    static_cast<void>(texture);
    static_cast<void>(depth);
}

void EndSmokeDraw() {
#if defined(_WIN32)
    if (!smokeDraw.mapped)
        return;
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    const SmokeDrawState draw = smokeDraw;
    smokeDraw = {};
    if (!context || !context->deviceContext)
        return;

    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleList)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    if (draw.compact)
        SetEngineFlag(0x66, 0);
    // RenderPassFuncs[3 * pass]. Pass one brackets its draw with blending on and depth writes off, then
    // restores InitShenmueRendering's saved blend (0) and depth-write (1) settings.
    if (draw.count) {
        if (draw.renderPass == 1) {
            SetEngineFlag(0x40, 0);
            SetEngineFlag(0x3b, 1);
        }
        UpdateShaderParams();
        d3t::DX11_ResetMappedResources(context);
        d3t::DX11_UnmapAndSyncShaderBindings(context);
        d3t::DX11_SetupShadersForDXAndSetViewports(context);
        static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(6U * draw.count, 0);
        if (draw.renderPass == 1) {
            SetEngineFlag(0x3b, 0);
            SetEngineFlag(0x40, 1);
        }
    } else {
        d3t::DX11_ResetMappedResources(context);
        d3t::DX11_UnmapAndSyncShaderBindings(context);
    }
    RestoreLegacyEffectRenderFlags(state);
#endif
}

bool QueueFireQuad(const LegacyFireQuad &quad) {
    if (!quad.width || !quad.height || fireQuadCount >= fireQuads.size() || legacyQueuedDrawCount >= legacyQueuedDrawCapacity ||
        legacyRenderPacketCount >= legacyRenderPacketCapacity) {
        return false;
    }
#if defined(_WIN32)
    if (!d3t::DX11::g_ppD3DDeviceContext || !d3t::DX11::g_ppD3DDeviceContext->deviceContext || !g_fireVertexShader || !g_fvfPtdPixelShader ||
        !g_fvfLayouts[22] || !EnsureFireVertexBuffer()) {
        return false;
    }
#else
    return false;
#endif
    if (GetLegacyRenderMask())
        return false;
    const float depth = -quad.worldTransform.rows[3].z;
    if (g_cameraControl && (depth > g_cameraControl->farDistance || depth < g_cameraControl->nearDistance)) {
        return false;
    }
    const float radius = std::sqrt((quad.width * 0.5f) * (quad.width * 0.5f) + (quad.height * 0.5f) * (quad.height * 0.5f));
    const Vector4 centreClip = TransformEffectPoint(stripProjection, quad.worldTransform.rows[3]);
    Vector4 radiusPoint = quad.worldTransform.rows[3];
    radiusPoint.x += radius;
    const Vector4 radiusClip = TransformEffectPoint(stripProjection, radiusPoint);
    if (centreClip.w <= 0.0f || radiusClip.w <= 0.0f)
        return false;
    const float screenWidth = static_cast<float>(d3t::g_originalScreenWidth);
    const float screenHeight = static_cast<float>(d3t::g_originalScreenHeight);
    const float centreX = (centreClip.x / centreClip.w + 1.0f) * screenWidth * 0.5f;
    const float centreY = (1.0f - centreClip.y / centreClip.w) * screenHeight * 0.5f;
    const float screenRadius = (radiusClip.x / radiusClip.w + 1.0f) * screenWidth * 0.5f - centreX;
    if (centreX + screenRadius < 0.0f || centreX - screenRadius > screenWidth || centreY < 0.0f || centreY - 2.0f * screenRadius > screenHeight) {
        return false;
    }
    LegacyFireQuad &queued = fireQuads[fireQuadCount++];
    queued = quad;
    queued.alpha = std::clamp(queued.alpha, 0.0f, 1.0f);
    Matrix4x4 billboard = quad.worldTransform;
    billboard.rows[0] = {quad.width, 0.0f, 0.0f, 0.0f};
    billboard.rows[1] = {0.0f, quad.height, 0.0f, 0.0f};
    billboard.rows[2] = {0.0f, 0.0f, 1.0f, 0.0f};
    const Matrix4x4 saved = workTransformMatrix;
    workTransformMatrix = billboard;
    const std::uint32_t before = legacyQueuedDrawCount;
    SetUpdateCallback(DrawQueuedFireQuad, &queued, billboard.rows[3].z);
    workTransformMatrix = saved;
    if (legacyQueuedDrawCount == before) {
        --fireQuadCount;
        return false;
    }
    return true;
}

void SetUpdateCallback(LegacyUpdateCallback callback, void *data, float priority) {
    if (!callback || legacyQueuedDrawCount >= legacyQueuedDrawCapacity)
        return;
    LegacyRenderPacketState *const packet = CaptureLegacyRenderPacketState();
    if (!packet)
        return;
    legacyQueuedDrawSortKeys[legacyQueuedDrawCount] = priority;
    LegacyQueuedDraw &entry = legacyQueuedDraws[legacyQueuedDrawCount++];
    entry.packet = packet;
    entry.type = 2;
    entry.callback = callback;
    entry.callbackData = data;
    ++legacyQueuedCallbackCount;
}

namespace {

const LegacyQueuedDraw *FindQueuedLegacyModelView(const std::byte *model) {
    for (std::uint32_t index = 0; index < legacyQueuedDrawCount; ++index) {
        const LegacyQueuedDraw &entry = legacyQueuedDraws[index];
        if ((entry.type == 1 || entry.type == 4) && entry.resources && entry.xb01Offset < entry.modelBytes.size() &&
            entry.modelBytes.data() + entry.xb01Offset == model)
            return &entry;
    }
    return nullptr;
}

void RestoreLegacyQueuedShaderState(const LegacyRenderPacketState &packet) {
    const bool pushed = Matrix4x4_Push();
    Matrix4x4 *const transparentCell = g_matrixStack.entries[2].stack;
    const Matrix4x4 transparent = *transparentCell;
    Matrix4x4::SetActiveTransform(&packet.worldMatrix);
    CommitFrameForRaster();
    *transparentCell = transparent;
    if (packet.flags & 4U) {
        legacyVertexColourStates = packet.savedVertexColourState;
        PrepareLegacyModelVertexColourState();
    } else {
        legacyVertexColourStates[0] = packet.savedVertexColourState[0];
        modelReferenceDirection = legacyVertexColourStates[0].direction;
        PrepareLegacyModelLambertState();
    }
    modelLightVector = packet.lightDir;
    particleLightColour = packet.modelLight;
    shaderParameters.constants[12] = {packet.modelLight.x, packet.modelLight.y, packet.modelLight.z, 1.0f};
    shaderParameters.constants[13] = {packet.modelRef.x, packet.modelRef.y, packet.modelRef.z, 1.0f};
    legacyDepthFadeNear = packet.depthFade.x;
    legacyDepthFadeFar = packet.depthFade.y;
    legacyDepthFadeIntensity = packet.depthFade.z;
    UpdateLegacyDepthFadeConstants();
    legacyMirroredDrawMode = (packet.flags >> 5U) & 1U;
    legacyModelFogEnabled = (packet.flags >> 4U) & 1U;
    if (pushed)
        Matrix4x4::Pop(1);
}

} // namespace

std::uint32_t RenderQueuedLegacyShaderPasses() {
    const LegacyRenderPacketState *previous = nullptr;
    std::uint32_t draws = 0;
    if (!legacyQueuedShaderCount)
        return draws;
    const std::uint32_t *const indices =
        RadixSortIndicesBy4ByteKey(legacyQueuedShaderSortKeys, legacyQueuedShaderCount, legacySortedQueueIndices, legacySortScratch);
    for (std::uint32_t index = 0; index < legacyQueuedShaderCount; ++index) {
        const LegacyQueuedShaderEntry &entry = legacyQueuedShaderEntries[indices[index]];
        if (entry.packet && previous != entry.packet) {
            RestoreLegacyQueuedShaderState(*entry.packet);
            previous = entry.packet;
        }
        const LegacyQueuedDraw *const view = FindQueuedLegacyModelView(entry.model);
        if (!view) {
            ++unboundLegacyShaderModels;
            continue;
        }
        CommitFrameForRaster();
        if (HasLegacyAlphaPassMatrix())
            PrepareLegacyAlphaPassMatrix();
        draws += ExecuteLegacyDrawCommandStreamInternal(view->modelBytes, view->xb01Offset, *view->resources,
            legacyVertexColourStateActive ? LegacyModelShaderPath::VertexColour : LegacyModelShaderPath::Lambert, legacyAlphaMultiplier, false);
        if (HasLegacyAlphaPassMatrix())
            RestoreLegacyAlphaPassSamplers();
    }
    return draws;
}

// @verified - p
void AssignGeoLayout(std::span<const std::byte> modelBytes, std::uint32_t xb01Offset, const LegacyDrawResources &resources, std::int32_t flags) {
    legacyAlphaPassEnabled = 0;
    if (xb01Offset >= modelBytes.size() || legacyQueuedDrawCount >= legacyQueuedDrawCapacity)
        return;
    LegacyRenderPacketState *const packet = CaptureLegacyRenderPacketState();
    if (!packet)
        return;
    legacyQueuedDrawSortKeys[legacyQueuedDrawCount] = 0.0f;
    LegacyQueuedDraw &entry = legacyQueuedDraws[legacyQueuedDrawCount++];
    entry.packet = packet;
    entry.type = 4;
    entry.xb01Offset = xb01Offset;
    entry.command = modelBytes.data() + xb01Offset;
    entry.resources = &resources;
    entry.modelBytes = modelBytes;
    static_cast<void>(flags);
}

// @verified - p
std::uint32_t GetTextureMappedData() {
    if (!legacyModelQueryBatchReady)
        return 0;
#if defined(_WIN32)
    return static_cast<std::uint32_t>(d3t::ReadOcclusionQuerySamples((legacyModelQueryIndex + 1) % 3));
#else
    return 0;
#endif
}

// @verified - p
std::uint32_t ComputeResolutionScaledIntensity() {
    const std::uint32_t samples = GetTextureMappedData();
    float width{}, height{};
    if (!d3t::GetSceneFrameTargetSize(&width, &height))
        return 0;
    const std::uint32_t area = static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(height);
    const float scaled = (307200.0f / static_cast<float>(area)) * static_cast<float>(static_cast<std::int32_t>(samples));
    return static_cast<std::uint32_t>(scaled >= -2147483648.0f && scaled < 2147483648.0f ? static_cast<std::int32_t>(scaled) : INT32_MIN);
}

void BeginAndEndLegacyModelQuery(const std::byte *model) {
    legacyRenderQueueEnabled = false;
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x43, 0);
#if defined(_WIN32)
    if (d3t::BeginOcclusionQuery(legacyModelQueryIndex)) {
        std::uint32_t signature{};
        std::memcpy(&signature, model, sizeof(signature));
        if (signature == SHEN_NAME(XB01)) {
            const LegacyQueuedDraw *const view = FindQueuedLegacyModelView(model);
            if (!view)
                ++unboundLegacyQueryModels;
            else {
                std::uint32_t packetDwordOffset{};
                ReadU32(view->modelBytes, view->xb01Offset + 24, packetDwordOffset);
                std::size_t offset = view->xb01Offset + (static_cast<std::size_t>(packetDwordOffset) + 6) * 4;
                for (;;) {
                    std::uint32_t header{};
                    if (!ReadU32(view->modelBytes, offset, header) || !header)
                        break;
                    const std::uint32_t length = header >> 8U;
                    if (!length || length > (view->modelBytes.size() - offset) / 4)
                        break;
                    if (header & 1U)
                        SetEngineFlag(0x3b, 1);
                    ExecuteLegacyDrawPacket(view->modelBytes, view->xb01Offset, *view->resources, offset);
                    if (header & 1U)
                        SetEngineFlag(0x3b, 0);
                    offset += static_cast<std::size_t>(length) * 4;
                }
            }
        } else {
            ExecuteModelRenderCommandStream(model + 24);
            RestoreLegacyCpuMaterialFlags();
        }
        d3t::EndOcclusionQuery(legacyModelQueryIndex++);
        if (legacyModelQueryIndex >= 3) {
            legacyModelQueryBatchReady = 1;
            legacyModelQueryIndex = 0;
        }
    }
#endif
    RestoreEngineFlag(0x40);
    RestoreEngineFlag(0x43);
    legacyRenderQueueEnabled = true;
}

namespace {
struct LegacyFlagDebugQuadCompatibility {
    std::array<std::uint32_t, 128> commands{};
    LegacyDrawResources resources;
};
std::vector<LegacyFlagDebugQuadCompatibility *> flagDebugCompatibilityDraws;
std::uint32_t flagDebugCompatibilityReplayDepth{};
void ReleaseLegacyFlagDebugCompatibilityDraws() {
    if (flagDebugCompatibilityReplayDepth || legacyQueuedDrawCount)
        return;
    for (LegacyFlagDebugQuadCompatibility *draw : flagDebugCompatibilityDraws) {
        ReleaseLegacyDrawResources(draw->resources);
        delete draw;
    }
    flagDebugCompatibilityDraws.clear();
}
void RetireLegacyFlagDebugCompatibilityResources() {
    if (flagDebugCompatibilityReplayDepth || flagDebugCompatibilityDraws.empty())
        return;
    if (legacyQueuedDrawCount)
        FlushQueuedLegacyDrawPasses();
    ReleaseLegacyFlagDebugCompatibilityDraws();
}
}




std::uint32_t SubmitLegacyFlagDebugQuadCompatibility(std::span<const std::byte> nativeModel) {
    if (nativeModel.size() != 248)
        return 0;
    LegacyFlagDebugQuadCompatibility *const draw = new LegacyFlagDebugQuadCompatibility;
    std::array<std::uint32_t, 64> translated{};
    std::memcpy(translated.data(), nativeModel.data(), 40);
    std::memcpy(translated.data() + 12, nativeModel.data() + 40, nativeModel.size() - 40);
    const LegacyMaterialCommand &material = *reinterpret_cast<const LegacyMaterialCommand *>(translated.data() + 6);
    std::array<std::uint32_t, 128> &words = draw->commands;
    words[0] = xb01Signature;
    words[6] = 2;
    const std::uint32_t packet = 8;
    std::uint32_t cursor = packet + 5;
    std::memcpy(words.data() + packet + 1, &material.sortPosition, sizeof(Vector3));
    words[cursor++] = 0x202;
    words[cursor++] = 0;
    words[cursor++] = 0xf04;
    for (unsigned index = 0; index < 4; ++index)
        words[cursor++] = std::bit_cast<std::uint32_t>(1.0f);
    words[cursor++] = std::bit_cast<std::uint32_t>(material.colourScale * material.colour.x);
    words[cursor++] = std::bit_cast<std::uint32_t>(material.colourScale * material.colour.y);
    words[cursor++] = std::bit_cast<std::uint32_t>(material.colourScale * material.colour.z);
    words[cursor++] = std::bit_cast<std::uint32_t>(material.colourAlpha);
    for (unsigned index = 0; index < 4; ++index)
        words[cursor++] = std::bit_cast<std::uint32_t>(1.0f);
    words[cursor++] = 0;
    words[cursor++] = std::bit_cast<std::uint32_t>(material.colourAlpha);
    words[cursor++] = 0x100a;
    words[cursor++] = 7;
    const std::uint32_t fog = (material.blendFlags & 0xc00000U) == 0x800000U ? 0U : legacySceneFogEnabled & legacyModelFogEnabled;
    const std::uint32_t states[][2]{{0x5c, fog}, {0x39, LEGACY_MATERIAL_DEPTH_ARGUMENTS[material.renderFlags >> 29U]},
        {0x3b, LEGACY_MATERIAL_BLEND_ARGUMENTS[(material.blendFlags >> 19U) & 3U]}, {0x3e, LEGACY_MATERIAL_SOURCE_BLEND_ARGUMENTS[material.blendFlags >> 29U]},
        {0x3f, LEGACY_MATERIAL_DESTINATION_BLEND_ARGUMENTS[(material.blendFlags >> 26U) & 7U]},
        {0x95, std::uint32_t(material.depthBias - 1) <= 15U ? material.depthBias : 0U}, {0x93, 0}};
    for (const std::uint32_t *state : states) {
        words[cursor++] = state[0];
        words[cursor++] = state[1];
    }
    words[cursor++] = 0x80e;
    words[cursor++] = 3;
    words[cursor++] = 0;
    words[cursor++] = LEGACY_MATERIAL_ALPHA_ARGUMENTS[(material.blendFlags >> 16U) & 5U];
    words[cursor++] = 0x100;
    words[cursor++] = LEGACY_MATERIAL_ALPHA_ARGUMENTS[(material.blendFlags >> 15U) & 5U];
    words[cursor++] = 0x600;
    words[cursor++] = std::bit_cast<std::uint32_t>(material.alphaSlot6);
    words[cursor++] = 0x401;
    words[cursor++] = 1;
    words[cursor++] = 0;
    words[cursor++] = 0;
    words[cursor++] = 0x510;
    words[cursor++] = 6;
    words[cursor++] = 4;
    words[cursor++] = 0x00010000;
    words[cursor++] = 0x00030002;
    words[packet] = ((cursor - packet) << 8U) | 1U;
    words[cursor++] = 0;
    words[7] = cursor - 7;
    words[cursor++] = 0x2401;
    words[cursor++] = 1;
    words[cursor++] = 0x01120020;
    words[cursor++] = 128;
    std::memcpy(words.data() + cursor, nativeModel.data() + 112, 128);
    cursor += 32;
    words[cursor] = 0;
    const std::span<const std::byte> bytes = std::as_bytes(std::span(words));
    if (!PrepareLegacyDrawResources(bytes, 0, draw->resources)) {
        delete draw;
        return 0;
    }
    const MatrixStackTable savedStacks = g_matrixStack;
    const std::uint8_t savedStack = g_CurrentMatrixStackIdx;
    const Matrix4x4 savedWork = workTransformMatrix;
    Matrix4x4 *const cells[]{g_matrixStack.top.stack, g_matrixStack.entries[2].stack, g_matrixStack.entries[2].stack + 1};
    const Matrix4x4 savedCells[]{*cells[0], *cells[1], *cells[2]};
    legacyAlphaPassEnabled = false;
    legacyAlphaMultiplier = 1.0f;
    PrepareLegacyModelLambertState();
    CommitFrameForRaster();
    for (unsigned index = 0; index < 3; ++index)
        *cells[index] = savedCells[index];
    g_matrixStack = savedStacks;
    g_CurrentMatrixStackIdx = savedStack;
    workTransformMatrix = savedWork;
    const std::uint32_t queued = legacyQueuedDrawCount;
    const std::uint32_t immediate = ExecuteLegacyDrawCommandStreamInternal(bytes, 0, draw->resources, LegacyModelShaderPath::Lambert, 1.0f, false);
    legacyAlphaPassEnabled = false;
    legacyVertexColourStateActive = false;
    temporaryModelScale = 1.0f;
    const std::uint32_t retained = legacyQueuedDrawCount - queued;
    if (retained)
        flagDebugCompatibilityDraws.push_back(draw);
    else {
        ReleaseLegacyDrawResources(draw->resources);
        delete draw;
    }
    return immediate + retained;
}

std::uint32_t RenderQueuedLegacyDrawPasses() {

    const Vector3 savedReference = modelReferenceDirection;
    const Vector4 savedFvfReference = lambertFvfReference;
    const Vector3 savedLight = modelLightVector;
    const std::uint32_t savedModelFog = legacyModelFogEnabled;
    const std::uint32_t savedNavigation = legacyMirroredDrawMode;
    Vector4 replayReferenceBoard = legacyModelColour;
    const LegacyRenderPacketState *previousPacket = nullptr;
    std::uint32_t draws = 0;
    for (const std::uint32_t index : std::span<const std::uint32_t>(legacySortedQueueIndices, legacyQueuedDrawCount)) {
        const LegacyQueuedDraw &entry = legacyQueuedDraws[index];
        const LegacyRenderPacketState &packet = *entry.packet;
        SetEngineFlag(0x3c, 1);
        SetEngineFlag(0x3d, 0);
        SetEngineFlag(0x3a, 517);
        SetEngineFlag(0x3b, 1);
        SetEngineFlag(0x40, (packet.flags & 1U) ? ((packet.flags >> 3U) & 1U) : 0);
        const bool changedPacket = previousPacket != &packet;
        if (changedPacket) {
            RestoreLegacyRenderPacketState(packet);
            previousPacket = &packet;
        }
        if (entry.type == 0) {
            legacyMaterialBucketCounts[0] = static_cast<std::uint32_t>(entry.savedMaterialState0);
            legacyMaterialBucketCounts[1] = static_cast<std::uint32_t>(entry.savedMaterialState0 >> 32U);
            legacyMaterialBucketCounts[2] = static_cast<std::uint32_t>(entry.materialReplay.savedMaterialState1);
            legacyMaterialBucketCounts[3] = static_cast<std::uint32_t>(entry.materialReplay.savedMaterialState1 >> 32U);
            legacyMaterialBucketCounts[4] = entry.materialReplay.savedMaterialCount;
            shenProcessMaterialCommands(*reinterpret_cast<const LegacyMaterialCommand *>(entry.command));
            RestoreLegacyCpuMaterialFlags();
        } else if (entry.type == 2) {
            entry.callback(entry.callbackData);
        } else if (entry.type == 4) {
            BeginAndEndLegacyModelQuery(entry.command);
        } else if (entry.type == 1) {
            replayReferenceBoard.w = (packet.flags & 1U) ? packet.alphaScale : 1.0f;
            legacyModelReferenceBoard = replayReferenceBoard;
            shaderParameters.constants[25] = {1.0f, 1.0f, 1.0f, (packet.flags & 1U) ? packet.alphaScale : 1.0f};
            if (HasLegacyAlphaPassMatrix())
                PrepareLegacyAlphaPassMatrix();
            draws += ExecuteLegacyDrawPacket(
                entry.modelBytes, entry.xb01Offset, *entry.resources, static_cast<std::size_t>(entry.command - entry.modelBytes.data()));
            if (HasLegacyAlphaPassMatrix())
                RestoreLegacyAlphaPassSamplers();
        }
        if (changedPacket)
            Matrix4x4::Pop(1);
    }
    if (HasLegacyAlphaPassMatrix())
        RestoreLegacyAlphaPassSamplers();
    modelReferenceDirection = savedReference;
    lambertFvfReference = savedFvfReference;
    modelLightVector = savedLight;
    legacyModelFogEnabled = savedModelFog;
    legacyMirroredDrawMode = savedNavigation;
    RestoreEngineFlag(0x3c);
    RestoreEngineFlag(0x3d);
    RestoreEngineFlag(0x3a);
    RestoreEngineFlag(0x3b);
    RestoreEngineFlag(0x40);
    RestoreEngineFlag(0x39);
    return draws;
}

std::uint32_t FlushQueuedLegacyDrawPasses() {
    ++flagDebugCompatibilityReplayDepth;
    RenderQueuedLegacyShaderPasses();
    SortLegacyQueuedDraws();
    const std::uint32_t draws = RenderQueuedLegacyDrawPasses();
    legacyRenderPacketCount = 0;
    legacyQueuedDrawCount = 0;
    legacyQueuedShaderCount = 0;
    legacyQueuedDrawCommandCount = 0;
    legacyQueuedCallbackCount = 0;
    fireQuadCount = 0;
    --flagDebugCompatibilityReplayDepth;
    ReleaseLegacyFlagDebugCompatibilityDraws();
    return draws;
}

void RenderQueuedLegacyDrawPassesOnly() {
#if defined(_WIN32)
    SetupPostFXRenderTargetsAndViewport(d3t::DX11::g_ppD3DDeviceContext);
    if (legacyQueuedDrawCount > 0)
        SortLegacyQueuedDraws();
    ++flagDebugCompatibilityReplayDepth;
    RenderQueuedLegacyDrawPasses();
    --flagDebugCompatibilityReplayDepth;
#endif
}

std::int64_t ResetLegacyRenderQueue() {
    legacyRenderPacketCount = 0;
    legacyQueuedDrawCount = 0;
    legacyQueuedShaderCount = 0;
    legacyQueuedDrawCommandCount = 0;
    legacyQueuedCallbackCount = 0;
    fireQuadCount = 0;
    ReleaseLegacyFlagDebugCompatibilityDraws();
    return 0;
}

void SDTransparentHack() {
#if defined(_WIN32)
    if (!g_mainModelCache)
        return;
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    d3t::D3TRenderState &legacyState = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    if (legacyState.topology != 0) {
        legacyState.bufferStateFlags |= 1U;
        legacyState.topology = 0;
    }
    SetupPostFXRenderTargetsAndViewport(d3t::DX11::g_ppD3DDeviceContext);
    SetupPostFXRenderTargetsAndViewport(d3t::DX11::g_ppD3DDeviceContext);
    if (legacyQueuedDrawCount > 0)
        SortLegacyQueuedDraws();
    ++flagDebugCompatibilityReplayDepth;
    RenderQueuedLegacyDrawPasses();
    legacyRenderPacketCount = 0;
    legacyQueuedDrawCount = 0;
    legacyQueuedShaderCount = 0;
    legacyQueuedDrawCommandCount = 0;
    legacyQueuedCallbackCount = 0;
    fireQuadCount = 0;
    --flagDebugCompatibilityReplayDepth;
    ReleaseLegacyFlagDebugCompatibilityDraws();
    if (d3t::DX11::g_ppD3DDeviceContext) {
        d3t::DX11::WaitForCommandListFinish(d3t::DX11::g_ppD3DDeviceContext);
        d3t::DX11::g_ppD3DDeviceContext = nullptr;
    }
    CameraConstantBlock camera;
    camera.projectedOrigin = {};
    d3t::D3TDeviceContext *contexts[5];
    std::uint32_t tasks[5];
    for (std::uint32_t index = 0; index < 5; ++index) {
        contexts[index] = d3t::DXGetPipelineContext(index != 0);
        SetupPostFXRenderTargetsAndViewport(contexts[index]);
        d3t::D3TRenderState &state = d3t::GetRenderState(*contexts[index]);
        if (state.topology != 0) {
            state.bufferStateFlags |= 1U;
            state.topology = 0;
        }

        tasks[index] = d3t::g_invalidTaskHandle;
    }
    CreateCameraMatrices(camera);
    void *const mappedCamera = d3t::MapTempConstantBufferForWriteDiscard(contexts[3], 3, 0, sizeof(CameraConstantBlock));
    if (mappedCamera)
        std::memcpy(mappedCamera, &camera, sizeof(camera));
    if (g_frameWaitEnabled[0] && GetNumFramesProcessed() == 0) {
        for (;;) {
            std::uint32_t index = 0;
            for (; index < 5; ++index)
                if (tasks[index] != d3t::g_invalidTaskHandle && d3t::d3t_IsTaskActive(tasks[index]))
                    break;
            if (index == 5)
                break;
            if (!g_frameWaitEnabled[1] || !d3t::RunTask(0, tasks, 5))
                _mm_pause();
        }
    }
    for (d3t::D3TDeviceContext *context : contexts)
        d3t::DX11::WaitForCommandListFinish(context);
#endif
}
bool InitializeLegacyStripBuffers(std::uint32_t type0Capacity, std::uint32_t type3Capacity) {
    ReleaseLegacyStripType3();
    stripType3BatchCapacity = (type0Capacity + type3Capacity) >> 2U;
    stripType3Batches = new LegacyStripType3Batch[stripType3BatchCapacity];
    stripType3SortPointers = new LegacyStripType3Batch *[stripType3BatchCapacity];
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    for (unsigned type = 0; type < 2; ++type) {
        const std::uint32_t capacity = type ? type3Capacity : type0Capacity, stride = type ? 0x20U : 0x14U;
        const d3t::BufferDescription description{.flags = 1,
            .size = static_cast<std::int32_t>(capacity * stride),
            .count = 0,
            .flags3 = 3,
            .buffer = nullptr,
            .name = type ? "Strip type3" : "Strip type0"};
        std::int32_t &buffer = type ? stripType3VertexBuffer : stripType0VertexBuffer;
        buffer = d3t::CreateBuffer(&description);
        if (!buffer) {
            ReleaseLegacyStripType3();
            return false;
        }
        std::byte *const vertices = static_cast<std::byte *>(d3t::GetWriteMappedBufferPointer(context, buffer, 1, 0));
        if (!vertices) {
            ReleaseLegacyStripType3();
            return false;
        }
        for (std::uint32_t index = 0; index < capacity; ++index)
            *reinterpret_cast<float *>(vertices + index * stride + 12) = 1.0f;
        d3t::DX11_CopySceneNodeToGPU(context, buffer);
    }
    stripType3VertexCapacity = type3Capacity;
    stripBufferStateFlags |= 1U;
    return true;
}

void ReleaseLegacyStripType3() {
    if (stripType3VertexBuffer)
        d3t::ValidateAndCommitBuffer(stripType3VertexBuffer);
    if (stripType0VertexBuffer)
        d3t::ValidateAndCommitBuffer(stripType0VertexBuffer);
    stripType0VertexBuffer = 0;
    delete[] stripType3Batches;
    delete[] stripType3SortPointers;
    stripType3Batches = nullptr;
    stripType3SortPointers = nullptr;
    stripType3BatchCapacity = stripType3BatchCount = 0;
    stripBufferStateFlags &= ~1U;
    stripType3VertexBuffer = 0;
    stripType3VertexCapacity = 0;
    stripType3WriteOffset = 0;
}

void ResetLegacyStripType3Frame() {
    stripType3WriteOffset = 0;
    stripType3BatchCount = 0;
}

bool IsStripBatchingEnabled() {
    return (stripBufferStateFlags & 2U) == 0;
}

void SetStripBatchingEnabled(std::int32_t enabled) {
    if (enabled)
        stripBufferStateFlags &= ~2U;
    else
        stripBufferStateFlags |= 2U;
}

void ApplyLegacyStripRenderMode() {
    const std::uint32_t savedCull = renderingMode & 0x18U;
    SetEngineFlag(0x93, 0);
    renderingMode = (renderingMode & ~0x18U) | savedCull;
}

void SubmitStripType3Quad(std::span<const LegacyStripType0Vertex> vertices) {
    if (!(stripBufferStateFlags & 2U)) {
        QueueStripType3DrawBatch(static_cast<std::int32_t>(vertices.size()), vertices.data(), -1);
        return;
    }
    const std::uint32_t savedDepth = static_cast<std::uint32_t>((engineFlags >> 1U) & 1U);
    const std::uint32_t savedAlpha = shaderParameters.base.alphaTestEnabled;
    const std::uint32_t savedBlend = static_cast<std::uint32_t>(blendFlags & 1U), savedCull = renderingMode & 0x18U;
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x3c, 1);
    SetEngineFlag(0x3b, 1);
    SetEngineFlag(0x93, 0);
    renderingMode = (renderingMode & ~0x18U) | savedCull;
    SetEngineFlag(0x3b, savedBlend);
    SetEngineFlag(0x3c, savedAlpha);
    SetEngineFlag(0x40, savedDepth);
}

namespace {
void PrepareStripBatchBindings() {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.vertexShader != g_originalStrip03VertexShader) {
        state.vertexShader = g_originalStrip03VertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != g_originalStrip03PixelShader) {
        state.pixelShader = g_originalStrip03PixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_stripType3InputLayout) {
        state.inputLayout = g_stripType3InputLayout;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    d3t::DX11_ResetMappedResources(context);
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(stripType3VertexBuffer)) {
        state.vertexBufferHandles[0] = static_cast<std::uint32_t>(stripType3VertexBuffer);
        state.vertexBufferDirtyMask |= 1U;
    }
#endif
}

void DrawStripBatchRange(const LegacyStripType3Batch &batch) {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::uint32_t texture = batch.texture == -1 ? 0U :
        batch.texture < 0                             ? static_cast<std::uint32_t>(batch.texture) :
                                                        ResolveNamedTextureHandle(batch.texture);
    if (batch.texture >= 0 && !texture)
        return;
    if (texture)
        BindPixelTexture0(state, ResolveEffectTextureSource(texture, perspectiveProjection));
    else
        ClearPixelShaderEffectSRVSlot(state, 0);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(static_cast<std::uint32_t>(batch.vertexCount), batch.firstVertex);
#else
    static_cast<void>(batch);
#endif
}

void DrawQueuedStripType3Batch(void *data) {
    const LegacyStripType3Batch &batch = stripType3Batches[reinterpret_cast<std::uintptr_t>(data)];
    PrepareStripBatchBindings();
    const std::uint32_t savedCull = renderingMode & 0x18U, savedSpecular = shaderParameters.base.specularEnabled;
    const std::uint32_t savedFog = shaderParameters.base.fogEnabled;
    const std::uint32_t savedAddressU = ((pixelSamplerKeys[0][0] >> 6U) & 3U) + 1U;
    const std::uint32_t savedAddressV = ((pixelSamplerKeys[0][0] >> 8U) & 3U) + 1U;
    SetEngineFlag(0x93, 0);
    SetEngineFlag(0x67, 1);
    SetEngineFlag(0x5c, 0);
    DrawStripBatchRange(batch);
    SetEngineFlag(0x5c, savedFog);
#if defined(_WIN32)
    if (d3t::DX11::g_ppD3DDeviceContext)
        ClearPixelShaderEffectSRVSlot(d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext), 0);
#endif
    SetAlphaFlags(0, 1, savedAddressV);
    SetAlphaFlags(0, 0, savedAddressU);
    SetAlphaFlags(0, 4, 0);
    SetAlphaFlags(0, 3, 0);
    SetEngineFlag(0x67, savedSpecular);
    renderingMode = (renderingMode & ~0x18U) | savedCull;
}

int CompareStripBatchDepth(const void *left, const void *right) {
    const LegacyStripType3Batch *a = *static_cast<LegacyStripType3Batch *const *>(left);
    const LegacyStripType3Batch *b = *static_cast<LegacyStripType3Batch *const *>(right);
    if (a->sortDepth > b->sortDepth)
        return -1;
    if (b->sortDepth > a->sortDepth)
        return 1;
    return a > b ? 1 : -1;
}
}

bool QueueStripType3DrawBatch(std::int32_t count, const void *vertices, std::int32_t texture) {
    if (count <= 0 || !vertices || stripType3BatchCount >= stripType3BatchCapacity ||
        static_cast<std::uint32_t>(count) > stripType3VertexCapacity - stripType3WriteOffset)
        return false;
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    StripType3GpuVertex *const target = static_cast<StripType3GpuVertex *>(d3t::GetWriteMappedBufferPointer(
        context, stripType3VertexBuffer, static_cast<std::int32_t>(stripType3WriteOffset != 0) + 1, stripType3WriteOffset * sizeof(StripType3GpuVertex)));
    if (!target)
        return false;
    const float depth = static_cast<const LegacyStripType0Vertex *>(vertices)->position.z;
    LegacyStripType3Batch &batch = stripType3Batches[stripType3BatchCount];
    batch = {.texture = texture, .vertexCount = count, .firstVertex = stripType3WriteOffset, .sortDepth = depth};
    if (stripNearClip <= depth) {
        batch.sortDepth = -1.0f;
        SetUpdateCallback(DrawQueuedStripType3Batch, reinterpret_cast<void *>(static_cast<std::uintptr_t>(stripType3BatchCount)), -depth);
    }
    stripType3SortPointers[stripType3BatchCount++] = &batch;
#if defined(_WIN32)
    if (texture >= 0 && context) {
        const std::uint32_t handle = ResolveNamedTextureHandle(texture);
        if (handle)
            BindPixelTexture0(d3t::GetRenderState(*context), ResolveEffectTextureSource(handle, perspectiveProjection));
    }
#endif
    for (std::int32_t index = 0; index < count; ++index) {
        const Vector3 *position;
        if (texture == -1) {
            const LegacyStripType0Vertex &source = static_cast<const LegacyStripType0Vertex *>(vertices)[index];
            position = &source.position;
            target[index].diffuse = source.diffuse;
            target[index].specular = 0;
            target[index].u = target[index].v = 0.0f;
        } else {
            const LegacyStripType3Vertex &source = static_cast<const LegacyStripType3Vertex *>(vertices)[index];
            position = &source.position;
            target[index].diffuse = source.diffuse;
            target[index].specular = source.specular;
            target[index].u = source.u;
            target[index].v = source.v;
        }
        target[index].position.x = position->x;
        target[index].position.y = position->y;
        target[index].position.z =
            stripNearClip <= position->z ? (stripProjection.rows[3].z - position->z * stripProjection.rows[2].z) / position->z : position->z;
    }
    d3t::DX11_CopySceneNodeToGPU(context, stripType3VertexBuffer);
    stripType3WriteOffset += static_cast<std::uint32_t>(count);
    return true;
}

void FlushQueuedStripType3Batches() {
    std::qsort(stripType3SortPointers, stripType3BatchCount, sizeof(*stripType3SortPointers), CompareStripBatchDepth);
    PrepareStripBatchBindings();
    const std::uint32_t savedDepth = static_cast<std::uint32_t>((engineFlags >> 1U) & 1U);
    const std::uint32_t savedDepthFunction = static_cast<std::uint32_t>(engineFlags & 0x3cU);
    const std::uint32_t savedBlend = static_cast<std::uint32_t>(blendFlags & 1U), savedCull = renderingMode & 0x18U;
    const std::uint32_t savedSpecular = shaderParameters.base.specularEnabled, savedFog = shaderParameters.base.fogEnabled;
    const std::uint32_t savedAddressU = ((pixelSamplerKeys[0][0] >> 6U) & 3U) + 1U;
    const std::uint32_t savedAddressV = ((pixelSamplerKeys[0][0] >> 8U) & 3U) + 1U;
    SetEngineFlag(0x40, 0);
    SetEngineFlag(0x3b, 1);
    SetEngineFlag(0x93, 0);
    SetEngineFlag(0x67, 1);
    SetAlphaFlags(0, 3, 2);
    SetAlphaFlags(0, 4, 1);
    SetAlphaFlags(0, 0, 3);
    SetAlphaFlags(0, 1, 3);
    SetEngineFlag(0x5c, 0);
    bool transitioned = false;
    for (std::uint32_t index = 0; index < stripType3BatchCount; ++index) {
        const LegacyStripType3Batch &batch = *stripType3SortPointers[index];
        if (!transitioned && stripNearClip > batch.sortDepth) {
            SetEngineFlag(0x39, 519);
            transitioned = true;
        }
        if (batch.sortDepth != -1.0f)
            DrawStripBatchRange(batch);
    }
#if defined(_WIN32)
    d3t::BindSceneFrameRenderTarget();
    if (d3t::DX11::g_ppD3DDeviceContext)
        ClearPixelShaderEffectSRVSlot(d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext), 0);
#endif
    SetEngineFlag(0x5c, savedFog);
    SetAlphaFlags(0, 1, savedAddressV);
    SetAlphaFlags(0, 0, savedAddressU);
    SetAlphaFlags(0, 4, 0);
    SetAlphaFlags(0, 3, 0);
    SetEngineFlag(0x67, savedSpecular);
    renderingMode = (renderingMode & ~0x18U) | savedCull;
    SetEngineFlag(0x3b, savedBlend);
    SetEngineFlag(0x40, savedDepth);
    engineFlags = (engineFlags & ~0x3cULL) | savedDepthFunction;
    ResetLegacyStripType3Frame();
}

void SetLegacyStripProjection(const Matrix4x4 &projection, float nearClip) {
    stripProjection = projection;
    stripNearClip = nearClip;
}

void SetLegacyStripDepthTest(bool enabled) {
    SetEngineFlag(7, enabled ? 1 : 0);
}

void SetLegacyStripScissor(bool enabled, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom) {

    ApplyEngineFlag(0xae, enabled ? 1U : 0U);
    g_legacyScissorEnabled = enabled ? 1 : 0;
    if (!enabled)
        return;
    auto *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return;
    auto &state = d3t::GetRenderState(*context);
    CommitGeometryState(state);
    const std::array<std::int32_t, 4> rectangle{left, top, right, bottom};
    if (state.scissorRect != rectangle) {
        state.scissorRect = rectangle;
        state.bufferStateFlags |= 4U;
    }
}

static bool DrawLegacyStripType3Immediate(std::span<const LegacyStripType3Vertex> vertices, std::uint32_t texture, bool alphaTest) {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext || !texture || !stripType3VertexBuffer || !g_originalStrip03VertexShader || !g_originalStrip03PixelShader ||
        !g_stripType3InputLayout || vertices.size() > stripType3VertexCapacity - stripType3WriteOffset)
        return false;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.vertexShader != g_originalStrip03VertexShader) {
        state.vertexShader = g_originalStrip03VertexShader;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != g_originalStrip03PixelShader) {
        state.pixelShader = g_originalStrip03PixelShader;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_stripType3InputLayout) {
        state.inputLayout = g_stripType3InputLayout;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(stripType3VertexBuffer)) {
        state.vertexBufferHandles[0] = static_cast<std::uint32_t>(stripType3VertexBuffer);
        state.vertexBufferDirtyMask |= 1;
    }
    StripType3GpuVertex *const destination = static_cast<StripType3GpuVertex *>(
        d3t::GetWriteMappedBufferPointer(context, stripType3VertexBuffer, 2, stripType3WriteOffset * sizeof(StripType3GpuVertex)));
    if (!destination)
        return false;
    for (std::size_t index = 0; index < vertices.size(); ++index) {
        const LegacyStripType3Vertex &source = vertices[index];
        StripType3GpuVertex &target = destination[index];
        target.position.x = source.position.x - 0.5f;
        target.position.y = source.position.y - 0.5f;
        target.position.z = source.position.z >= stripNearClip ?
            (stripProjection.rows[3].z - source.position.z * stripProjection.rows[2].z) / source.position.z :
            source.position.z;
        target.diffuse = source.diffuse;
        target.specular = source.specular;
        target.u = source.u;
        target.v = source.v;
    }
    d3t::DX11_CopySceneNodeToGPU(context, stripType3VertexBuffer);
    const std::uint32_t savedDepthWrite = static_cast<std::uint32_t>((engineFlags >> 1) & 1);
    const std::uint32_t savedAlphaTest = shaderParameters.base.alphaTestEnabled;
    const std::uint32_t savedBlend = static_cast<std::uint32_t>(blendFlags & 1);
    const std::uint32_t savedCull = renderingMode & 0x18;
    const std::uint32_t savedFog = shaderParameters.base.fogEnabled;
    const std::uint32_t savedSpecular = shaderParameters.base.specularEnabled;
    const auto savedSampler = pixelSamplerKeys[0];
    if (alphaTest) {
        SetEngineFlag(0x40, 0);
        SetEngineFlag(0x3c, 1);
        SetEngineFlag(0x3b, 1);
        SetEngineFlag(0x5c, 0);
    } else {
        SetEngineFlag(0x67, 1);
        SetAlphaFlags(0, 3, 2);
        SetAlphaFlags(0, 4, 1);
        SetAlphaFlags(0, 0, 3);
        SetAlphaFlags(0, 1, 3);
    }
    SetEngineFlag(0x93, 0);
    BindPixelTexture0(state, texture);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    UpdateShaderParams();
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(static_cast<std::uint32_t>(vertices.size()), stripType3WriteOffset);
    SetEngineFlag(0x5c, savedFog);
    ClearPixelShaderEffectSRVSlot(state, 0);
    stripType3WriteOffset += static_cast<std::uint32_t>(vertices.size());
    renderingMode = (renderingMode & ~0x18U) | savedCull;
    SetEngineFlag(0x3b, savedBlend);
    SetEngineFlag(0x3c, savedAlphaTest);
    SetEngineFlag(0x40, savedDepthWrite);
    if (!alphaTest) {
        pixelSamplerKeys[0] = savedSampler;
        SetEngineFlag(0x67, savedSpecular);
    }
    return true;
#else
    static_cast<void>(vertices);
    static_cast<void>(texture);
    return false;
#endif
}

bool DrawLegacyStripType3AlphaTestImmediate(std::span<const LegacyStripType3Vertex> vertices, std::uint32_t texture) {
    return DrawLegacyStripType3Immediate(vertices, texture, true);
}

bool DrawLegacyStripType3AlphaBlendImmediate(std::span<const LegacyStripType3Vertex> vertices, std::uint32_t texture) {
    return DrawLegacyStripType3Immediate(vertices, texture, false);
}

void GetLegacyStripBlendModes(std::int32_t *source, std::int32_t *destination) {
    *source = stripPacketBlendModes[0];
    *destination = stripPacketBlendModes[1];
}

void SetLegacyStripBlendModes(std::int32_t source, std::int32_t destination) {


    stripPacketBlendModes = {source, destination};
}

} // namespace shenmue::s2
