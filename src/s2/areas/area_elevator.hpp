#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct AreaElevatorBinding {
    std::uint32_t active;
    shen_name characters[4];
    Vector3 position;
    Vector3i rotation;
    std::uint32_t floorMasks[4];
};
static_assert(sizeof(AreaElevatorBinding) == 60);
static_assert(offsetof(AreaElevatorBinding, position) == 20);
static_assert(offsetof(AreaElevatorBinding, rotation) == 32);
static_assert(offsetof(AreaElevatorBinding, floorMasks) == 44);
extern AreaElevatorBinding areaElevatorsQA00[5], areaElevatorsQD00[4], areaElevatorsQF00[5];
extern AreaElevatorBinding areaElevatorsQC00[3], areaElevatorsQE00[4], areaElevatorsQJBT[2];
extern AreaElevatorBinding *g_areaElevatorBindings;
void ElevatorStuff();
Vector3 *UpdateAreaElevatorBindings(const Vector3 *floorPosition, std::uint32_t floor);
} // namespace shenmue::s2
