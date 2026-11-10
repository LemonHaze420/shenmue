#include "s2/sd_light.hpp"

#include "s2/simple_shader.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/math.hpp"
#include "s2/renderer_scratch.hpp"
#include "s2/sd_probe.hpp"
#include "d3t/light_tiles.hpp"
#include "d3t/buffer.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
std::array<std::uint32_t, 4> g_sdDirectionalLightIndices{};
std::uint32_t g_sdDirectionalLightCount{};
std::uint8_t g_sdBroadcastLightTiles = 1;

namespace {
Vector3 UnprojectClipPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    const Vector4 &x = matrix.rows[0], &y = matrix.rows[1], &z = matrix.rows[2], &w = matrix.rows[3];
    const float reciprocal = 1.0f / (((point.x * w.x + point.y * w.y) + point.z * w.z) + w.w);
    return {.x = reciprocal * (((point.x * x.x + point.y * x.y) + point.z * x.z) + x.w),
        .y = reciprocal * (((point.x * y.x + point.y * y.y) + point.z * y.z) + y.w),
        .z = reciprocal * (((point.x * z.x + point.y * z.y) + point.z * z.z) + z.w)};
}

Vector4 ProjectClipPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    const Vector4 &x = matrix.rows[0], &y = matrix.rows[1], &z = matrix.rows[2], &w = matrix.rows[3];
    Vector4 clip{.x = ((point.x * x.x + point.y * x.y) + point.z * x.z) + x.w,
        .y = ((point.x * y.x + point.y * y.y) + point.z * y.z) + y.w,
        .z = ((point.x * z.x + point.y * z.y) + point.z * z.z) + z.w,
        .w = ((point.x * w.x + point.y * w.y) + point.z * w.z) + w.w};
    if (clip.w != 0.0f)
        clip = {.x = clip.x / clip.w, .y = clip.y / clip.w, .z = clip.z / clip.w, .w = clip.w / clip.w};
    return clip;
}

std::int32_t ClampClusterCell(float scaled) {
    const std::int32_t cell = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(scaled)));
    return cell <= 19 ? (cell <= 0 ? 0 : cell) : 19;
}

Vector4 NegatePlane(const Vector4 &plane) {
    return {.x = -plane.x, .y = -plane.y, .z = -plane.z, .w = -plane.w};
}

Vector3 ProjectPointOntoPlane(const Vector4 &plane, const Vector3 &point) {
    const float distance = ((plane.x * point.x + plane.y * point.y) + plane.z * point.z) -
        ((plane.x * plane.w * plane.x + plane.y * plane.w * plane.y) + plane.z * plane.w * plane.z);
    return {.x = point.x - distance * plane.x, .y = point.y - distance * plane.y, .z = point.z - distance * plane.z};
}

float SignedPlaneDistance(const Vector4 &plane, const Vector3 &point) {
    const float length = static_cast<float>(std::sqrt(static_cast<double>((plane.x * plane.x + plane.y * plane.y) + plane.z * plane.z)));
    return (((plane.x * point.x + plane.y * point.y) + plane.z * point.z) + plane.w) / length;
}
} // namespace

// @verified - p
SDGpuLight *InitializeSDGpuLight(SDGpuLight *light) {
    std::memset(light->transformData, 0, 192);
    light->colour = {};
    light->position = {};
    light->direction = {};
    return light;
}

Vector4 ComputeUnprojectedPlane(Vector3 first, Vector3 second, Vector3 third, const Matrix4x4 &inverseViewProjection) {
    first = UnprojectClipPoint(inverseViewProjection, first);
    second = UnprojectClipPoint(inverseViewProjection, second);
    third = UnprojectClipPoint(inverseViewProjection, third);
    Vector3 normal = Vector3::Cross(Vector3::Subtract(third, second), Vector3::Subtract(first, third));
    NormalizeVector3(&normal);
    Vector4 plane{.x = normal.x, .y = normal.y, .z = normal.z, .w = -Vector3::Dot(second, normal)};
    NormalizeVector4(&plane);
    return plane;
}

