#include "s1/frame_pipeline.hpp"
#include "s1/d3t/namespace.hpp"

#include "platform/performance_counter.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/d3t_compression.hpp"
#include "s1/d3t/d3t_pipeline_state.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/boot.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/render_passes.hpp"
#include "s1/scene_memory.hpp"
#include "s1/render_array.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/simple_shader.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/timing.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/window.hpp"
#include "s1/archive_data.hpp"
#include "d3t/ui_nuklear.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/initialization.hpp"
#include "s1/input.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/wzog.hpp"
#include "s1/modern_scene.hpp"
#include "s1/modern_material.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/modern_probes.hpp"
#include "s1/mt5_render.hpp"
#include "s1/date_time.hpp"
#include "s1/task/time.hpp"
#ifdef SHENMUE_RT_ARCADE
#include "s1/enhancements/rt_arcade.hpp"
#endif

#include <chrono>
#include <cmath>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>

namespace shenmue::s1 {

using UpdateTimerFunction = void (*)();
using RenderPipelineFunction = void (*)(void *, void *);

extern UpdateTimerFunction fnptr_UpdateTimer;
extern RenderPipelineFunction fnptr_DispatchRenderPipeline;
extern void (*ExecuteWeatherTaskQueue)();

void SetNewWindowDimensions(std::uint32_t width, std::uint32_t height);
extern std::int32_t (*g_getAspectRatioModeFunction)();


UIBaseRuntime &LoadScreen = g_uiElements[0x4e];
std::int32_t g_currUIState = -1;
std::int32_t g_waitingForNextTick = 1;
std::int32_t g_NumFramesProcessed = 0;
std::int32_t g_FrameCounter = 0;
std::int32_t g_FramePacingFactor = 0;
std::uint8_t b60fps = 0;
std::int32_t g_NuklearInitialized = 0;
std::int32_t g_bNeedsLoadTextureResources = 0;



static_assert(offsetof(nk_context, input) == 0 && offsetof(nk_input, keyboard) == 0 && offsetof(nk_keyboard, text_len) == 0xf8);
static_assert(offsetof(nk_input, mouse) == 0xfc && offsetof(nk_mouse, pos) == 0x30 && offsetof(nk_mouse, scroll_delta) == 0x48);
static_assert(offsetof(nk_mouse, grab) == 0x4c && sizeof(nk_input) == 0x14c);

void (*g_pfnDrawNuklear)() = nullptr;
std::uint32_t g_Nuklear_num_indices = 0;
std::uint32_t g_Nuklear_num_verts = 0;
std::int32_t g_busy_reset = 0;
std::int32_t g_renderPipelineDispatchState = 0;

std::uint64_t g_presentDrawCounters[8]{};
double dblFrameTime = 0.0;
char szFrameTime[64] = "Frame: 33.33ms - FPS: 31";
std::int32_t g_pauseStateTemp = 0;
ModernRendererDebugConstants g_modernRendererDebugConstants{.useDualQuaternionSkinning = 1, .clampDirectRoughness = 0.4f};
float g_modernDayFraction = 0.0f;

struct TemporaryClearColour {
    float red;
    std::int32_t green;
};

TemporaryClearColour g_TmpClearColour{};
float g_previousRenderFrameValue = 0.0f;
float g_currentRenderFrameValue = 0.0f;
void (*g_harrier_loop_callback)() = nullptr;
float g_windowHeightScale = 0.0f;
Vector2 g_windowRectDimensions{};

namespace {

bool g_pausedRenderState = false;

} // namespace

#if defined(_WIN32)
void SubmitAndReleasePipelineContext() {
    if (!DX11::g_ppD3DDeviceContext)
        return;
    DX11::WaitForCommandListFinish(DX11::g_ppD3DDeviceContext);
    DX11::g_ppD3DDeviceContext = nullptr;
}

D3TDeviceContext *AcquireScenePipelineContext() {
    D3TDeviceContext *const context = d3t_DXGetPipelineContext(0);
    DX11::g_ppD3DDeviceContext = context;
    if (!context)
        return nullptr;

    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    constexpr auto triangleList = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
    if (state.topology != triangleList) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = triangleList;
    }
    SetupPPFX(context);
    return context;
}
#endif



