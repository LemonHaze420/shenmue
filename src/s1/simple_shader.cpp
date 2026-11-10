#include "s1/simple_shader.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_state.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/texture_loader.hpp"

#include <cJSON.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

namespace shenmue::s1 {

FoliageAnimationParameters *g_foliageAnimationParameters = nullptr;
std::uint32_t g_foliageAnimationParameterCount = 0;
std::uint32_t g_foliageAnimationParameterCapacity = 0;
std::uint32_t g_foliageAnimationBuffer = 0;
std::uint32_t g_wetnessMaskTexture = 0;
std::uint32_t g_simpleShaderLoad = 0;
std::uint32_t g_simpleShaderTemporaryBuffer = 0;
std::uint32_t g_simpleShaderFileHandle = 0;
std::uint32_t g_postFxTexture = 0;
float g_simpleShaderMainParameter = 0.0f;
std::uint32_t g_postFxEnabled = 1;
std::int32_t g_HasPPFXRendered = 0;

PostFxState g_postFxState = kPostFxDefaults;
namespace {

std::uint32_t g_simpleShaderInitFlag = 0;
std::uint8_t g_postFxInitialized = 0;
bool g_ppfxMiddlewareAvailable = false;

void InitializeSimpleShaderParameters() {
    g_modelShadowSlots.fill(0);
    g_modelShadowDirty.fill(0);

    ModelLightRecord &sun = g_modelLights[0];
    sun.colour = {1.0f, 1.0f, 1.0f};
    sun.type = 1;
    sun.position = {};
    sun.direction = {-0.0f, 1.0f, -0.0f};
    sun.shadowMode = 0;
    sun.shadowBias = 0.0020000001f;
    sun.parameterEnabled = 1;
    sun.parameter104 = 0.25f;

    sun.parameter108 = 0.0f;
    sun.parameter128 = 0;
    ModelLightRecord &fill = g_modelLights[1];
    fill.colour = sun.colour;
    fill.type = 1;
    fill.position = sun.position;
    fill.direction = {0.0f, -1.0f, 0.0f};
    fill.shadowMode = 1;
    fill.shadowBias = 0.0020000001f;
    fill.parameterEnabled = 1;
    fill.parameter104 = 5.0f;
    fill.parameter108 = 0.0f;
    fill.parameter128 = 0;

    for (ModelLightRecord &light : g_modelLights)
        light.parameterEnabled = 1;

    g_simpleShaderMainParameter = 0.0020000001f;
    g_modelLightCount = 2;
}

FoliageAnimationParameters *AppendFoliageAnimationParameters() {
    constexpr std::uint32_t kInitialCapacity = 16;
    if (g_foliageAnimationParameterCount >= g_foliageAnimationParameterCapacity) {
        const std::uint32_t newCapacity = g_foliageAnimationParameterCapacity == 0 ? kInitialCapacity : g_foliageAnimationParameterCapacity * 2;
        void *const allocation = std::realloc(g_foliageAnimationParameters, sizeof(FoliageAnimationParameters) * newCapacity);
        if (!allocation)
            return nullptr;
        g_foliageAnimationParameters = static_cast<FoliageAnimationParameters *>(allocation);
        g_foliageAnimationParameterCapacity = newCapacity;
    }
    return &g_foliageAnimationParameters[g_foliageAnimationParameterCount++];
}

void FoliageAnimationLoaded(void *, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    g_foliageAnimationParameterCount = 0;
    const std::string text = loadedData ? std::string(static_cast<const char *>(loadedData), static_cast<std::size_t>(loadedSize)) : std::string();
    cJSON *const root = loadedData ? cJSON_Parse(text.c_str()) : nullptr;
    if (root) {
        const cJSON *const data = cJSON_GetObjectItem(root, "data");
        const std::int32_t count = data ? cJSON_GetArraySize(const_cast<cJSON *>(data)) : 0;
        for (std::int32_t index = 0; index < count; ++index) {
            cJSON *const entry = cJSON_GetArrayItem(const_cast<cJSON *>(data), index);
            FoliageAnimationParameters *const parameters = AppendFoliageAnimationParameters();
            if (const cJSON *const value = cJSON_GetObjectItem(entry, "bendiness"))
                parameters->bendiness = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(entry, "maxHeight"))
                parameters->maxHeight = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(entry, "startHeight"))
                parameters->startHeight = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(entry, "wiggleSpeed"))
                parameters->wiggleSpeed = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(entry, "deadzone"))
                parameters->deadzone = static_cast<float>(value->valuedouble);
        }
        cJSON_Delete(root);
    }
    ProcessThreadStates(contentHandle, 0);
}

