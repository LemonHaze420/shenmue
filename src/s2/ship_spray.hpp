#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#if !SHENMUE_IS_DC
#include <array>
#endif
#if !SHENMUE_IS_DC
#include <bit>
#endif
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

#if SHENMUE_IS_DC
#define TASK_SHIP SHEN_NAME(SHIP)
#else
inline constexpr shen_name TASK_SHIP{SHEN_NAME(SHIP)};
#endif
#if SHENMUE_IS_DC
static const std::uint32_t kShipSprayColourK = 0x3ef0ffffU;
#else
inline constexpr std::uint32_t kShipSprayColourK = 0x3ef0ffffU;
#endif
#if SHENMUE_IS_DC
static const std::uint32_t kShipSprayColourFive = 0x74785b46U;
#else
inline constexpr std::uint32_t kShipSprayColourFive = std::bit_cast<std::uint32_t>(7.8707328e31f);
#endif
#if SHENMUE_IS_DC
static const std::uint32_t kShipSprayColourDefault = 0x3ec6f8f6U;
#else
inline constexpr std::uint32_t kShipSprayColourDefault = std::bit_cast<std::uint32_t>(0.38861817f);
#endif
struct ShipSprayEmitter {
    Vector3 position;
    Vector3 spawnVariance;
    Vector3 velocity;
    Vector3 velocityVariance;
    float birthRate;
    float width;
    float height;
    float widthOffset;
    std::int32_t slot;
    std::int32_t count;
    void *particles;
};
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char ShipSprayLayout868[(sizeof(ShipSprayEmitter) == 0x4c) ? 1 : -1];
#else
typedef char ShipSprayLayout923[(sizeof(ShipSprayEmitter) == 80) ? 1 : -1];
#endif
typedef char ShipSprayLayout977[(offsetof(ShipSprayEmitter, widthOffset) == 60) ? 1 : -1];
typedef char ShipSprayLayout1039[(offsetof(ShipSprayEmitter, slot) == 64) ? 1 : -1];
typedef char ShipSprayLayout1094[(offsetof(ShipSprayEmitter, particles) == 72) ? 1 : -1];