void UpdateMouseForNuklear() {
    if (!g_NuklearInitialized)
        return;
    nk_input &input = GNuklearWindow.input;
    input.mouse.prev = input.mouse.pos;
    for (nk_mouse_button &button : input.mouse.buttons)
        button.clicked = 0;
    input.keyboard.text_len = 0;
    input.mouse.delta.y = 0.0f;
    input.mouse.scroll_delta = 0.0f;
    input.mouse.delta.x = 0.0f;
    for (nk_key &key : input.keyboard.keys)
        key.clicked = 0;
}

namespace d3t {
void ProcessMessages() {
    UpdateMouseForNuklear();

#if defined(_WIN32)
    MSG message{};
    while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
#endif


    if (g_NuklearInitialized) {
        nk_mouse &mouse = GNuklearWindow.input.mouse;
        mouse.grab = 0;
        if (mouse.ungrab) {
            mouse.grabbed = 0;
            mouse.ungrab = 0;
            mouse.grab = 0;
        }
    }

    ProcessRemappings();
    ExecuteADynamicFunctionOnce();

    constexpr float kThirtyFramesPerSecond = 0.03333333507180214f;
    TexSortWrapper(g_bEnableFrameTimeSmoothing ? fFrameTimeMs : kThirtyFramesPerSecond);

    if (g_shouldQuit)
        Shutdown();
    if (g_bNeedsLoadTextureResources) {
        InitGeneralTextureResources();
        g_bNeedsLoadTextureResources = 0;
    }
    GenerateMipsAndUpdateResourcesForDirectX();

    std::atomic_ref<std::int32_t>(g_dxPipelineLock).store(0, std::memory_order_seq_cst);
    RenderPipeline(0, 0, 0);

    if (g_getAspectRatioModeFunction)
        g_getAspectRatioModeFunction();

#if defined(_WIN32)
    g_RenderDeviceContext = g_NuklearInitialized ? d3t_DXGetPipelineContext(1) : nullptr;
#else
    g_RenderDeviceContext = nullptr;
#endif
    if (g_harrier_loop_callback)
        g_harrier_loop_callback();

    for (std::uint32_t index = 0; index < 1024; ++index) {
        RenderArrayEntry *const entry = GetRenderArrayEntry(index);
        if (!entry || entry->state != 1 || (entry->flags & 6U) != 0)
            continue;
        ModifyArrayElementsByMurmurHash(entry->handle, 8, 0);
        if (entry->owner && (entry->owner->flags & 2U) != 0) {
            ModifyArrayElementsByMurmurHash(entry->handle, 1, 0);
        }
    }
    g_previousRenderFrameValue = g_currentRenderFrameValue;
}
} // namespace d3t

[[noreturn]] void Shutdown() {
#ifdef SHENMUE_RT_ARCADE
    RTArcade_Shutdown();
#endif
    SOND_Shutdown();
    ShutdownThreads();
    ReleaseModernSceneLightMetadata();
    ReleaseAicaRam();
    ReleaseRGBA8TextureResources();
    d3t_DestroyAllShaders();
    ReleaseD3TPipelineStateCaches();
    FreeBuffers();
    ReleaseLargeDataBuffer();
#if defined(_WIN32)
    ShutdownDX11();
#endif
    ReleaseRenderHandleLogs();
    ArchiveData_ClearArchives();
    if (g_oodleMode) {
        g_shouldShutdownOodle = 0;
        g_oodleMode = 0;
    }
    Shenmue_Free();
    std::free(g_globalMemory);
    g_globalMemory = nullptr;
    g_globalMemorySize = 0;
    std::exit(0);
}



void GetWindowCoords(std::uint32_t height, bool windowed) {
    if (!windowed) {
        g_windowHeightScale = 1.0f;
        return;
    }
    float width = g_windowRectDimensions.x;
    float rectHeight = g_windowRectDimensions.y;
#if defined(_WIN32)
    RECT rect{};
    if (GetWindowRect(g_windowHandle, &rect)) {
        width = static_cast<float>(rect.right - rect.left);
        rectHeight = static_cast<float>(rect.bottom - rect.top);
        g_windowRectDimensions = {width, rectHeight};
    }
#endif
    if (width != 0.0f && rectHeight != 0.0f)
        g_windowHeightScale = static_cast<float>(height) / rectHeight;
}



