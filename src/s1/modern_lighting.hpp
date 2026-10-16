#pragma once

#include "s1/modern_scene.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Mt5RuntimeNode;

struct ModernSceneLight {
    Vector3 colour;
    std::uint32_t type;
    Vector3 position;
    Vector3 direction;
    float range;
    float cone;
    std::uint32_t enabled;
};

struct ModernGpuLight {
    std::array<std::byte, 192> transformData;
    Vector3 colour;
    std::uint32_t type;
    Vector3 position;
    float range;
    Vector3 direction;
    float cone;
    std::array<std::uint32_t, 4> shadow;
};

struct ModernLightConstants {
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

struct ModernTileOffset {
    std::uint32_t indexOffset;
    std::uint16_t spotCount;
    std::uint16_t pointCount;
    std::uint32_t probeCount;
};

struct SceneLightDataHeader {
    std::uint32_t version;
    std::uint32_t count;
    std::uint32_t stride;
    std::uint32_t reserved;
    std::uint64_t recordsOffset;
};

struct SceneLightMetadataTable {
    std::uint64_t *values;
    std::uint32_t *hashes;
    std::uint16_t *links;
    std::uint32_t capacity;
    std::uint32_t count;
};

static_assert(sizeof(ModernSceneLight) == 52);
static_assert(sizeof(ModernGpuLight) == 256);
static_assert(offsetof(ModernGpuLight, colour) == 192);
static_assert(offsetof(ModernGpuLight, direction) == 224);
static_assert(sizeof(ModernLightConstants) == 0x874);
static_assert(offsetof(ModernLightConstants, directionalIndices) == 2128);
static_assert(sizeof(ModernTileOffset) == 12);
static_assert(sizeof(SceneLightDataHeader) == 24);
static_assert(sizeof(SceneLightMetadataTable) == 32);

extern std::array<ModernSceneLight, 128> g_modernSceneLights;
extern std::array<ModernGpuLight, 16> g_modernUploadedLights;
extern ModernLightConstants g_modernLightConstants;
extern std::uint32_t g_modernUploadedLightCount;
extern Vector3 g_modernAmbientColour;
extern SceneLightDataHeader g_sceneLightDataHeader;
extern SceneLightMetadataTable g_sceneLightMetadata;
extern std::uint32_t g_modernSceneId;
extern shen_name g_modernSceneName;
extern std::int32_t g_modernDebugRenderMode;
extern std::int32_t g_modernAlternateNodes;

std::int64_t SetNewSceneDetails(std::uint32_t sceneId, shen_name area);
void InitializeModernSceneLightMetadata();
void ReleaseModernSceneLightMetadata();
void ExtractAndNormalizeSixFrustumPlanes(std::array<Vector4, 6> &planes, const Matrix4x4 &matrix);
bool IsAabbInsideSelectedFrustumPlanes(const std::array<Vector4, 6> &planes, const Vector3 &minimum, const Vector3 &maximum, std::uint8_t skipPlanePairs);
void BindSceneFallbackTextureAndHighSamplers(D3TDeviceContext *context);
void PrepareTiledSceneLightingAndShadowBuffers(D3TDeviceContext *context, const Matrix4x4 &view);
void ConfigureAndSubmitModernScenePass(D3TDeviceContext *context, ModernScenePass &pass, RenderLayerGeometryContext &geometry);
void ArmSingleLightOverrideForCachedMapModel(Mt5RuntimeNode *model);
void CompleteSceneLightDataLoad(void *callbackData, std::uint32_t taskHandle, void *data, std::uint64_t size, const char *path);

} // namespace shenmue::s1
