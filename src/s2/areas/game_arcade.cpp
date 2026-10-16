#include "s2/areas/game_arcade.hpp"
#include "s2/areas/game_arcade_tables.hpp"
#include "s2/areas/game_arcade_render.hpp"
#include "s2/areas/game_arcade_sound.hpp"
#include "s2/areas/game_emulator_overlay.hpp"
#include "s2/areas/game_outrun_board.hpp"
#include "s2/areas/game_outrun_render.hpp"
#include "s2/areas/game_outrun_sound.hpp"
#include "s2/areas/game_afterburner_board.hpp"
#include "s2/areas/game_afterburner_render.hpp"
#include "s2/areas/game_afterburner_sound.hpp"
#include "s2/areas/game_harrier_render.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/initialization.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"
#include "s1/modules/arcade.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_cpu.hpp"
#include "s1/input.hpp"
#include "s1/audio_stream.hpp"
#include "s1/d3t/settings.hpp"
#include "d3t/content.hpp"
#include "d3t/texture.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d11.h>
#endif
namespace shenmue::s2 {
GameHarrierInfo g_gameHarrierInfo{};
GameLongArcadeInfo g_gameOutRunInfo{}, g_gameAfterBurnerInfo{};
GameHangOnInfo g_gameHangOnInfo{};
std::int32_t g_gameArcadeRunning{};
std::uint8_t g_arcadeRoadControl{};
GameArcadeRuntime g_gameArcadeRuntime{};
float g_gameHangOnSteeringResponse{};
namespace {
ArcadeSessionContext session{};
EmulatorInfoHeader *activeInfo;
bool arcadeReturnToTitle;
bool harrierCoreActive;
bool outRunCoreActive;
bool afterBurnerCoreActive;
bool emulatorFontActive;
std::int32_t previousEmulator = -1;
std::int32_t emulatorExitState;
std::uint32_t hangOnTexture;
void *hangOnHandle;
void *hangOnWorkspace;
void *hangOnVoice;
std::array<void *, 4> hangOnAudio{};
std::uint8_t hangOnAudioIndex;
std::int32_t hangOnAudioStarted, hangOnPaused, hangOnPauseDebounce, hangOnFramesToSkip = 60, hangOnPlayedFrames;
std::int32_t hangOnCompletionEnabled = 1;
s1::HangOnSharedState hangOnShared{};
s1::HangOnConfig hangOnConfig{};
s1::HangOnUpdateBlock hangOnUpdate{};




bool LoadGameHangOnLibrary() {
#if defined(_WIN32)
    static HMODULE libraryModule{};
    if (libraryModule)
        return true;
    const char *const configuredRoot = std::getenv("SHEN_ROOT");
    const std::filesystem::path directory = configuredRoot && *configuredRoot ? std::filesystem::path(configuredRoot) / "sm2" : std::filesystem::current_path();
    const HMODULE module = LoadLibraryW((directory / L"GameHangon_Winx64_Release.dll").c_str());
    if (!module)
        return false;
    const s1::HangOnLibrary library{
        .computeMemoryRequired = reinterpret_cast<std::size_t (*)(const s1::HangOnConfig *)>(GetProcAddress(module, "HangOn_ComputeMemoryRequired")),
        .create = reinterpret_cast<void *(*)(const s1::HangOnConfig *, void *)>(GetProcAddress(module, "HangOn_Create")),
        .update = reinterpret_cast<void (*)(void *, s1::HangOnUpdateBlock *)>(GetProcAddress(module, "HangOn_Update")),
        .destroy = reinterpret_cast<void (*)(void *)>(GetProcAddress(module, "HangOn_Destroy")),
    };
    if (!library.computeMemoryRequired || !library.create || !library.update || !library.destroy) {
        FreeLibrary(module);
        return false;
    }
    s1::g_hangOnLibrary = library;
    libraryModule = module;
    return true;
#else
    return false;
#endif
}
constexpr SystemModule modules[]{SystemModule::OutRun, SystemModule::AfterBurner, SystemModule::HangOn, SystemModule::SpaceHarrier};
constexpr std::uint32_t thresholds[]{5000000, 500000, 10000000, 10000000};
std::uint32_t ByteSwap(std::uint32_t value) {
    return (value >> 24) | ((value >> 8) & 0xff00U) | ((value << 8) & 0xff0000U) | (value << 24);
}
std::uint32_t UnpackScore(const std::uint8_t *bytes) {
    std::uint32_t value = 0;
    for (int i = 0; i < 4; ++i)
        value = 100 * value + 10 * (bytes[i] >> 4) + (bytes[i] & 15);
    return value;
}
void PackScore(std::uint32_t value, std::uint8_t *bytes) {
    for (int i = 3; i >= 0; --i) {
        const std::uint32_t low = value % 10;
        value /= 10;
        bytes[i] = static_cast<std::uint8_t>(16 * (value % 10) + low);
        value /= 10;
    }
}
EmulatorInfoHeader *Info(EmulatorGame game) {
    switch (game) {
    case EmulatorGame::OutRun:
        return &g_gameOutRunInfo.header;
    case EmulatorGame::AfterBurner:
        return &g_gameAfterBurnerInfo.header;
    case EmulatorGame::HangOn:
        return &g_gameHangOnInfo.header;
    case EmulatorGame::SpaceHarrier:
        return &g_gameHarrierInfo.header;
    }
    return nullptr;
}
std::uint8_t *Rankings(EmulatorGame game) {
    switch (game) {
    case EmulatorGame::OutRun:
        return g_gameOutRunInfo.rankings.data();
    case EmulatorGame::AfterBurner:
        return g_gameAfterBurnerInfo.rankings.data();
    case EmulatorGame::HangOn:
        return g_gameHangOnInfo.rankings.data();
    case EmulatorGame::SpaceHarrier:
        return g_gameHarrierInfo.rankings.data();
    }
    return nullptr;
}
std::array<ArcadeRankingRecord, 20> &SavedRankings(EmulatorGame game) {
    switch (game) {
    case EmulatorGame::OutRun:
        return g_outRunRankings;
    case EmulatorGame::AfterBurner:
        return g_afterBurnerRankings;
    case EmulatorGame::HangOn:
        return g_hangOnRankings;
    case EmulatorGame::SpaceHarrier:
        return g_harrierRankings;
    }
    return g_harrierRankings;
}
std::size_t RankingStride(EmulatorGame game) {
    return game == EmulatorGame::SpaceHarrier ? 10 : game == EmulatorGame::HangOn ? 12 : 14;
}
void AdmitRankings(EmulatorGame game) {
    std::uint8_t *bytes = Rankings(game);
    const std::size_t stride = RankingStride(game);
    if (session.entryId < 0) {
        switch (game) {
        case EmulatorGame::SpaceHarrier:
            std::memcpy(bytes, gameHarrierDefaultRanking.data(), gameHarrierDefaultRanking.size());
            break;
        case EmulatorGame::HangOn:
            std::memcpy(bytes, gameHangOnDefaultRanking.data(), gameHangOnDefaultRanking.size());
            break;
        case EmulatorGame::AfterBurner:
            std::memcpy(bytes, gameAfterBurnerDefaultRanking.data(), gameAfterBurnerDefaultRanking.size());
            break;
        case EmulatorGame::OutRun:
            std::memcpy(bytes, gameOutRunDefaultRanking.data(), gameOutRunDefaultRanking.size());
            break;
        }
        return;
    }
    const std::array<ArcadeRankingRecord, 20> &scores = SavedRankings(game);
    for (std::size_t i = 0; i < 20; ++i) {
        std::uint8_t *record = bytes + i * stride;
        PackScore(scores[i].score, record);
        std::memcpy(record + 4, scores[i].initials.data(), 4);
        if (game == EmulatorGame::HangOn || game == EmulatorGame::AfterBurner)
            std::memcpy(record + 8, &scores[i].extra, 4);
        else if (game == EmulatorGame::OutRun) {
            std::memcpy(record + 10, &scores[i].extra, 4);
            std::memcpy(record + 8, &g_harrierRankings[i].extra, 2);
        }
    }
}
void PublishRankings(EmulatorGame game) {
    if (session.entryId < 0)
        return;
    std::uint8_t *bytes = Rankings(game);
    const std::size_t stride = RankingStride(game);
    std::array<ArcadeRankingRecord, 20> &scores = SavedRankings(game);
    for (std::size_t i = 0; i < 20; ++i) {
        const std::uint8_t *record = bytes + i * stride;
        scores[i].score = UnpackScore(record);
        std::memcpy(scores[i].initials.data(), record + 4, 4);
        if (game == EmulatorGame::HangOn || game == EmulatorGame::AfterBurner)
            std::memcpy(&scores[i].extra, record + 8, 4);
        else if (game == EmulatorGame::OutRun) {


            std::memcpy(&scores[i].extra, record + 10, 4);
            scores[i].extra = record[8] | (static_cast<std::uint32_t>(record[9]) << 8);
        }
    }
}
void ResetSession(SystemModuleTaskData *module) {
    session.result = -1;
    session.mode = 2;
    session.field0c = 0;
    session.audioBootrom = 0;
    session.scoreBcd = 0;
    session.flags = 0;
    if (module->previousModule != SystemModule::Game)
        session.entryId = -1;
    // initials are intentionally carried through admission.
}
void CopySessionToInfo(EmulatorInfoHeader *info, EmulatorGame game) {
    info->game = game;
    info->mode = static_cast<std::int32_t>(session.mode);
    info->result = session.result;
    info->audioBootrom = session.audioBootrom;
    info->entryId = session.entryId;
    info->scoreBcd = session.scoreBcd;
    info->initials = session.initials;
    info->flags = session.flags;
    activeInfo = info;
}
void PublishSession(EmulatorGame game) {
    const EmulatorInfoHeader *info = Info(game);
    session.result = info->result;
    session.entryId = info->entryId;
    session.scoreBcd = ByteSwap(info->scoreBcd);
    session.initials = info->initials;
    session.flags = info->flags;
    PublishRankings(game);
}
void ReturnFromArcade(SystemModuleTaskData *module, EmulatorGame game) {
    previousEmulator = static_cast<std::int32_t>(game);
    PublishSession(game);
    module->currentModule = modules[static_cast<int>(game)];
    ChangeSystemModule(SystemModule::Game, 0);
    if (arcadeReturnToTitle)
        SetShouldReturnToTitleScreen(1);
    emulatorExitState = 0;
}
void InitializeRuntime(const EmulatorInfoHeader *info) {
    GameArcadeRuntime &run = g_gameArcadeRuntime;
    run.coinEnabled = 1;
    run.controlsActive = 0;
    run.controlEnableDelay = 0;
    run.attractReady = 0;
    run.playing = 0;
    run.exitRequested = 0;
    run.displaySuppressed = 0;
    run.pauseFrames = 0;
    run.paused = 0;
    run.debugEnabled = 0;
    run.controllerDisconnected = 0;
    run.disconnectedTimer = 0;
    run.startPulse = 0;
    run.coinPulse = 0;
    run.completion = 0;
    run.bestScore = 0;
    run.exitAfterGame = 0;
    run.skipHighScore = 0;
    run.exitReason = 0;
    run.resetRequested = 0;
    run.debugHoldCount = 0;
    run.playedFrames = 0;
    run.debugAllowed = 1;
    run.coreState = 2;
    run.inputOrientation = 1;
    s1::emu68_insn_defs[0].pattern = 1;
    s1::g_harrierInputOrientation = 1;
    run.credits = info->entryId;
    run.displayCredits = run.credits;
    run.exitDelay = info->exitDelay;
    if (info->mode == 1)
        run.credits = run.displayCredits = -1;
    else if (info->mode == 2)
        run.skipHighScore = 1;
}
void UpdateController() {
    GameArcadeRuntime &run = g_gameArcadeRuntime;
    const LegacyInputSlot &pad = g_inputBuffer[0];
    const std::uint16_t pressed = pad.pressedPriority;
    if (run.coinPulse > 0)
        --run.coinPulse;
    if (run.coinEnabled && run.credits && (pressed & 0x800U))
        run.coinPulse = 5;
    if (run.startPulse > 0)
        --run.startPulse;
    if (run.attractReady && (pressed & 0x800U))
        run.startPulse = 5;
    if (run.playing == 1 && (pressed & 0x800U))
        run.startPulse = 1;
    if ((pad.held & 0x801U) == 0x801U) {
        const std::uint32_t count = static_cast<std::uint32_t>(run.debugHoldCount) + 1U;
        run.debugHoldCount = (run.debugHoldCount & 0xffffffff00000000ULL) | count;
        if (static_cast<std::int32_t>(count) > 180)
            run.resetRequested = 1;
    } else
        run.debugHoldCount &= 0xffffffff00000000ULL;
    if (run.controlEnableDelay) {
        if (run.controlEnableDelay-- == 1)
            run.controlsActive = 1;
    }
    if (run.resumeDelay > 0)
        --run.resumeDelay;
    if (run.controlsActive) {
        if (run.paused) {
            if (run.controllerDisconnected) {
                if (pressed & 0x800U)
                    run.controllerDisconnected = 0;
            } else if (pressed & 0xa00U) {
                run.paused = 0;
                run.resumeDelay = run.coreState ? 10 : 0;
                DestroyElementByIndex(UIElement::GameHud);
            }
        } else if (pressed & 0x800U) {
            run.debugEnabled = 0;
            run.pauseFrames = 0;
            run.paused = 1;
            ShowElementByIndex(UIElement::GameHud);
        }
    } else {
        if (run.paused) {
            run.paused = 0;
            run.resumeDelay = run.coreState ? 10 : 0;
        }
        if (run.controllerDisconnected && (pressed & 0x800U))
            run.controllerDisconnected = 0;
    }
    if (pad.connectionState) {
        if (run.paused == 1)
            ShowArcadeHelp(155);
    } else {
        ShowArcadeHelp(156);
        if (!run.paused) {
            run.debugEnabled = 0;
            run.pauseFrames = 0;
            run.paused = 1;
        }
    }
    if (run.coreState == 1) {
        if (!run.credits) {
            run.exitReason = 0;
            run.exitRequested = 1;
        }
        if (run.exitAfterGame) {
            run.exitReason = 1;
            run.exitRequested = 1;
        }
    }
    if ((pressed & 2U) && (run.coreState == 1 || run.paused == 1)) {
        run.exitReason = 0;
        run.exitRequested = 1;
    }
    if (run.resetRequested) {
        run.exitReason = 2;
        run.exitRequested = 1;
    }
    if (run.exitDelay) {
        if (run.exitDelay-- == 1)
            run.exitAfterGame = 1;
    }
}
void CopyInputToHarrier() {
    const LegacyInputSlot &input = g_inputBuffer[0];
    s1::InputSlot &pad = s1::g_inputState.slots[0];
    pad.enabled = input.enabled;
    pad.held = input.held;
    pad.previousHeld = input.previousHeld;
    pad.pressedPriority = input.pressedPriority;
    pad.repeatedPresses = input.repeatedPresses;
    pad.triggers = input.analog0c | (static_cast<std::uint16_t>(input.analog0d) << 8);
    pad.rightStick = input.analog0e | (static_cast<std::uint16_t>(input.analog0f) << 8);
    pad.leftStick = input.analog10 | (static_cast<std::uint32_t>(input.analog11) << 8) | (static_cast<std::uint32_t>(input.newPresses) << 16);
    pad.xinputReserved = input.connectionState;
}
void PushHarrierRuntime() {
    const GameArcadeRuntime &run = g_gameArcadeRuntime;
    s1::g_harrierCoinEnabled = run.coinEnabled;
    s1::g_harrierAttractReady = run.attractReady;
    s1::g_harrierPlaying = run.playing;
    s1::g_harrierControlsActive = run.controlsActive;
    s1::g_harrierControlEnableDelay = run.controlEnableDelay;
    s1::g_harrierRemainingCredits = run.credits;
    s1::g_harrierDisplayCredits = run.displayCredits;
    s1::g_harrierStartPulse = run.startPulse;
    s1::g_harrierCoinPulse = run.coinPulse;
    s1::g_harrierPaused = run.paused;
    s1::g_harrierPauseFrames = run.pauseFrames;
    s1::g_harrierResumeDelay = run.resumeDelay;
    s1::g_harrierControllerDisconnected = run.controllerDisconnected;
    s1::g_harrierDebugEnabled = run.debugEnabled;
    s1::g_harrierHelpChoice = run.disconnectedTimer;
    s1::g_emu68_state = run.coreState;
    s1::g_harrierSkipHighScore = run.skipHighScore;
    s1::g_harrierContinuePlay = run.continuePlay;
    s1::g_harrierHighScoreEntered = run.completion;
    s1::g_harrierBestScore = run.bestScore;
    s1::g_harrierPlayedFrames = run.playedFrames;
    s1::emu68_insn_defs[0].a = run.debugAllowed;
}
void PullHarrierRuntime() {
    GameArcadeRuntime &run = g_gameArcadeRuntime;
    run.coinEnabled = s1::g_harrierCoinEnabled;
    run.attractReady = s1::g_harrierAttractReady;
    run.playing = s1::g_harrierPlaying;
    run.controlsActive = s1::g_harrierControlsActive;
    run.controlEnableDelay = s1::g_harrierControlEnableDelay;
    run.credits = s1::g_harrierRemainingCredits;
    run.displayCredits = s1::g_harrierDisplayCredits;
    run.startPulse = s1::g_harrierStartPulse;
    run.coinPulse = s1::g_harrierCoinPulse;
    run.coreState = s1::g_emu68_state;
    run.continuePlay = s1::g_harrierContinuePlay;
    run.completion = s1::g_harrierHighScoreEntered;
    run.bestScore = s1::g_harrierBestScore;
    run.debugEnabled = s1::g_harrierDebugEnabled;
    run.pauseFrames = s1::g_harrierPauseFrames;
    run.disconnectedTimer = s1::g_harrierHelpChoice;
}
// Bind the immutable core's actual host consumers, not a second S1 title
// initialization path. Its audio pool uses encoded handles; S2's bank voices
// use pointers and must never be passed into those unchanged functions.
void BindArcadeAudioSettings() {
    const S2AudioSettings &settings = GetS2AudioSettings();
    s1::g_applicationSettings.savedAudioDevice = settings.device;
    s1::g_applicationSettings.savedSfxVolume = settings.sfxVolume;
    s1::g_applicationSettings.savedMusicVolume = settings.musicVolume;
    s1::g_applicationSettings.savedDialogueVolume = settings.dialogueVolume;
}
void HarrierLocalDebugNullsub() {
}

// The immutable core reads the common archive registry mounted by S2
// Initialization(root/sm2,"dx11",2). Neither core initializer mounts S1
// archives or invokes an S1 title/module admission path.
void HarrierStart(EmulatorInfoHeader *info, std::uint8_t *rankings, GameArcadeRuntime *) {
    BindArcadeAudioSettings();
    s1::g_harrierSession.mode = info->mode;
    s1::g_harrierSession.aicaBootrom = info->audioBootrom;
    s1::g_harrierSession.credits = info->entryId;
    std::memcpy(s1::g_harrierSession.rankings, rankings, 200);
    PushHarrierRuntime();
    s1::HarrierInit();
    HarrierLocalDebugNullsub();
    harrierCoreActive = true;
}
void HarrierUpdate(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *run) {
    BindArcadeAudioSettings();
    CopyInputToHarrier();
    PushHarrierRuntime();
    if (!run->paused && !run->resumeDelay) {
        std::uint8_t mask = run->coinPulse ? 0xfeU : 0xffU;
        if (run->startPulse)
            mask -= 16;
        const std::uint16_t held = g_inputBuffer[0].held;
        if (held & 0x600U)
            mask -= 32;
        if (held & 6U)
            mask -= 64;
        if (held & 0x80U)
            mask += 0x80U;
        s1::g_harrierInputMask = mask;
        s1::emu68_Run();
        PullHarrierRuntime();
        if (run->controlsActive) {
            SetXInputAnalogProcessing(1);
            ++run->playedFrames;
        }
    }
    s1::DoAudio(run->paused != 0);
    s1::AudioFlushFromArchivesAndPlay();
    if (!run->paused)
        s1::Harrier_UpdateAudioCommands();
    if (!run->renderFrame) {
        RenderHarrierFrame();
        RenderArcadeControllerOverlay(*run);
    }
}
void HarrierStop(EmulatorInfoHeader *, std::uint8_t *rankings, GameArcadeRuntime *) {
    s1::HarrierDestroy();
    harrierCoreActive = false;
    std::memcpy(rankings, s1::g_harrierSession.rankings, 200);
}
const GameArcadeCoreDriver harrierDriver{.start = HarrierStart, .update = HarrierUpdate, .stop = HarrierStop};
void OutRunStart(EmulatorInfoHeader *, std::uint8_t *rankings, GameArcadeRuntime *run) {
    BindArcadeAudioSettings();
    InitializeOutRunBoard(*run, rankings);
    outRunCoreActive = true;
}
void OutRunUpdate(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *run) {
    BindArcadeAudioSettings();
    CopyInputToHarrier();
    UpdateOutRunBoard(*run);
    PauseArcadeSoundEffects(run->paused != 0);
    AdvanceArcadeMusic(1);
    UpdateOutRunSound(*run);
    if (!run->renderFrame) {
        RenderOutRunFrame();
        RenderArcadeControllerOverlay(*run);
    }
}
void OutRunStop(EmulatorInfoHeader *, std::uint8_t *rankings, GameArcadeRuntime *) {
    CopyOutRunRankings(rankings);
    ReleaseOutRunGraphics();
    ReleaseOutRunSound();
    outRunCoreActive = false;
}
const GameArcadeCoreDriver outRunDriver{.start = OutRunStart, .update = OutRunUpdate, .stop = OutRunStop};
void AfterBurnerStart(EmulatorInfoHeader *, std::uint8_t *rankings, GameArcadeRuntime *run) {
    BindArcadeAudioSettings();
    InitializeAfterBurnerBoard(*run, rankings);
    afterBurnerCoreActive = true;
}
void AfterBurnerUpdate(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *run) {
    BindArcadeAudioSettings();
    UpdateAfterBurnerBoard(*run);
    UpdateAfterBurnerSound(*run);
    if (!run->renderFrame) {
        RenderAfterBurnerFrame(*run);
        RenderArcadeControllerOverlay(*run);
    }
}
void AfterBurnerStop(EmulatorInfoHeader *, std::uint8_t *rankings, GameArcadeRuntime *) {
    CopyAfterBurnerRankings(rankings);
    ReleaseAfterBurnerGraphics();
    ReleaseAfterBurnerSound();
    afterBurnerCoreActive = false;
}
const GameArcadeCoreDriver afterBurnerDriver{.start = AfterBurnerStart, .update = AfterBurnerUpdate, .stop = AfterBurnerStop};
const GameArcadeCoreDriver *Driver(EmulatorGame game) {
    switch (game) {
    case EmulatorGame::SpaceHarrier:
        return &harrierDriver;
    case EmulatorGame::OutRun:
        return &outRunDriver;
    case EmulatorGame::AfterBurner:
        return &afterBurnerDriver;
    default:
        return nullptr;
    }
}
void RunEmulator(EmulatorInfoHeader *info, std::uint8_t *rankings, const GameArcadeCoreDriver &driver) {
    ExecuteCurrentCommandList();
    PresentD3TFrame(2);
    g_gameArcadeRunning = 1;
    InitializeRuntime(info);
    arcadeReturnToTitle = false;
    const d3t::Texture2DDescription description{.width = 640, .height = 480, .arraySize = 1, .mipLevels = 1, .format = 3, .bindFlags = 4, .sampleCount = 1};
    const std::uint32_t texture = d3t::CreateTexture2D(&description);
    InitializeEmulatorFont();
    emulatorFontActive = true;
    DestroyElementByIndex(UIElement::GameMessage);
    driver.start(info, rankings, &g_gameArcadeRuntime);
    g_gameArcadeRuntime.renderFrame = 0;
    SetEngineFlag(7, 0);
    shenSetForcedFilmbars(1);
    while (!g_gameArcadeRuntime.exitRequested && !arcadeReturnToTitle) {
        SetXInputFlags();
#if defined(_WIN32)
        MSG message{};
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            if (message.message == WM_QUIT)
                ShutdownS2Runtime();
        }
#endif
        d3t::GenerateMipsAndUpdateResourcesForDirectX();
        BeginGameArcadeFrame();
        d3t::D3TDeviceContext *context = d3t::DX11::g_ppD3DDeviceContext;
        BindGameArcadeTarget(texture);
#if defined(_WIN32)
        const float black[]{0, 0, 0, 1};
        d3t::UnmapAndClearViews(context, texture, 0, black);
#endif
        HandleInputs();
        UpdateController();
        driver.update(info, rankings, &g_gameArcadeRuntime);
        DrawGameArcadeFullscreen(texture, 2);
        ExecuteCurrentCommandList();
        d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(1);
        RenderUIContents();
        ExecuteCurrentCommandList();
        ResetInputHandler();
        UpdateMipMaps();
#if defined(_WIN32)
        d3t::PresentFrame(2);
#endif
    }
    shenSetForcedFilmbars(0);
    driver.stop(info, rankings, &g_gameArcadeRuntime);
    g_gameArcadeRunning = 0;
    ReleaseEmulatorFont();
    emulatorFontActive = false;
    d3t::InitTexture2DResource(texture);
    RestoreEngineFlag(7);
    info->result = g_gameArcadeRuntime.exitReason;
    info->entryId = g_gameArcadeRuntime.credits;
    info->flags = (static_cast<std::uint64_t>(static_cast<std::uint32_t>(g_gameArcadeRuntime.playedFrames)) << 32) | g_gameArcadeRuntime.completion;
    info->scoreBcd = g_gameArcadeRuntime.bestScore;
    SetProjectionOff();
}
void LongModule(void *data, EmulatorGame game) {
    SystemModuleTaskData *module = static_cast<SystemModuleTaskData *>(data);
    SetXInputFlags();
    SetArcadeHelpContext(game == EmulatorGame::AfterBurner ? 1 : 0);
    EmulatorInfoHeader *info = Info(game);
    std::memset(info, 0, sizeof(GameLongArcadeInfo));
    ResetSession(module);
    AdmitRankings(game);
    CopySessionToInfo(info, game);
    // The system-module admission owner must require GameArcadeModuleAvailable. There is
    // no substitute CPU, fake result, or silent callback for an absent AB/OutRun driver.
    RunEmulator(info, Rankings(game), *Driver(game));
    ReturnFromArcade(module, game);
}
void UploadHangOnFrame() {
#if defined(_WIN32)
    d3t::D3TTextureBackendView view{};
    d3t::GetTexture2DBackendView(hangOnTexture, &view);
    ID3D11DeviceContext *context = static_cast<ID3D11DeviceContext *>(d3t::DX11::g_ppD3DDeviceContext->deviceContext);
    context->UpdateSubresource(static_cast<ID3D11Texture2D *>(view.texture), 0, nullptr, hangOnUpdate.frame, 512 * 4, 0);
#endif
}
void PublishHangOn(SystemModuleTaskData *module) {
    session.result = 0;
    session.entryId = hangOnShared.credits;
    session.scoreBcd = ByteSwap(hangOnShared.field08);
    std::memcpy(session.initials.data(), &hangOnShared.field0c, 4);
    session.flags = (static_cast<std::uint64_t>(static_cast<std::uint32_t>(hangOnShared.field00)) << 32) |
        ((hangOnShared.exitRequested && hangOnCompletionEnabled) ? 1U : 0U);
    if (session.entryId >= 0) {
        std::memcpy(g_gameHangOnInfo.rankings.data(), hangOnShared.scores.data(), 240);
        PublishRankings(EmulatorGame::HangOn);
    }
    module->currentModule = SystemModule::HangOn;
    ChangeSystemModule(SystemModule::Game, 0);
    RetireGameHangOn();
    shenSetForcedFilmbars(0);
    SetPPFXEnabled(1);
    hangOnFramesToSkip = 60;
    hangOnCompletionEnabled = 1;
}
} // namespace
bool GameArcadeModuleAvailable(EmulatorGame game) {
    if (game == EmulatorGame::HangOn)
        return LoadGameHangOnLibrary();
    const GameArcadeCoreDriver *driver = Driver(game);
    return driver && driver->start && driver->update && driver->stop;
}
ArcadeSessionContext *GetArcadeSessionContext() {
    return &session;
}
EmulatorInfoHeader *GetGameArcadeInfo() {
    return activeInfo;
}
void RequestGameArcadeReturnToTitle() {
    g_gameArcadeRuntime.exitRequested = 1;
    arcadeReturnToTitle = true;
}
bool GetIsGameHangOnHandleValid() {
    return hangOnHandle != nullptr;
}
// @verified - p
void SetHangOnCompletionEnabled(std::int32_t enabled) {
    hangOnCompletionEnabled = enabled;
}
void ResetGameArcadeEmulatorResult() {
    previousEmulator = -1;
}
std::int32_t emuGetResult() {
    const std::int32_t game = previousEmulator;
    if (game != -1)
        previousEmulator = -1;
    return game;
}
std::int32_t SetArcadeEmulatorState(EmulatorState state, std::int32_t argument) {
    const std::int32_t index = static_cast<std::int32_t>(state);
    if (index >= 0 && index <= 3) {
        const EmulatorGame game = static_cast<EmulatorGame>(index);
        activeInfo = Info(game);
        constexpr int stats[]{25, 24, 26, 23};
        IncrementSteamStat(stats[index]);
        CheckSteamStats();
        shenQueueChangeModuleAndScene(modules[index], g_actNumber, g_currentMap, argument);
        return 0;
    }
    switch (state) {
    case EmulatorState::SetEntryID:
        session.entryId = argument;
        return argument;
    case EmulatorState::GetEntryID:
        return session.entryId;
    case EmulatorState::GetScoreThresholdReached:
        return thresholds[static_cast<std::size_t>(activeInfo->game)] <= UnpackScore(reinterpret_cast<const std::uint8_t *>(&session.scoreBcd));
    case EmulatorState::GetCompletionFlag:
        return static_cast<std::int32_t>(session.flags);
    default:
        return 0;
    }
}
void GameOutRunModuleCallback(void *data) {
    LongModule(data, EmulatorGame::OutRun);
}
void GameAfterBurnerModuleCallback(void *data) {
    LongModule(data, EmulatorGame::AfterBurner);
}
void GameSpaceHarrierModuleCallback(void *data) {
    SystemModuleTaskData *module = static_cast<SystemModuleTaskData *>(data);
    SetArcadeHelpContext(1);
    g_gameHarrierInfo = {};
    ResetSession(module);
    AdmitRankings(EmulatorGame::SpaceHarrier);
    CopySessionToInfo(&g_gameHarrierInfo.header, EmulatorGame::SpaceHarrier);
    RunEmulator(activeInfo, g_gameHarrierInfo.rankings.data(), *Driver(EmulatorGame::SpaceHarrier));
    SetArcadeHelpContext(1);
    ReturnFromArcade(module, EmulatorGame::SpaceHarrier);
}
void RetireGameHangOn() {
    if (!hangOnHandle)
        return;
    s1::StopRawSoundEffectPlayback(hangOnVoice);
    hangOnAudioStarted = 0;
    s1::g_hangOnLibrary.destroy(hangOnHandle);
    std::free(hangOnWorkspace);
    hangOnHandle = nullptr;
    hangOnWorkspace = nullptr;
    while (s1::RawSoundEffectIsPlaying(hangOnVoice)) {
    }
    for (void *&buffer : hangOnAudio) {
        std::free(buffer);
        buffer = nullptr;
    }
    hangOnFramesToSkip = 60;
}
void ReleaseGameArcadeHostResources() noexcept {
    RetireGameHangOn();
    if (harrierCoreActive) {
        s1::HarrierDestroy();
        harrierCoreActive = false;
    }
    if (outRunCoreActive) {
        ReleaseOutRunGraphics();
        ReleaseOutRunSound();
        outRunCoreActive = false;
    }
    if (afterBurnerCoreActive) {
        ReleaseAfterBurnerGraphics();
        ReleaseAfterBurnerSound();
        afterBurnerCoreActive = false;
    }
    if (emulatorFontActive) {
        ReleaseEmulatorFont();
        emulatorFontActive = false;
    }
    s1::ShutdownXAudio();
    if (hangOnTexture) {
        d3t::InitTexture2DResource(hangOnTexture);
        hangOnTexture = 0;
    }
}
void GameHangOnModuleCallback(void *data) {
    SystemModuleTaskData *module = static_cast<SystemModuleTaskData *>(data);
    SetXInputFlags();
    SetArcadeHelpContext(0);
    BindArcadeAudioSettings();
    if (!hangOnTexture) {
        const d3t::Texture2DDescription description{
            .width = 512, .height = 256, .arraySize = 1, .mipLevels = 1, .format = 0x25, .bindFlags = 1, .sampleCount = 1};
        hangOnTexture = d3t::CreateTexture2D(&description);
    }
    if (!hangOnHandle) {
        DestroyElementByIndex(UIElement::GameMessage);
        DestroyElementByIndex(UIElement::GameHud);
        shenSetForcedFilmbars(1);
        SetPPFXEnabled(0);
        ResetSession(module);
        if (module->previousModule == SystemModule::Game) {
            g_gameHangOnInfo.header.game = EmulatorGame::HangOn;
            activeInfo = &g_gameHangOnInfo.header;
            for (std::size_t i = 0; i < 20; ++i) {
                s1::HangOnScoreRecord &record = hangOnShared.scores[i];
                PackScore(g_hangOnRankings[i].score, record.scoreBcd.data());
                std::memcpy(record.initials.data(), g_hangOnRankings[i].initials.data(), 4);
                record.extra = g_hangOnRankings[i].extra;
            }
        } else
            std::memcpy(hangOnShared.scores.data(), gameHangOnDefaultRanking.data(), 240);
        hangOnShared.field00 = static_cast<std::int32_t>(session.flags >> 32);
        hangOnShared.credits = session.entryId;
        hangOnShared.field08 = session.scoreBcd;
        std::memcpy(&hangOnShared.field0c, session.initials.data(), 4);
        hangOnShared.exitRequested = 0;
        hangOnConfig.mode = static_cast<std::int32_t>(session.mode);
        hangOnConfig.field04 = session.field0c;
        hangOnConfig.shared = &hangOnShared;
        hangOnPlayedFrames = 0;
        const std::size_t bytes = s1::g_hangOnLibrary.computeMemoryRequired(&hangOnConfig);
        hangOnWorkspace = std::malloc(bytes);
        std::memset(hangOnWorkspace, 0, bytes);
        for (void *&buffer : hangOnAudio)
            buffer = std::malloc(3200);
        hangOnHandle = s1::g_hangOnLibrary.create(&hangOnConfig, hangOnWorkspace);
        hangOnVoice = s1::AllocateRawSoundEffectVoice(16, 2, false);
    }
    if (hangOnHandle) {
        const LegacyInputSlot &pad = g_inputBuffer[0];
        const float magnitude = static_cast<float>(std::abs(static_cast<int>(pad.analog0e) - 128)) * 0.0078125f;
        float response = magnitude - ((1 - magnitude) * g_gameHangOnSteeringResponse) * magnitude;
        if (response > 0)
            response = response > .15625f ? response : .15625f;
        const float steering = pad.analog0e < 128 ? 128 - response * 128 : response * 128 + 128;
        hangOnUpdate.steering = static_cast<std::uint8_t>(255 - static_cast<int>(steering));
        hangOnUpdate.accelerator = pad.analog0c;
        hangOnUpdate.brake = (hangOnUpdate.brake & 0xff00U) | pad.analog0d;
        int start = 0;
        if (!hangOnPauseDebounce) {
            if (!hangOnShared.gameInProgress) {
                if (pad.held & 0x800U) {
                    hangOnPauseDebounce = 20;
                    start = 2;
                }
            } else if (hangOnPaused) {
                if (pad.held & 0xa00U) {
                    hangOnPaused = 0;
                    hangOnPauseDebounce = 20;
                    DestroyElementByIndex(UIElement::GameHud);
                }
            } else if (pad.held & 0x800U) {
                hangOnPaused = 1;
                hangOnPauseDebounce = 20;
                ShowElementByIndex(UIElement::GameHud);
            }
        }
        if (hangOnPauseDebounce > 0)
            --hangOnPauseDebounce;
        else
            hangOnPauseDebounce = 0;
        if (pad.held & 0x8000U)
            hangOnUpdate.steering = 0;
        else if (pad.held & 0x4000U)
            hangOnUpdate.steering = 255;
        if (pad.held & 0x1000U)
            hangOnUpdate.accelerator = 255;
        if (pad.held & 0x2000U)
            hangOnUpdate.brake = (hangOnUpdate.brake & 0xff00U) | 255U;
        hangOnUpdate.startSignal = start;
        hangOnUpdate.audioBytes = 3200;
        hangOnUpdate.audioPlayLength = 800;
        hangOnUpdate.audioData = hangOnAudio[hangOnAudioIndex];
        hangOnAudioIndex = (hangOnAudioIndex + 1) & 3U;
        if (pad.connectionState && !hangOnPaused) {
            s1::g_hangOnLibrary.update(hangOnHandle, &hangOnUpdate);
            UploadHangOnFrame();
            if (hangOnVoice) {
                if (!hangOnAudioStarted) {
                    s1::StartRawSoundEffectVoice(hangOnVoice);
                    hangOnAudioStarted = 1;
                }
                s1::SubmitRawSoundEffectBuffer(hangOnVoice, static_cast<const std::byte *>(hangOnUpdate.audioData), 3200, 800);
            }
        } else
            ShowArcadeHelp(pad.connectionState ? 155 : 156);
        if (hangOnFramesToSkip <= 0)
            DrawGameHangOnFrame(hangOnTexture);
        else
            --hangOnFramesToSkip;
        Set60FPSForCredits();
        ++hangOnPlayedFrames;
    }
    if (hangOnShared.gameInProgress) {
        if (!hangOnPaused || !(g_inputBuffer[0].held & 2U))
            return;
    } else if (!(g_inputBuffer[0].held & 0x200U) && hangOnShared.credits && !hangOnShared.exitRequested)
        return;
    shenSetForcedFilmbars(0);
    hangOnPaused = 0;
    PublishHangOn(module);
}
} // namespace shenmue::s2
