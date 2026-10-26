#include "s1/timing.hpp"

#include "platform/performance_counter.hpp"

#include <cmath>

namespace shenmue::s1 {
constexpr float kFrameTimeConstantDelta = 12500000.0f;
constexpr double kFrameTimeModulus = 4294967295.0;

float QueryFrameTime() {
    if (!CycleManager_bHasQueriedFrameTime) {
        CycleManager_bHasQueriedFrameTime = 1;
        CycleManager_timePerTick = kFrameTimeConstantDelta / static_cast<float>(platform::performanceCounterFrequency());
    }
    const float scaled = static_cast<float>(platform::performanceCounter()) * CycleManager_timePerTick;
    return static_cast<float>(std::fmod(static_cast<double>(scaled), kFrameTimeModulus));
}

void UpdateFrameTiming() {
    g_CurrentFrameDelta = FrameTimeTicks(g_LastFrameTime - QueryFrameTime());
}

std::uint8_t CycleManager_bHasQueriedFrameTime = 0;
float CycleManager_timePerTick = 0.0f;
float frameTime = 0.0f;
float g_LastFrameTime = 0.0f;
std::int32_t g_CurrentFrameDelta = 0;
std::int32_t CycleManager_DeltaTime = 0;
std::int32_t CycleManager_DeltaTime_0 = 0;
std::int32_t g_TimeDelta = 0;
std::uint8_t g_bEnableFrameTimeSmoothing = 0;
float fFrameTimeMs = 0.0f;

std::int64_t FrameTimeUtil(TimeMode mode) {
    const float currentTime = QueryFrameTime();
    switch (mode) {
    case TimeMode::InitializeGlobalTime:
        g_LastFrameTime = currentTime;
        return 0;
    case TimeMode::UpdateGlobalTime:
        g_LastFrameTime += currentTime - frameTime;
        break;
    case TimeMode::UpdateInputTime:
        frameTime = currentTime;
        break;
    case TimeMode::ReturnNewTime:
        return static_cast<std::int64_t>(g_LastFrameTime - currentTime);
    }
    return static_cast<std::int64_t>(currentTime);
}

#if SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX
float GetNpcMovementFrameScale() {
    if (!g_bEnableFrameTimeSmoothing)
        return 1.0f;
    const float scale = fFrameTimeMs * 30.0f;
    return scale > 0.0f && scale <= 1.0f ? scale : 1.0f;
}
#endif

float ScaleByFrameTime30fps(float value) {
    if (g_bEnableFrameTimeSmoothing) {
        return value * 30.0f * fFrameTimeMs;
    }
    return value;
}

} // namespace shenmue::s1
