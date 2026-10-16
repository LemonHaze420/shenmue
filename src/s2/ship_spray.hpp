#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_SHIP{SHEN_NAME(SHIP)};
inline constexpr std::uint32_t kShipSprayColourK = 0x3ef0ffffU;
inline constexpr std::uint32_t kShipSprayColourFive = std::bit_cast<std::uint32_t>(7.8707328e31f);
inline constexpr std::uint32_t kShipSprayColourDefault = std::bit_cast<std::uint32_t>(0.38861817f);
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
static_assert(sizeof(ShipSprayEmitter) == 80);
static_assert(offsetof(ShipSprayEmitter, widthOffset) == 60);
static_assert(offsetof(ShipSprayEmitter, slot) == 64);
static_assert(offsetof(ShipSprayEmitter, particles) == 72);

inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayK{{
    {.position = {.x = 0.08f, .y = 0.01f, .z = 0.16f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.1f, .y = 0.03f, .z = -0.075f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.69f,
        .height = 0.66f,
        .widthOffset = 0,
        .slot = -1,
        .count = 60,
        .particles = nullptr},
    {.position = {.x = -0.2f, .y = 0.01f, .z = 0.16f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.1f, .y = 0.03f, .z = -0.075f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.69f,
        .height = 0.66f,
        .widthOffset = 0,
        .slot = -1,
        .count = 60,
        .particles = nullptr},
    {.position = {.x = 1.16f, .y = 0, .z = -0.72f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.075f, .y = 0.015f, .z = -0.09f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.6f,
        .height = 0.41f,
        .widthOffset = 0,
        .slot = -1,
        .count = 45,
        .particles = nullptr},
    {.position = {.x = -1.36f, .y = 0, .z = -0.72f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.075f, .y = 0.015f, .z = -0.09f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.6f,
        .height = 0.41f,
        .widthOffset = 0,
        .slot = -1,
        .count = 45,
        .particles = nullptr},
    {.position = {.x = 1.92f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.045f, .y = 0.015f, .z = -0.11f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.44f,
        .height = 0.34f,
        .widthOffset = 0,
        .slot = -1,
        .count = 30,
        .particles = nullptr},
    {.position = {.x = -2.2f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.045f, .y = 0.015f, .z = -0.11f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = 0.44f,
        .height = 0.34f,
        .widthOffset = 0,
        .slot = -1,
        .count = 30,
        .particles = nullptr},
}};
inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayFive{{
    {.position = {.x = 0.08f, .y = 0.01f, .z = 0.16f},
        .spawnVariance = {.x = 0.1f, .y = 0, .z = 0.3f},
        .velocity = {.x = 0.02f, .y = 0.03f, .z = -0.02f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = -2.36f,
        .height = 0.225f,
        .widthOffset = 0.36f,
        .slot = -1,
        .count = 40,
        .particles = nullptr},
    {.position = {.x = -0.2f, .y = 0.01f, .z = 0.16f},
        .spawnVariance = {.x = 0.1f, .y = 0, .z = 0.3f},
        .velocity = {.x = -0.02f, .y = 0.03f, .z = -0.02f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0.6f,
        .width = -2.36f,
        .height = 0.225f,
        .widthOffset = 0.36f,
        .slot = -1,
        .count = 40,
        .particles = nullptr},
    {.position = {.x = 1.06f, .y = 0, .z = -0.72f},
        .spawnVariance = {.x = 0.2f, .y = 0, .z = 0.2f},
        .velocity = {.x = 0.04f, .y = 0.015f, .z = -0.05f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0,
        .width = 0.2f,
        .height = 0.2f,
        .widthOffset = -0.05f,
        .slot = -1,
        .count = 30,
        .particles = nullptr},
    {.position = {.x = -1.26f, .y = 0, .z = -0.72f},
        .spawnVariance = {.x = 0.2f, .y = 0, .z = 0.2f},
        .velocity = {.x = -0.04f, .y = 0.015f, .z = -0.05f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0,
        .width = 0.2f,
        .height = 0.2f,
        .widthOffset = -0.05f,
        .slot = -1,
        .count = 30,
        .particles = nullptr},
    {.position = {.x = 1.52f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.1f, .y = 0, .z = 0.2f},
        .velocity = {.x = 0.01f, .y = 0.015f, .z = -0.6f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0,
        .width = 0.17f,
        .height = 0.17f,
        .widthOffset = -0.05f,
        .slot = -1,
        .count = 15,
        .particles = nullptr},
    {.position = {.x = -1.8f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.1f, .y = 0, .z = 0.2f},
        .velocity = {.x = -0.01f, .y = 0.015f, .z = -0.6f},
        .velocityVariance = {.x = 0.001f, .y = 0.005f, .z = 0.001f},
        .birthRate = 0,
        .width = 0.17f,
        .height = 0.17f,
        .widthOffset = -0.05f,
        .slot = -1,
        .count = 15,
        .particles = nullptr},
}};
inline constexpr std::array<ShipSprayEmitter, 6> kShipSprayDefault{{
    {.position = {.x = 0.2f, .y = 0.01f, .z = 0.41f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.2f, .y = 0.06f, .z = -0.15f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.73f,
        .height = 1.65f,
        .widthOffset = 0,
        .slot = -1,
        .count = 100,
        .particles = nullptr},
    {.position = {.x = -0.5f, .y = 0.01f, .z = 0.41f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.2f, .y = 0.06f, .z = -0.15f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.73f,
        .height = 1.65f,
        .widthOffset = 0,
        .slot = -1,
        .count = 100,
        .particles = nullptr},
    {.position = {.x = 2.9f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.15f, .y = 0.03f, .z = -0.18f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.49f,
        .height = 1.02f,
        .widthOffset = 0,
        .slot = -1,
        .count = 75,
        .particles = nullptr},
    {.position = {.x = -3.4f, .y = 0, .z = -1.8f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.15f, .y = 0.03f, .z = -0.18f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.49f,
        .height = 1.02f,
        .widthOffset = 0,
        .slot = -1,
        .count = 75,
        .particles = nullptr},
    {.position = {.x = 4.8f, .y = 0, .z = -4.5f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = 0.09f, .y = 0.03f, .z = -0.22f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.1f,
        .height = 0.86f,
        .widthOffset = 0,
        .slot = -1,
        .count = 50,
        .particles = nullptr},
    {.position = {.x = -5.5f, .y = 0, .z = -4.5f},
        .spawnVariance = {.x = 0.4f, .y = 0.3f, .z = 0.4f},
        .velocity = {.x = -0.09f, .y = 0.03f, .z = -0.22f},
        .velocityVariance = {.x = 0.002f, .y = 0.01f, .z = 0.002f},
        .birthRate = 0.6f,
        .width = 1.1f,
        .height = 0.86f,
        .widthOffset = 0,
        .slot = -1,
        .count = 50,
        .particles = nullptr},
}};

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

} // namespace shenmue::s2
