#include "auto_test.hpp"
#if defined(SHENMUE_STEAM)
#include "d3t/steam.hpp"
#endif

#include "s2/initialization.hpp"
#include "s2/pak.hpp"
#include "s2/area_jump.hpp"

#include "d3t/asset_remap.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/archives.hpp"
#include "d3t/compression.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/crash_reporter.hpp"
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
#include "s2/lighting.hpp"
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
#include "s2/particle_effects.hpp"
#include "d3t/ui_runtime.hpp"
#include "s2/ui_scene.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/game/arcade.hpp"
#include "s2/time_program.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#include <filesystem>

#include <string>
#if defined(_WIN32)
#include <windows.h>
#include <crtdbg.h>
#include "imgui.h"
#endif

    namespace shenmue::s2 {
namespace {

#if defined(_WIN32)
bool DebugMenuSliderChar(const char *label, std::int8_t *value, std::int8_t minimum, std::int8_t maximum, const char *format) {
    std::int32_t slider = *value;
    const bool changed = ImGui::SliderInt(label, &slider, minimum, maximum, format);
    *value = static_cast<std::int8_t>(slider);
    return changed;
}

void DebugMenuSliderRow(const char *label, std::int8_t *value, std::int8_t minimum, std::int8_t maximum) {
    ImGui::Text(label);
    ImGui::NextColumn();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::PushID(label);
    DebugMenuSliderChar("", value, minimum, maximum, "%.0f");
    ImGui::PopID();
    ImGui::PopItemWidth();
    ImGui::NextColumn();
}
#endif

void DebugMenu(const char *title, std::int32_t *open) {
    if (!open || !*open)
        return;
#if defined(_WIN32)
    if (ImGui::Begin(title, reinterpret_cast<bool *>(open), 0)) {
        ImGui::Columns(2);
        DebugMenuSliderRow("Day", &g_currentDateTime.day, 1, 31);
        DebugMenuSliderRow("Week", reinterpret_cast<std::int8_t *>(&g_currentDateTime.week), 1, 4);
        DebugMenuSliderRow("Month", &g_currentDateTime.month, 1, 12);
        DebugMenuSliderRow("Year", &g_currentDateTime.year, 86, 99);
        DebugMenuSliderRow("Hour", &g_currentDateTime.hour, 1, 24);
        DebugMenuSliderRow("Minute", &g_currentDateTime.minute, 1, 60);
        DebugMenuSliderRow("Second", &g_currentDateTime.second, 1, 60);
        ImGui::Text("Time Speed Multiplier");
        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushID("Time Speed Multiplier");
        ImGui::SliderFloat("Game Time Multiplier", &g_gameTimeMultiplier, 1.0f, 1000.0f, "%.3f");
        ImGui::PopID();
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::Text("Freeze Game Time");
        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::PushID("Freeze Game Time");
        ImGui::Checkbox("", reinterpret_cast<bool *>(&g_freezeGameTime));
        ImGui::PopID();
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        ImGui::Columns(1);
    }
    ImGui::End();
#endif
}

alignas(32) std::array<Matrix4x4, 64> transformMatrices{};
bool runtimeActive = false;
bool frameOpen = false;
std::int32_t rootRenderDeferredMode = 0;

std::int32_t unusedEventState = -1;
std::int32_t shouldReturnToTitleScreen{};
std::int32_t rootSoftResetEnabled{};
std::int32_t rootSaveTransitionEnabled{1};

std::int32_t g_debugCameraDisplay{};
std::array<std::int32_t, 9> cameraProperties{};
constexpr std::array<const char *, 5> debugCameraFormats{"O %4.1f", "OM%4.1f", "ZS%4.1f", "ZD%4.1f", "LC%4.1f"};
constexpr std::array<std::int32_t, 5> debugCameraPropertyIndices{0, 3, 6, 7, 8};

std::int32_t GetCameraProperty(std::int32_t index) {
    return cameraProperties[static_cast<std::size_t>(index)];
}

std::int32_t debugCounterMode{};
std::int32_t debugCounterArmed{};
std::int32_t debugCounter{};
void *debugCounterOwner{};
std::int32_t debugCounterReported{};

void DoNothing() {
    if (debugCounterMode == 1) {
        nullsub();
        (void)ReturnZero();
        nullsub();
    }
}

using GlobalFlagCallback = void (*)();
GlobalFlagCallback incrementGlobalFlagFunc{};
GlobalFlagCallback frameCallbackSlot{};

void IncrementGlobalFlag() {
    ++g_rootTaskIterationCount;
    nullsub();
}

void CallIncrementGlobalFlag() {
    if (incrementGlobalFlagFunc)
        incrementGlobalFlagFunc();
}

void InstallIncrementGlobalFlagHook() {
    incrementGlobalFlagFunc = nullptr;
    frameCallbackSlot = CallIncrementGlobalFlag;
}

void SetIncrementGlobalFlagFunc(GlobalFlagCallback callback) {
    incrementGlobalFlagFunc = callback;
}

void IncrementGlobalCounter() {
    if (debugCounterMode == 1 && debugCounterArmed == 1) {
        if (!debugCounter) {
            nullsub(debugCounterOwner);
            debugCounterArmed = 0;
            debugCounterReported = 0;
        }
        --debugCounter;
    }
}

// @verified - p
void RootShadowLightingCallback(void *) {
    MaybeDoGlobalShadowLighting();
    ResolveGlobalShadowLighting(0.0f);
}

void RootTask_Callback(void *) {
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
    bool captured = false;
#endif
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::RunSteamCallbacks();
#endif
    ProcessPostFxFilterInput();
    UpdateGlobalShadowTiming();
    // ROOT presents the frame submitted by the preceding HLib task revolution.
    // Tasks submit in index order into the frame opened at the previous ROOT visit.
    // OBJ/DRWP/DRWH/DRWO retain that submission order.

    if (frameOpen) {
        if (rootRenderDeferredMode)
            SetUpdateCallback(RootShadowLightingCallback, nullptr, 800.0f);
        else
            RootShadowLightingCallback(nullptr);

        RenderQueuedLegacyShaderPasses();
        ExecuteCurrentCommandList();
        SD_Override();
        DateTime dateTime;
        shenGetCurrentDateTime(dateTime);
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
        nullsub(reinterpret_cast<void *>(5));
        nullsub();
        ClearQueuedDebugText();
        g_accumulatedFrameTimeDelta =
            (static_cast<float>(g_previousMainLoopFrameTime) - static_cast<float>(GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta))) /
            12.5f;
        ResetFireShaderInstanceCount();
        HandleInputs();
        if (GetGammaOptionsActive())
            ExecuteElementCallback(UIElement::OptionsGamma);
        nullsub();
        nullsub();
        DoNothing();
        if (rootSoftResetEnabled != 0 && CheckSoftResetInput()) {
            ResetGameArcadeEmulatorResult();
            InitializeScene();
        }
        DoSimplePixelShaderPass();
        AllocateAndCopyTexture();
        UpdateAuthorMultichannelDevice();
        ExecuteCurrentCommandList();
        ConsumeFrames();
        nullsub();
        d3t::D3TDeviceContext *const previousUiContext = d3t::DX11::g_ppD3DDeviceContext;
        d3t::DX11::g_ppD3DDeviceContext = g_nuklearRenderDeviceContext;
        PrepareUIFrame();
        UpdateScreenScaling();
        UpdateUIFrameStatus();
        d3t::DX11::g_ppD3DDeviceContext = previousUiContext;
        SetContrastShaderAndDraw();
#ifdef _DEBUG
        if (g_nuklearRenderDeviceContext) {
            d3t::D3TDeviceContext *const previousContext = d3t::DX11::g_ppD3DDeviceContext;
            d3t::DX11::g_ppD3DDeviceContext = g_nuklearRenderDeviceContext;
            DrawQueuedDebugText();
            d3t::DX11::g_ppD3DDeviceContext = previousContext;
        }
#endif
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
        captured = auto_test::FinishFrame();
        if (captured)
            d3t::DumpFramePng(auto_test::CapturePath());
#endif
        FinishNuklearFrame();
        nullsub();
        PresentD3TFrame(GetNumFramesProcessed() == 0 ? 1 : 0);
        if (GetShouldReturnToTitleScreen() != 0) {
            SetShouldReturnToTitleScreen(0);
            ResetToTitleScreen();
        }
    }
    runtimeActive = ProcessD3TMessages();
    HandleSceneTransitionForModule();
    IncrementGlobalCounter();
#if SHEN_AUTO_TEST_ENABLED && defined(_WIN32)
    if (captured)
        runtimeActive = false;
#endif
    AdvanceAuthorMultichannelAudio();
    g_previousMainLoopFrameTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    g_postPresentMainLoopFrameTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    nullsub();
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
#if defined(SHENMUE_STEAM)
    return ::shenmue::d3t::GetExecutableDirectory();
#else
    if (const char *root = std::getenv("SHEN_ROOT"); root && *root) {
        return std::filesystem::path(root) / "sm2";
    }
    return std::filesystem::current_path();
#endif
}

struct LoadedAssetLog {
    void *data;
    std::uint32_t position;
    std::uint32_t capacity;
};
static_assert(sizeof(LoadedAssetLog) == 0x10);
std::uint8_t loadedAssetLogEnabled = 0;
LoadedAssetLog loadedAssetLog{};

void SetupAssetLogging(std::uint8_t enabled) {
    loadedAssetLogEnabled = enabled;
    loadedAssetLog = {.data = std::malloc(0x100000), .position = 0, .capacity = 0x100000};
}

bool Initialization(int argc, char **argv) {
    const SteamAppInfo appInfo{.appName = "Mango", .buildId = "MD22_112855", .appId = 758330};
    if (!SteamInit(&appInfo))
        return false;
    SetSoftResetInputEnabled(0);
    rootSoftResetEnabled = 0;
    rootSaveTransitionEnabled = 1;
    for (int index = 1; index < argc; ++index) {
        if (argv[index] && std::strcmp(argv[index], "-logLoadedAssets") == 0)
            SetupAssetLogging(1);
    }
    (void)InitializeHLibMemory(0x1200000);
    g_characterShadowRuntime = g_characterShadowStorage.data();
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Start);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Reset);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Accumulate);

    d3t::d3t_CreateThreads();
    InitializeRendererScratch();
    for (d3t::AsyncArchiveRequest &request : d3t::g_asyncArchiveRequests)
        request.state.exchange(0, std::memory_order_seq_cst);
    d3t::d3t_InitializeOodleZLib();
    const std::filesystem::path root = gameRoot();
    const std::string rootString = root.string();
    const d3t::D3TArchiveDirectory directory{
        .root = rootString.c_str(), .backend = "dx11", .type = 2, .reserved14 = 0, .reserved18 = 0, .asynchronous = 0, .reserved21 = {}};
    d3t::d3t_ReadArchives(&directory);
    if (d3t::g_archiveState.archiveCount == 0) {
        return false;
    }
    d3t::d3t_ReadAssetRemapping();
    SetSaveDataStuff();



