#include "s1/modules/arcade.hpp"
#include "logging.hpp"

#include "s1/archive_data.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/initialization.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_audio.hpp"
#include "s1/modules/harrier_render.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/screen_space.hpp"
#include "s1/simple_shader.hpp"
#include "d3t/render_state.hpp"
#ifdef SHENMUE_RT_ARCADE
#include "s1/enhancements/rt_arcade.hpp"
#endif
#include "s1/input.hpp"
#include "s1/render_passes.hpp"
#include "s1/task/hplg.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/save.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/time.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/window.hpp"

#include <array>
#include <atomic>
#include <cmath>
#include <filesystem>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#if defined(_WIN32) && !defined(SHENMUE_LOAD_HANGON_DLL)
extern "C" {
__declspec(dllimport) std::size_t HangOn_ComputeMemoryRequired(const shenmue::s1::HangOnConfig *);
__declspec(dllimport) void *HangOn_Create(const shenmue::s1::HangOnConfig *, void *);
__declspec(dllimport) void HangOn_Update(void *, shenmue::s1::HangOnUpdateBlock *);
__declspec(dllimport) void HangOn_Destroy(void *);
}
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#endif

namespace shenmue::s1 {
extern void (*g_harrier_loop_callback)();
extern std::int32_t g_busy_reset;
extern std::int32_t g_forcedSleepWindowStartMinutes;

EmulatorSessionContext g_emulatorSession{};
GameDateTime g_harrierCurrentTime{};
std::uint8_t g_harrierNeedsTimeUpdate = 0;
float g_harrierSeconds = 0.0f;
std::int32_t g_harrierHaltRequested = 0;
std::int32_t g_harrierExitedEmulator = 0;
std::int32_t g_harrierShouldReturn = 0;
GameDateTime g_hangOnCurrentTime{};
float g_hangOnSeconds = 0.0f;
std::int32_t g_hangOnPaused = 0;
std::int32_t g_hangOnPauseDebounce = 0;
std::int32_t g_hangOnFramesToSkip = 60;
float g_hangOnSteeringResponse = 0.9f;
HangOnUpdateBlock g_hangOnUpdate{};

HangOnLibrary g_hangOnLibrary{};
namespace {
struct Runtime {
    ArcadeGame game{ArcadeGame::None};
    std::uint32_t texture{};
    bool active{};
} g_runtime;
std::int32_t g_harrierSavedDepthEnable;

std::int32_t ProbeArcadeBootromWorkspace() {
    HLib::TaskData *const memory = static_cast<HLib::TaskData *>(HLib_AllocHeapMemBlock(0x4000, static_cast<std::int32_t>(TASK_BOOT_ROM.name)));
    return static_cast<std::int32_t>(HLib_FreeTaskData(memory, 0));
}

// UI_Game_HUD and UI_Game_HUD_Watch in the 80-entry element table.
constexpr std::size_t kGameHudElement = 0x41;
constexpr std::size_t kGameHudWatchElement = 0x22;

// ShenmueLegacyInput bits.
constexpr std::uint16_t kPadY = 0x0002;
constexpr std::uint16_t kPadB = 0x0200;
constexpr std::uint16_t kPadStart = 0x0800;
constexpr std::uint16_t kPadUp = 0x1000;
constexpr std::uint16_t kPadDown = 0x2000;
constexpr std::uint16_t kPadLeft = 0x4000;
constexpr std::uint16_t kPadRight = 0x8000;

bool UploadVideo(const ArcadeVideoFrame &frame) {
    if (!frame.pixels || !frame.width || !frame.height || frame.rowPitch < frame.width * 4U)
        return false;
    const std::uint32_t format = frame.format == ArcadePixelFormat::Bgra8 ? 0x25U : 0x12U;
    std::uint32_t width{}, height{};
    Texture2DDescription old{};
    if (!g_runtime.texture || !GetTexture2DDimensions(g_runtime.texture, &width, &height) || width != frame.width || height != frame.height ||
        !GetTexture2DDescription(g_runtime.texture, &old) || old.format != format) {
        if (g_runtime.texture)
            InitTexture2DResource(g_runtime.texture);
        const Texture2DDescription description{
            .width = frame.width, .height = frame.height, .arraySize = 1, .mipLevels = 1, .format = format, .sampleCount = 1, .initialData = frame.pixels};
        g_runtime.texture = CreateTexture2D(&description);
        return g_runtime.texture != 0;
    }
#if defined(_WIN32)
    D3TTextureBackendView view{};
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext || !GetTexture2DBackendView(g_runtime.texture, &view) || !view.texture)
        return false;
    static_cast<ID3D11DeviceContext *>(context->deviceContext)
        ->UpdateSubresource(static_cast<ID3D11Texture2D *>(view.texture), 0, nullptr, frame.pixels, frame.rowPitch, 0);
    return true;
#else
    return false;
#endif
}

bool EnteredFromMap(const HLib::TaskData *taskData) {
    return reinterpret_cast<const ModuleTaskData *>(taskData)->previousModule == SystemModule::Map;
}

void HideGameHud() {
    UIBaseRuntime &hud = g_uiElements[kGameHudElement];
    if (!hud.isVisible || !hud.destroy || hud.destroy() == 1 || g_busy_reset == 1)
        hud.isVisible = 0;
}

void ShowWatch() {
    if (g_currUIState != -1 && g_currUIState != static_cast<std::int32_t>(kGameHudWatchElement))
        return;
    UIBaseRuntime &watch = g_uiElements[kGameHudWatchElement];
    if (!watch.isVisible) {
        std::int64_t (*tick)() = nullptr;
        if (watch.render) {
            watch.render();
            tick = watch.tick;
        } else if (watch.initialize) {
            watch.initialize();
            tick = watch.postInit;
        }
        if (tick) {
            g_waitingForNextTick = 0;
            tick();
            g_waitingForNextTick = 1;
        }
        watch.isVisible = 1;
    }
    watch.isHandledExternally = 0;
}

void HideWatch() {
    if (g_currUIState != -1 && g_currUIState != static_cast<std::int32_t>(kGameHudWatchElement))
        return;
    UIBaseRuntime &watch = g_uiElements[kGameHudWatchElement];
    if (!watch.isVisible || !watch.destroy || watch.destroy() == 1 || g_busy_reset == 1)
        watch.isVisible = 0;
}

void PublishWatchHands(const GameDateTime &time) {
    const std::int32_t hours = static_cast<std::int8_t>(time.hours);
    const std::int32_t minutes = static_cast<std::int8_t>(time.minutes);
    const std::int32_t seconds = static_cast<std::int8_t>(time.seconds);
    g_timeClockUi.minuteHand = 0U - static_cast<std::uint16_t>(seconds * -18 - minutes * 1092);
    g_timeClockUi.hourHand = 0U - static_cast<std::uint16_t>(minutes * -91 - hours * 5461 - seconds);
    constexpr double kWatchAngleScale = 10430.37835047046;
    g_minuteHandRadians = static_cast<float>(static_cast<float>(static_cast<std::int32_t>(g_timeClockUi.minuteHand)) / kWatchAngleScale);
    g_hourHandRadians = static_cast<float>(static_cast<float>(static_cast<std::int32_t>(g_timeClockUi.hourHand)) / kWatchAngleScale);
}

float EmulatorTimeStep(float carriedSeconds) {
    float step = 0.0f;
    if (!g_unusedTimeBypass)
        step = g_timeMultiplier * g_cycleTimeMultiplier;
    return step * 0.5f + carriedSeconds;
}


void DecrementDateByDay(const GameDateTime &source, GameDateTime &destination) {
    destination.day = source.day;
    destination.month = source.month;
    destination.year = source.year;
    --destination.day;
    if (static_cast<std::int8_t>(destination.day) >= 1)
        return;
    --destination.month;
    if (static_cast<std::int8_t>(destination.month) < 1) {
        --destination.year;
        if (static_cast<std::int8_t>(destination.year) < 0)
            destination.year = 99;
        destination.month = 12;
    }
    destination.day = (destination.year & 3U) != 0 || destination.month != 2 ? kDaysInMonth[destination.month] : 29;
}

// Earliest of the forced-sleep boundary and the next queued timed transition.
const GameDateTime *ArcadeTimeLimit() {
    GameDateTime *const sleepBoundary = CalculateElapsedTime();
    if (g_timedTransitionUpcoming != 0xff && CompareDates(sleepBoundary, &g_timedTransitionUpcomingTime) != DateComparisonResult::lessThan)
        return &g_timedTransitionUpcomingTime;
    return sleepBoundary;
}

bool Harrier_CheckShenmueTimeTooLate() {
    return g_harrierNeedsTimeUpdate && CompareDates(&g_harrierCurrentTime, ArcadeTimeLimit()) != DateComparisonResult::lessThan;
}

void CopyCurrentDateTime(GameDateTime &destination) {
    destination.year = g_currentDateTime.year;
    destination.month = g_currentDateTime.month;
    destination.day = g_currentDateTime.day;
    destination.dayOfWeek = g_currentDateTime.dayOfWeek;
    destination.hours = g_currentDateTime.hours;
    destination.minutes = g_currentDateTime.minutes;
    destination.seconds = g_currentDateTime.seconds;
}

void UpdateHangOnInput(const InputSlot &pad, bool gameInProgress) {
    const std::uint8_t stick = static_cast<std::uint8_t>(pad.rightStick);
    const float deflection = static_cast<float>(std::abs(static_cast<std::int32_t>(stick) - 128)) * 0.0078125f;
    float response = deflection - (1.0f - deflection) * g_hangOnSteeringResponse * deflection;
    if (response > 0.0f)
        response = response > 0.15625f ? response : 0.15625f;
    const float steering = stick >= 0x80U ? response * 128.0f + 128.0f : 128.0f - response * 128.0f;

    const std::uint16_t held = pad.held;
    std::int32_t startSignal = 0;
    if (g_hangOnPauseDebounce != 0) {
        if (g_hangOnPauseDebounce > 0)
            --g_hangOnPauseDebounce;
        else
            g_hangOnPauseDebounce = 0;
    } else if (gameInProgress) {
        if (!g_hangOnPaused && (held & kPadStart) != 0) {
            g_hangOnPaused = 1;
            g_hangOnPauseDebounce = 19;
        } else if (g_hangOnPaused && (held & (kPadStart | kPadB)) != 0) {
            g_hangOnPaused = 0;
            g_hangOnPauseDebounce = 19;
        }
    } else if ((held & kPadStart) != 0) {
        startSignal = 2;
        g_hangOnPauseDebounce = 19;
    }

    g_hangOnUpdate.startSignal = startSignal;
    if ((held & kPadRight) != 0)
        g_hangOnUpdate.steering = 0;
    else if ((held & kPadLeft) != 0)
        g_hangOnUpdate.steering = 0xff;
    else
        g_hangOnUpdate.steering = static_cast<std::uint8_t>(0xff - static_cast<std::int32_t>(steering));
    g_hangOnUpdate.accelerator = (held & kPadUp) != 0 ? 0xff : static_cast<std::uint8_t>(pad.triggers);
    g_hangOnUpdate.brake = static_cast<std::uint16_t>((g_hangOnUpdate.brake & 0xff00U) | ((held & kPadDown) != 0 ? 0xffU : pad.triggers >> 8U));
}



void *g_hangOnSession = nullptr;
void *g_hangOnMemory = nullptr;
std::array<std::byte *, 4> g_hangOnAudioBuffers{};
std::uint8_t g_hangOnAudioRing = 0;
void *g_hangOnVoice = nullptr;
bool g_hangOnVoiceStarted = false;
std::uint32_t g_hangOnFrames = 0;
HangOnSharedState g_hangOnShared{};
HangOnConfig g_hangOnConfig{};


std::array<std::uint8_t, 4> HangOnPackScore(std::int32_t value) {
    constexpr std::array<std::int32_t, 8> kPlaces{10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
    std::array<std::uint8_t, 4> bcd{};
    for (std::size_t index = 0; index < bcd.size(); ++index) {
        std::int8_t high = -1;
        for (; value >= 0; value -= kPlaces[2 * index])
            ++high;
        value += kPlaces[2 * index];
        std::int8_t low = -1;
        for (; value >= 0; value -= kPlaces[2 * index + 1])
            ++low;
        value += kPlaces[2 * index + 1];
        bcd[index] = static_cast<std::uint8_t>(static_cast<std::uint8_t>(high << 4) | static_cast<std::uint8_t>(low));
    }
    return bcd;
}

std::uint32_t HangOnUnpackScore(const std::array<std::uint8_t, 4> &bcd) {
    std::uint32_t value = 0;
    for (const std::uint8_t digits : bcd)
        value = (value * 10U + (digits >> 4U)) * 10U + (digits & 0x0fU);
    return value;
}


void ReleaseHangOnSession() {
#ifdef SHENMUE_RT_ARCADE
    RTArcade_EndInteraction(ArcadeGame::HangOn);
#endif
    StopRawSoundEffectPlayback(g_hangOnVoice);
    g_hangOnVoiceStarted = false;
    g_hangOnLibrary.destroy(g_hangOnSession);
    std::free(g_hangOnMemory);
    g_hangOnSession = nullptr;
    g_hangOnMemory = nullptr;
    StopRawSoundEffect(g_hangOnVoice);
    for (std::byte *&buffer : g_hangOnAudioBuffers) {
        std::free(buffer);
        buffer = nullptr;
    }
    if (g_runtime.game == ArcadeGame::HangOn) {
        if (g_runtime.texture)
            InitTexture2DResource(g_runtime.texture);
        g_runtime = {};
    }
}



void HangOn_DestroySession(HLib::TaskData *taskData) {
    const std::int8_t credits = static_cast<std::int8_t>(g_hangOnShared.credits);
    g_emulatorSession.field1c = g_hangOnShared.field0c;
    g_emulatorSession.result = 0;
    g_emulatorSession.flags =
        (static_cast<std::uint64_t>(static_cast<std::uint32_t>(g_hangOnShared.field00)) << 32U) | (g_hangOnShared.exitRequested != 0 ? 1U : 0U);
    g_emulatorSession.credits = credits;
    const std::uint32_t serial = g_hangOnShared.field08;
    g_emulatorSession.lastScoreBcd = {static_cast<std::uint8_t>(serial >> 24U), static_cast<std::uint8_t>(serial >> 16U),
        static_cast<std::uint8_t>(serial >> 8U), static_cast<std::uint8_t>(serial)};
    if (EnteredFromMap(taskData))
        SetNewDateTime(g_hangOnCurrentTime);
    if (credits >= 0) {
        std::memcpy(g_hangOnScores.data(), g_hangOnShared.scores.data(), sizeof(g_hangOnScores));
        for (std::size_t index = 0; index < g_hangOnScores.size(); ++index)
            g_hangOnScores[index].score = HangOnUnpackScore(g_hangOnShared.scores[index].scoreBcd);
    }
    reinterpret_cast<ModuleTaskData *>(taskData)->currentModule = SystemModule::HangOn;
    HLib_UpdateTaskQueue(SystemModule::Map);
    if (g_hangOnSession)
        ReleaseHangOnSession();
    g_hangOnFramesToSkip = 60;
    g_postFxState.enabled = 1;
}
} // namespace
bool LoadHangOnLibrary() {
#if defined(_WIN32)
    if (g_hangOnLibrary.create)
        return true;
#if !defined(SHENMUE_LOAD_HANGON_DLL)
    g_hangOnLibrary = {.computeMemoryRequired = HangOn_ComputeMemoryRequired, .create = HangOn_Create, .update = HangOn_Update, .destroy = HangOn_Destroy};
    return true;
#else
    const char *const shenRoot = std::getenv("SHEN_ROOT");
    const std::filesystem::path directory = shenRoot && *shenRoot ? std::filesystem::path(shenRoot) / "sm1" : std::filesystem::current_path();
    const HMODULE module = LoadLibraryW((directory / L"GameHangon_Winx64_Release.dll").c_str());
    if (!module)
        return false;
    g_hangOnLibrary = {
        .computeMemoryRequired = reinterpret_cast<std::size_t (*)(const HangOnConfig *)>(GetProcAddress(module, "HangOn_ComputeMemoryRequired")),
        .create = reinterpret_cast<void *(*)(const HangOnConfig *, void *)>(GetProcAddress(module, "HangOn_Create")),
        .update = reinterpret_cast<void (*)(void *, HangOnUpdateBlock *)>(GetProcAddress(module, "HangOn_Update")),
        .destroy = reinterpret_cast<void (*)(void *)>(GetProcAddress(module, "HangOn_Destroy")),
    };
    if (g_hangOnLibrary.computeMemoryRequired && g_hangOnLibrary.create && g_hangOnLibrary.update && g_hangOnLibrary.destroy)
        return true;
    g_hangOnLibrary = {};
    FreeLibrary(module);
#endif
#endif
    return false;
}

void HangOnDestroy() {
    if (!g_hangOnSession)
        return;
    ReleaseHangOnSession();
    g_hangOnFramesToSkip = 60;
}

ArcadeGame Arcade_ActiveGame() noexcept {
    return g_runtime.game;
}

GameDateTime *CalculateElapsedTime() {
    std::int32_t boundary = g_forcedSleepWindowStartMinutes;
    if (boundary > 1440)
        boundary -= 1440;
    GameDateTime &limit = g_temporaryDateTime;
    limit.hours = g_currentDateTime.hours;
    const std::int32_t hours = boundary / 60;
    const std::int32_t minutes = boundary % 60;
    limit.year = g_currentDateTime.year;
    limit.month = g_currentDateTime.month;
    limit.day = g_currentDateTime.day;
    limit.dayOfWeek = g_currentDateTime.dayOfWeek;
    limit.minutes = g_currentDateTime.minutes;
    limit.seconds = g_currentDateTime.seconds;
    if (hours >= 8) {
        if (static_cast<std::int8_t>(g_currentDateTime.hours) < 8)
            DecrementDateByDay(limit, limit);
    } else if (static_cast<std::int8_t>(g_currentDateTime.hours) >= 8) {
        IncrementDateTimeByDays(limit, 1);
    }
    limit.minutes = static_cast<std::uint8_t>(minutes);
    g_temporaryDateTimeWord = 0;
    limit.hours = static_cast<std::uint8_t>(hours);
    limit.seconds = 0;
    return &limit;
}

float UpdateShenmueTimeForEmulator(GameDateTime &time, float seconds) {
    while (seconds >= 60.0f) {
        ++time.minutes;
        time.seconds = 0;
        seconds -= 60.0f;
        if (static_cast<std::int8_t>(time.minutes) <= 59)
            continue;
        ++time.hours;
        time.minutes = 0;
        if (static_cast<std::int8_t>(time.hours) <= 23)
            continue;
        time.hours = 0;
        const std::uint8_t year = time.year;
        const std::uint8_t lastDay = (year & 3U) != 0 || time.month != 2 ? kDaysInMonth[time.month] : 29;
        ++time.day;
        if (static_cast<std::int8_t>(time.day) > static_cast<std::int8_t>(lastDay)) {
            time.day = 1;
            ++time.month;
            if (static_cast<std::int8_t>(time.month) > 12) {
                time.month = 1;
                time.year = static_cast<std::int8_t>(year) + 1 > 99 ? 0 : static_cast<std::uint8_t>(year + 1);
            }
        }
        RecalculateDayInTheWeek(&time);
    }
    time.seconds = static_cast<std::uint8_t>(static_cast<std::int32_t>(seconds));
    g_currentDeltaTimeSim = static_cast<float>(std::fmod(static_cast<double>(seconds), 1.0) * 1000.0);
    return seconds;
}

void Harrier_UpdateShenmueTime() {
    if (!g_harrierNeedsTimeUpdate)
        return;
    SetNewDateTime(g_harrierCurrentTime);
    g_harrierNeedsTimeUpdate = 0;
}

void HarrierLoop() {
    RenderScreen();
    const Texture2DDescription description{.width = 640, .height = 480, .arraySize = 1, .mipLevels = 1, .format = 3, .bindFlags = 4, .sampleCount = 1};
    g_runtime = {.game = ArcadeGame::SpaceHarrier, .texture = CreateTexture2D(&description), .active = true};
    HarrierInit();
    g_harrierHaltRequested = 0;
    g_harrierEmulatorFrameCount = 0;
    g_harrierShouldReturn = 0;
    if (g_perspectiveProjection >= 1 && g_perspectiveProjection <= 3)
        g_perspectiveProjection = 0;
    g_harrierSavedDepthEnable = static_cast<std::int32_t>(g_legacyEngineFlags & 1U);
    SetRenderFlags(7, 0);
    g_isRenderSuppressed = 1;
    g_harrierHighScoreEntered = 0;
    g_harrierBestScore = 0;
    while (!g_harrierExitedEmulator) {
        g_xInputReserved1 = 1;
        g_forceXInputProcessing = 1;
#if defined(_WIN32)
        MSG message{};
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            if (message.message == WM_QUIT)
                Shutdown();
        }
        GenerateMipsAndUpdateResourcesForDirectX();
#endif
        std::atomic_ref<std::int32_t>(g_dxPipelineLock).store(0, std::memory_order_seq_cst);
        RenderPipeline(0, 0, 0);
        D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
        if (context) {
            SetRenderTarget(context, g_runtime.texture);
            SetDirectXWindowConfig(0, 0, 640, 480, 0.0f, 0.0f);
#if defined(_WIN32)
            UnmapAndClearViews(context, g_runtime.texture, 0, kArcadeClearColour.data());
#endif
        }
        UpdateInputsFromInputDevices();
        if (g_harrierNeedsTimeUpdate)
            g_harrierSeconds = UpdateShenmueTimeForEmulator(g_harrierCurrentTime, EmulatorTimeStep(g_harrierSeconds));
        HarrierExecute();
        if (context) {
#ifdef SHENMUE_RT_ARCADE
            RTArcade_SetInteractiveFrame(ArcadeGame::SpaceHarrier, g_runtime.texture);
#endif
            const RenderPassHandles pass{g_runtime.texture};
            DrawFullPass_Quad(&pass, DrawPassOpType::PreserveSourceAspect);
#if defined(_WIN32)
            SubmitAndReleasePipelineContext();
#endif
        }
        if (g_harrierNeedsTimeUpdate) {
            if (g_getAspectRatioModeFunction)
                g_getAspectRatioModeFunction();
#if defined(_WIN32)
            g_RenderDeviceContext = d3t_DXGetPipelineContext(1);
#endif
            if (g_harrier_loop_callback)
                g_harrier_loop_callback();
            PublishWatchHands(g_harrierCurrentTime);
            ShowWatch();
        } else {
            HideWatch();
        }
        HandleUICallbacks();
        if (g_pfnDrawNuklear)
            g_pfnDrawNuklear();
#if defined(_WIN32)
        if (g_RenderDeviceContext) {
            DX11::WaitForCommandListFinish(g_RenderDeviceContext);
            g_RenderDeviceContext = nullptr;
            g_Nuklear_num_indices = 0;
            g_Nuklear_num_verts = 0;
        }
#endif
#if defined(_WIN32)
        UpdateMipMaps();
        d3t_PresentFrame(2);
#endif
        g_windowInput.customInputHandler = 0;
        g_windowInput.previousKeyStateChangedFlag = g_windowInput.currentKeyStateChangedFlag;
        g_windowInput.currentKeyStateChangedFlag = 0;
        for (KeyState &key : g_windowInput.keyStates)
            key.released = 0;
        if (g_returnToTitleRequested) {
            g_returnToTitleRequested = 0;
            ReturnToTitleScreen();
        }
        if (Harrier_CheckShenmueTimeTooLate())
            g_harrierHaltRequested = 1;
        if (g_harrierHaltRequested && g_emu68_state == 1)
            break;
    }
    g_isRenderSuppressed = 0;
    Harrier_UpdateShenmueTime();
    HarrierDestroy();
#ifdef SHENMUE_RT_ARCADE
    RTArcade_EndInteraction(ArcadeGame::SpaceHarrier);
#endif
    InitTexture2DResource(g_runtime.texture);
    g_runtime = {};
    g_xInputReserved1 = 0;
    g_perspectiveProjection = 0;
    SetRenderFlags(7, g_harrierSavedDepthEnable);
}