void SetNewWindowDimensions(std::uint32_t width, std::uint32_t height) {
#if defined(_WIN32)
    d3t::DirectXUpdateWindowDimensions(width, height);
#endif
    if (g_screenWindow.texture2D[0]) {
        InitTexture2DResource(g_screenWindow.texture2D[0]);
        SetupImmutableDXTex2D();
    }
    InitTexture2DResource(g_fullScreenTexture2D);
    InitTexture2DResource(g_superSamplingTexture2D);
    InitTexture2DResource(g_legacyFullScreenTexture2D);
    InitTexture2DResource(g_originalStripsTexture2D);
    InitGeneralTextureResources();
    g_windowInput.width = static_cast<std::int32_t>(width);
    g_windowInput.height = static_cast<std::int32_t>(height);
    g_pendingClientRectUpdate = 1;
    GetWindowCoords(height, g_currentSettings.display.windowMode != 0);
    g_screenHeight = static_cast<float>(height);
    g_screenWidth = static_cast<float>(static_cast<std::int32_t>(width));
}

void DispatchRenderPipeline(void *renderContext, void *frameInfo) {
    constexpr std::int32_t kLoadScreenState = 0x4e;
    if (!renderContext) {
        if ((g_currUIState == -1 || g_currUIState == kLoadScreenState) && !LoadScreen.isVisible) {
            std::int64_t (*callback)() = nullptr;
            if (LoadScreen.render) {
                LoadScreen.render();
                callback = LoadScreen.tick;
            } else if (LoadScreen.initialize) {
                LoadScreen.initialize();
                callback = LoadScreen.postInit;
            }
            if (callback) {
                g_waitingForNextTick = 0;
                callback();
                g_waitingForNextTick = 1;
            }
            LoadScreen.isVisible = 1;
        }
        LoadScreen.isHandledExternally = 0;
        RenderScreen();
        return;
    }

    if (!frameInfo) {
        if ((g_currUIState == -1 || g_currUIState == kLoadScreenState) &&
            (!LoadScreen.isVisible || !LoadScreen.destroy || LoadScreen.destroy() == 1 || g_busy_reset == 1)) {
            LoadScreen.isVisible = 0;
        }
        g_renderPipelineDispatchState = 0;
    }
}

void RenderPipeline(std::int64_t unused, std::int32_t deferred, std::int32_t renderType) {
    static_cast<void>(unused);
#if defined(_WIN32)
    D3TDeviceContext *const context = d3t_DXGetPipelineContext(deferred != 0 ? 1 : 0);
    DX11::g_ppD3DDeviceContext = context;

    std::byte *const state = context->renderState.data();
    std::uint32_t topology = 0;
    std::memcpy(&topology, state + 0x93c, sizeof(topology));
    if (topology != 0) {
        state[0x976] |= std::byte{1};
        topology = 0;
        std::memcpy(state + 0x93c, &topology, sizeof(topology));
    }
    SetupPPFX(context);
    if (renderType != 0)
        return;

    const auto *const packed = reinterpret_cast<const std::uint8_t *>(&g_ClearColour);
    const float alpha = static_cast<float>(packed[3]) / 255.0f;
    float colour[4]{
        (static_cast<float>(packed[0]) / 255.0f) * alpha,
        (static_cast<float>(packed[1]) / 255.0f) * alpha,
        (static_cast<float>(packed[2]) / 255.0f) * alpha,
        alpha,
    };
    if (!g_pauseStateTemp) {
        UnmapAndClearViews(context, CreateNewTexture2D(), 0, colour);
    }
    if (static_cast<std::uint32_t>(g_perspectiveProjection - 1) > 2U) {
        UnmapAndClearViews(context, g_fullScreenTexture2D, 0, colour);
        colour[0] = 0.0f;
        UnmapAndClearViews(context, g_auxiliaryTexture2D, 0, colour);
        colour[0] = g_TmpClearColour.red;
        colour[1] = static_cast<float>(g_TmpClearColour.green);
        UnmapAndClearViews(context, g_originalStripsTexture2D, 0, colour);
    }
#else
    static_cast<void>(deferred);
    static_cast<void>(renderType);
#endif
}


