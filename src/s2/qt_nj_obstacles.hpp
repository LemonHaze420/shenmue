#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"

#define QTNJ_OBSTACLE_BASE SHEN_NAME4('T', 'N', 'J', 'B')
#define QTNJ_OBSTACLE_0 SHEN_NAME4('T', 'N', 'J', '0')
#define QTNJ_OBSTACLE_1 SHEN_NAME4('T', 'N', 'J', '1')
#define QTNJ_OBSTACLE_2 SHEN_NAME4('T', 'N', 'J', '2')
#define QTNJ_OBSTACLE_3 SHEN_NAME4('T', 'N', 'J', '3')
#define QTNJ_OBSTACLE_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define QTNJ_OBSTACLE_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QTNJ_OBSTACLE_INVALID 0xffffffffU
#define QTNJ_STAGGER_MOTION 0x2143
#define QTNJ_STAGGER_LOOP_MOTION 0x2142
#define QTNJ_REN_COME_ON_MOTION 0x2147
#define QTNJ_BREAK_STICK_MOTION 0x2155

typedef struct QtnjObstacleWork {
    shen_name obstacle;
    int trial;
    const shen_name *npcs;
    int waitFrames;
} QtnjObstacleWork;

int PlayQtnjObstacle(QtnjObstacleWork *work, int delay);
void InitializeQtnjObstacle(shen_name obstacle);
int DispatchQtnjObstacleImpact(shen_name obstacle, int trial, int delay);
int QueueQtnjFirstObstacleImpact(shen_name obstacle, shen_name hidden, int delay);
int QueueQtnjSecondObstacleImpact(shen_name obstacle, shen_name hidden, int delay);
int QueueQtnjThirdObstacleImpact(shen_name obstacle, shen_name hidden, int delay);
int QueueQtnjFourthObstacleImpact(shen_name obstacle, shen_name hidden, int delay);
#else
#pragma once

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct QtnjObstacleWork {
    shen_name obstacle;
    std::int32_t trial;
    const shen_name *npcs;
    std::int32_t waitFrames;
    std::int32_t reserved14;
};
static_assert(sizeof(QtnjObstacleWork) == 0x18);
static_assert(offsetof(QtnjObstacleWork, npcs) == 8);
static_assert(offsetof(QtnjObstacleWork, waitFrames) == 0x10);
static_assert(offsetof(QtnjObstacleWork, reserved14) == 0x14);

void InitializeQtnjObstacle(shen_name obstacle);
std::uint32_t PlayQtnjObstacle(QtnjObstacleWork *work);
std::uint32_t DispatchQtnjObstacleImpact(shen_name obstacle, std::int32_t trial);
std::uint32_t QueueQtnjFirstObstacleImpact(shen_name obstacle);
std::uint32_t QueueQtnjSecondObstacleImpact(shen_name obstacle);
std::uint32_t QueueQtnjThirdObstacleImpact(shen_name obstacle);
std::uint32_t QueueQtnjFourthObstacleImpact(shen_name obstacle);

} // namespace shenmue::s2
#endif
