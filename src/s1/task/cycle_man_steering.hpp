#pragma once
#include "s1/task/cycle_man.hpp"

namespace shenmue::s1 {

namespace CycleManager {


struct CycleAreaCapacity {
    std::uint32_t areaId;
    std::int32_t capacity;
};

enum RadialStepResult : std::int32_t {
    NoHit = 0,
    InsideBoth = 1,
    StayInside = 2,
    CrossesCircle = 3,
};
} // namespace CycleManager

} // namespace shenmue::s1