void ProjectSphereToClusterGrid20(
    const Vector3 &centre, float radius, const Matrix4x4 &worldToClip, float nearZ, float farZ, Vector3 &minimum, Vector3 &maximum, Vector3 &centreCell) {
    const float x = centre.x, y = centre.y, z = centre.z;
    const std::array<Vector3, 8> corners{{{.x = x - radius, .y = y - radius, .z = z - radius}, {.x = x + radius, .y = y - radius, .z = z - radius},
        {.x = x + radius, .y = y + radius, .z = z - radius}, {.x = x - radius, .y = y + radius, .z = z - radius},
        {.x = x - radius, .y = y - radius, .z = z + radius}, {.x = x + radius, .y = y - radius, .z = z + radius},
        {.x = x + radius, .y = y + radius, .z = z + radius}, {.x = x - radius, .y = y + radius, .z = z + radius}}};
    const Vector4 first = ProjectClipPoint(worldToClip, corners[7]);
    float minX = first.x, minY = first.y, minZ = first.z, maxX = first.x, maxY = first.y, maxZ = first.z;
    for (std::uint32_t corner = 0; corner < 7; ++corner) {
        const Vector4 clip = ProjectClipPoint(worldToClip, corners[corner]);
        minX = clip.x < minX ? clip.x : minX;
        minY = clip.y < minY ? clip.y : minY;
        minZ = clip.z < minZ ? clip.z : minZ;
        maxY = clip.y > maxY ? clip.y : maxY;
        maxX = clip.x > maxX ? clip.x : maxX;
        maxZ = clip.z > maxZ ? clip.z : maxZ;
    }
    const float depthRange = farZ - nearZ;
    const float depthSum = nearZ + farZ;
    const double twiceNear = static_cast<double>(nearZ) + static_cast<double>(nearZ);
    const float nearDepth = static_cast<float>(twiceNear / static_cast<double>(depthSum - minZ * depthRange));
    const float farDepth = static_cast<float>(twiceNear / static_cast<double>(depthSum - maxZ * depthRange));
    minimum = {.x = static_cast<float>(ClampClusterCell(((minX * 0.5f) + 0.5f) * 20.0f)),
        .y = static_cast<float>(ClampClusterCell((1.0f - ((maxY * 0.5f) + 0.5f)) * 20.0f)),
        .z = static_cast<float>(ClampClusterCell(nearDepth * 20.0f))};
    maximum = {.x = static_cast<float>(ClampClusterCell(((maxX * 0.5f) + 0.5f) * 20.0f)),
        .y = static_cast<float>(ClampClusterCell((1.0f - ((minY * 0.5f) + 0.5f)) * 20.0f)),
        .z = static_cast<float>(ClampClusterCell(farDepth * 20.0f))};
    if (farDepth < 0.0f || nearDepth < 0.0f) {
        minimum.x = 0.0f;
        minimum.y = 0.0f;
        maximum.x = 19.0f;
        maximum.y = 19.0f;
    }
    const Vector4 projected = ProjectClipPoint(worldToClip, centre);
    const float centreDepth = static_cast<float>(twiceNear / static_cast<double>(depthSum - depthRange * projected.z));
    centreCell = {.x = static_cast<float>(ClampClusterCell(((projected.x * 0.5f) + 0.5f) * 20.0f)),
        .y = static_cast<float>(ClampClusterCell((1.0f - ((projected.y * 0.5f) + 0.5f)) * 20.0f)),
        .z = static_cast<float>(ClampClusterCell(centreDepth * 20.0f))};
}

