#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"

namespace shenmue {
namespace s2 {

struct OrientationArc {
    Vector2 origin;
    Vector2 entry;
    Vector2 center;
    float radius;
    std::int32_t startAngle;
    std::int32_t endAngle;
};
typedef char OrientationArcSizeCheck[sizeof(OrientationArc) == 0x24 ? 1 : -1];

std::int32_t UnwrapAngleNear(std::int32_t reference, std::int32_t target, std::int32_t angle);
void SolveOrientationAngles(OrientationArc *arc, const Vector2 *points, float radius);
std::int32_t GetOrientationArcHeading(const OrientationArc *arc, const Vector2 *position, const Vector2 *target);

} // namespace s2
} // namespace shenmue

#endif
