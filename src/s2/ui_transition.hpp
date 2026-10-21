#pragma once

#include "d3t/ui_runtime.hpp"
#include <cstdint>

namespace shenmue::s2 {
inline constexpr float UI_ANGLE_FULL_TURN = 6.2831898f;

void InitializeUITransitions();
void StartUIIntegerTransition(std::int32_t index, std::int32_t from, std::int32_t to, float duration, float delay);
void StartUIAngleTransition(std::int32_t index, float from, float to, std::int32_t direction, float duration, float delay);
void SetUITransitionLoopMode(std::int32_t index, std::int32_t mode);
bool IsUITransitionActive(std::int32_t index);
float GetUITransitionValue(std::int32_t index);
std::int32_t GetUIIntegerTransitionValue(std::int32_t index);
void ResetUITransition(std::int32_t index);
void RestartUITransition(std::int32_t index);
void SetUITransitionValue(std::int32_t index, float value);

} // namespace shenmue::s2
