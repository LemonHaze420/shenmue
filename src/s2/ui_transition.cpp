#include "s2/ui_transition.hpp"

#include <bit>
#include <cmath>

namespace shenmue::s2 {
namespace {
void ClearTransition(UIAnimSlot &slot, std::int32_t allocated) {
    slot.allocated = allocated;
    slot.interpolation = 0;
    slot.status = 0;
    slot.start[0] = 0.0f;
    slot.current[0] = 0.0f;
    slot.end[0] = 0.0f;
    slot.duration = 0.0f;
    slot.delay = 0.0f;
    slot.elapsed = 0.0f;
    slot.loopMode = 0;
    slot.reserved40 = -1;
}
}

// @verified - p
void InitializeUITransitions() {
    for (std::size_t index = 0; index < g_uiAnimSlots.size(); ++index) {
        ClearTransition(g_uiAnimSlots[index], 0);
        UIWidget_ClearBinding(static_cast<std::int32_t>(index));
    }
}

// @verified - p
void StartUIIntegerTransition(std::int32_t index, std::int32_t from, std::int32_t to, float duration, float delay) {
    if (!(duration > 0.0f))
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(index)];
    slot.start[0] = std::bit_cast<float>(from);
    slot.current[0] = std::bit_cast<float>(from);
    slot.end[0] = std::bit_cast<float>(to);
    slot.duration = duration;
    slot.delay = delay;
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 3;
    slot.elapsed = 0.0f;
    slot.reserved40 = -1;
}

// @verified - p
void StartUIAngleTransition(std::int32_t index, float from, float to, std::int32_t direction, float duration, float delay) {
    if (!(duration > 0.0f))
        return;
    if (direction == -1) {
        if (from > to)
            to += UI_ANGLE_FULL_TURN;
    } else if (to > from) {
        to += -UI_ANGLE_FULL_TURN;
    }
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(index)];
    slot.start[0] = from;
    slot.current[0] = from;
    slot.end[0] = to;
    slot.duration = duration;
    slot.delay = delay;
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 2;
    slot.elapsed = 0.0f;
    slot.reserved40 = -1;
}

// @verified - p
void SetUITransitionLoopMode(std::int32_t index, std::int32_t mode) {
    g_uiAnimSlots[static_cast<std::size_t>(index)].loopMode = mode;
}

// @verified - p
bool IsUITransitionActive(std::int32_t index) {
    return g_uiAnimSlots[static_cast<std::size_t>(index)].status != 0;
}

// @verified - p
float GetUITransitionValue(std::int32_t index) {
    return g_uiAnimSlots[static_cast<std::size_t>(index)].current[0];
}

// @verified - p
std::int32_t GetUIIntegerTransitionValue(std::int32_t index) {
    return std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(index)].current[0]);
}
void ResetUITransition(std::int32_t index) {
    ClearTransition(g_uiAnimSlots[static_cast<std::size_t>(index)], 0);
}

// @verified - p
void RestartUITransition(std::int32_t index) {
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(index)];
    slot.status = 0;
    slot.elapsed = 0.0f;
    slot.allocated = 1;
}

// @verified - p
void SetUITransitionValue(std::int32_t index, float value) {
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(index)];
    slot.current[0] = value;
    const float distance = std::fabs(value - slot.start[0]);
    const float extent = std::fabs(slot.end[0] - slot.start[0]);
    float progress = 0.0f;
    if (!(distance >= extent)) {
        if (distance != 0.0f && extent != 0.0f)
            progress = distance / extent;
    } else {
        progress = 1.0f;
    }
    slot.elapsed = (slot.duration + slot.delay) * progress;
}

} // namespace shenmue::s2
