#include "s2/sd_light.hpp"

#include "s2/simple_shader.hpp"
#include "s2/legacy_draw.hpp"
#include "d3t/light_tiles.hpp"
#include "d3t/buffer.hpp"
#include <cstring>

namespace shenmue::s2 {
std::array<std::uint32_t, 4> g_sdDirectionalLightIndices{};
std::uint32_t g_sdDirectionalLightCount{};

// @verified - p
SDGpuLight *InitializeSDGpuLight(SDGpuLight *light) {
    std::memset(light->transformData, 0, 192);
    light->colour = {};
    light->position = {};
    light->direction = {};
    return light;
}

void BuildLightTileLists(
    d3t::D3TDeviceContext *context, const SDGpuLight *lights, std::uint32_t count, std::uint64_t, std::int32_t, const Matrix4x4 *, float, float) {
    SDLightTileIndices indices{};
    for (std::uint32_t index = 0; index < count && indices.pointCount + indices.spotCount < 50U; ++index) {
        const SDGpuLight &light = lights[index];
        const float magnitude = (light.colour.x * light.colour.x + light.colour.y * light.colour.y) + light.colour.z * light.colour.z;
        if (!(magnitude >= SD_TILE_LIGHT_THRESHOLD))
            continue;
        if (light.type == SDLT_Point)
            indices.points[indices.pointCount++] = index;
        else if (light.type == SDLT_Spot)
            indices.spots[indices.spotCount++] = index;
    }
    SDLightTileOffset *const offsets =
        static_cast<SDLightTileOffset *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(d3t::g_simpleVertexBufferHandles[2]), 1, 0));
    if (offsets) {
        std::uint32_t cursor = 0;
        for (std::uint32_t tile = 0; tile < 8000; ++tile) {
            offsets[tile] = {.indexOffset = cursor,
                .spotCount = static_cast<std::uint16_t>(indices.spotCount),
                .pointCount = static_cast<std::uint16_t>(indices.pointCount),
                .probeCount = indices.probeCount};
            cursor += indices.pointCount + indices.probeCount + indices.spotCount;
        }
    }
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(d3t::g_simpleVertexBufferHandles[2]));
    std::uint32_t *const destination =
        static_cast<std::uint32_t *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(d3t::g_simpleIndexBufferHandles[2]), 1, 0));
    if (destination) {
        std::uint32_t cursor = 0;
        for (std::uint32_t tile = 0; tile < 8000; ++tile) {
            for (std::uint32_t index = 0; index < indices.pointCount; ++index)
                destination[cursor++] = indices.points[index];
            for (std::uint32_t index = 0; index < indices.spotCount; ++index)
                destination[cursor++] = indices.spots[index];
            for (std::uint32_t index = 0; index < indices.probeCount; ++index)
                destination[cursor++] = indices.probes[index];
        }
    }
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(d3t::g_simpleIndexBufferHandles[2]));
}

void UpdateModelSceneLights(d3t::D3TDeviceContext *context, const Matrix4x4 *viewProjection) {
    std::array<SDGpuLight, 512> lights;
    for (SDGpuLight &light : lights)
        InitializeSDGpuLight(&light);
    std::memset(lights.data(), 0, sizeof(lights));
    std::uint32_t count = 0;
    for (const SDSceneLight &source : g_sdSceneLights) {
        if (count >= 512U)
            break;
        if (!source.enabled)
            continue;
        SDGpuLight &light = lights[count++];
        light.colour = {.x = SD_GPU_LIGHT_SCALE * source.colour.x, .y = SD_GPU_LIGHT_SCALE * source.colour.y, .z = SD_GPU_LIGHT_SCALE * source.colour.z};
        light.type = source.type;
        light.position = source.position;
        light.direction = source.direction;
        light.range = source.type ? source.range : SD_GPU_POINT_RANGE_SCALE * source.range;
        light.cone = source.cone;
    }
    BuildLightTileLists(context, lights.data(), count, 256, 0, viewProjection, 0.15000000596046448f, 3000.0f);
    g_sdDirectionalLightCount = 0;
    for (std::uint32_t index = 0; index < count; ++index)
        if (lights[index].type == SDLT_Directional)
            g_sdDirectionalLightIndices[g_sdDirectionalLightCount++] = index;
    g_sdLightConstants.directionalCount = g_sdDirectionalLightCount;
    g_sdLightConstants.directionalIndices = g_sdDirectionalLightIndices;
    const float multiplier = GetLegacyParticleLightIntensity();
    const Vector4 ambient{.x = lights[0].colour.x * multiplier, .y = lights[0].colour.y * multiplier, .z = lights[0].colour.z * multiplier, .w = 1.0f};
    g_sdLightConstants.ambient[0] = ambient;
    g_sdLightConstants.ambient[1] = ambient;
    g_sdLightConstants.ambient[2] = ambient;
    void *const mapped = d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(g_sdLightBuffer), 1, 0);
    if (mapped)
        std::memcpy(mapped, lights.data(), sizeof(lights));
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(g_sdLightBuffer));
    BindModelSceneLights(context);
}
} // namespace shenmue::s2
