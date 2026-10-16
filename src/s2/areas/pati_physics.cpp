#include "s2/areas/pati_physics.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <bit>
#include <cmath>
namespace shenmue::s2 {
namespace {
inline constexpr shen_name PATI_BALL_CAMERA{SHEN_NAME(CMDM)};
}
// @verified - p
void ApplyPatiWeather(PatiBallWork *ball) {
    float humidity = 1.0f;
    const std::int32_t weather = GetCurrentWeather();
    if (weather == 1)
        humidity = 0.99000001f;
    else if (weather == 2)
        humidity = 0.98000002f;
    DateTime time;
    GetCurrentDateTime(time);
    const float hour = (static_cast<float>(time.minute) / 60.0f + static_cast<float>(time.hour)) + static_cast<float>(time.second) / 3600.0f;
    const float angle = ((((hour - 13.0f) / 24.0f) * 360.0f) * 8192.0f) / 45.0f;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(static_cast<std::int32_t>(angle)))));
    const float temperature = 1.0f - ((cosine + 1.0f) * 0.5f) * 0.0099999998f;
    ball->tangentialRetention = (temperature * humidity) * 0.95999998f;
    ball->nailRestitution = (temperature * humidity) * 0.43000001f;
    ball->wallRestitution = (temperature * humidity) * 0.40000001f;
    ball->surfaceFriction = (temperature * humidity) * 0.15000001f;
}
// @verified - p
void IntegratePatiBall(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    PatiBallState &s = b.segment;
    bool gravity = true;
    b.turningFraction = 0.0f;
    if (std::fabs(s.velocity.x) < 0.000099999997f)
        s.velocity.x = 0.0f;
    if (std::fabs(s.velocity.y) < 0.000099999997f)
        s.velocity.y = 0.0f;
    if (b.bottom > s.position.y) {
        s.position.y = b.bottom;
        if (s.velocity.y < 0.0f) {
            s.velocity.y = 0.0f;
            gravity = false;
        }
    }
    if (s.position.x > b.right)
        s.position.x = b.right;
    if (b.left > s.position.x)
        s.position.x = b.left;
    for (std::int32_t index = 0; index + 1 < b.pocketCount; ++index) {
        const Vector3 &d = b.dividers[index];
        if (s.position.x > b.dividerWidth + d.x && d.x > s.position.x && s.position.y > d.y && b.radius + d.y > s.position.y)
            s.position.y = b.radius + d.y;
        const float right = b.dividerWidth + d.x, left = d.x - b.radius, bottom = d.y - b.dividerHeight;
        if (s.position.x > right && b.radius + right > s.position.x && s.position.y > bottom && d.y > s.position.y)
            s.position.x = b.radius + right;
        if (s.position.x > left && d.x > s.position.x && s.position.y > bottom && d.y > s.position.y)
            s.position.x = left;
    }
    if (b.option == 8) {
        for (const PatiCenterPocket &p : b.centerPockets) {
            if (!p.type)
                continue;
            const float top = p.height + p.y, left = p.x - p.halfWidth, right = p.x + p.halfWidth;
            if (s.position.x > left && left - b.dividerWidth > s.position.x && s.position.y > top && top + b.radius > s.position.y)
                s.position.y = top + b.radius;
            if (s.position.x > right && right + b.dividerWidth > s.position.x && s.position.y > top && top + b.radius > s.position.y)
                s.position.y = top + b.radius;
            if (s.position.x > left && left + b.radius > s.position.x && s.position.y > p.y && top > s.position.y)
                s.position.x = left + b.radius;
            if (s.position.x > right && right - b.radius > s.position.x && s.position.y > p.y && top > s.position.y)
                s.position.x = right - b.radius;
            const float outerLeft = left - b.dividerWidth, outerRight = right + b.dividerWidth, bottom = p.y - b.dividerWidth;
            if (s.position.x > outerLeft - b.radius && outerLeft > s.position.x && s.position.y > bottom && top > s.position.y)
                s.position.x = outerLeft - b.radius;
            if (s.position.x > outerRight + b.radius && outerRight > s.position.x && s.position.y > bottom && top > s.position.y)
                s.position.x = outerRight + b.radius;
            if (s.position.x > left && right > s.position.x && s.position.y > p.y && p.y + b.radius > s.position.y) {
                s.position.y = p.y + b.radius;
                if (s.velocity.y < 0.0f) {
                    s.velocity.y = 0.0f;
                    gravity = false;
                }
            }
            if (s.position.x > outerLeft && outerRight > s.position.x && s.position.y > bottom - b.radius && bottom > s.position.y)
                s.position.y = bottom - b.radius;
        }
    }
    b.current = s;
    const float speedY = std::fabs(s.velocity.y);
    bool floorFriction = speedY < 0.00000011920929f && s.position.y == b.bottom;
    if (!floorFriction && b.option == 8 && speedY < 0.00000011920929f) {
        for (std::int32_t index = 0; index < 6; ++index) {
            const PatiCenterPocket &p = b.centerPockets[index];
            if (p.type && s.position.y == p.y + b.radius && s.position.x > p.x - p.halfWidth && p.halfWidth + p.x > s.position.x) {
                if (index == 5)
                    floorFriction = true;
                else {
                    gravity = false;
                    b.soundFlags |= 8;
                    b.current.velocity.x = (1.0f - b.remainingTime * b.surfaceFriction) * b.current.velocity.x;
                }
            }
        }
    }
    if (floorFriction) {
        b.soundFlags |= 8;
        b.current.velocity.x = (1.0f - b.remainingTime * b.surfaceFriction) * b.current.velocity.x;
    } else if (gravity) {
        b.current.velocity.y = b.remainingTime * b.gravity + b.current.velocity.y;
        if (s.velocity.y > 0.001f && b.current.velocity.y < -0.001f) {
            const float next = b.current.velocity.y;
            b.current.velocity.y = 0.0f;
            b.turningFraction = next / (next - s.velocity.y);
        }
    }
    if (std::fabs(b.current.velocity.x) < 0.000099999997f)
        b.current.velocity.x = 0.0f;
    if (std::fabs(b.current.velocity.y) < 0.000099999997f)
        b.current.velocity.y = 0.0f;
    const float fraction = 1.0f - b.turningFraction;
    b.current.position.y = (b.remainingTime * s.velocity.y) * fraction + b.current.position.y;
    b.current.position.x = (b.remainingTime * s.velocity.x) * fraction + b.current.position.x;
}
// @verified - p
void PlayPatiCollisionSound(PatiBallWork *ball, std::int16_t material) {
    PatiBallWork &b = *ball;
    std::int32_t sound;
    std::int16_t period;
    float loud, quiet = 0.0f;
    if (material == 0) {
        sound = 283;
        loud = 0.0049999999f;
        period = 5;
    } else if (material == 1) {
        sound = 289;
        loud = 0.0099999998f;
        period = 2;
    } else if (material == 2) {
        sound = 293;
        loud = 0.0080000004f;
        quiet = 0.0049999999f;
        period = 5;
    } else {
        sound = 295;
        loud = 0.0f;
        period = 60;
    }
    if (b.previousSound != sound)
        b.soundFrames = 0;
    if (b.soundFrames >= period)
        b.soundFrames = 0;
    if (b.soundFrames < period) {
        if (b.soundFrames <= 0) {
            const Vector3 origin{};
            float context = std::fabs(GetSquaredDistanceXZ(&origin, &b.current.velocity));
            std::int32_t command = sound;
            if (context <= loud) {
                context = GetSquaredDistanceXZ(&origin, &b.current.velocity);
                command = std::fabs(context) <= quiet ? sound + 2 : sound + 1;
            }
            Loop3DSoundEffect(static_cast<std::uint32_t>(command), context);
        }
        b.previousSound = sound;
    }
}
// @verified - p
void FollowPatiBall(PatiBallWork *ball) {
    Vector3 current;
    EV_GetCharaPosition(PATI_BALL_CAMERA, CHARACTER_TRANSLATION_ROOT, &current, EventTransformFlags::Local);
    const float top = ball->top - 0.13f, y = ball->current.position.y;
    Vector3 position{0.0f, 0.25999999f, 0.0f};
    if (y > top)
        position.y = current.y > top || current.y < 0.25999999f ? top : current.y;
    else if (y > 0.25999999f && top > y)
        position.y = y <= current.y ? y : current.y;
    EV_AT_SetPos(PATI_BALL_CAMERA, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(PATI_BALL_CAMERA, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, ball->boardRotation.y, 0}, nullptr);
    EVS_LoopCamera(ball->boardSize == 3 ? 994 : ball->boardSize == 4 ? 997 : 991, PATI_BALL_CAMERA, PATI_BALL_CAMERA);
}
// @verified - p
void AdvancePatiBall(PatiBallWork *ball) {
    PatiBallWork &b = *ball;
    b.segment = b.previous;
    b.remainingTime = 1.0f;
    b.soundFlags = 0;
    b.collision = 0;
    b.collisionIndex = 0;
    b.collisionTime = -1.0f;
    std::int32_t iteration = 0;
    do {
        IntegratePatiBall(ball);
        b.collision = 0;
        b.collisionIndex = 0;
        b.collisionTime = -1.0f;
        FindPatiCollision(ball);
        if (!b.collision)
            break;
        if (!iteration)
            MarkPatiCollisionSound(ball);
        ResolvePatiCollision(ball);
        b.remainingTime = (b.turningFraction * b.collisionTime) * b.remainingTime;
        if (b.remainingTime < 0.00030000001f)
            break;
        ++iteration;
        b.segment = b.current;
    } while (b.collision);
    if (b.soundFlags & 1)
        PlayPatiCollisionSound(ball, 0);
    if (b.soundFlags & 2)
        PlayPatiCollisionSound(ball, 1);
    if (b.soundFlags & 4)
        PlayPatiCollisionSound(ball, 2);
    if (b.soundFlags & 8)
        PlayPatiCollisionSound(ball, 3);
    if (IsPatiBallSettled(ball) && !b.settled)
        b.settled = 1;
    FollowPatiBall(ball);
    camCtrlDisableFilmBars();
    b.soundFrames = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(b.soundFrames) + 1));
    b.previous = b.current;
}
} // namespace shenmue::s2
