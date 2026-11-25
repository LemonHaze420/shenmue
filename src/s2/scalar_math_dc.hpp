#pragma once

#include "s2/math.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

float InterpolateTo(float target, float value, float weight);
float GetRandomNumberInRange(float bound);
std::int32_t shenGetRandomInt32(std::int32_t bound);

} // namespace s2
} // namespace shenmue

#endif
