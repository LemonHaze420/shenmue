#pragma once
#include "s2/pati.hpp"
#include "s2/pati_nails.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct PatiGameWork;
struct PatiCenterPocket {
    float x, y, halfWidth, height;
    std::int32_t type;
    std::uint32_t reserved14, reserved18;
};
struct PatiBallState {
    Vector3 position, velocity;
};
struct PatiNailContact {
    std::uint32_t age;
    std::int32_t nail;
    std::uint32_t count;
};
struct PatiBallWork {
    PatiGameWork *game;
    HLib::Task *task;
    HLib::Task *nailTask;
    PatiTableData tables;
    float scale;
    std::uint16_t phase, reserved85b6;
    float tangentialRetention, wallRestitution, nailRestitution, surfaceFriction, gravity;
    std::uint32_t reserved85cc;
    const char *ballModel;
    float radiusScale;
    std::int32_t releaseX, releaseJitter;
    float radius;
    const char *labelModel;
    Vector3 boardPosition;
    Vector3i boardRotation;
    float top, bottom, left, right;
    std::byte reserved8618[16];
    const char *nailFile;
    PatiNailData *nails;
    std::int16_t pocketCount;
    std::uint16_t reserved863a;
    Vector3 dividers[10];
    float dividerWidth, dividerHeight;
    std::byte reserved86bc[84];
    char *prizes;
    PatiCenterPocket centerPockets[6];
    const char *centerModes;
    const char *centerPrizes;
    std::uint16_t gameType, option, ballLimit, winLimit, missLimit, winMask, deathMask, reserved87de;
    std::int32_t settled;
    float cameraHeight, remainingTime, turningFraction;
    std::uint16_t collision, reserved87f2, collisionIndex, reserved87f6;
    float collisionTime;
    PatiBallState previous, segment, current, beforeCollision;
    std::uint32_t frames;
    std::uint16_t repeatFrames, reserved8862;
    std::int32_t historyCursor;
    Vector2 history[90];
    float movement[90];
    PatiNailContact contacts[8];
    std::uint16_t boardSize, reserved8d02;
    std::uint32_t soundFlags;
    std::int32_t previousSound;
    std::int8_t soundFrames;
    std::byte reserved8d0d[3];
};
static_assert(sizeof(PatiCenterPocket) == 28 && sizeof(PatiBallState) == 24 && sizeof(PatiNailContact) == 12);
static_assert(sizeof(PatiBallWork) == 0x8d10 && offsetof(PatiBallWork, tables) == 0x18);
static_assert(offsetof(PatiBallWork, scale) == 0x85b0 && offsetof(PatiBallWork, nails) == 0x8630);
static_assert(offsetof(PatiBallWork, centerPockets) == 0x8718 && offsetof(PatiBallWork, collision) == 0x87f0);
static_assert(offsetof(PatiBallWork, previous) == 0x87fc && offsetof(PatiBallWork, current) == 0x882c);
static_assert(offsetof(PatiBallWork, contacts) == 0x8ca0 && offsetof(PatiBallWork, soundFrames) == 0x8d0c);
void ApplyPatiWeather(PatiBallWork *ball);
void AdvancePatiBall(PatiBallWork *ball);
void IntegratePatiBall(PatiBallWork *ball);
void FindPatiCollision(PatiBallWork *ball);
float ProbePatiPoint(PatiBallWork *ball, const Vector3 *point);
void FindPatiDividerCollision(PatiBallWork *ball);
void FindPatiCenterCollision(PatiBallWork *ball);
void FindPatiNailCollision(PatiBallWork *ball);
std::uint8_t IntersectPatiSegments(const Vector3 *current, const Vector3 *previous, const Vector3 *first, const Vector3 *second);
void MarkPatiCollisionSound(PatiBallWork *ball);
void ResolvePatiCollision(PatiBallWork *ball);
void BouncePatiFloor(PatiBallWork *ball);
void BouncePatiDivider(PatiBallWork *ball);
void BouncePatiPocketFloor(PatiBallWork *ball);
void BouncePatiNail(PatiBallWork *ball);
void BouncePatiPoint(PatiBallWork *ball, const Vector3 *point, float restitution);
bool IsPatiBallSettled(PatiBallWork *ball);
void PlayPatiCollisionSound(PatiBallWork *ball, std::int16_t material);
void FollowPatiBall(PatiBallWork *ball);
} // namespace shenmue::s2
