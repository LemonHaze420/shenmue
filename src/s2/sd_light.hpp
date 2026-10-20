#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct SDLightDataHeader {
    std::uint32_t version;
    std::uint32_t count;
    std::uint32_t stride;
    std::uint32_t reserved0c;
    std::uint64_t recordsOffset;
};
struct SDLightMetadataTable {
    std::uint64_t *values;
    std::uint32_t *hashes;
    std::uint16_t *links;
    std::uint32_t capacity;
    std::uint32_t count;
};
struct SDLightOverrides {
    std::uint64_t *values;
    std::uint32_t count;
    std::uint32_t capacity;
};
static_assert(sizeof(SDLightDataHeader) == 0x18);
static_assert(offsetof(SDLightDataHeader, recordsOffset) == 0x10);
static_assert(sizeof(SDLightMetadataTable) == 0x20);
static_assert(offsetof(SDLightMetadataTable, capacity) == 0x18);
static_assert(offsetof(SDLightMetadataTable, count) == 0x1c);
static_assert(sizeof(SDLightOverrides) == 0x10);
extern SDLightDataHeader g_sdLightDataHeader;
extern SDLightMetadataTable g_sdLightMetadata;
extern SDLightOverrides g_sdLightOverrides;
extern std::int32_t g_sdLightSceneId;
extern shen_name g_sdLightArea;
extern std::uint32_t g_sdShadowPassCount;
void InitializeSDLightMetadata();
void ReleaseSDLightMetadata();
void CompleteSDLightDataLoad(void *callbackData, std::uint32_t contentHandle, void *data, std::uint64_t size, const char *path);
std::int64_t LoadSDLightMetadata(std::int32_t sceneId, shen_name area);
void InitializeSDLightFlags();

enum SDLightType : std::uint32_t { SDLT_Point = 0, SDLT_Directional = 1, SDLT_Spot = 2 };

struct SDSceneLight {
    Vector3 colour;
    std::uint32_t type;
    Vector3 position;
    Vector3 direction;
    float range;
    float cone;
    std::uint32_t enabled;
};

struct SDLightConstants {
    std::array<Vector4, 3> ambient;
    Vector2 shadowAtlasSize;
    float environmentStrength;
    float irradianceStrength;
    Vector4 cascadeSplits;
    std::array<Matrix4x4, 32> shadowViewProjection;
    std::array<std::uint32_t, 4> directionalIndices;
    std::uint32_t directionalCount;
    float lightIntensity;
    std::uint32_t noShadows;
    std::uint32_t fillLightOffsetScaler;
    std::uint32_t cascadeCount;
};

static_assert(sizeof(SDSceneLight) == 0x34);
static_assert(offsetof(SDSceneLight, type) == 0x0c);
static_assert(offsetof(SDSceneLight, position) == 0x10);
static_assert(offsetof(SDSceneLight, direction) == 0x1c);
static_assert(offsetof(SDSceneLight, range) == 0x28);
static_assert(offsetof(SDSceneLight, cone) == 0x2c);
static_assert(offsetof(SDSceneLight, enabled) == 0x30);
static_assert(sizeof(SDLightConstants) == 0x874);
static_assert(offsetof(SDLightConstants, shadowAtlasSize) == 0x30);
static_assert(offsetof(SDLightConstants, directionalIndices) == 0x850);
static_assert(offsetof(SDLightConstants, lightIntensity) == 0x864);
static_assert(offsetof(SDLightConstants, noShadows) == 0x868);
static_assert(offsetof(SDLightConstants, fillLightOffsetScaler) == 0x86c);

inline constexpr float SD_AMBIENT_SCALE = 0.8f;
inline constexpr float SD_AMBIENT_OFFSET = 0.0f;
extern std::array<SDSceneLight, 256> g_sdSceneLights;
extern SDLightConstants g_sdLightConstants;
extern Vector2 g_sdShadowAtlasSize;
extern Vector3 g_sdCameraPosition;
extern Vector3 g_sdSunDirection;
extern std::int32_t g_sdNightTime;
extern std::int32_t g_sdFillLightFlag;
void PublishSDLights(const std::array<SDSceneLight, 256> &lights, const Vector3 &camera, std::uint8_t hour);

} // namespace shenmue::s2
