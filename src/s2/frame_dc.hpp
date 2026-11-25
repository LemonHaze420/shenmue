#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"

namespace shenmue {
namespace s2 {

static const std::uintptr_t kFrameTimerCounterRegister = 0xffd8000c;
extern std::uint32_t g_defaultVblankInterval;
extern std::uint32_t g_frameRateVblankInterval;
extern std::uint32_t g_savedFrameRateVblankInterval;
extern std::uint32_t g_frameCounter;
extern volatile std::uint32_t g_vblankCounter;
extern std::uint32_t g_developerDisplayEnabled;
extern std::uint32_t g_gameSystemsInitialized;
extern std::int32_t g_softResetProcessingEnabled;
extern std::int32_t g_backupProcessingEnabled;
extern std::uint32_t g_frameTimerStart;
extern std::uint32_t g_framePostUpdateTicks;
extern float g_frameMicroseconds;
extern float g_frameMicrosecondsHistory[32];
extern std::uint32_t g_previousFrameTimer;
extern std::uint32_t g_previousVblankCounter;
extern std::int32_t g_discTrayFrames;
extern std::int32_t g_exitOnSoftReset;
extern std::uint32_t g_pvrTaOlBase;
extern std::uint32_t g_vertexBufferUsage[5];
extern std::uint32_t g_vertexBufferHighWater[5];
extern std::uint32_t g_additionalVertexBufferUsage;
extern std::uint32_t g_additionalVertexBufferHighWater;

typedef void (*FrameInterrupt)();
extern FrameInterrupt g_frameInterrupt;

void SetLoadingBusy();
bool SetTargetFramerate(std::uint32_t framesPerSecond);
void SetSoftResetProcessing(std::int32_t enabled);
void SetBackupProcessing(std::int32_t enabled);
void InitializeFrameTiming();
void FrameInterruptCallback();
void DispatchFrameInterrupt();
void InitializeFrameInterrupt();
void SetFrameInterruptCallback(FrameInterrupt callback);
void WaitForVblankCounter(std::uint32_t counter);
void ProcessSoftReset();
void ProcessDiscTrayState();
void FramePostUpdateCallback();
void RootTaskInitialize(void *taskData);
void RootTaskCallback(void *taskData);

} // namespace s2
} // namespace shenmue

#endif
