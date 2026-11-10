#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/buffer.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;

struct CameraConfigPreset {
    char name[128];
    std::uint8_t fpsMode;
    std::byte reserved81[3];
    float fov;
    float nearPlane;
    float farPlane;
    std::byte reserved90[4];
    float position[4];
    float direction[4];
    std::byte reservedB4[32];
    float lookAt[4];
    float eulerAngles[2];
};
static_assert(sizeof(CameraConfigPreset) == 236);
static_assert(offsetof(CameraConfigPreset, position) == 0x94 && offsetof(CameraConfigPreset, lookAt) == 0xd4);
inline constexpr float kCameraConfigDefaultFov = 47.0f;
inline constexpr float kCameraConfigDefaultNearPlane = 0.15000001f;
inline constexpr float kCameraConfigDefaultFarPlane = 3000.0f;
extern std::array<CameraConfigPreset, 16> g_cameraConfigPresets;
extern std::int32_t g_cameraConfigPresetCount;

extern std::uint32_t g_activeRenderHandle;
extern std::int32_t g_bHasSetupShaders;

std::int64_t LoadShaders();
void ReadCameraConfig();
void SetupShaders();
namespace d3t {
std::int64_t SetupForDepthBufferPass(D3TDeviceContext *renderer);
} // namespace d3t
std::int64_t ClearEffectABuffer();
std::int64_t ComposeEffectABuffer();

} // namespace shenmue::s1
