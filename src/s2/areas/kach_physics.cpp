#include "s2/areas/area_kach.hpp"
#include "s2/areas/kach_tables.hpp"
#include <cmath>
namespace shenmue::s2 {
namespace {

float ImpactRoot(float a, float b, float c) {
    if (a == 0)
        return b > 0 ? -(c / (b + b)) : -1;
    const float discriminant = b * b - a * c;
    if (discriminant < 0)
        return -1;
    const float root = std::sqrt(discriminant);
    const float time = (root - b) / a;
    return (a + a) * time + (b + b) < 0 ? (-b - root) / a : time;
}
float ConeImpact(const KachMotionState &state, const KachCone &cone) {
    const auto &p = state.position;
    const auto &v = state.velocity;
    const float y = p.y - cone.height, square = cone.slope * cone.slope;
    return ImpactRoot((v.x * v.x + v.z * v.z) * square - v.y * v.y, (v.x * p.x + v.z * p.z) * square - v.y * y, (p.x * p.x + p.z * p.z) * square - y * y);
}
void PushOutDisk(KachMotionState &state, const KachDisk &disk) {
    auto &p = state.position;
    const float dx = p.x - disk.x, dz = p.z - disk.z;
    if (dx * dx + dz * dz > disk.radius * disk.radius)
        return;
    const float radius = std::sqrt(p.z * p.z + p.x * p.x), nx = p.x / radius, nz = p.z / radius;
    const float step = ImpactRoot(nx * nx + nz * nz, dz * nz + dx * nx, dx * dx + dz * dz - disk.radius * disk.radius) * 1.01f;
    p.x = step * nx + p.x;
    p.y = step * kachBowlCones[3].slope + p.y;
    p.z = step * nz + p.z;
}
void ProjectSurface(KachMotionState &state) {
    auto &p = state.position;
    const float radius = std::sqrt(p.z * p.z + p.x * p.x);
    for (const auto &cone : kachBowlCones) {
        const float reach = (p.y - cone.height) / cone.slope;
        if (radius > reach || reach < 0)
            p.y = (((cone.slope * radius + cone.height) - p.y) + 0.000099999997f) + p.y;
    }
    for (const auto &disk : kachBowlDisks)
        PushOutDisk(state, disk);
}
bool AdvanceCollision(KachMotionState &state, std::int32_t &strength) {
    float time = state.remainingTime;
    std::int32_t index = -1, kind = 0;
    for (std::int32_t i = 0; i < 4; ++i) {
        const float hit = ConeImpact(state, kachBowlCones[i]);
        if (hit > 0 && time >= hit) {
            time = hit;
            index = i;
            kind = 1;
        }
    }
    const auto oldPosition = state.position, oldVelocity = state.velocity;
    for (std::int32_t i = 0; i < 4; ++i) {
        const auto &disk = kachBowlDisks[i];
        const float dx = oldPosition.x - disk.x, dz = oldPosition.z - disk.z;
        const float hit = ImpactRoot(-(oldVelocity.z * oldVelocity.z + oldVelocity.x * oldVelocity.x), -(oldVelocity.z * dz + oldVelocity.x * dx),
            -(dz * dz + dx * dx - disk.radius * disk.radius));
        if (hit > 0 && time >= hit) {
            time = hit;
            index = i;
            kind = 2;
        }
    }
    if (kind) {
        Vector3 position{oldVelocity.x * time + oldPosition.x, oldVelocity.y * time + oldPosition.y, oldVelocity.z * time + oldPosition.z};
        Vector3 normal;
        float rebound;
        if (kind == 1) {
            const float radius = std::sqrt(position.z * position.z + position.x * position.x);
            normal = {-(kachBowlCones[index].slope * position.x / radius), 1, -(kachBowlCones[index].slope * position.z / radius)};
            const float magnitude = std::sqrt((normal.x * normal.x + 1) + normal.z * normal.z);
            normal.x /= magnitude;
            normal.y = 1 / magnitude;
            normal.z /= magnitude;
            rebound = 0.5f;
        } else {
            const auto &disk = kachBowlDisks[index];
            const float x = position.x - disk.x, z = position.z - disk.z, radius = std::sqrt(z * z + x * x);
            normal = {x / radius, (radius / 10) / radius, z / radius};
            rebound = 0.34999999f;
        }
        const float projection = (oldVelocity.x * normal.x + oldVelocity.y * normal.y) + oldVelocity.z * normal.z;
        const Vector3 velocity{(oldVelocity.x - normal.x * projection) - (normal.x * projection) * rebound,
            (oldVelocity.y - normal.y * projection) - (normal.y * projection) * rebound,
            (oldVelocity.z - normal.z * projection) - (normal.z * projection) * rebound};
        if (kind == 2) {
            const float x = oldVelocity.x - velocity.x, y = oldVelocity.y - velocity.y, z = oldVelocity.z - velocity.z;
            std::int32_t hit = static_cast<std::int32_t>(1 - std::sqrt((y * y + x * x) + z * z) * -0.5f * 256);
            if (hit > 255)
                hit = 255;
            if (strength < hit)
                strength = hit;
        }
        state.position = position;
        state.velocity = velocity;
        state.remainingTime -= time;
        ProjectSurface(state);
        state.velocity.y -= time * 4.9000001f;
        return true;
    }
    state.position.x = state.remainingTime * oldVelocity.x + oldPosition.x;
    state.position.y = state.remainingTime * oldVelocity.y + oldPosition.y;
    state.position.z = state.remainingTime * oldVelocity.z + oldPosition.z;
    ProjectSurface(state);
    state.velocity.y -= state.remainingTime * 4.9000001f;
    return false;
}
} // namespace
// @verified - p
std::uint32_t UpdateKachBall(Vector3 *position, Vector3 *velocity, const Vector3 *origin, std::uint16_t angle) {
    const float x = position->x - origin->x, y = position->y - origin->y, z = position->z - origin->z;
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle)), cosine = cos_fixed16(angle);
    KachMotionState state{{sine * z + cosine * x, y, cosine * z - sine * x}, *velocity, 0.033333335f};
    const float radiusSquare = state.position.z * state.position.z + state.position.x * state.position.x;
    std::int32_t strength = 0;
    std::uint32_t result = 0;
    if (y - (std::sqrt(radiusSquare) * 0.066666998f + 0.030022001f) >= 0.001f ||
        (velocity->y * velocity->y + velocity->x * velocity->x) + velocity->z * velocity->z >= 0.0001f || radiusSquare >= 0.00033124f) {
        for (std::int32_t i = 0; i < 10; ++i)
            if (!AdvanceCollision(state, strength))
                break;
    } else {
        state.velocity = {};
        result = (state.position.z < 0 ? 2U : 0U) + (state.position.x < 0) + 1;
    }
    position->x = (state.position.x * cosine - state.position.z * sine) + origin->x;
    position->y = state.position.y + origin->y;
    position->z = (state.position.z * cosine + state.position.x * sine) + origin->z;
    velocity->x = state.velocity.x * 0.98000002f;
    velocity->y = state.velocity.y * 0.98000002f;
    velocity->z = state.velocity.z * 0.98000002f;
    return result + (static_cast<std::uint32_t>(strength) << 8);
}
} // namespace shenmue::s2
