#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include "common/math.hpp"
#include <stddef.h>
#if !SHENMUE_IS_DC
#include <array>
#endif

namespace shenmue { namespace s2 {

extern std::int32_t g_freezeGameTime;
#if SHENMUE_IS_DC
#define TASK_TIME SHEN_NAME(TIME)
#else
inline constexpr shen_name TASK_TIME{SHEN_NAME(TIME)};
#endif

struct TimeCompassContext {
    std::int32_t enabled;
    std::int32_t heading;
    std::int32_t direction;
    std::int32_t counter;
    Vector3 previousCameraPosition;
    std::uint32_t previousFrame;
    std::int32_t previousSearch;
};
typedef char TimeProgramLayoutCheck0[((sizeof(TimeCompassContext) == 0x24) ? 1 : -1)];
#if SHENMUE_S2_GCC295
typedef char TimeProgramLayoutCheck1[((__alignof__(TimeCompassContext) == 4) ? 1 : -1)];
#else
static_assert(alignof(TimeCompassContext) == 4);
#endif
typedef char TimeProgramLayoutCheck2[((offsetof(TimeCompassContext, heading) == 4) ? 1 : -1)];
typedef char TimeProgramLayoutCheck3[((offsetof(TimeCompassContext, counter) == 0x0c) ? 1 : -1)];
typedef char TimeProgramLayoutCheck4[((offsetof(TimeCompassContext, previousCameraPosition) == 0x10) ? 1 : -1)];
typedef char TimeProgramLayoutCheck5[((offsetof(TimeCompassContext, previousFrame) == 0x1c) ? 1 : -1)];
typedef char TimeProgramLayoutCheck6[((offsetof(TimeCompassContext, previousSearch) == 0x20) ? 1 : -1)];

#if SHENMUE_IS_DC
static const Vector2 TIME_HUD_POSITIONS[2] = {{581.6f, 96.0f}, {581.6f, 96.0f}};
static const float TIME_HUD_SCALES[2] = {1.0f, 1.0f};
static const std::uint16_t TIME_HUD_PERIOD_ICONS[2] = {1545, 1546};
#else
inline constexpr std::array<Vector2, 2> TIME_HUD_POSITIONS{{{578.6f, 96.0f}, {578.6f, 96.0f}}};
inline constexpr std::array<float, 2> TIME_HUD_SCALES{1.0f, 1.0f};
inline constexpr std::array<std::uint16_t, 2> TIME_HUD_PERIOD_ICONS{1545, 1546};
#endif

} } // namespace shenmue::s2
