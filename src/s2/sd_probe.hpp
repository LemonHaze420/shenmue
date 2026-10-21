#pragma once

#include "common/math.hpp"
#include "d3t/model.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {
struct D3TDeviceContext;
}

namespace shenmue::s2 {

#pragma pack(push, 4)
struct SDProbeRecord {
    const void *owner;
    std::array<Matrix4x4, 6> views;
    std::array<Matrix4x4, 6> projections;
    std::array<std::array<Vector4, 6>, 6> frustums;
    std::uint64_t parameter548;
    std::uint32_t parameter550;
    std::uint64_t parameter554;
    std::uint32_t parameter55c;
    std::uint64_t root;
    std::uint64_t node;
    std::uint32_t parameter570;
    float parameter574[2];
    float parameter57c;
    float parameter580;
    float parameter584;
    std::uint32_t reserved588;
    std::uint8_t parameter58c;
    std::byte reserved58d[3];
};
#pragma pack(pop)
static_assert(sizeof(SDProbeRecord) == 1424);
static_assert(offsetof(SDProbeRecord, views) == 8);
static_assert(offsetof(SDProbeRecord, projections) == 392);
static_assert(offsetof(SDProbeRecord, frustums) == 776);
static_assert(offsetof(SDProbeRecord, parameter548) == 1352);
static_assert(offsetof(SDProbeRecord, parameter554) == 1364);
static_assert(offsetof(SDProbeRecord, root) == 1376);
static_assert(offsetof(SDProbeRecord, node) == 1384);
static_assert(offsetof(SDProbeRecord, parameter580) == 1408);
static_assert(offsetof(SDProbeRecord, parameter58c) == 1420);
inline constexpr std::array<Vector3, 6> SD_PROBE_UP{{{0, 1, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, 1, 0}}};
inline constexpr Vector4 SD_PROBE_PROJECTION_DEPTH{0, 0, -1.0000033378601074f, -0.010000033304095268f};
inline constexpr Vector4 SD_PROBE_FRUSTUM_DEPTH{0, 0, -1.0006670951843262f, -0.010006670840084553f};
inline constexpr Vector4 SD_PROBE_PROJECTION_W{0, 0, -1, 0};
inline constexpr float SD_PROBE_HALF_FOV = 0.7853975892066956f;
extern std::array<SDProbeRecord, 32> g_sdProbes;
extern std::int32_t g_sdProbeCount;
struct SDProbeConstants {
    std::int32_t disableLocalProbes, probeMapPass, globalProbeIndex;
};
static_assert(sizeof(SDProbeConstants) == 12);
extern std::uint32_t g_sdProbeBuffer;
extern std::int32_t g_sdGlobalProbeIndex;
extern std::array<std::uint32_t, 2> g_sdProbeTextures;
extern std::uint32_t g_sdGlobalProbeTexture;
void UpdateModelSceneProbes(d3t::D3TDeviceContext *context);
void UpdateSDProbeFrustums(std::uint32_t index);
void ConfigureSDProbe(std::uint32_t index, std::uint64_t node, const D3TModelProbe *parameters, const void *owner, std::uint64_t root);
void ConfigureOwnerSDProbes(std::int32_t count, const std::uint64_t *nodes, const D3TModelProbe *parameters, const void *owner, std::uint64_t root);
void RemoveOwnerSDProbes(const void *owner);

} // namespace shenmue::s2