void Matrix4x4_Create(CameraConstantBlock &block) {
    const auto transpose = [](const Matrix4x4 &source) {
        Matrix4x4 output{};
        for (std::size_t row = 0; row < 4; ++row) {
            for (std::size_t column = 0; column < 4; ++column)
                output.rows[row][column] = source.rows[column][row];
        }
        return output;
    };
    SetActiveStack(MatrixStackType::PostRender);
    *g_matrixStack.top.stack = workTransformMatrix;
    block.postRender = transpose(*g_matrixStack.top.stack);
    SetActiveStack(MatrixStackType::Transparent);
    *g_matrixStack.top.stack = workTransformMatrix;
    block.transparent = transpose(*g_matrixStack.top.stack);
    Matrix4x4 matrix{};
    Matrix4x4::invertMatrix(&block.transparent, &matrix);
    block.inverseTransparent = matrix;
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            matrix.rows[row][column] = block.transparent.rows[row].x * block.postRender.rows[0][column] +
                block.transparent.rows[row].y * block.postRender.rows[1][column] + block.transparent.rows[row].z * block.postRender.rows[2][column] +
                block.transparent.rows[row].w * block.postRender.rows[3][column];
        }
    }
    block.transparentPostRender = matrix;
    block.inversePostRender = *matrix_stack::createTransformedMatrix(&block.postRender, &matrix);
    const Matrix4x4 &inverse = block.inversePostRender;
    const float w = inverse.rows[3].y * 0.0f + inverse.rows[3].x * 0.0f + inverse.rows[3].z * 0.0f + inverse.rows[3].w;
    const float reciprocal = 1.0f / w;
    block.projectedOrigin = {
        reciprocal * (inverse.rows[0].y * 0.0f + inverse.rows[0].x * 0.0f + inverse.rows[0].z * 0.0f + inverse.rows[0].w),
        reciprocal * (inverse.rows[1].y * 0.0f + inverse.rows[1].x * 0.0f + inverse.rows[1].z * 0.0f + inverse.rows[1].w),
        reciprocal * (inverse.rows[2].y * 0.0f + inverse.rows[2].x * 0.0f + inverse.rows[2].z * 0.0f + inverse.rows[2].w),
        reciprocal * w,
    };
    constexpr Matrix4x4 kDefaultPerObjectBufferData{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    block.postEffectTransform = transpose(kDefaultPerObjectBufferData);
    block.previousPostEffectTransform = transpose(kDefaultPerObjectBufferData);
    SetActiveStack(MatrixStackType::DepthPrepass);
}


void UploadModernRendererDebugConstants(D3TDeviceContext *context) {
#if defined(_WIN32)
    ModernRendererDebugConstants *const mapped = static_cast<ModernRendererDebugConstants *>(MapTempConstantBufferForWriteDiscard(context, 3, 9, 36));
    if (mapped) {
        *mapped = g_modernRendererDebugConstants;
        mapped->renderMode = g_modernDebugRenderMode;
        mapped->environmentProbesEnabled = 0;
        mapped->probeVisualizationIndex = 0;
        mapped->visualizeCascade = static_cast<std::uint8_t>(mapped->visualizeCascade);
        mapped->useDualQuaternionSkinning = static_cast<std::uint8_t>(mapped->useDualQuaternionSkinning);
        mapped->visualizeClusters = static_cast<std::uint8_t>(mapped->visualizeClusters);
    }
#else
    static_cast<void>(context);
#endif
}

void InitGameBuffers() {
#if defined(_WIN32)
    if (!g_sdOverrideSkinningRoot || g_perspectiveProjection == 1 || g_forcePrimaryCachedModel)
        return;
    D3TDeviceContext *const context = d3t_DXGetPipelineContext(1);
    if (!context)
        return;
    SetupPPFX(context);
    SetTopology(context, static_cast<std::uint32_t>(d3t::TopologyType::TriangleList));
    const std::uint32_t layer = static_cast<std::uint32_t>(g_sdOverrideSkinningRoot);
    RefreshDirtyRenderLayerMaterialEntities(layer, context, g_simpleShaderFileHandle);
    CameraConstantBlock camera{};
    Matrix4x4_Create(camera);
    PrepareTiledSceneLightingAndShadowBuffers(context, camera.transparentPostRender);
    UploadModernRendererDebugConstants(context);
    BindSceneFallbackTextureAndHighSamplers(context);
    if (void *const mapped = MapTempConstantBufferForWriteDiscard(context, 3, 0, sizeof(camera))) {
        std::memcpy(mapped, &camera, sizeof(camera));
    }
    if (void *const mapped = MapTempConstantBufferForWriteDiscard(context, 3, 3, 0x94))
        std::memcpy(static_cast<std::byte *>(mapped) + 68, &g_modernDayFraction, sizeof(g_modernDayFraction));
    UpdateSceneProbes(context);
    ModernScenePass pass{.materialMask = 7,
        .type = 0,
        .cameraPosition = {camera.projectedOrigin.x, camera.projectedOrigin.y, camera.projectedOrigin.z},
        .fogDisabled = 1,
        .disableAspectCulling = 1};
    ExtractAndNormalizeSixFrustumPlanes(g_modernFrustumPlanes, camera.transparentPostRender);
    RenderLayerGeometryContext *const geometry = BuildRenderLayerGeometryContext(layer, context);
    if (geometry) {
        ModernScenePass opaque = pass;
        ConfigureAndSubmitModernScenePass(context, opaque, *geometry);
        pass.materialMask = 8;
            ConfigureAndSubmitModernScenePass(context, pass, *geometry);
    }
    DX11::WaitForCommandListFinish(context);
#endif
}