#if SHENMUE_IS_DC
static const ShipSprayEmitter kShipSprayK[6] = {
#else
inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayK{{
#endif
    {SHEN_FIELD(position) {SHEN_FIELD(x) 0.08f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.16f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.1f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.075f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.69f,
        SHEN_FIELD(height) 0.66f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 60,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -0.2f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.16f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.1f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.075f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.69f,
        SHEN_FIELD(height) 0.66f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 60,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 1.16f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -0.72f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.075f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.09f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.6f,
        SHEN_FIELD(height) 0.41f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 45,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -1.36f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -0.72f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.075f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.09f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.6f,
        SHEN_FIELD(height) 0.41f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 45,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 1.92f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.045f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.11f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.44f,
        SHEN_FIELD(height) 0.34f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 30,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -2.2f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.045f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.11f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 0.44f,
        SHEN_FIELD(height) 0.34f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 30,
        SHEN_FIELD(particles) 0},
#if SHENMUE_IS_DC
};
#else
}};
#endif
#if SHENMUE_IS_DC
static const ShipSprayEmitter kShipSprayFive[6] = {
#else
inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayFive{{
#endif
    {SHEN_FIELD(position) {SHEN_FIELD(x) 0.08f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.16f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.1f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.3f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.02f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.02f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) -2.36f,
        SHEN_FIELD(height) 0.225f,
        SHEN_FIELD(widthOffset) 0.36f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 40,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -0.2f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.16f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.1f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.3f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.02f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.02f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) -2.36f,
        SHEN_FIELD(height) 0.225f,
        SHEN_FIELD(widthOffset) 0.36f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 40,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 1.06f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -0.72f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.2f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.2f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.04f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.05f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0,
        SHEN_FIELD(width) 0.2f,
        SHEN_FIELD(height) 0.2f,
        SHEN_FIELD(widthOffset) -0.05f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 30,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -1.26f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -0.72f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.2f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.2f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.04f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.05f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0,
        SHEN_FIELD(width) 0.2f,
        SHEN_FIELD(height) 0.2f,
        SHEN_FIELD(widthOffset) -0.05f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 30,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 1.52f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.1f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.2f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.01f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.6f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0,
        SHEN_FIELD(width) 0.17f,
        SHEN_FIELD(height) 0.17f,
        SHEN_FIELD(widthOffset) -0.05f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 15,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -1.8f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.1f, SHEN_FIELD(y) 0, SHEN_FIELD(z) 0.2f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.01f, SHEN_FIELD(y) 0.015f, SHEN_FIELD(z) -0.6f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.001f, SHEN_FIELD(y) 0.005f, SHEN_FIELD(z) 0.001f},
        SHEN_FIELD(birthRate) 0,
        SHEN_FIELD(width) 0.17f,
        SHEN_FIELD(height) 0.17f,
        SHEN_FIELD(widthOffset) -0.05f,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 15,
        SHEN_FIELD(particles) 0},
#if SHENMUE_IS_DC
};
#else
}};
#endif
#if SHENMUE_IS_DC
static const ShipSprayEmitter kShipSprayDefault[6] = {
#else
inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayDefault{{
#endif
    {SHEN_FIELD(position) {SHEN_FIELD(x) 0.2f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.41f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.2f, SHEN_FIELD(y) 0.06f, SHEN_FIELD(z) -0.15f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.73f,
        SHEN_FIELD(height) 1.65f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 100,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -0.5f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.41f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.2f, SHEN_FIELD(y) 0.06f, SHEN_FIELD(z) -0.15f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.73f,
        SHEN_FIELD(height) 1.65f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 100,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 2.9f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.15f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.18f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.49f,
        SHEN_FIELD(height) 1.02f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 75,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -3.4f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -1.8f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.15f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.18f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.49f,
        SHEN_FIELD(height) 1.02f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 75,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) 4.8f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -4.5f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) 0.09f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.22f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.1f,
        SHEN_FIELD(height) 0.86f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 50,
        SHEN_FIELD(particles) 0},
    {SHEN_FIELD(position) {SHEN_FIELD(x) -5.5f, SHEN_FIELD(y) 0, SHEN_FIELD(z) -4.5f},
        SHEN_FIELD(spawnVariance) {SHEN_FIELD(x) 0.4f, SHEN_FIELD(y) 0.3f, SHEN_FIELD(z) 0.4f},
        SHEN_FIELD(velocity) {SHEN_FIELD(x) -0.09f, SHEN_FIELD(y) 0.03f, SHEN_FIELD(z) -0.22f},
        SHEN_FIELD(velocityVariance) {SHEN_FIELD(x) 0.002f, SHEN_FIELD(y) 0.01f, SHEN_FIELD(z) 0.002f},
        SHEN_FIELD(birthRate) 0.6f,
        SHEN_FIELD(width) 1.1f,
        SHEN_FIELD(height) 0.86f,
        SHEN_FIELD(widthOffset) 0,
        SHEN_FIELD(slot) -1,
        SHEN_FIELD(count) 50,
        SHEN_FIELD(particles) 0},
#if SHENMUE_IS_DC
};
#else
}};
#endif

extern ShipSprayEmitter *g_shipSprayParameters;
extern Vector3 g_shipSprayPosition;
extern Vector3 g_shipSprayLocalPosition;
extern std::int32_t g_shipSprayYaw;
extern float g_shipSprayFadeRate;
extern std::uint32_t g_shipSprayColour;
void ResetShipSprayParameters();
void InitializeShipSpray();
void AssignShipSprayPosition(const Vector3 &position);
std::uint64_t AssignShipSprayLocalPosition(const Vector3 &position);
void AssignShipSprayYaw(std::int32_t yaw);
void ShipSprayControlCallback(void *taskData);

} } // namespace shenmue::s2
