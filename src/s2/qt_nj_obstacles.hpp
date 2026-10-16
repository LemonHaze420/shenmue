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