void RecomputeMatricesAndWaitSortTextureStack() {
#if defined(_WIN32)
    if (!g_mainSkinningRoot)
        return;
    if (g_perspectiveProjection == 1) {
        RenderPipeline(0, 1, 0);
        EFFE_ReplayQueuedEffects(0);
        EFFE_FinishQueuedEffects();
        SubmitAndReleasePipelineContext();
    }
    std::array<D3TDeviceContext *, 5> contexts{};
    std::array<std::uint32_t, 5> textureSortTasks{};
    for (std::size_t index = 0; index < contexts.size(); ++index) {
        D3TDeviceContext *const context = d3t_DXGetPipelineContext(index != 0 ? 1 : 0);
        contexts[index] = context;
        if (!context)
            continue;
        SetupPPFX(context);
        d3t::D3TRenderState &state = d3t::GetRenderState(*context);
        constexpr std::uint32_t triangleList = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
        if (state.topology != triangleList) {
            state.bufferStateFlags |= d3t::BufferTopology;
            state.topology = triangleList;
        }
    }
    CameraConstantBlock block{};
    Matrix4x4_Create(block);
    if (void *const mapped = MapTempConstantBufferForWriteDiscard(contexts[3], 3, 0, sizeof(block))) {
        std::memcpy(mapped, &block, sizeof(block));
    }


    if (!g_NumFramesProcessed) {
        bool active = true;
        while (active) {
            active = false;
            for (const std::uint32_t task : textureSortTasks)
                active |= task != 0 && d3t_IsTaskActive(task);
            if (active && !RunTask(0, textureSortTasks.data(), static_cast<std::uint32_t>(textureSortTasks.size())))
                std::this_thread::yield();
        }
    }
    for (D3TDeviceContext *const context : contexts)
        DX11::WaitForCommandListFinish(context);
#endif
}

void BeginSceneFrameSubmission() {
#if defined(_WIN32)
    SubmitAndReleasePipelineContext();
#ifdef SHENMUE_RT_ARCADE
    RTArcade_Tick();
#endif
#endif
    const std::int32_t seconds = static_cast<std::int8_t>(g_currentDateTime.seconds) +
        60 * (static_cast<std::int8_t>(g_currentDateTime.minutes) + 60 * static_cast<std::int8_t>(g_currentDateTime.hours));
    g_modernDayFraction = (static_cast<float>(1000 * seconds) + g_currentDeltaTimeSim) / 86400000.0f;
    ProcessHmdlResourceTransitions(1);
    RecomputeMatricesAndWaitSortTextureStack();
#if defined(_WIN32)
    AcquireScenePipelineContext();
    InitGameBuffers();
    if (DX11::g_ppD3DDeviceContext)
        SetupPPFX(DX11::g_ppD3DDeviceContext);
#endif
}

void BeginSceneCompositionSubmission() {
#if defined(_WIN32)
    SubmitAndReleasePipelineContext();
    AcquireScenePipelineContext();
#endif
}

namespace {

void ManageFramePacing() {
    if (fnptr_UpdateTimer)
        fnptr_UpdateTimer();
    if (!g_NumFramesProcessed)
        return;

    const std::int32_t threshold = ++g_FrameCounter;
    if (g_NumFramesProcessed != 3 || threshold < 6 * g_FramePacingFactor)
        return;

    if (fnptr_DispatchRenderPipeline) {
        fnptr_DispatchRenderPipeline(reinterpret_cast<void *>(std::uintptr_t{3}), nullptr);
    }
    g_NumFramesProcessed = 0;
    g_FrameCounter = 0;
}

} // namespace