bool DoesSpotConeCrossPlane(const Vector4 &plane, const SDGpuLight &light, const Vector3 &apex, bool acceptBehind) {
    const Vector3 direction = light.direction;
    const float angle = std::acos(light.cone);
    const Vector3 normal{.x = plane.x, .y = plane.y, .z = plane.z};
    const float lengthSquared = (plane.x * plane.x + plane.y * plane.y) + plane.z * plane.z;
    const float apexDistance = (Vector3::Dot(normal, apex) + plane.w) / lengthSquared;
    const Vector3 side = apexDistance > 0.0f ? Vector3::Cross(normal, direction) : Vector3::Cross(direction, normal);
    Vector3 rim = Vector3::Cross(side, direction);
    if ((rim.x * rim.x + rim.y * rim.y) + rim.z * rim.z > SD_SPOT_RIM_EPSILON)
        NormalizeVector3(&rim);
    const float spread = std::tan(angle) * light.range;
    const Vector3 edge{.x = (direction.x * light.range + apex.x) + rim.x * spread,
        .y = (direction.y * light.range + apex.y) + rim.y * spread,
        .z = (direction.z * light.range + apex.z) + rim.z * spread};
    const float edgeDistance = (Vector3::Dot(normal, edge) + plane.w) / lengthSquared;
    return (edgeDistance >= 0.0f) == (apexDistance < 0.0f) || (acceptBehind && (edgeDistance < 0.0f || apexDistance < 0.0f));
}

