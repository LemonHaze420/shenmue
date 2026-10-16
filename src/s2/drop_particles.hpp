#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_DROP{SHEN_NAME(DROP)};
struct CompactParticleRecord {
    std::uint32_t flags;
    Vector3 position;
    Vector3 velocity;
    std::uint32_t reserved1c;
    void (*callback)(CompactParticleRecord &);
};
static_assert(sizeof(CompactParticleRecord) == 0x28);
static_assert(offsetof(CompactParticleRecord, position) == 0x04);
static_assert(offsetof(CompactParticleRecord, callback) == 0x20);

struct DropEmitterSettings {
    std::int32_t count;
    bool compact;
    float width, height, birthRate;
    Vector3 position, spawnVariance, velocity, velocityVariance;
};
inline constexpr std::array<DropEmitterSettings, 5> dropEmitterSettings{{{.count = 30,
                                                                             .compact = false,
                                                                             .width = 0.01f,
                                                                             .height = 0.2f,
                                                                             .birthRate = 0.015f,
                                                                             .position = {3.8f, 17.700001f, 3.8f},
                                                                             .spawnVariance = {3.0f, 0.0f, 3.0f},
                                                                             .velocity = {0.0f, -0.1f, 0.0f},
                                                                             .velocityVariance = {0.0f, 0.02f, 0.0f}},
    {.count = 30,
        .compact = false,
        .width = 0.01f,
        .height = 0.5f,
        .birthRate = 0.01f,
        .position = {-2.8f, 17.700001f, -1.5599999f},
        .spawnVariance = {3.0f, 0.0f, 3.0f},
        .velocity = {0.0f, -0.1f, 0.0f},
        .velocityVariance = {0.0f, 0.04f, 0.0f}},
    {.count = 30,
        .compact = true,
        .width = 0.01f,
        .height = 0.5f,
        .birthRate = 0.1f,
        .position = {6.5f, 14.0f, 0.4f},
        .spawnVariance = {0.05f, 0.0f, 0.05f},
        .velocity = {0.0f, -0.1f, 0.0f},
        .velocityVariance = {0.0f, 0.05f, 0.0f}},
    {.count = 40,
        .compact = true,
        .width = 0.01f,
        .height = 0.5f,
        .birthRate = 0.2f,
        .position = {8.9299927f, 13.889999f, 10.270004f},
        .spawnVariance = {0.03f, 0.0f, 0.03f},
        .velocity = {},
        .velocityVariance = {0.0f, 0.05f, 0.0f}},
    {.count = 25,
        .compact = true,
        .width = 0.01f,
        .height = 0.5f,
        .birthRate = 0.2f,
        .position = {1.1600037f, 11.75f, 2.3000031f},
        .spawnVariance = {0.01f, 0.0f, 0.1f},
        .velocity = {},
        .velocityVariance = {0.0f, 0.05f, 0.0f}}}};

void InitializeDropParticles();
void ReleaseDropParticles();
void ResetDropParticleAdmission();

} // namespace shenmue::s2