void FinishSceneFrameSubmission() {
#if defined(_WIN32)
    D3TDeviceContext *context = DX11::g_ppD3DDeviceContext;
    if (context && g_superSampling + 1.0f > 1.0f) {
        const std::uint32_t output = g_superSamplingTextureActive ? g_superSamplingTexture2D : g_legacyFullScreenTexture2D;
        DrawSuperSampled(context, g_fullScreenTexture2D, output);
    }
    SubmitAndReleasePipelineContext();

    context = AcquireScenePipelineContext();
    if (context)
        UpdatePPFX();
    if (context && g_isInGame) {
        const bool paused = (Shenmue_nPauseState & 2) != 0;
        if (paused) {
            g_pausedRenderState = true;
            const RenderPassHandles pass{g_screenWindow.texture2D[0]};
            DrawFullPass_Quad(&pass, DrawPassOpType::None);
        } else if (g_pausedRenderState) {
            g_pausedRenderState = false;
        } else {
            const RenderPassHandles pass{
                IsPostProcessPass() ? g_postFxTexture : (g_superSamplingTextureActive ? g_superSamplingTexture2D : g_legacyFullScreenTexture2D),
            };
            DrawFullPass_Quad(&pass, DrawPassOpType::GammaCorrect);
        }
    }
    SubmitAndReleasePipelineContext();

    if (!g_pausedRenderState && g_isInGame) {
        context = AcquireScenePipelineContext();
        if (context) {
            CopyOrResolveTexture(context, CreateNewTexture2D(), g_screenWindow.texture2D[0]);
        }
        SubmitAndReleasePipelineContext();
    }
    if (g_pMovesScreenSceneBuffer) {
        InitMovesScreenPipelineContext();
        g_pMovesScreenSceneBuffer();
        RenderMovesScreenSceneToTexture();
    }
#endif
    ManageFramePacing();

    if (g_pfnDrawNuklear)
        g_pfnDrawNuklear();
    Do_contrast_Pass();
#if defined(_WIN32)
    if (g_pfnDrawNuklear && g_RenderDeviceContext) {
        DX11::WaitForCommandListFinish(g_RenderDeviceContext);
        g_RenderDeviceContext = nullptr;
        g_Nuklear_num_indices = 0;
        g_Nuklear_num_verts = 0;
    }
#endif
}


std::int64_t g_LastPPFXRenderTime = 0;



void FlushRenderQueue() {
    if (g_NumFramesProcessed)
        return;
    g_HasPPFXRendered = 1;
    g_LastPPFXRenderTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    if (fnptr_DispatchRenderPipeline)
        fnptr_DispatchRenderPipeline(reinterpret_cast<void *>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(g_NumFramesProcessed))),
            reinterpret_cast<void *>(std::uintptr_t{1}));
    g_NumFramesProcessed = 1;
    g_FrameCounter = 0;
    RenderScreen();
}


void Shenmue_DispatchRenderPipeline() {
    if (g_NumFramesProcessed) {
        if (fnptr_DispatchRenderPipeline)
            fnptr_DispatchRenderPipeline(reinterpret_cast<void *>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(g_NumFramesProcessed))), nullptr);
        g_NumFramesProcessed = 0;
        g_FrameCounter = 0;
        g_HasPPFXRendered = 0;
    }
    g_isRenderSuppressed = 0;
}


std::int32_t g_forceLoadFrames = 0;

namespace d3t {

float GetSecondsByPerformanceQuery() {
    if (!g_NumFramesProcessed)
        return 0.0f;
    const std::int64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    return static_cast<float>(now - g_LastPPFXRenderTime) / 1000000.0f / 1000.0f;
}
} // namespace d3t



std::int32_t GetFrameWaitingFor() {
    if (!g_bHasSetupShaders)
        return 2;
    if (g_forceLoadFrames)
        return 1;
    if (g_deltaTime > 0.0f && g_NumFramesProcessed && g_deltaTime >= d3t::GetSecondsByPerformanceQuery())
        return 1;
    if ((g_framesPerSecond > 0 && g_NumFramesProcessed && d3t::GetSecondsByPerformanceQuery() >= static_cast<float>(g_framesPerSecond)) ||
        !g_frameWaitEnabled[0])
        return 0;
    return 2;
}



