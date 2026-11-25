#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
typedef struct ButterflyRegion {
    short flags, totalRuns;
    Vector3 position;
} ButterflyRegion;
typedef struct ButterflyMotion {
    Vector3 velocity, filteredVelocity, target, position;
    int regionIndex, phase, phaseStep, flags;
} ButterflyMotion;
typedef struct ButterflyTaskData {
    ButterflyRegion *regions;
    int butterflyCount;
    ButterflyMotion motion[16];
} ButterflyTaskData;
void EVT_MoveButterfly_Callback(void *taskData);
void QueueButterflies(ButterflyRegion *regions, int count);
#else
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {

struct ButterflyRegion {
    std::int16_t flags;
    std::int16_t totalRuns;
    Vector3 position;
};
struct ButterflyMotion {
    Vector3 velocity;
    Vector3 filteredVelocity;
    Vector3 target;
    Vector3 position;
    std::int32_t regionIndex;
    std::int32_t phase;
    std::int32_t phaseStep;
    std::int32_t flags;
};
struct ButterflyTaskData {
    const ButterflyRegion *regions;
    std::int32_t butterflyCount;
    std::array<ButterflyMotion, 16> motion;
    std::uint32_t reserved40c;
};
inline constexpr shen_name TASK_BUTTERFLY{SHEN_NAME(bfly)};
static_assert(sizeof(ButterflyRegion) == 0x10);
static_assert(offsetof(ButterflyRegion, position) == 0x04);
static_assert(sizeof(ButterflyMotion) == 0x40);
static_assert(offsetof(ButterflyMotion, filteredVelocity) == 0x0c);
static_assert(offsetof(ButterflyMotion, target) == 0x18);
static_assert(offsetof(ButterflyMotion, position) == 0x24);
static_assert(offsetof(ButterflyMotion, regionIndex) == 0x30);
static_assert(offsetof(ButterflyMotion, flags) == 0x3c);
static_assert(offsetof(ButterflyTaskData, motion) == 0x0c);
static_assert(sizeof(ButterflyTaskData) == 0x410);
void EVT_MoveButterfly_Callback(void *taskData);

} // namespace shenmue::s2
#endif
