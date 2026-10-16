#pragma once
#include "s1/task/cycle_man.hpp"

namespace shenmue::s1 {

namespace CycleManager {

struct BikeAnimCurve {
    std::uint16_t startFrame;
    std::int16_t startValue;
    std::uint16_t endFrame;
    std::int16_t endValue;
};
} // namespace CycleManager

} // namespace shenmue::s1