void RenderScreen_Wrapper(const std::uint32_t *renderHandles, std::int32_t count) {
    if (!g_NumFramesProcessed)
        return;
    if (fnptr_DispatchRenderPipeline)
        fnptr_DispatchRenderPipeline(reinterpret_cast<void *>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(g_NumFramesProcessed))),
            reinterpret_cast<void *>(std::uintptr_t{2}));
    g_NumFramesProcessed = 2;
    g_FrameCounter = 0;
    std::int32_t remaining = count;
    while (true) {
        const bool objectsSettled = ProcessHmdlResourceTransitions(1) == 0;
        bool settled = objectsSettled & (d3t::PollTextureOverrides() == 0);
        if (renderHandles && remaining > 0) {
            do {
                settled = settled & !d3t_IsRenderHandlePending(*renderHandles++);
                --remaining;
            } while (remaining > 0);
            --remaining;
        }
        const std::int32_t wait = GetFrameWaitingFor();
        if (!wait || (wait != 1 && settled))
            break;
        RenderScreen();
    }
}

void RenderScreen() {
    if (!g_NumFramesProcessed)
        return;
    ManageFramePacing();

    if (DX11::g_ppD3DDeviceContext && g_superSampling + 1.0f > 1.0f) {
        const std::uint32_t output = g_superSamplingTextureActive ? g_superSamplingTexture2D : g_legacyFullScreenTexture2D;
        DrawSuperSampled(DX11::g_ppD3DDeviceContext, g_fullScreenTexture2D, output);
    }

    UpdatePPFX();
    if (DX11::g_ppD3DDeviceContext) {
        const RenderPassHandles pass{
            IsPostProcessPass() ? g_postFxTexture : (g_superSamplingTextureActive ? g_superSamplingTexture2D : g_legacyFullScreenTexture2D),
        };
        DrawFullPass_Quad(&pass, DrawPassOpType::GammaCorrect);
#if defined(_WIN32)
        DX11::WaitForCommandListFinish(DX11::g_ppD3DDeviceContext);
#endif
        DX11::g_ppD3DDeviceContext = nullptr;
    }

    if (g_pfnDrawNuklear) {
        g_pfnDrawNuklear();
        if (g_pfnDrawNuklear && g_RenderDeviceContext) {
#if defined(_WIN32)
            DX11::WaitForCommandListFinish(g_RenderDeviceContext);
#endif
            g_RenderDeviceContext = nullptr;
            g_Nuklear_num_indices = 0;
            g_Nuklear_num_verts = 0;
        }
    }

    PresentLatestFrame();
    d3t::ProcessMessages();
}

void PresentLatestFrame() {
    g_presentDrawCounters[0] = 0;
    g_presentDrawCounters[1] = 0;
    UpdateMipMaps();
    std::int32_t renderMode = g_NumFramesProcessed == 0 ? 1 : 0;
    if (b60fps) {
        renderMode = 2;
        b60fps = 0;
    }
#if defined(_WIN32)
    d3t_PresentFrame(renderMode);
    g_hasPresentedGameFrame = true;
    if (g_shouldResetWindow) {
        ResetWindow();
        if (DisplayModeInfo *const screenResolution = GetScreenResolution()) {
            SetNewWindowDimensions(screenResolution->width, screenResolution->height);
        }
    }
#else
    static_cast<void>(renderMode);
#endif
    if (ExecuteWeatherTaskQueue) {
        ExecuteWeatherTaskQueue();
    }
    ClearDebugTextQueue();
    g_originalStrip00VertexCursor = 0;
    g_originalStrip03VertexCursor = 0;
    g_originalStripPassCount = 0;
    for (std::uint64_t &counter : g_presentDrawCounters) {
        counter = 0;
    }
    d3t::ProfilePerf();
}

namespace d3t {
void ProfilePerf() {
    static const std::int64_t frequency = platform::performanceCounterFrequency();
    static std::int64_t previous = platform::performanceCounter();
    const std::int64_t current = platform::performanceCounter();
    const std::int64_t elapsed = current - previous;
    dblFrameTime = frequency > 0 ? 1000.0 * static_cast<double>(elapsed) / static_cast<double>(frequency) : 0.0;

    const std::int32_t fps = dblFrameTime <= 0.0000001192092895507812 ? 0 : static_cast<std::int32_t>(std::ceil(1000.0 / dblFrameTime));
    std::snprintf(szFrameTime, sizeof(szFrameTime), "Frame: %0.2fms - FPS: %d", dblFrameTime, fps);
    fFrameTimeMs = static_cast<float>(dblFrameTime * 0.001);
    previous = current;
}
} // namespace d3t

} // namespace shenmue::s1
