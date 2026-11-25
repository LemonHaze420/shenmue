#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/collision_primitives.hpp"

#include <math.h>

namespace shenmue {
namespace s2 {

std::int32_t collPointEqual3(const Vector3 *first, const Vector3 *second) {
    if (first->x == second->x && first->y == second->y && first->z == second->z)
        return 1;
    return 0;
}

std::int32_t collPointOnSegment(const Vector3 *point, const CollisionSegment *segment) {
    if (segment->direction.x == 0.0f && segment->direction.y == 0.0f && segment->direction.z == 0.0f)
        return collPointEqual3(point, &segment->origin);
    Vector3 offset, direction, end;
    offset.x = segment->origin.x - point->x;
    offset.y = segment->origin.y - point->y;
    offset.z = segment->origin.z - point->z;
    direction = segment->direction;
    if (offset.x * direction.y != direction.x * offset.y || offset.y * direction.z != offset.z * direction.y)
        return 0;
    end.x = offset.x + direction.x;
    end.y = offset.y + direction.y;
    end.z = offset.z + direction.z;
    if (offset.x * end.x + offset.y * end.y + offset.z * end.z > 0.0f)
        return 0;
    return 1;
}

std::int32_t collSphereSpherePair(const Vector3 *point, const CollisionSphere *sphere) {
    Vector3 offset;
    offset.x = point->x - sphere->center.x;
    offset.y = point->y - sphere->center.y;
    offset.z = point->z - sphere->center.z;
    if (Vector3::CalcMagnitude(offset) > sphere->radius)
        return 0;
    return 1;
}

std::int32_t collSegmentSphere(const CollisionSegment *segment, const CollisionSphere *sphere) {
    if (!(sphere->radius > 0.0f))
        return collPointOnSegment(&sphere->center, segment);
    if (segment->direction.x == 0.0f && segment->direction.y == 0.0f && segment->direction.z == 0.0f)
        return collSphereSpherePair(&segment->origin, sphere);
    Vector3 offset, direction;
    float length, projection, start;
    offset.x = segment->origin.x - sphere->center.x;
    offset.y = segment->origin.y - sphere->center.y;
    offset.z = segment->origin.z - sphere->center.z;
    direction = segment->direction;
    start = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z - sphere->radius * sphere->radius;
    if (!(start > 0.0f))
        return 1;
    length = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    if (length == 0.0f)
        return 0;
    projection = direction.x * offset.x + direction.y * offset.y + direction.z * offset.z;
    if (!(length + projection + projection + start > 0.0f))
        return 1;
    const float distance = -projection;
    float zero = 0.0f;
    if (zero > distance || distance > length)
        return 0;
    if (zero > projection * projection - length * start)
        return 0;
    return 1;
}

std::int32_t collRaySphereSolve(CollisionRay *ray, float offsetSquared, const Vector3 *offset) {
    float length, projection, discriminant, root, time;
    if (!(offsetSquared > 0.0f)) {
        ray->hitState = 2;
        return 0;
    }
    length = ray->direction.x * ray->direction.x + ray->direction.y * ray->direction.y + ray->direction.z * ray->direction.z;
    if (length == 0.0f)
        return 0;
    projection = ray->direction.x * offset->x + ray->direction.y * offset->y + ray->direction.z * offset->z;
    if (projection > 0.0f)
        return 0;
    discriminant = projection * projection - length * offsetSquared;
    if (0.0f > discriminant)
        return 0;
    root = sqrtf(discriminant);
    time = (-projection - root) / length;
    if ((ray->hitState == 1 && !(0.0f > time) && ray->hitTime > time) || (ray->hitState == 0 && !(0.0f > time))) {
        ray->hitState = 1;
        ray->hitTime = time;
    }
    return 0;
}

void collSweptRayPoint(const Vector3 *point, CollisionRay *ray) {
    if (ray->hitState == 2)
        return;
    Vector3 offset;
    offset.x = ray->origin.x - point->x;
    offset.y = ray->origin.y - point->y;
    offset.z = ray->origin.z - point->z;
    collRaySphereSolve(ray, offset.x * offset.x + offset.y * offset.y + offset.z * offset.z - ray->radius * ray->radius, &offset);
}

void collRaySphere(const CollisionSphere *sphere, CollisionRay *ray) {
    if (ray->hitState == 2)
        return;
    Vector3 offset;
    offset.x = ray->origin.x - sphere->center.x;
    offset.y = ray->origin.y - sphere->center.y;
    offset.z = ray->origin.z - sphere->center.z;
    collRaySphereSolve(ray, offset.x * offset.x + offset.y * offset.y + offset.z * offset.z - sphere->radius * sphere->radius, &offset);
}

void collSweptRaySphere(const CollisionSphere *sphere, CollisionRay *ray) {
    if (ray->hitState == 2)
        return;
    Vector3 offset;
    offset.x = ray->origin.x - sphere->center.x;
    offset.y = ray->origin.y - sphere->center.y;
    offset.z = ray->origin.z - sphere->center.z;
    collRaySphereSolve(
        ray, offset.x * offset.x + offset.y * offset.y + offset.z * offset.z - (ray->radius + sphere->radius) * (ray->radius + sphere->radius), &offset);
}

std::int32_t collPrimitiveNullsubZero() {
    return 0;
}

void collPrimitiveNullsubA() {
}

void collPrimitiveNullsubB() {
}

} // namespace s2
} // namespace shenmue

#endif
