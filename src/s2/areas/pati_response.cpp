#include "s2/areas/pati_physics.hpp"
#include <cmath>
namespace shenmue::s2 {
// @verified - p
void MarkPatiCollisionSound(PatiBallWork *ball) {
    if (!ball->collision)
        return;
    switch (ball->collision - 1) {
    case 0:
    case 9:
        if (std::fabs(ball->segment.velocity.y) > 0.0f)
            ball->soundFlags |= 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        ball->soundFlags |= 4;
        break;
    case 19:
        ball->soundFlags |= 2;
        break;
    default:
        break;
    }
}
// @verified - p
void BouncePatiFloor(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    b.current.velocity.x = b.tangentialRetention * b.current.velocity.x;
    b.current.velocity.y = -(b.wallRestitution * b.current.velocity.y);
    if (b.current.velocity.y <= 0.0f || b.current.velocity.y + b.gravity < 0.0f) {
        b.current.position.y = b.bottom;
        b.segment.velocity.y = 0.0f;
        b.current.velocity.y = 0.0f;
    }
    if (b.bottom > b.current.position.y)
        b.current.position.y = b.bottom;
}
// @verified - p
void BouncePatiDivider(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    b.current.velocity.x = b.tangentialRetention * b.current.velocity.x;
    b.current.velocity.y = -(b.wallRestitution * b.current.velocity.y);
    if (std::fabs(b.current.velocity.x) < 0.001f) {
        b.current.velocity.x = b.current.velocity.y * 0.03125f;
        if (std::fabs(b.current.velocity.x) < 0.001f)
            b.current.velocity.x = static_cast<float>(b.current.velocity.x > 0.0f ? 1 : -1) * 0.001f;
    }
}
// @verified - p
void BouncePatiPocketFloor(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    b.current.velocity.x = b.tangentialRetention * b.current.velocity.x;
    b.current.velocity.y = -(b.wallRestitution * b.current.velocity.y);
    if (b.current.velocity.y <= 0.0f || b.current.velocity.y + b.gravity < 0.0f) {
        b.current.position.y = b.centerPockets[b.collisionIndex].y + b.radius;
        b.segment.velocity.y = 0.0f;
        b.current.velocity.y = 0.0f;
    }
    const float floor = b.centerPockets[b.collisionIndex].y + b.radius;
    if (floor > b.current.position.y)
        b.current.position.y = floor;
}
// @verified - p
void BouncePatiPoint(PatiBallWork *ball, const Vector3 *point, float restitution) {
    PatiBallWork &b = *ball;
    float nx = point->x - b.current.position.x, ny = point->y - b.current.position.y, nz = 0.0f;
    const float length = std::sqrt((ny * ny + nx * nx) + 0.0f);
    if (length > 0.0f) {
        nx = nx * (1.0f / length);
        ny = ny * (1.0f / length);
        nz = (1.0f / length) * 0.0f;
    }
    const float impulse = ((ny * b.current.velocity.y + nx * b.current.velocity.x) + nz * b.current.velocity.z) * (restitution + 1.0f);
    const float vy = b.current.velocity.y - impulse * ny;
    b.current.velocity.y = vy;
    b.current.velocity.x = b.current.velocity.x - impulse * nx;
    if (b.current.position.y > point->y) {
        if (std::fabs(b.current.position.x - point->x) < 0.0049999999f && std::fabs(b.current.velocity.x) < 0.001f && std::fabs(b.segment.velocity.x) < 0.001f)
            b.current.velocity.x = (std::fabs(vy) * static_cast<float>(b.current.position.x > point->x ? 1 : -1)) * 0.03125f;
        if (b.current.position.y > point->y && std::fabs(b.current.velocity.x) < 0.001f && std::fabs(vy) < 0.0050000004f)
            b.current.velocity.x = static_cast<float>(b.current.position.x > point->x ? 1 : -1) * 0.001f;
    }
}
// @verified - p
void BouncePatiNail(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    std::uint32_t oldest = 0;
    std::int32_t oldestIndex = 0;
    for (std::int32_t index = 0; index < 8; ++index) {
        PatiNailContact &contact = b.contacts[index];
        if (contact.age < 99999)
            ++contact.age;
        if (contact.age >= oldest) {
            oldest = contact.age;
            oldestIndex = index;
        }
    }
    std::int32_t index = 0;
    while (index < 8 && b.contacts[index].nail != b.collisionIndex)
        ++index;
    if (index >= 8) {
        PatiNailContact &contact = b.contacts[oldestIndex];
        contact.nail = b.collisionIndex;
        contact.age = 0;
        contact.count = 1;
    } else {
        b.contacts[index].age = 0;
        ++b.contacts[index].count;
    }
    const Vector3 &nail = b.nails->nails[b.collisionIndex].position;
    const Vector3 point{nail.x, nail.y, 0.0f};
    BouncePatiPoint(ball, &point, b.nailRestitution);
}
// @verified - p
void ResolvePatiCollision(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    if (!b.collision)
        return;
    b.beforeCollision = b.current;
    const float t = b.collisionTime;
    b.current.position.x = b.segment.position.x * (1.0f - t) + b.segment.position.x * t;
    b.current.position.y = b.segment.position.y * (1.0f - t) + b.segment.position.y * t;
    b.current.velocity.x = b.segment.velocity.x * (1.0f - t) + b.segment.velocity.x * t;
    b.current.velocity.y = b.segment.velocity.y * (1.0f - t) + b.segment.velocity.y * t;
    Vector3 point;
    switch (b.collision - 1) {
    case 0:
        BouncePatiFloor(ball);
        return;
    case 1:
    case 2:
        b.current.velocity.x = -(b.segment.velocity.x * b.wallRestitution);
        return;
    case 3:
    case 8:
        BouncePatiDivider(ball);
        return;
    case 4:
    case 5:
    case 11:
    case 12:
        b.current.velocity.x = -(b.current.velocity.x * b.wallRestitution);
        return;
    case 6:
        point = {b.dividers[b.collisionIndex].x + b.dividerWidth, b.dividers[b.collisionIndex].y, 0.0f};
        break;
    case 7:
        point = {b.dividers[b.collisionIndex].x, b.dividers[b.collisionIndex].y, 0.0f};
        break;
    case 9:
        BouncePatiPocketFloor(ball);
        return;
    case 10: {
        const float vx = b.current.velocity.x * b.tangentialRetention;
        b.current.velocity.y = -(b.current.velocity.y * b.wallRestitution);
        b.current.velocity.x = vx;
        return;
    }
    case 13: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {(p.x - p.halfWidth) - b.dividerWidth, p.y - b.dividerWidth, 0.0f};
        break;
    }
    case 14: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {(p.x + p.halfWidth) + b.dividerWidth, p.y - b.dividerWidth, 0.0f};
        break;
    }
    case 15: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {(p.x - p.halfWidth) - b.dividerWidth, p.y + p.height, 0.0f};
        break;
    }
    case 16: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {p.x - p.halfWidth, p.y + p.height, 0.0f};
        break;
    }
    case 17: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {p.x + p.halfWidth, p.y + p.height, 0.0f};
        break;
    }
    case 18: {
        const PatiCenterPocket &p = b.centerPockets[b.collisionIndex];
        point = {(p.x + p.halfWidth) + b.dividerWidth, p.y + p.height, 0.0f};
        break;
    }
    case 19:
        BouncePatiNail(ball);
        return;
    default:
        return;
    }
    BouncePatiPoint(ball, &point, b.wallRestitution);
}
// @verified - p
bool IsPatiBallSettled(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    const std::uint32_t frames = b.frames++;
    if (static_cast<float>(frames) > 2700.0f)
        return true;
    const Vector2 old = b.history[b.historyCursor];
    const float dx = old.x - b.current.position.x, dy = old.y - b.current.position.y;
    if (++b.historyCursor >= 45)
        b.historyCursor = 0;
    const Vector2 oldest = b.history[b.historyCursor];
    b.history[b.historyCursor] = {b.current.position.x, b.current.position.y};
    b.movement[b.historyCursor] = (dy * dy + dx * dx) + 0.0f;
    const float oldestX = oldest.x - b.current.position.x, oldestY = oldest.y - b.current.position.y;
    const float distance = (oldestY * oldestY + oldestX * oldestX) + 0.0f;
    float sum = 0.0f;
    for (std::int32_t index = 0; index < 44; index += 4)
        sum = ((sum + b.movement[index]) + b.movement[index + 1]) + b.movement[index + 2] + b.movement[index + 3];
    sum = sum + b.movement[44];
    if (sum < 0.000045000001f && distance < 0.0000030000001f)
        return true;
    std::uint32_t contacts = 0;
    for (const PatiNailContact &contact : b.contacts)
        if (contact.count > contacts)
            contacts = contact.count;
    return contacts > 20;
}
} // namespace shenmue::s2