void BuildLightTileLists(d3t::D3TDeviceContext *context, const SDGpuLight *lights, std::uint32_t count, std::uint64_t, std::int32_t,
    const Matrix4x4 *viewProjection, float nearZ, float farZ) {
    if (!g_sdBroadcastLightTiles) {
        Matrix4x4 inverse{};
        Matrix4x4::invertMatrix(viewProjection, &inverse);
        std::array<Vector4, 21> columnPlanes{};
        for (std::int32_t column = 0; column < 20; ++column) {
            const float x = static_cast<float>(column) * 0.1f - 1.0f;
            columnPlanes[column] =
                ComputeUnprojectedPlane({.x = x, .y = 1.0f, .z = 1.0f}, {.x = x, .y = -1.0f, .z = 0.0f}, {.x = x, .y = -1.0f, .z = 1.0f}, inverse);
        }
        columnPlanes[20] = NegatePlane(
            ComputeUnprojectedPlane({.x = 1.0f, .y = -1.0f, .z = 1.0f}, {.x = 1.0f, .y = 1.0f, .z = 0.0f}, {.x = 1.0f, .y = 1.0f, .z = 1.0f}, inverse));
        std::array<Vector4, 21> rowPlanes{};
        for (std::int32_t row = 0; row < 20; ++row) {
            const float y = 1.0f - static_cast<float>(row) * 0.1f;
            rowPlanes[row] = ComputeUnprojectedPlane({.x = 1.0f, .y = y, .z = 1.0f}, {.x = -1.0f, .y = y, .z = 0.0f}, {.x = -1.0f, .y = y, .z = 1.0f}, inverse);
        }
        rowPlanes[20] = NegatePlane(
            ComputeUnprojectedPlane({.x = -1.0f, .y = -1.0f, .z = 1.0f}, {.x = 1.0f, .y = -1.0f, .z = 0.0f}, {.x = 1.0f, .y = -1.0f, .z = 1.0f}, inverse));
        std::array<Vector4, 21> slicePlanes{};
        for (std::int32_t slice = 0; slice < 20; ++slice) {
            float z = 0.0f;
            if (slice) {
                const float fraction = static_cast<float>(slice) * 0.050000001f;
                z = ((fraction * farZ + fraction * nearZ) - (nearZ + nearZ)) / (fraction * farZ - fraction * nearZ);
            }
            slicePlanes[slice] =
                ComputeUnprojectedPlane({.x = -1.0f, .y = 1.0f, .z = z}, {.x = 1.0f, .y = -1.0f, .z = z}, {.x = -1.0f, .y = -1.0f, .z = z}, inverse);
        }
        slicePlanes[20] = NegatePlane(
            ComputeUnprojectedPlane({.x = 1.0f, .y = 1.0f, .z = 1.0f}, {.x = -1.0f, .y = 1.0f, .z = 1.0f}, {.x = -1.0f, .y = -1.0f, .z = 1.0f}, inverse));
        std::array<Vector4, 6> frustum{};
        ExtractAndNormalizeSixFrustumPlanes(frustum, *viewProjection);
        SDLightTileIndices *const tiles = static_cast<SDLightTileIndices *>(d3t::g_simpleCpuBuffers[2]);
        for (std::uint32_t tile = 0; tile < 8000; ++tile) {
            tiles[tile].pointCount = 0;
            tiles[tile].spotCount = 0;
            tiles[tile].probeCount = 0;
        }
        std::uint8_t *const visible = static_cast<std::uint8_t *>(GetRendererScratchBlock(count));
        std::memset(visible, 0, count);
        for (std::uint32_t index = 0; index < count; ++index) {
            const SDGpuLight &light = lights[index];
            if ((light.type & ~static_cast<std::uint32_t>(SDLT_Spot)) != 0)
                continue;
            if (SD_TILE_LIGHT_THRESHOLD > (light.colour.x * light.colour.x + light.colour.y * light.colour.y) + light.colour.z * light.colour.z)
                continue;
            const Vector3 position = light.position;
            const float dx = g_sdCameraPosition.x - position.x, dy = g_sdCameraPosition.y - position.y, dz = g_sdCameraPosition.z - position.z;
            if (std::sqrt((dx * dx + dy * dy) + dz * dz) > SD_CLUSTER_LIGHT_DISTANCE)
                continue;
            const float radius = light.range;
            const std::array<Vector3, 8> corners{{{.x = position.x - radius, .y = position.y - radius, .z = position.z - radius},
                {.x = position.x + radius, .y = position.y - radius, .z = position.z - radius},
                {.x = position.x + radius, .y = position.y + radius, .z = position.z - radius},
                {.x = position.x - radius, .y = position.y + radius, .z = position.z - radius},
                {.x = position.x - radius, .y = position.y - radius, .z = position.z + radius},
                {.x = position.x + radius, .y = position.y - radius, .z = position.z + radius},
                {.x = position.x + radius, .y = position.y + radius, .z = position.z + radius},
                {.x = position.x - radius, .y = position.y + radius, .z = position.z + radius}}};
            std::uint32_t plane = 0;
            for (; plane < 6; ++plane) {
                const Vector4 &bound = frustum[plane];
                std::uint32_t corner = 0;
                while (corner < 8 && !(((bound.x * corners[corner].x + bound.y * corners[corner].y) + bound.z * corners[corner].z) + bound.w >= 0.0f))
                    ++corner;
                if (corner >= 8)
                    break;
            }
            if (plane < 6)
                continue;
            Vector3 minimum{}, maximum{}, centre{};
            ProjectSphereToClusterGrid20(position, radius, *viewProjection, nearZ, farZ, minimum, maximum, centre);
            bool added = false;
            if (light.type == SDLT_Spot) {
                std::int32_t column = static_cast<std::int32_t>(minimum.x);
                const std::int32_t lastColumn = static_cast<std::int32_t>(maximum.x);
                do
                    ++column;
                while (column < lastColumn && !DoesSpotConeCrossPlane(columnPlanes[column], light, position, true));
                std::int32_t highColumn = lastColumn + 1;
                do
                    --highColumn;
                while (highColumn > column && !DoesSpotConeCrossPlane(NegatePlane(columnPlanes[highColumn]), light, position, false));
                std::int32_t row = static_cast<std::int32_t>(minimum.y);
                const std::int32_t lastRow = static_cast<std::int32_t>(maximum.y);
                do
                    ++row;
                while (row < lastRow && !DoesSpotConeCrossPlane(rowPlanes[row], light, position, true));
                std::int32_t highRow = lastRow + 1;
                do
                    --highRow;
                while (highRow > row && !DoesSpotConeCrossPlane(NegatePlane(rowPlanes[highRow]), light, position, false));
                std::int32_t slice = static_cast<std::int32_t>(minimum.z);
                const std::int32_t lastSlice = static_cast<std::int32_t>(maximum.z);
                do
                    ++slice;
                while (slice < lastSlice && !DoesSpotConeCrossPlane(slicePlanes[slice], light, position, true));
                std::int32_t highSlice = lastSlice + 1;
                do
                    --highSlice;
                while (highSlice > slice && !DoesSpotConeCrossPlane(NegatePlane(slicePlanes[highSlice]), light, position, false));
                for (std::int32_t z = slice - 1; z <= highSlice; ++z)
                    for (std::int32_t y = row - 1; y <= highRow; ++y)
                        for (std::int32_t x = column - 1; x <= highColumn; ++x) {
                            SDLightTileIndices &tile = tiles[x + 20 * (20 * z + y)];
                            if (tile.spotCount + tile.pointCount < 50U) {
                                tile.spots[tile.spotCount++] = index;
                                added = true;
                            }
                        }
            } else if (light.type == SDLT_Point) {
                const float rangeSquared = light.range * light.range;
                const std::int32_t lastColumn = static_cast<std::int32_t>(maximum.x);
                for (std::int32_t slice = static_cast<std::int32_t>(minimum.z); slice <= static_cast<std::int32_t>(maximum.z); ++slice) {
                    Vector3 slicePoint = position;
                    if (centre.z != static_cast<float>(slice))
                        slicePoint = ProjectPointOntoPlane(slicePlanes[slice], position);
                    for (std::int32_t row = static_cast<std::int32_t>(minimum.y); row <= static_cast<std::int32_t>(maximum.y); ++row) {
                        Vector3 rowPoint = slicePoint;
                        if (centre.y != static_cast<float>(row))
                            rowPoint = ProjectPointOntoPlane(rowPlanes[row], slicePoint);
                        float distance = 0.0f;
                        std::int32_t firstColumn = 0, nextColumn = static_cast<std::int32_t>(minimum.x);
                        do {
                            firstColumn = nextColumn++;
                            if (nextColumn >= lastColumn)
                                break;
                            distance = SignedPlaneDistance(columnPlanes[nextColumn], rowPoint);
                        } while (distance >= rangeSquared);
                        std::int32_t highColumn = lastColumn + 1;
                        do {
                            if (--highColumn < nextColumn)
                                break;
                            distance = SignedPlaneDistance(columnPlanes[highColumn], rowPoint);
                        } while (-distance >= rangeSquared);
                        for (std::int32_t x = firstColumn; x <= highColumn; ++x) {
                            SDLightTileIndices &tile = tiles[x + 20 * (20 * slice + row)];
                            if (tile.pointCount + tile.spotCount < 50U) {
                                tile.points[tile.pointCount++] = index;
                                added = true;
                            }
                        }
                    }
                }
            }
            if (added)
                visible[index] = 1;
        }
        RestoreRendererScratchBlock(visible);
        SDLightTileOffset *const offsets =
            static_cast<SDLightTileOffset *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(d3t::g_simpleVertexBufferHandles[2]), 1, 0));
        if (offsets) {
            std::uint32_t cursor = 0;
            for (std::uint32_t tile = 0; tile < 8000; ++tile) {
                offsets[tile] = {.indexOffset = cursor,
                    .spotCount = static_cast<std::uint16_t>(tiles[tile].spotCount),
                    .pointCount = static_cast<std::uint16_t>(tiles[tile].pointCount),
                    .probeCount = tiles[tile].probeCount};
                cursor += tiles[tile].pointCount + tiles[tile].probeCount + tiles[tile].spotCount;
            }
        }
        d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(d3t::g_simpleVertexBufferHandles[2]));
        std::uint32_t *const destination =
            static_cast<std::uint32_t *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(d3t::g_simpleIndexBufferHandles[2]), 1, 0));
        if (destination) {
            std::uint32_t cursor = 0;
            for (std::uint32_t tile = 0; tile < 8000; ++tile) {
                for (std::uint32_t index = 0; index < tiles[tile].pointCount; ++index)
                    destination[cursor++] = tiles[tile].points[index];
                for (std::uint32_t index = 0; index < tiles[tile].spotCount; ++index)
                    destination[cursor++] = tiles[tile].spots[index];
                for (std::uint32_t index = 0; index < tiles[tile].probeCount; ++index)
                    destination[cursor++] = tiles[tile].probes[index];
            }
        }
        d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(d3t::g_simpleIndexBufferHandles[2]));
        return;
    }
    SDLightTileIndices indices{};
    for (std::uint32_t index = 0; index < count && indices.pointCount + indices.spotCount < 50U; ++index) {
        const SDGpuLight &light = lights[index];
        const float magnitude = (light.colour.x * light.colour.x + light.colour.y * light.colour.y) + light.colour.z * light.colour.z;
        if (SD_TILE_LIGHT_THRESHOLD > magnitude)
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
