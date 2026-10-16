#include "s2/areas/pati_physics.hpp"
#include <cmath>
namespace shenmue::s2 {
namespace {
void SelectCollision(PatiBallWork &b, float time, std::uint16_t kind, std::uint16_t index) {
    if (time > b.collisionTime) {
        b.collisionTime = time;
        b.collision = kind;
        b.collisionIndex = index;
    }
}
void SelectPoint(PatiBallWork &b, const Vector3 &point, std::uint16_t kind, std::uint16_t index) {
    const float time = ProbePatiPoint(&b, &point);
    if (time >= 0.0f)
        SelectCollision(b, time, kind, index);
}
float CrossingTime(float edge, float current, float previous) {
    const float delta = previous - current;
    return std::fabs(delta) >= 0.001f ? (edge - current) / delta : 0.0f;
}
} // namespace
// @verified - p
float ProbePatiPoint(PatiBallWork *ball, const Vector3 *point) {
    const PatiBallState &from = ball->segment;
    const PatiBallState &to = ball->current;
    const float dx = to.position.x - from.position.x, dy = to.position.y - from.position.y;
    const float px = point->x - to.position.x, py = point->y - to.position.y;
    if (((point->y - from.position.y) * to.velocity.y + (point->x - from.position.x) * to.velocity.x) + to.velocity.z * 0.0f <= 0.0f)
        return -1.0f;
    const float length = dy * dy + dx * dx;
    const float dot = py * dy + px * dx;
    const float discriminant = dot * dot - ((py * py + px * px) - ball->radius * ball->radius) * length;
    if (discriminant < 0.0f)
        return -2.0f;
    if (std::fabs(length) < 0.00000011920929f)
        return -3.0f;
    const float root = static_cast<float>(std::sqrt(static_cast<double>(discriminant)));
    const float reciprocal = 1.0f / length;
    float time = (root - dot) * reciprocal;
    if (!(time < 0.0f) && !(time > 1.0f))
        return time;
    time = (-dot - root) * reciprocal;
    if (!(time < 0.0f) && !(time > 1.0f))
        return time;
    return -4.0f;
}
// @verified - p
std::uint8_t IntersectPatiSegments(const Vector3 *current, const Vector3 *previous, const Vector3 *first, const Vector3 *second) {
    const float ax = previous->x - first->x, ay = second->y - previous->y;
    const float bx = second->x - previous->x, by = previous->y - first->y;
    const float cy = first->y - current->y, cx = first->x - current->x;
    const float dy = current->y - second->y, dx = current->x - second->x;
    const float a = ay * ax - bx * by, b = dy * bx - dx * ay, c = dx * cy - dy * cx;
    if (cx * by - ax * cy < 0.0f) {
        if (a >= 0.0f || b >= 0.0f || c >= 0.0f)
            return 2;
    } else if (a < 0.0f || b < 0.0f || c < 0.0f)
        return 2;
    return 1;
}
// @verified - p
void FindPatiDividerCollision(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    for (std::int16_t index = 0; index + 1 < b.pocketCount; ++index) {
        const Vector3 &d = b.dividers[index];
        const float right = d.x + b.dividerWidth, outerRight = d.x + (b.radius + b.dividerWidth);
        const float top = d.y + b.radius, bottom = d.y - b.dividerHeight, left = d.x - b.radius;
        Vector3 first{right, top, d.z}, second{d.x, top, d.z};
        if (b.current.velocity.y < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(top, b.current.position.y, b.segment.position.y), 4, index);
        first = {outerRight, d.y, d.z};
        second = {outerRight, bottom, d.z};
        if (b.current.velocity.x < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(outerRight, b.current.position.x, b.segment.position.x), 5, index);
        first = {left, d.y, d.z};
        second = {left, bottom, d.z};
        if (b.current.velocity.x > 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(left, b.current.position.x, b.segment.position.x), 6, index);
        SelectPoint(b, Vector3{right, d.y, d.z}, 7, index);
        SelectPoint(b, d, 8, index);
    }
}
// @verified - p
void FindPatiCenterCollision(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    if (b.option != 8)
        return;
    for (std::uint16_t index = 0; index < 6; ++index) {
        const PatiCenterPocket &p = b.centerPockets[index];
        if (!p.type)
            continue;
        const float radius = b.radius, left = p.x - p.halfWidth, right = p.x + p.halfWidth;
        const float outerLeft = left - b.dividerWidth, outerRight = right + b.dividerWidth;
        const float top = p.height + p.y, bottom = p.y - b.dividerWidth;
        const float vx = b.current.velocity.x, vy = b.current.velocity.y;
        Vector3 first{left, top + radius, 0.0f}, second{outerLeft, top + radius, 0.0f};
        if (vy < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(top + radius, b.current.position.y, b.segment.position.y), 9, index);
        first = {left + radius, p.y, 0.0f};
        second = {left + radius, top, 0.0f};
        if (vx < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(left + radius, b.current.position.x, b.segment.position.x), 13, index);
        first = {outerLeft - radius, bottom, 0.0f};
        second = {outerLeft - radius, top, 0.0f};
        if (vx > 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(outerLeft - radius, b.current.position.x, b.segment.position.x), 12, index);
        first = {outerRight, top + radius, 0.0f};
        second = {right, top + radius, 0.0f};
        if (vy < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(top + radius, b.current.position.y, b.segment.position.y), 9, index);
        first = {outerRight + radius, bottom, 0.0f};
        second = {outerRight + radius, top, 0.0f};
        if (vx < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(outerRight + radius, b.current.position.x, b.segment.position.x), 13, index);
        first = {right - radius, p.y, 0.0f};
        second = {right - radius, top, 0.0f};
        if (vx > 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(right - radius, b.current.position.x, b.segment.position.x), 12, index);
        first = {left, radius + p.y, 0.0f};
        second = {right, radius + p.y, 0.0f};
        if (vy < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(radius + p.y, b.current.position.y, b.segment.position.y), 10, index);
        first = {outerLeft, bottom - radius, 0.0f};
        second = {outerRight, bottom - radius, 0.0f};
        if (vy < 0.0f && IntersectPatiSegments(&b.current.position, &b.segment.position, &first, &second) == 1)
            SelectCollision(b, CrossingTime(bottom - radius, b.current.position.y, b.segment.position.y), 11, index);
        SelectPoint(b, Vector3{left, top, 0.0f}, 17, index);
        SelectPoint(b, Vector3{outerLeft, top, 0.0f}, 16, index);
        SelectPoint(b, Vector3{outerRight, top, 0.0f}, 19, index);
        SelectPoint(b, Vector3{right, top, 0.0f}, 18, index);
        SelectPoint(b, Vector3{outerLeft, bottom, 0.0f}, 14, index);
        SelectPoint(b, Vector3{outerRight, bottom, 0.0f}, 15, index);
    }
}
// @verified - p
void FindPatiNailCollision(PatiBallWork *ball) {
    for (std::uint16_t index = 0; index < 512; ++index) {
        const Vector3 &point = ball->nails->nails[index].position;
        if (point.y > 0.0f)
            SelectPoint(*ball, point, 20, index);
    }
}
// @verified - p
void FindPatiCollision(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    if (b.current.velocity.y < 0.0f && b.bottom > b.current.position.y)
        SelectCollision(b, CrossingTime(b.bottom, b.current.position.y, b.segment.position.y), 1, b.collisionIndex);
    if (b.current.velocity.x < 0.0f && b.left > b.current.position.x)
        SelectCollision(b, CrossingTime(b.left, b.current.position.x, b.segment.position.x), 3, b.collisionIndex);
    if (b.current.velocity.x > 0.0f && b.current.position.x > b.right)
        SelectCollision(b, CrossingTime(b.right, b.current.position.x, b.segment.position.x), 2, b.collisionIndex);
    FindPatiDividerCollision(ball);
    FindPatiCenterCollision(ball);
    FindPatiNailCollision(ball);
}
} // namespace shenmue::s2
