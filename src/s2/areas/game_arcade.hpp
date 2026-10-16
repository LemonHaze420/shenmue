#pragma once
#include "s2/arcade.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
#pragma pack(push, 1)
struct GameHarrierInfo {
    EmulatorInfoHeader header;
    std::array<std::uint8_t, 200> rankings;
};
struct GameLongArcadeInfo {
    EmulatorInfoHeader header;
    std::array<std::uint8_t, 280> rankings;
};
struct GameHangOnInfo {
    EmulatorInfoHeader header;
    std::array<std::uint8_t, 240> rankings;
};
#pragma pack(pop)
static_assert(sizeof(GameHarrierInfo) == 0xf8 && offsetof(GameHarrierInfo, rankings) == 0x30);
static_assert(sizeof(GameLongArcadeInfo) == 0x148 && offsetof(GameLongArcadeInfo, rankings) == 0x30);
static_assert(sizeof(GameHangOnInfo) == 0x120 && offsetof(GameHangOnInfo, rankings) == 0x30);

struct GameArcadeRuntime {
    std::int32_t credits, displayCredits, coinEnabled, attractReady, playing, controlsActive, controlEnableDelay;
    std::int32_t coinPulse, startPulse, pauseFrames, paused, resumeDelay, controllerDisconnected;
    std::int32_t exitRequested, exitReason, exitAfterGame, resetRequested, exitDelay, playedFrames;
    std::int32_t skipHighScore, continuePlay, debugEnabled, debugAllowed, inputOrientation;
    std::int32_t disconnectedTimer, coreState, renderFrame, displaySuppressed;
    std::uint32_t bestScore;
    std::uint32_t completion;
    std::uint64_t debugHoldCount;
};
// Game-owned start/update/stop selection. The Harrier interpreter alone does
// not provide AfterBurner/OutRun's page maps, opcode hooks, graphics or sound.
struct GameArcadeCoreDriver {
    void (*start)(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *);
    void (*update)(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *);
    void (*stop)(EmulatorInfoHeader *, std::uint8_t *, GameArcadeRuntime *);
};
bool GameArcadeModuleAvailable(EmulatorGame);
ArcadeSessionContext *GetArcadeSessionContext();
std::int32_t SetArcadeEmulatorState(EmulatorState, std::int32_t argument);
EmulatorInfoHeader *GetGameArcadeInfo();
void RequestGameArcadeReturnToTitle();
void ResetGameArcadeEmulatorResult();
std::int32_t emuGetResult();
void GameOutRunModuleCallback(void *);
void GameAfterBurnerModuleCallback(void *);
void GameSpaceHarrierModuleCallback(void *);
void GameHangOnModuleCallback(void *);
void RetireGameHangOn();
// Final S2 runtime retirement, before renderer/archive/thread owners disappear.
void ReleaseGameArcadeHostResources() noexcept;
bool GetIsGameHangOnHandleValid();
void SetHangOnCompletionEnabled(std::int32_t enabled);
extern GameHarrierInfo g_gameHarrierInfo;
extern GameLongArcadeInfo g_gameOutRunInfo, g_gameAfterBurnerInfo;
extern GameHangOnInfo g_gameHangOnInfo;
extern std::int32_t g_gameArcadeRunning;
extern std::uint8_t g_arcadeRoadControl;
extern GameArcadeRuntime g_gameArcadeRuntime;
extern float g_gameHangOnSteeringResponse;
} // namespace shenmue::s2
