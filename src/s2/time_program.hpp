#pragma once

#include "common/types.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

extern std::int32_t g_freezeGameTime;
inline constexpr shen_name TASK_TIME{SHEN_NAME(TIME)};

struct TimeCompassContext {
    std::int32_t enabled;
    std::int32_t heading;
    std::int32_t direction;
    std::int32_t counter;
    Vector3 previousCameraPosition;
    std::uint32_t previousFrame;
    std::int32_t previousSearch;
};
static_assert(sizeof(TimeCompassContext) == 0x24);
static_assert(alignof(TimeCompassContext) == 4);
static_assert(offsetof(TimeCompassContext, heading) == 4);
static_assert(offsetof(TimeCompassContext, counter) == 0x0c);
static_assert(offsetof(TimeCompassContext, previousCameraPosition) == 0x10);
static_assert(offsetof(TimeCompassContext, previousFrame) == 0x1c);
static_assert(offsetof(TimeCompassContext, previousSearch) == 0x20);

inline constexpr std::array<Vector2, 2> TIME_HUD_POSITIONS{{{578.6f, 96.0f}, {578.6f, 96.0f}}};
inline constexpr std::array<float, 2> TIME_HUD_SCALES{1.0f, 1.0f};
inline constexpr std::array<std::uint16_t, 2> TIME_HUD_PERIOD_ICONS{1545, 1546};

} // namespace shenmue::s2
