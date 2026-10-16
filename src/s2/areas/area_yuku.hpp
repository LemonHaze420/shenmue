#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name YUKU_TASK{SHEN_NAME(EVNT)}, YUKU_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name YUKU_STOP{SHEN_NAME(STOP)}, YUKU_INVALID{0xffffffffU};
inline constexpr Vector3 YUKU_LOOK_POSITION{.x = 1398.3f, .y = 100.0f, .z = 798.0f};
inline constexpr Vector3 YUKU_WALK_POSITION{.x = 1393.64f, .y = 100.0f, .z = 796.4f};
inline constexpr Vector3 YUKU_STOP_POSITION{.x = 1392.45f, .y = 99.88f, .z = 795.86f};
inline constexpr Vector3 YUKU_RETURN_POSITION{.x = 1390.71f, .y = 99.86f, .z = 794.86f};
inline constexpr Vector3i YUKU_LOOK_ROTATION{.x = 0, .y = 9648, .z = 0};
inline constexpr Vector3i YUKU_STOP_ROTATION{.x = 0, .y = 43455, .z = 0};
extern std::uint64_t yukuFrameCounter;
extern std::uint32_t yukuState;
extern shen_name yukuEntry;
void YukuInit();
void YukuMain(std::uintptr_t argument);
void YukuInitializeTask(void *taskData);
void YukuEventCallback(void *taskData);
void YukuStopCallback(void *taskData);
void YukuDormantNullsub(void *taskData);
extern const ScriptAreaDefinition yukuScriptDefinition;

} // namespace shenmue::s2
