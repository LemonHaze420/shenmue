#include "s2/initialization.hpp"
#include "s2/area_jump.hpp"

#include "d3t/asset_remap.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/archive_data.hpp"
#include "s2/audio_data.hpp"
#include "s2/conversation.hpp"
#include "s2/sound_program.hpp"
#include "logging.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/cold_archive.hpp"
#include "s2/character.hpp"
#include "s2/player.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/date_time.hpp"
#include "s2/frame_pipeline.hpp"
#include "s2/flag.hpp"
#include "s2/debug_text.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/load_title.hpp"
#include "s2/title.hpp"
#include "s2/map_control.hpp"
#include "s2/inventory.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/movie.hpp"
#include "s2/pause.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/renderer_scratch.hpp"
#include "s2/screen_film.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/snapshot.hpp"
#include "s2/system_modules.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/ui_scene.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/game_arcade.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
#include <crtdbg.h>
#endif

    namespace shenmue::s2 {
namespace {

alignas(32) std::array<Matrix4x4, 64> transformMatrices{};
bool runtimeActive = false;
bool frameOpen = false;
std::int32_t rootRenderDeferredMode = 0;

std::int32_t unusedEventState = -1;
std::int32_t shouldReturnToTitleScreen{};
std::int32_t rootSoftResetEnabled{};

std::int32_t g_debugCameraDisplay{};
std::array<std::int32_t, 9> cameraProperties{};
constexpr std::array<const char *, 5> debugCameraFormats{"O %4.1f", "OM%4.1f", "ZS%4.1f", "ZD%4.1f", "LC%4.1f"};
constexpr std::array<std::int32_t, 5> debugCameraPropertyIndices{0, 3, 6, 7, 8};

std::int32_t GetCameraProperty(std::int32_t index) {
    return cameraProperties[static_cast<std::size_t>(index)];
}

// @verified - p
void RootShadowLightingCallback(void *) {
    MaybeDoGlobalShadowLighting();
    ResolveGlobalShadowLighting(0.0f);
}

void RootTask_Callback(void *) {
    ProcessPostFxFilterInput();
    UpdateGlobalShadowTiming();
    // ROOT is the frame boundary: index-ordered HLib tasks submit into the
    // frame opened on the preceding ROOT visit, and ROOT presents it only
    // after the complete task revolution.  This keeps OBJ/DRWP/DRWH/DRWO
    // submission order intact instead of drawing those owners from ROOT.
    if (frameOpen) {
        if (rootRenderDeferredMode)
            SetUpdateCallback(RootShadowLightingCallback, nullptr, 800.0f);
        else
            RootShadowLightingCallback(nullptr);

        RenderQueuedLegacyShaderPasses();
        ExecuteCurrentCommandList();
        SD_Override();
        DateTime dateTime;
        GetCurrentDateTime(dateTime);
        const float fractionOfDay =
            (static_cast<float>(1000 * (dateTime.second + 60 * (dateTime.minute + 60 * dateTime.hour))) + g_gameTimeMilliseconds) / 86400000.0f;
        shenIsGamePaused();
        const float frameTime = GetFrameTime();
        const float sceneFrameTime = shenIsGamePaused() ? 0.0f : GetFrameTime();
        UpdateFractionOfDay(fractionOfDay, 0, sceneFrameTime);
        IncrementTimeSinceStart(frameTime);
        d3t::ProcessObjectStore(true, -1);
        SDTransparentHack();
        if (!GetLegacyRenderMask()) {
            RenderPipeline("SD Transparent", 1);
            RenderQueuedLegacyDrawPassesOnly();
            ResetLegacyRenderQueue();
            FlushQueuedStripType3Batches();
            DrawSDTransparent();
            ExecuteCurrentCommandList();
        }
        RenderUIScene();
        RenderPipeline("Default_PostRender", 1);

        RenderPostProcessing(0);
        ResetVertCount();
        nullsub();
        RenderPostProcessing(1);
        PerformPostFxFilterRenderPass();
        nullsub();
        UpdateConversationStreams();
        if (g_debugCameraDisplay) {
            // Each property passes through the XOR_EAX boundary, so every line prints 0.0.
            for (std::size_t row = 0; row < debugCameraFormats.size(); ++row) {
                SetDebugStringPosition(55, 10 + static_cast<std::int32_t>(row));
                GetCameraProperty(debugCameraPropertyIndices[row]);
                DebugTextPrintf(debugCameraFormats[row], static_cast<double>(static_cast<float>(static_cast<std::uint32_t>(ReturnZero())) * 0.001f));
            }
        }
        ClearQueuedDebugText();
        g_accumulatedFrameTimeDelta =
            (static_cast<float>(g_previousMainLoopFrameTime) - static_cast<float>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta))) /
            12.5f;
        HandleInputs();
        if (rootSoftResetEnabled != 0 && CheckSoftResetInput()) {
            ResetGameArcadeEmulatorResult();
            InitializeScene();
        }
        DoSimplePixelShaderPass();
        AllocateAndCopyTexture();
        UpdateAuthorMultichannelDevice();
        ConsumeFrames();
        RenderUIContents();
        SetContrastShaderAndDraw();
#ifdef _DEBUG
        DrawQueuedDebugText();
#endif
        PresentD3TFrame(GetNumFramesProcessed() == 0 ? 1 : 0);
        if (GetShouldReturnToTitleScreen() != 0) {
            SetShouldReturnToTitleScreen(0);
            ResetToTitleScreen();
        }
    }
    runtimeActive = ProcessD3TMessages();
    AdvanceAuthorMultichannelAudio();
    g_previousMainLoopFrameTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    g_postPresentMainLoopFrameTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    frameOpen = runtimeActive;
    ++g_rootTaskIterationCount;
}

