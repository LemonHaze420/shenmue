#pragma once

#include <cstdint>

namespace shenmue::s1 {

enum class TimeMode : std::int32_t {
    InitializeGlobalTime = 0,
    UpdateGlobalTime = 1,
    UpdateInputTime = 2,
    ReturnNewTime = 3,
};


inline constexpr float kSeqFramesPerSecond = 30.0f;

extern std::uint8_t CycleManager_bHasQueriedFrameTime;
extern float CycleManager_timePerTick;
extern float frameTime;
extern float g_LastFrameTime;
extern std::int32_t g_CurrentFrameDelta;
extern std::int32_t CycleManager_DeltaTime;
extern std::int32_t CycleManager_DeltaTime_0;
extern std::int32_t g_TimeDelta;
extern std::uint8_t g_bEnableFrameTimeSmoothing;
extern float fFrameTimeMs;

float QueryFrameTime();
void UpdateFrameTiming();
std::int64_t FrameTimeUtil(TimeMode mode);
float ScaleByFrameTime30fps(float value);
#if SHENMUE_ENHANCEMENT_S1_NPC_60FPS_FIX
float GetNpcMovementFrameScale();
#endif


inline std::int32_t FrameTimeTicks(float elapsed) {
    return static_cast<std::int32_t>(static_cast<std::int64_t>(elapsed));
}

} // namespace shenmue::s1
