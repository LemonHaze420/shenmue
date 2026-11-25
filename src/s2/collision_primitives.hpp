#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

struct CollisionSegment {
    Vector3 origin;
    Vector3 direction;
};
struct CollisionSphere {
    Vector3 center;
    float radius;
};
struct CollisionRay {
    Vector3 origin;
    Vector3 direction;
    std::int32_t hitState;
    float hitTime;
    float radius;
};
typedef char CollisionSegmentSizeCheck[sizeof(CollisionSegment) == 0x18 ? 1 : -1];
typedef char CollisionSphereSizeCheck[sizeof(CollisionSphere) == 0x10 ? 1 : -1];
typedef char CollisionRayHitStateCheck[offsetof(CollisionRay, hitState) == 0x18 ? 1 : -1];
typedef char CollisionRayHitTimeCheck[offsetof(CollisionRay, hitTime) == 0x1c ? 1 : -1];
typedef char CollisionRayRadiusCheck[offsetof(CollisionRay, radius) == 0x20 ? 1 : -1];

std::int32_t collPointEqual3(const Vector3 *first, const Vector3 *second);
std::int32_t collPointOnSegment(const Vector3 *point, const CollisionSegment *segment);
std::int32_t collSphereSpherePair(const Vector3 *point, const CollisionSphere *sphere);
std::int32_t collSegmentSphere(const CollisionSegment *segment, const CollisionSphere *sphere);
std::int32_t collRaySphereSolve(CollisionRay *ray, float offsetSquared, const Vector3 *offset);
void collSweptRayPoint(const Vector3 *point, CollisionRay *ray);
void collRaySphere(const CollisionSphere *sphere, CollisionRay *ray);
void collSweptRaySphere(const CollisionSphere *sphere, CollisionRay *ray);
std::int32_t collPrimitiveNullsubZero();
void collPrimitiveNullsubA();
void collPrimitiveNullsubB();

} // namespace s2
} // namespace shenmue

#endif
