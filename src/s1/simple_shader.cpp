#include "s1/simple_shader.hpp"

#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/file_state.hpp"
#include "s1/modern_lighting.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace shenmue::s1 {

std::array<SimpleShaderParameterBlock, 16> g_simpleShaderParameterBlocks{};

FoliageAnimationParameters *g_foliageAnimationParameters = nullptr;
std::uint32_t g_foliageAnimationParameterCount = 0;
std::uint32_t g_simpleShaderLoad = 0;
std::uint32_t g_simpleShaderTemporaryBuffer = 0;
std::uint32_t g_simpleShaderFileHandle = 0;
std::uint32_t g_postFxTexture = 0;
float g_simpleShaderMainParameter = 0.0f;
std::uint32_t g_simpleShaderMode = 0;
std::uint32_t g_postFxEnabled = 1;
std::int32_t g_HasPPFXRendered = 0;

namespace {

std::uint32_t g_simpleShaderInitFlag = 0;
std::uint8_t g_postFxInitialized = 0;
bool g_ppfxMiddlewareAvailable = false;



constexpr std::array<std::uint8_t, 0x224> MakePostFxDefaults() {
    std::array<std::uint8_t, 0x224> defaults{};
    defaults[0] = 1;
    defaults[4] = 1;
    defaults[5] = 1;
    defaults[8] = 0x3f;
    defaults[9] = 0x42;
    defaults[10] = 0x0f;
    return defaults;
}

constexpr auto kPostFxDefaults = MakePostFxDefaults();
std::array<std::uint8_t, 0x224> g_postFxState{};

template <typename T> void WriteParameter(std::size_t offset, T value) {
    std::memcpy(g_simpleShaderParameterBlocks.data()->data.data() + offset, &value, sizeof(value));
}

void InitializeSimpleShaderParameters() {
    std::memset(g_simpleShaderParameterBlocks.data(), 0, sizeof(g_simpleShaderParameterBlocks));



    WriteParameter(0x00, 1.0f);
    WriteParameter(0x04, 1.0f);
    WriteParameter(0x08, 1.0f);
    WriteParameter(0x0c, std::uint32_t{1});
    WriteParameter(0x20, -0.0f);
    WriteParameter(0x24, 1.0f);
    WriteParameter(0x28, -0.0f);
    WriteParameter(0x30, 0.0f);
    WriteParameter(0x34, 0.0020000001f);
    WriteParameter(0x40, std::uint32_t{1});
    WriteParameter(0x44, 0.25f);

    WriteParameter(0x138, 1.0f);
    WriteParameter(0x13c, 1.0f);
    WriteParameter(0x140, 1.0f);
    WriteParameter(0x144, std::uint32_t{1});
    WriteParameter(0x150, 0.0f);
    WriteParameter(0x158, 0.0f);
    WriteParameter(0x15c, -1.0f);
    WriteParameter(0x160, 0.0f);
    WriteParameter(0x168, std::uint32_t{1});
    WriteParameter(0x16c, 0.0020000001f);
    WriteParameter(0x178, std::uint32_t{1});
    WriteParameter(0x17c, 5.0f);

    for (SimpleShaderParameterBlock &block : g_simpleShaderParameterBlocks) {
        const std::uint32_t enabled = 1;
        std::memcpy(block.data.data() + 0x40, &enabled, sizeof(enabled));
    }

    g_simpleShaderMainParameter = 0.0020000001f;
    g_simpleShaderMode = 2;
}

FoliageAnimationParameters *AppendFoliageAnimationParameters() {
    constexpr std::uint32_t kInitialCapacity = 16;
    static std::uint32_t capacity = 0;
    if (g_foliageAnimationParameterCount >= capacity) {
        const std::uint32_t newCapacity = capacity == 0 ? kInitialCapacity : capacity * 2;
        void *const allocation = std::realloc(g_foliageAnimationParameters, sizeof(FoliageAnimationParameters) * newCapacity);
        if (!allocation)
            return nullptr;
        g_foliageAnimationParameters = static_cast<FoliageAnimationParameters *>(allocation);
        capacity = newCapacity;
    }
    return &g_foliageAnimationParameters[g_foliageAnimationParameterCount++];
}




void InitializeOptionalPpfxMiddleware() {
    g_ppfxMiddlewareAvailable = false;
}

} // namespace

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
    return g_postFxState[0] != 0 && g_ppfxMiddlewareAvailable;
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
#if defined(_WIN32)
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
