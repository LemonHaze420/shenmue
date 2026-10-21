#pragma once

#include <cstdint>

namespace shenmue::s2 {

using FramePacingDecision = std::int32_t (*)() noexcept;
using LoadingScreenUpdate = void (*)();
using LoadingScreenToggle = void (*)(std::int32_t vSyncCount, std::int32_t mode);
extern LoadingScreenUpdate g_updateLoadingScreen;
extern LoadingScreenToggle g_toggleLoadingScreen;
extern bool framePacingTimerUpdated;
extern std::int32_t g_skipArchiveReadWait;

extern std::uint8_t g_stabilizeFps;

extern float g_stabilizedFrameTime;
extern double measuredFrameMilliseconds;
extern char framePerformanceText[64];
extern std::int32_t framePerformanceSampleState;
std::int64_t ProfileFramePerformance();

// Frame-pacing fields written by InitializeDebugDefaults: the minimum and maximum unlocked-framerate waits and the
// renderer-readiness wait.
extern std::int32_t minimumUnlockedWaitSeconds;
extern std::int32_t maximumUnlockedWaitSeconds;
extern bool waitForRendererReadiness;


void UpdateFramePacing();
void UpdateCurrentFramesRendered(std::uint32_t updateUi);
std::int32_t GetNumFramesProcessed() noexcept;

std::int32_t ConsumeFrames();
void FlushFramePacingRenderQueue();
extern std::int32_t g_archiveExternalChecksEnabled;
bool IsArchiveReadComplete() noexcept;
bool IsArchiveReadPending() noexcept;
std::int64_t ReadArchivesAndFlushRenderQueue();
bool FlushCurrentMessageAndRenderQueue();
float GetFrameTime() noexcept;
float GetLoadingElapsedSeconds() noexcept;
float shenGetScaledDeltaTickTime(float frames) noexcept;




void WaitForNextFrameWithCallback(const std::uint32_t *handles, std::int32_t count, FramePacingDecision getTotalFramesToRender);

void SetFramePacingRendererInitialized(bool initialized) noexcept;
std::int32_t GetTotalFramesToRender() noexcept;


std::int64_t AssignPreviousFrameTimeDelta(std::int32_t paused);

std::int64_t ReturnWindowHandle() noexcept;

} // namespace shenmue::s2