void HarrierModule(HLib::TaskData *taskData) {
    if (!g_runtime.active) {
#ifdef SHENMUE_RT_ARCADE
        RTArcade_BeginInteraction(ArcadeGame::SpaceHarrier);
#endif
        g_emulatorSession.result = -1;
        g_emulatorSession.mode = 2;
        g_emulatorSession.field0c = 0;
        g_emulatorSession.aicaBootrom = ProbeArcadeBootromWorkspace();
        g_emulatorSession.lastScoreBcd = {};
        g_emulatorSession.flags = 0;
        if (!EnteredFromMap(taskData))
            g_emulatorSession.credits = -1;
        g_harrierSession = {.mode = 2,
            .aicaBootrom = g_emulatorSession.aicaBootrom,
            .coreKind = 3,
            .result = -1,
            .credits = g_emulatorSession.credits,
            .gameFlags = g_emulatorSession.field1c};
        if (g_harrierSession.credits >= 0) {
            for (std::size_t index = 0; index < g_spaceHarrierScores.size(); ++index) {
                const ArcadeScoreEntry &entry = g_spaceHarrierScores[index];
                const std::array<std::uint8_t, 4> score = HangOnPackScore(static_cast<std::int32_t>(entry.score));
                std::uint8_t *const rank = g_harrierSession.rankings + index * 10;
                std::memcpy(rank, score.data(), score.size());
                std::memcpy(rank + 4, entry.initials, 3);
                rank[7] = entry.reserved[0];
            }
        } else {
            std::memcpy(g_harrierSession.rankings, kHarrierDefaultRankings.data(), kHarrierDefaultRankings.size());
        }
        Harrier_Init();
        g_harrierRemainingCredits = g_harrierSession.credits;
        g_harrierDisplayCredits = g_harrierRemainingCredits;
        g_harrierExitDelay = g_harrierSession.exitDelay;
        g_harrierSkipHighScore = 1;
        HideGameHud();
        if (EnteredFromMap(taskData)) {
            CopyCurrentDateTime(g_harrierCurrentTime);
            g_harrierSeconds = 0.0f;
            g_harrierNeedsTimeUpdate = 1;
        }
        HarrierLoop();
    }
    g_harrierSession.result = g_harrierExitReason;
    g_harrierSession.credits = g_harrierRemainingCredits;
    g_harrierSession.scoreBcd = g_harrierBestScore;
    g_harrierSession.completed = g_harrierHighScoreEntered;
    g_harrierSession.playedFrames = g_harrierPlayedFrames;
    g_emulatorSession.result = g_harrierSession.result;
    g_emulatorSession.credits = g_harrierSession.credits;
    g_emulatorSession.field1c = g_harrierSession.gameFlags;
    g_emulatorSession.flags =
        static_cast<std::uint32_t>(g_harrierSession.completed) | (static_cast<std::uint64_t>(static_cast<std::uint32_t>(g_harrierSession.playedFrames)) << 32U);
    const std::uint32_t score = g_harrierSession.scoreBcd;
    g_emulatorSession.lastScoreBcd = {static_cast<std::uint8_t>(score >> 24U), static_cast<std::uint8_t>(score >> 16U), static_cast<std::uint8_t>(score >> 8U),
        static_cast<std::uint8_t>(score)};
    if (g_harrierSession.credits >= 0) {
        for (std::size_t index = 0; index < g_spaceHarrierScores.size(); ++index) {
            const std::uint8_t *const rank = g_harrierSession.rankings + index * 10;
            const std::array<std::uint8_t, 4> bcd{rank[0], rank[1], rank[2], rank[3]};
            g_spaceHarrierScores[index].score = HangOnUnpackScore(bcd);
            std::memcpy(g_spaceHarrierScores[index].initials, rank + 4, 3);
            g_spaceHarrierScores[index].reserved[0] = rank[7];
        }
    }
    reinterpret_cast<ModuleTaskData *>(taskData)->currentModule = SystemModule::Harrier;
    HLib_UpdateTaskQueue(SystemModule::Map);
    if (g_harrierShouldReturn)
        g_returnToTitleRequested = 1;
}



