#include "s2/fog.hpp"

#include "s2/legacy_draw.hpp"

#include <cmath>

namespace shenmue::s2 {
namespace {

struct FogState {
    float nearDistance = 20.0f;
    float farDistance = 800.0f;
    float intensity = 1.0f;
    float publishedNearDistance = 20.0f;
    float publishedFarDistance = 800.0f;
    float publishedIntensity = 1.0f;
    std::uint32_t colour = 0xffffffffU;
    bool enabled = true;
    bool dirty = false;
};

FogState fog;

std::uint64_t SetFogScalar(float &current, float published, float value) {
    current = value;

    if (std::abs(published - value) > std::abs(value) * 0.002) {
        fog.dirty = true;
    }
    return static_cast<std::uint32_t>(fog.dirty);
}

void ZeroFogParams() {
    if (!fog.dirty)
        return;
    fog.dirty = false;
    SetLegacyFogEnabled(0);
}

void PublishFogParameters() {
    if (!fog.dirty)
        return;
    fog.publishedNearDistance = fog.nearDistance;
    fog.publishedFarDistance = fog.farDistance;
    fog.publishedIntensity = fog.intensity;
    fog.dirty = false;




    SetLegacyFogColour(fog.colour);
    SetLegacyFogEnabled(1);
    SetLegacyDepthFadeRange(fog.nearDistance, fog.farDistance);
    SetLegacyDepthFadeBlue(fog.intensity);
}

} // namespace

void SetupDefaultFogParams() {
    fog = {};
    fog.dirty = true;



    ZeroFogParams();
    PublishFogParameters();
}

// @verified - p
std::uint64_t SetFogRed(float value) {
    return SetFogScalar(fog.nearDistance, fog.publishedNearDistance, value);
}

// @verified - p
std::uint64_t SetFogGreen(float value) {
    return SetFogScalar(fog.farDistance, fog.publishedFarDistance, value);
}

// @verified - p
std::uint64_t SetFogBlue(float value) {
    return SetFogScalar(fog.intensity, fog.publishedIntensity, value);
}

// @verified - p
std::uint64_t SetFogDistance(std::uint32_t colour) {
    if (fog.colour != colour)
        fog.dirty = true;
    fog.colour = colour;
    return static_cast<std::uint32_t>(fog.dirty);
}

// @verified - p
float GetFogRed() {
    return fog.nearDistance;
}

// @verified - p
float GetFogGreen() {
    return fog.farDistance;
}

// @verified - p
float GetFogBlue() {
    return fog.intensity;
}

// @verified - p
std::uint64_t GetFogColour() {
    return fog.colour;
}

// @verified - p
std::uint64_t GetFogEnabled() {
    return static_cast<std::uint32_t>(fog.enabled);
}

void ReinitFogTasks(std::int32_t enabled) {
    const bool nextEnabled = enabled == 1;
    if (fog.enabled != nextEnabled)
        fog.dirty = true;
    fog.enabled = nextEnabled;
    if (nextEnabled) {
        PublishFogParameters();
    } else {
        ZeroFogParams();
    }
}

} // namespace shenmue::s2
