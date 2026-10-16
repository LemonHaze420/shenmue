#pragma once

#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

extern std::int32_t g_doLowMipsOnly;
extern std::int32_t g_dxPipelineLock;
extern std::uint32_t g_bPauseGameTasks;

extern std::uint8_t g_bSkipCycles;
std::uint8_t GetSkipCyclesFlag();
std::uint32_t GetFramesPast();
std::uint32_t GetFramesPastCopy();
void UpdateRootFrameCounters();
void IncrementRootFrameCopy();

struct DisplayInfo {
    std::int32_t flags;
    std::uint32_t width;
    std::uint32_t height;
};

struct GameConfig {
    DisplayInfo display;
    std::int32_t refreshRateNumerator;
    std::int64_t refreshRateDenominator;
    std::int32_t two;
    std::int32_t sixteen;
    std::int32_t one;
    std::byte reserved_24[4];
    Matrix4x4 *matrices;
    std::uint32_t matrixCount;
    std::int32_t memoryLimit;
};

struct SteamAppInfo {
    std::uint32_t appId;
    std::uint32_t reserved;
    const char *appName;
    const char *buildId;
};

static_assert(sizeof(DisplayInfo) == 0xc);
static_assert(sizeof(GameConfig) == 0x38);
static_assert(offsetof(GameConfig, matrices) == 0x28);
static_assert(sizeof(SteamAppInfo) == 0x18);

bool Steam_InitApp(const SteamAppInfo *appInfo);
bool Steam_TryGetCurrentUserId(std::uint64_t *steamId);
void SteamInit();
void UpdateSteamStat(std::uint32_t statId);
void UpdateSteamAchievement(std::uint32_t achievementId);

extern std::uint32_t g_memAlignment;
extern std::uint64_t g_initializationTemporaryClearColour;
extern void *g_globalMemory;
extern std::uint32_t g_globalMemorySize;
extern std::uint8_t g_frameWaitEnabled[2];
extern float g_deltaTime;
extern std::int32_t g_framesPerSecond;

extern std::uint8_t g_debugUiDisabled;



struct EyeRigParameters {
    float lidYawScale;
    float lidPitchScale;
    float yawLimit;
    float pitchLimit;
};

static_assert(sizeof(EyeRigParameters) == 0x10);

extern EyeRigParameters g_eyeRigParameters;

extern float g_blinkDeltaStepTime;

std::int64_t InitShenmue(GameConfig *config);
std::int64_t ShenmueInit(GameConfig *config);
bool IsMainRenderHandleBusy();
void ReleaseAicaRam();
void ReleaseLargeDataBuffer();
void ReleaseRenderHandleLogs();
} // namespace shenmue::s1