void HangOnCreate(HLib::TaskData *taskData) {
    UI_HELPClearEmulatorPrompt();
    if (g_perspectiveProjection >= 1 && g_perspectiveProjection <= 3)
        g_perspectiveProjection = 0;
    g_xInputReserved1 = 1;
    g_forceXInputProcessing = 1;
    if (!g_hangOnSession) {
        if (!LoadHangOnLibrary()) {
            HLib_UpdateTaskQueue(SystemModule::Map);
            return;
        }
#ifdef SHENMUE_RT_ARCADE
        RTArcade_BeginInteraction(ArcadeGame::HangOn);
#endif
        g_isRenderSuppressed = 1;
        g_postFxState.enabled = 0;
        g_emulatorSession.result = -1;
        g_emulatorSession.mode = 2;
        g_emulatorSession.field0c = 0;
        g_emulatorSession.lastScoreBcd = {};
        g_emulatorSession.flags = 0;
        const bool fromMap = EnteredFromMap(taskData);
        if (fromMap) {
            CopyCurrentDateTime(g_hangOnCurrentTime);
            for (std::size_t index = 0; index < g_hangOnScores.size(); ++index) {
                const ArcadeScoreEntry &entry = g_hangOnScores[index];
                HangOnScoreRecord &record = g_hangOnShared.scores[index];
                record.scoreBcd = HangOnPackScore(static_cast<std::int32_t>(entry.score));
                std::memcpy(record.initials.data(), entry.initials, sizeof(entry.initials));
                record.initials[3] = static_cast<char>(entry.reserved[0]);
                std::memcpy(&record.extra, entry.reserved + 1, sizeof(record.extra));
            }
        } else {
            g_emulatorSession.credits = -1;
            g_hangOnShared.scores = kHangOnCreateDefaultRanking;
        }
        g_hangOnShared.field0c = g_emulatorSession.field1c;
        g_hangOnConfig = {.mode = static_cast<std::int32_t>(g_emulatorSession.mode), .field04 = g_emulatorSession.field0c, .shared = &g_hangOnShared};
        g_hangOnFrames = 0;
        g_hangOnShared.field00 = 0;
        g_hangOnShared.credits = g_emulatorSession.credits;
        g_hangOnShared.field08 = 0;
        const std::size_t size = g_hangOnLibrary.computeMemoryRequired(&g_hangOnConfig);
        g_hangOnMemory = std::malloc(size);
        std::memset(g_hangOnMemory, 0, size);
        for (std::byte *&buffer : g_hangOnAudioBuffers)
            buffer = static_cast<std::byte *>(std::malloc(3200));
        g_hangOnSession = g_hangOnLibrary.create(&g_hangOnConfig, g_hangOnMemory);
        g_hangOnVoice = AllocateRawSoundEffectVoice(16, 2, false);
        HideGameHud();
        g_runtime = {.game = ArcadeGame::HangOn, .active = true};
    }

    if (g_hangOnSession) {
        UpdateHangOnInput(g_inputState.slots[0], g_hangOnShared.gameInProgress != 0);
        g_hangOnUpdate.audioBytes = 3200;
        g_hangOnUpdate.audioPlayLength = 800;
        g_hangOnUpdate.audioData = g_hangOnAudioBuffers[g_hangOnAudioRing];
        g_hangOnAudioRing = static_cast<std::uint8_t>((g_hangOnAudioRing + 1U) & 3U);
        if (g_xinputReserved != 0 && !g_hangOnPaused) {
            g_hangOnLibrary.update(g_hangOnSession, &g_hangOnUpdate);
            if (g_hangOnUpdate.frame)
                UploadVideo({.pixels = g_hangOnUpdate.frame, .width = 512, .height = 256, .rowPitch = 512 * 4, .format = ArcadePixelFormat::Bgra8});
            if (g_hangOnVoice) {
                if (!g_hangOnVoiceStarted) {
                    StartRawSoundEffectVoice(g_hangOnVoice);
                    g_hangOnVoiceStarted = true;
                }
                SetRawSoundEffectVolume(g_hangOnVoice, g_applicationSettings.savedSfxVolume);
                SubmitRawSoundEffectBuffer(g_hangOnVoice, static_cast<const std::byte *>(g_hangOnUpdate.audioData),
                    static_cast<std::uint32_t>(g_hangOnUpdate.audioBytes), g_hangOnUpdate.audioPlayLength);
            }
            if (EnteredFromMap(taskData) && !g_hangOnUpdate.debugFreezeTime)
                g_hangOnSeconds = UpdateShenmueTimeForEmulator(g_hangOnCurrentTime, EmulatorTimeStep(g_hangOnSeconds));
        }
        if (!g_xinputReserved || g_hangOnPaused) {
            const bool paused = g_hangOnPaused != 0;
            const std::uint8_t token = paused ? 0x41 : 0x42;
            g_previousHelpTokenId = g_currentHelpTokenId;
            g_currentHelpTokenId = token;
            g_defaultHelpTokenId = token;
            UI_HELPSetEmulatorContext(paused);
            g_helpButtonId = 0xff;
            g_currentHelpIconState = 0;
            UI_HELPPreparePage(token);
            UI_HELPStartTransitionAnim();
            g_uiElements[kUiHelpNew].isActive = 1;
        }
#ifdef SHENMUE_RT_ARCADE
        RTArcade_SetInteractiveFrame(ArcadeGame::HangOn, g_runtime.texture);
#endif
        if (g_hangOnFramesToSkip <= 0)
            HangOnRender_DrawQuad(g_runtime.texture);
        else
            --g_hangOnFramesToSkip;
        PublishWatchHands(g_hangOnCurrentTime);
        ShowWatch();
        ++g_hangOnFrames;
        b60fps = 1;
    }

    const InputSlot &pad = g_inputState.slots[0];
    if (g_hangOnShared.gameInProgress) {
        if (!g_hangOnPaused || (pad.held & kPadY) == 0)
            return;
    } else if ((pad.held & kPadB) == 0 && g_hangOnShared.credits != 0 && !g_hangOnShared.exitRequested &&
        (!EnteredFromMap(taskData) || CompareDates(&g_hangOnCurrentTime, ArcadeTimeLimit()) == DateComparisonResult::lessThan)) {
        return;
    }
    g_isRenderSuppressed = 0;
    g_hangOnPaused = 0;
    HangOn_DestroySession(taskData);
}
} // namespace shenmue::s1