void RootTask(void *) {
    ExecuteSystemModule(SystemModule::Cold, 0);
    ExecuteCurrentCommandList();
    frameOpen = BeginD3TRenderPass(true);
#if defined(_WIN32)
    if (frameOpen && !InitializeLegacyStripBuffers(4000, 4000)) {
        runtimeActive = false;
        return;
    }
#endif
    HLib::SetCurrentTaskTo(RootTask_Callback);
}

std::filesystem::path gameRoot() {
    if (const char *root = std::getenv("SHEN_ROOT"); root && *root) {
        return std::filesystem::path(root) / "sm2";
    }
    return std::filesystem::current_path();
}

bool Initialization(int, char **) {
    SetSoftResetInputEnabled(0);
    (void)InitializeHLibMemory(0x1200000);
    g_characterShadowRuntime = g_characterShadowStorage.data();
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Start);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Reset);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Accumulate);

    d3t::d3t_CreateThreads();
    InitializeRendererScratch();
    const std::filesystem::path root = gameRoot();
    ArchiveDataLoadResult archives{};
    if (!ArchiveData_LoadArchives(root.string().c_str(), "dx11", 2, &archives)) {
        return false;
    }
    d3t::d3t_ReadAssetRemapping();



    if (!InitializeD3TRenderer(1280, 720, false)) {
        return false;
    }
    d3t::InitializePvrTextureSlotStorage();
    InitializeDebugDefaults();
    if (!InitRenderer()) {
        return false;
    }
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Start);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Reset);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Accumulate);
    InitializeSprites(0, 0);
    if (!InitSystemMatrices()) {
        return false;
    }
    SetScreenDimensions(d3t::g_originalScreenWidth, d3t::g_originalScreenHeight);
    InitializePostProcessingState();

    SetPPFXEnabled(0);
    MatrixStack_InitTransformMatrixStack(transformMatrices.data(), 48);
    SetupLegacyScreenMatrix(0.0f, 0.0f, static_cast<float>(d3t::g_originalScreenWidth), static_cast<float>(d3t::g_originalScreenHeight));
    SetActiveStack(MatrixStackType::Transparent);
    SetActiveStack(MatrixStackType::DepthPrepass);

    InitializeInputSelectionState();
    HLib::InitializeAllocator();
    g_rootTaskIterationCount = 0;
    HLib::InitializeTasks(RootTask);
    DisableAicaEmulation();
    InitializePostFxFilterState();
    SetTitleVersionOverride(0);
    SetRootRenderDeferredMode(1);
    ResetRunningMovieRender();
    (void)ResetSceneFolderReadiness();
    SetSceneFolderReadiness(0);
    InstallLoadingScreenCallbacks();
    runtimeActive = true;
    return true;
}

void ShutdownS2RuntimeResources() {
    ReleaseGameArcadeHostResources();
    ShutdownSoundProgramControl();
    ReleaseS2AicaMiddleware();
    ReleaseConversationResources();
    ReleaseLegacyStripType3();
    ShutdownRendererContent();
    ShutdownD3TRenderer();
    FreeColdArchive();
    ArchiveData_ClearArchives();
}

} // namespace

// @verified - p
std::int32_t GetRootRenderDeferredMode() noexcept {
    return rootRenderDeferredMode;
}
// @verified - p
void SetRootRenderDeferredMode(std::int32_t mode) noexcept {
    rootRenderDeferredMode = mode;
}
// @verified - p
void SetUnusedEventState(std::int32_t state) noexcept {
    unusedEventState = state;
}

// @verified - p: SetShouldReturnToTitleScreen.
void SetShouldReturnToTitleScreen(std::int32_t requested) noexcept {
    shouldReturnToTitleScreen = requested;
}
std::int32_t GetShouldReturnToTitleScreen() noexcept {
    return shouldReturnToTitleScreen;
}
// @verified - p: SetRootSoftResetEnabled.
void SetRootSoftResetEnabled(std::int32_t enabled) noexcept {
    rootSoftResetEnabled = enabled;
}

[[noreturn]] void ShutdownS2Runtime() {
    runtimeActive = false;
    ShutdownS2RuntimeResources();
    std::exit(0);
}

int Shenmue2_Main(int argc, char **argv) {
    if (!Initialization(argc, argv)) {
        ShutdownRendererContent();
        ShutdownD3TRenderer();
        FreeColdArchive();
        ArchiveData_ClearArchives();
        return 1;
    }

#if defined(_WIN32)
    while (runtimeActive) {
        HLib::MainLoop();
    }
#else


    HLib::MainLoop();
#endif

    ShutdownS2RuntimeResources();
    return 0;
}

} // namespace shenmue::s2
