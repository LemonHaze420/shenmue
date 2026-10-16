#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct PackModel;
struct ScenarioContext;

#pragma pack(push, 4)
struct GCPNWork {
    PackModel *modelOwner;
    Vector3 cameraTarget;
    Vector3 actorPosition;
    float horizontalOffset;
    float verticalOffset;
    float directionX;
    float directionZ;
    float fovOffset;
    std::uint32_t reserved34;
    std::uint64_t reserved38;
    std::uint32_t noisePhaseX;
    std::uint32_t noisePhaseY;
    std::uint32_t noisePhaseZ;
    std::uint16_t cameraActive;
    std::int16_t field4E;
};

struct GCPNScriptParameters {
    std::int32_t mode;
    std::int32_t characterOrTableOffset;
    std::int32_t modelIndexOrVectorOffset;
};
#pragma pack(pop)

static_assert(sizeof(GCPNWork) == 0x50);
static_assert(offsetof(GCPNWork, modelOwner) == 0x00);
static_assert(offsetof(GCPNWork, cameraTarget) == 0x08);
static_assert(offsetof(GCPNWork, actorPosition) == 0x14);
static_assert(offsetof(GCPNWork, horizontalOffset) == 0x20);
static_assert(offsetof(GCPNWork, directionX) == 0x28);
static_assert(offsetof(GCPNWork, fovOffset) == 0x30);
static_assert(offsetof(GCPNWork, noisePhaseX) == 0x40);
static_assert(offsetof(GCPNWork, cameraActive) == 0x4c);
static_assert(sizeof(GCPNScriptParameters) == 0x0c);

GCPNWork *GCPN_Find(Character *character);
std::int32_t GCPN_ExecuteScript(ScenarioContext *context, const GCPNScriptParameters &parameters);

} // namespace shenmue::s1