#if SHEN_AUTO_TEST_ENABLED
    SetForceSkipSplashScreens(true);
    if (!InitializeD3TRenderer(1280, 720, true)) {
#else
    if (!InitializeD3TRenderer(1280, 720, false)) {
#endif
        return false;
    }
    d3t::InitializePvrTextureSlotStorage(d3t::kS2PvrSurfaceCount);
    AssignHelperFuncs();
    d3t::InitialiseHashTable();
    DebugMenu(nullptr, nullptr);
    InitializeDebugDefaults();
    if (!InitRenderer()) {
        return false;
    }
    InitializeEffectRenderPassTable();
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Start);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Reset);
    GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Accumulate);
    InitializeSprites(0, 0);
    InitializeInputBuffers();
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
    SetupStack();
    Matrix4x4::SetPrePassToTempTransform();
    d3t::SetSceneClearColour(0xff000000U);
    InitEffect();
    SetupEffectTransforms();
    InstallIncrementGlobalFlagHook();
    SetIncrementGlobalFlagFunc(IncrementGlobalFlag);

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
    UI_InitializeElementReinitStates();
    UI_ResetImageLoadDiagnostics();
    runtimeActive = true;
    return true;
}

void ShutdownS2RuntimeResources() {
#if defined(SHENMUE_STEAM)
    while (IsSnapshotSaveInProgress()) {
        ::shenmue::d3t::RunSteamCallbacks();
        d3t::ExecuteADynamicFunctionOnce();
    }
    ::shenmue::d3t::ShutdownSteamStorage();
    d3t::ShutdownThreads();
#endif
    ReleaseGameArcadeHostResources();
    ShutdownSoundProgramControl();
    ReleaseS2AicaMiddleware();
    ReleaseConversationResources();
    ReleaseLegacyStripType3();
    ShutdownRendererContent();
    ShutdownD3TRenderer();
    std::free(loadedAssetLog.data);
    loadedAssetLog.position = 0;
    loadedAssetLog.capacity = 0;
    FreeColdArchive();
    ArchiveData_ClearArchives();
    d3t::FreeObjectStoreTables();
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
DebugMenuCallback g_debugMenuBody{};
DebugMenuCallback g_debugMenuHeader{};

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
// @verified - p
void SetRootSaveTransitionEnabled(std::int32_t enabled) noexcept {
    rootSaveTransitionEnabled = enabled;
}

// @verified - p: SetRootSoftResetEnabled.
void SetRootSoftResetEnabled(std::int32_t enabled) noexcept {
    rootSoftResetEnabled = enabled;
}

[[noreturn]] void ShutdownS2Runtime() {
    runtimeActive = false;
    ShutdownS2RuntimeResources();
#if defined(SHENMUE_STEAM)
    ::shenmue::d3t::ExitOriginalExecutable(0);
#else
    std::exit(0);
#endif
}

int Shenmue2_Main(int argc, char **argv) {
#if defined(_MSC_VER)
    __try {
#endif
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
#if defined(_MSC_VER)
    } __except (d3t::CrashReporter(GetExceptionInformation())) {
        return 0;
    }
#endif
}

} // namespace shenmue::s2
