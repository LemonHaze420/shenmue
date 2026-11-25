#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/orientation_arc.hpp"
#include "s2/math.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "common/native_sqrt.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

std::int32_t UnwrapAngleNear(std::int32_t reference, std::int32_t target, std::int32_t angle) {
    if (reference < target) {
        angle = (angle & 0xffff) + (target & static_cast<std::int32_t>(0xffff0000U)) + 0x10000;
        while (angle >= target)
            angle -= 0x10000;
    } else {
        angle = (angle & 0xffff) + (target & static_cast<std::int32_t>(0xffff0000U)) - 0x10000;
        while (angle <= target)
            angle += 0x10000;
    }
    return angle;
}

void SolveOrientationAngles(OrientationArc *arc, const Vector2 *points, float radius) {
    const std::int32_t approach = InverseFixedAtan2(points[0].x - arc->origin.x, points[0].y - arc->origin.y);
    const std::int32_t departure = InverseFixedAtan2(points[1].x - points[0].x, points[1].y - points[0].y);
    arc->entry.x = am2NegativeSineFixed(departure) * radius + points[0].x;
    arc->entry.y = am2NegativeCosineFixed(departure) * radius + points[0].y;
    float start[2], end[2], center[2];
    start[0] = am2NegativeSineFixed(approach) * -radius;
    start[1] = -radius * am2NegativeCosineFixed(approach);
    end[0] = am2NegativeSineFixed(departure) * radius;
    end[1] = am2NegativeCosineFixed(departure) * radius;
    float determinant = start[0] * end[1];
    determinant -= start[1] * end[0];
    if (-0.0002f > determinant || determinant > 0.0002f) {
        const float scale = radius * radius / determinant;
        center[0] = (end[1] - start[1]) * scale;
        center[1] = scale * (-end[0] + start[0]);
        const float dx = center[0] - start[0], dz = center[1] - start[1];
        arc->radius = shenmue::NativeSqrt(dx * dx + dz * dz);
        arc->startAngle = InverseFixedAtan2(start[0] - center[0], start[1] - center[1]);
        arc->endAngle = InverseFixedAtan2(end[0] - center[0], end[1] - center[1]);
    } else {
        center[0] = start[0];
        center[1] = start[1];
        arc->radius = 0.0f;
        arc->endAngle = 0;
        arc->startAngle = 0;
    }
    arc->center[0] = points[0].x + center[0];
    arc->center[1] = points[0].y + center[1];
    if (determinant > 0.0002f) {
        while (arc->endAngle < arc->startAngle)
            arc->endAngle += 0x10000;
    } else if (-0.0002f > determinant) {
        while (arc->endAngle > arc->startAngle)
            arc->endAngle -= 0x10000;
    } else {
        arc->endAngle = arc->startAngle;
    }
}

std::int32_t GetOrientationArcHeading(const OrientationArc *arc, const Vector2 *position, const Vector2 *target) {
    const float *radius = &arc->radius;
    std::int32_t heading;
    if (*radius > 0.0f) {
        const float dx = position->x - arc->center.x;
        std::uint32_t horizontal;
        std::memcpy(&horizontal, &dx, sizeof(horizontal));
        const float dz = position->y - arc->center.y;
        std::uint32_t vertical;
        std::memcpy(&vertical, &dz, sizeof(vertical));
        heading = InverseFixedAtan2(dx, dz);
        float x, z;
        std::memcpy(&x, &horizontal, sizeof(x));
        std::memcpy(&z, &vertical, sizeof(z));
        float distance;
        distance = shenmue::NativeSqrt(z * z + x * x);
        const float remaining = *radius - distance;
        heading = UnwrapAngleNear(arc->startAngle, arc->endAngle, heading);
        distance *= static_cast<float>(arc->endAngle - heading);
        distance *= 9.5873799e-05f;
        const std::int32_t turn = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(remaining, distance)) - 0x4000;
        heading -= turn;
    } else {
        heading = InverseFixedAtan2(target->x - position->x, target->y - position->y);
    }
    return heading;
}

} // namespace s2
} // namespace shenmue

#endif