void InitializeOptionalPpfxMiddleware() {
    g_ppfxMiddlewareAvailable = false;
}

} // namespace

namespace d3t {
void LoadFoliageAnimation() {
    const std::uint32_t renderHandle = g_activeRenderHandle;
    const D3TShaderLoadTask task{.path = "/foliageanim.json",
        .renderHandle = renderHandle,
        .reserved_0c = 0,
        .thread = {FoliageAnimationLoaded, nullptr},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    PushShaderLoadToThreadQueue(&task);
    g_wetnessMaskTexture = LoadTextureFromFilepath("/fx/wetness_mask01.png?usage=0", 1, renderHandle);
}

void Free() {
    ReleaseSimpleShaderBuffers();
    ValidateAndCommitBuffer(static_cast<std::int32_t>(g_foliageAnimationBuffer));
    g_foliageAnimationBuffer = 0;
    ReleaseModernSceneLightMetadata();
    if (g_simpleShaderFileHandle != 0xffffU)
        ReleaseFileLockHandle(g_simpleShaderFileHandle);
    g_simpleShaderFileHandle = 0xffffU;
    std::free(g_foliageAnimationParameters);
    g_foliageAnimationParameters = nullptr;
    g_foliageAnimationParameterCount = 0;
    g_foliageAnimationParameterCapacity = 0;
}
} // namespace d3t

void CreateDXBufferForShaders() {
    const BufferDescription description{.flags = 3,
        .size = static_cast<std::int32_t>(sizeof(FoliageAnimationParameters) * g_foliageAnimationParameterCount),
        .count = sizeof(FoliageAnimationParameters),
        .flags3 = 0,
        .buffer = g_foliageAnimationParameters,
        .name = nullptr};
    g_foliageAnimationBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));
}

void InitPostFX() {
    if (g_postFxInitialized)
        return;

    DisplayModeInfo *const resolution = GetScreenResolution();
    if (!resolution)
        return;

    g_postFxState = kPostFxDefaults;
    const Texture2DDescription description{
        resolution->width,
        resolution->height,
        1,
        1,
        4,
        1,
        0,
        4,
        0,
        0,
        nullptr,
    };
    g_postFxTexture = CreateTexture2D(&description);
    InitializeOptionalPpfxMiddleware();
    g_postFxInitialized = 1;
}

bool IsPostProcessPass() {
    return g_postFxState.enabled != 0 && g_ppfxMiddlewareAvailable;
}
void GPUPostEffect_InitializeAutoExposure(std::int32_t effect, float minimum, float maximum) {


    (void)effect;
    (void)minimum;
    (void)maximum;
}

void GPUPostEffect_SetAutoExposure(std::int32_t effect) {
    // See GPUPostEffect_InitializeAutoExposure.
    (void)effect;
}

void UpdatePPFX() {
#if defined(_WIN32) || defined(SHENMUE_GFX_DXVK_NATIVE)
    D3TDeviceContext *const context = d3t_DXGetPipelineContext(1);
    if (!context)
        return;





    DX11::WaitForCommandListFinish(context);
#endif
}

std::uint32_t LoadSimpleShader() {
    InitializeModernSceneLightMetadata();

    InitializeSimpleShaderParameters();
    InitPostFX();

    g_simpleShaderFileHandle = AllocateFileLockHandle();
    const BufferDescription temporaryDescription{
        3,
        4096,
        256,
        2,
        nullptr,
        nullptr,
    };
    g_simpleShaderTemporaryBuffer = static_cast<std::uint32_t>(CreateBuffer(&temporaryDescription));

    for (std::uint32_t index = 0; index < 32; ++index) {
        FoliageAnimationParameters *const parameters = AppendFoliageAnimationParameters();
        if (parameters) {
            *parameters = {0.02f, 4.0f, 0.5f, 16.0f, 0.0f};
        }
    }

    AllocateSimpleShaderBuffers();
    std::atomic_ref(g_simpleShaderInitFlag).store(0, std::memory_order_seq_cst);
    const std::uint32_t renderHandle = d3t_AllocateRenderHandle();
    g_simpleShaderLoad = LoadPrecompiledShader("/engine/assets/shaders/simple", renderHandle);
    return g_simpleShaderLoad;
}

} // namespace shenmue::s1
