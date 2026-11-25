#include "s2/field_depth_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include <cmath>
#include <cstring>
#include "s2/map_visibility_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_vertex_cache_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/pak_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include <cstdio>
#include "s2/lighting_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/search_dc.hpp"
#include "s2/map_draw_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/event_matrix_dc.hpp"

namespace shenmue {
namespace s2 {

void PublishFieldDepthSphere(const Vector4 &sphere, std::uint32_t id);
void PublishFieldDepthWall(std::int32_t cameraOnly, const Vector3 *firstPosition, const Vector3 *secondPosition);

void GetMapModelPosition(Vector3 *position, std::int32_t index);
std::uint32_t GetMapModelRotationY(std::int32_t index);
void GetFieldDepthCell(const Vector3 &position, MapCellOffset &cell);

namespace {

#include "s2/map_cell_offsets.inl"

} // namespace

__attribute__((aligned(32))) CameraFieldDepthSample worldFieldDepth[kFieldDepthSampleCount] = {}, cameraFieldDepth[kFieldDepthSampleCount] = {};
CameraFieldDepthSample *worldFieldDepthSamples = worldFieldDepth, *cameraFieldDepthSamples = cameraFieldDepth;
std::uint32_t fieldDepthPointCount;
__attribute__((aligned(32))) Matrix4x4 fieldDepthTransform = {}, fieldDepthModelTransform = {};
MapFieldDepthSlot g_mapFieldDepthSlots[32];

// @verified - d
const MapCellOffset *GetMapCellDirectionOffsets(std::int32_t direction) {
    return mapCellDirections[direction].offsets;
}

// @verified - d
std::int32_t GetMapVisibleCellCount(std::int32_t direction, std::int32_t tier) {
    return static_cast<std::int32_t>(mapCellDirections[direction].counts[tier]);
}

// @verified - d
void FreeMapFieldDepthSlot(std::int32_t index) {
    if (g_mapFieldDepthSlots[index].dangerSpheres)
        HLib::FreeTaskData(g_mapFieldDepthSlots[index].dangerSpheres);
    if (g_mapFieldDepthSlots[index].dangerCells)
        HLib::FreeTaskData(g_mapFieldDepthSlots[index].dangerCells);
    g_mapFieldDepthSlots[index].dangerSpheres = 0;
    g_mapFieldDepthSlots[index].dangerCount = 0;
    g_mapFieldDepthSlots[index].enabled = 0;
    g_mapFieldDepthSlots[index].dangerCellCount = 0;
    g_mapFieldDepthSlots[index].dangerCells = 0;
    if (g_mapFieldDepthSlots[index].walls)
        HLib::FreeTaskData(g_mapFieldDepthSlots[index].walls);
    if (g_mapFieldDepthSlots[index].cameraWalls)
        HLib::FreeTaskData(g_mapFieldDepthSlots[index].cameraWalls);
    g_mapFieldDepthSlots[index].walls = 0;
    g_mapFieldDepthSlots[index].wallCount = 0;
    g_mapFieldDepthSlots[index].cameraWalls = 0;
    g_mapFieldDepthSlots[index].cameraWallCount = 0;
}

// @verified - d
void InitializeMapFieldDepthSlots() {
    MapFieldDepthSlot *slot = g_mapFieldDepthSlots;
    for (std::int32_t index = 0; index < 32; ++index, ++slot) {
        slot->dangerSpheres = 0;
        slot->dangerCount = 0;
        slot->enabled = 0;
        slot->walls = 0;
        slot->wallCount = 0;
        slot->cameraWalls = 0;
        slot->cameraWallCount = 0;
    }
}

// @verified - d
void SetMapFieldDepthSpheres(Vector4 *value, std::int32_t index) {
    g_mapFieldDepthSlots[index].dangerSpheres = value;
}

// @verified - d
Vector4 *GetMapFieldDepthSpheres(std::int32_t index) {
    return g_mapFieldDepthSlots[index].dangerSpheres;
}

// @verified - d
void SetMapFieldDepthSphereCount(std::int32_t value, std::int32_t index) {
    g_mapFieldDepthSlots[index].dangerCount = value;
}

// @verified - d
std::int32_t GetMapFieldDepthSphereCount(std::int32_t index) {
    return g_mapFieldDepthSlots[index].dangerCount;
}

// @verified - d
void SetMapFieldDepthWalls(MapFieldDepthWall *value, std::int32_t index) {
    g_mapFieldDepthSlots[index].walls = value;
}

// @verified - d
MapFieldDepthWall *GetMapFieldDepthWalls(std::int32_t index) {
    return g_mapFieldDepthSlots[index].walls;
}

// @verified - d
void SetMapFieldDepthCameraWalls(MapFieldDepthWall *value, std::int32_t index) {
    g_mapFieldDepthSlots[index].cameraWalls = value;
}

// @verified - d
MapFieldDepthWall *GetMapFieldDepthCameraWalls(std::int32_t index) {
    return g_mapFieldDepthSlots[index].cameraWalls;
}

// @verified - d
void SetMapFieldDepthWallCount(std::int32_t value, std::int32_t index) {
    g_mapFieldDepthSlots[index].wallCount = value;
}

// @verified - d
std::int32_t GetMapFieldDepthWallCount(std::int32_t index) {
    return g_mapFieldDepthSlots[index].wallCount;
}

// @verified - d
void SetMapFieldDepthCameraWallCount(std::int32_t value, std::int32_t index) {
    g_mapFieldDepthSlots[index].cameraWallCount = value;
}

// @verified - d
std::int32_t GetMapFieldDepthCameraWallCount(std::int32_t index) {
    return g_mapFieldDepthSlots[index].cameraWallCount;
}

// @verified - d
void camCtrlBuildYawDepthTransform() {
    Vector3 position;
    camCtrlGetNextCamPos(&position);
    const std::int32_t yaw = camCtrlGetNextRotY();
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(yaw));
    Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
    matrix_stack::GetMatrix(&fieldDepthTransform);
    PopCurrentMatrix(1);
}

// @verified - d
void camCtrlSubmitFieldDepth() {
    std::int32_t index, runIndex, cellRouted;
    MapFieldDepthSlot *field;
    std::int32_t inverseYaw, tier;
    Vector4 *spheres;
    MapFieldDepthWall *walls;
    std::uint32_t yaw;
    std::int32_t count;
    Vector3 position, camera;
    MapCellOffset cameraCell, cell;
    camCtrlBuildYawDepthTransform();
    const float farDistance = camCtrlGetFarDistance();
    inverseYaw = -camCtrlGetNextRotY() & 0xFFFF;
    camCtrlGetNextCamPos(&camera);
    fieldDepthPointCount = 0;
    CameraFieldDepthSample *sample = worldFieldDepthSamples;
    for (index = kFieldDepthSampleCount; index-- > 0; ++sample) {
        sample->id = kInitialFieldDepthId;
        sample->depth = kInitialFieldDepth;
    }
    GetFieldDepthCell(camera, cameraCell);
    tier = camCtrlGetDrawTier(farDistance);
    cellRouted = GetMapControlStatusFlags() & 1;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&fieldDepthTransform);
    field = g_mapFieldDepthSlots;
    for (index = 0; index < 32; ++field, ++index) {
        if (!field->enabled)
            continue;
        spheres = field->dangerSpheres;
        walls = field->walls;
        if (spheres) {
            matrix_stack::Matrix4x4_Push(0);
            GetMapModelPosition(&position, index);
            yaw = GetMapModelRotationY(index);
            if (!cellRouted) {
                if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
                    Matrix4x4::preTranslate(position.x, position.y, position.z);
                    Matrix4x4::RotateAroundY_Wrapper(yaw);
                }
                count = field->dangerCount;
                while (count-- > 0) {
                    PublishFieldDepthSphere(*spheres, fieldDepthPointCount);
                    ++spheres;
                }
            } else {
                std::int32_t cellX, cellZ;
                const MapCellOffset *visible;
                std::int32_t runCount;
                if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || !yaw) {
                    Vector3 local;
                    Vector2 angle;
                    const float x = camera.x - position.x, z = camera.z - position.z;
                    CalculateFixedSinCos(yaw, &angle);
                    local.x = x * angle.y + angle.x * z, local.z = x * -angle.x + angle.y * z;
                    GetFieldDepthCell(local, cell);
                    Matrix4x4::preTranslate(position.x, position.y, position.z);
                    Matrix4x4::RotateAroundY_Wrapper(yaw);
                    cellX = cell.x;
                    cellZ = cell.z;
                } else {
                    cellX = cameraCell.x;
                    cellZ = cameraCell.z;
                }
                const std::int32_t direction = static_cast<std::int32_t>((inverseYaw - yaw) & 0xFFFF) >> 12;
                visible = GetMapCellDirectionOffsets(direction);
                const std::int32_t visibleCount = GetMapVisibleCellCount(direction, tier);
                runCount = field->dangerCellCount;
                const MapModelCommandCell *run = field->dangerCells;
                for (runIndex = 0; runIndex < runCount; ++run, ++runIndex) {
                    const MapCellOffset *offset = visible;
                    const std::int16_t dx = run->x - cellX, dz = run->z - cellZ;
                    for (count = 0; count < visibleCount; ++count, ++offset) {
                        if (dx != offset->x || dz != offset->z)
                            continue;
                        Vector4 *sphere = spheres + run->firstRecord;
                        count = run->count;
                        while (count-- > 0) {
                            PublishFieldDepthSphere(*sphere, fieldDepthPointCount);
                            ++sphere;
                        }
                        break;
                    }
                }
            }
            PopCurrentMatrix(1);
        }
        if (walls) {
            matrix_stack::Matrix4x4_Push(0);
            GetMapModelPosition(&position, index);
            yaw = GetMapModelRotationY(index);
            if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
                Matrix4x4::preTranslate(position.x, 0.0f, position.z);
                Matrix4x4::RotateAroundY_Wrapper(yaw);
            }
            for (count = 0; count < field->wallCount; ++count) {
                PublishFieldDepthWall(0, &walls->first, &walls->second);
                ++walls;
            }
            PopCurrentMatrix(1);
        }
    }
    std::memcpy(cameraFieldDepthSamples, worldFieldDepthSamples, kFieldDepthSampleCount * sizeof(CameraFieldDepthSample));
    field = g_mapFieldDepthSlots;
    for (index = 0; index < 32; ++field, ++index) {
        MapFieldDepthWall *cameraWalls;
        if (!field->enabled || !(cameraWalls = field->cameraWalls))
            continue;
        matrix_stack::Matrix4x4_Push(0);
        GetMapModelPosition(&position, index);
        yaw = GetMapModelRotationY(index);
        if (position.x != 0.0f || position.y != 0.0f || position.z != 0.0f || yaw) {
            Matrix4x4::preTranslate(position.x, 0.0f, position.z);
            Matrix4x4::RotateAroundY_Wrapper(yaw);
        }
        for (count = 0; count < field->cameraWallCount; ++count, ++cameraWalls)
            PublishFieldDepthWall(1, &cameraWalls->first, &cameraWalls->second);
        PopCurrentMatrix(1);
    }
    PopCurrentMatrix(1);
}

void PublishFieldDepthSphereSpan(float x, float depth, float radius, std::uint32_t id) {
    ++fieldDepthPointCount;
    const float reciprocal = 1.0f / depth;
    std::int32_t first = static_cast<std::int32_t>(((x - radius) * reciprocal) * 128.0f);
    std::int32_t last = static_cast<std::int32_t>(((x + radius) * reciprocal) * 128.0f);
    if (first < -kFieldDepthHalfWidth)
        first = -kFieldDepthHalfWidth;
    else if (first > kFieldDepthHalfWidth)
        first = kFieldDepthHalfWidth;
    if (last > kFieldDepthHalfWidth)
        last = kFieldDepthHalfWidth;
    else if (last < -kFieldDepthHalfWidth)
        last = -kFieldDepthHalfWidth;
    first += kFieldDepthHalfWidth;
    last += kFieldDepthHalfWidth;
    CameraFieldDepthSample **table = &worldFieldDepthSamples;
    const std::size_t edgeOffset = static_cast<std::size_t>(last) * sizeof(CameraFieldDepthSample);
    const std::uintptr_t samples = reinterpret_cast<std::uintptr_t>(*table);
    std::uintptr_t address = edgeOffset + samples;
    CameraFieldDepthSample *edge = reinterpret_cast<CameraFieldDepthSample *>(address);
    --last;
    if (!(edge->depth > depth)) {
        const std::size_t firstOffset = static_cast<std::size_t>(first) * sizeof(CameraFieldDepthSample);
        if (reinterpret_cast<CameraFieldDepthSample *>(samples + firstOffset)->id == reinterpret_cast<CameraFieldDepthSample *>(edgeOffset + samples)->id)
            return;
        address -= sizeof(CameraFieldDepthSample);
        last -= first;
    } else {
        edge->id = id;
        edge->depth = depth;
        address -= sizeof(CameraFieldDepthSample);
        last -= first;
    }
    while (last-- >= 0) {
        CameraFieldDepthSample *sample = reinterpret_cast<CameraFieldDepthSample *>(address);
        if (sample->depth > depth) {
            sample->id = id;
            sample->depth = depth;
        }
        address -= sizeof(CameraFieldDepthSample);
    }
}

// @verified - d
void PublishFieldDepthSphere(const Vector4 &sphere, std::uint32_t id) {
    const Vector3 centre = {SHEN_FIELD(x) sphere.x, SHEN_FIELD(y) sphere.y, SHEN_FIELD(z) sphere.z};
    Vector4 point;
    matrix_stack::TransformPosition(&centre, &point);
    if (!(point.z < 0.0f))
        return;
    const float depth = -point.z, extent = sphere.w + depth;
    if (0.0f > extent || depth - sphere.w > fieldDepthScale.farDistance)
        return;
    const float horizontal = fieldDepthScale.horizontalTangent * extent + sphere.w;
    if (point.x > horizontal || !(point.x > -horizontal))
        return;
    const float vertical = fieldDepthScale.verticalTangent * extent + sphere.w;
    if (point.y > vertical || !(point.y > -vertical))
        return;
    PublishFieldDepthSphereSpan(point.x, depth, sphere.w, id);
}

// @verified - d
bool camCtrlIsModelDepthVisible(const Vector3 &centre, const MT7Node &node, float radius) {
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&fieldDepthModelTransform);
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node.rotation.z);
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node.rotation.y);
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node.rotation.x);
    Vector3 point;
    Matrix4x4::TransformPoint(centre, point);
    PopCurrentMatrix(1);
    const float depth = -point.z;
    if (!(depth > 0.0f))
        return true;
    const float x = point.x;
    const float nearest = depth - radius;
    std::int32_t first = static_cast<std::int32_t>(((x - radius) / depth) * 128.0f);
    if (first < -kFieldDepthHalfWidth)
        first = -kFieldDepthHalfWidth;
    else if (first > kFieldDepthHalfWidth)
        first = kFieldDepthHalfWidth;
    first += kFieldDepthHalfWidth;
    if (!(nearest > worldFieldDepthSamples[first].depth))
        return true;
    std::int32_t last = static_cast<std::int32_t>(((x + radius) / depth) * 128.0f);
    if (last > kFieldDepthHalfWidth)
        last = kFieldDepthHalfWidth;
    else if (last < -kFieldDepthHalfWidth)
        last = -kFieldDepthHalfWidth;
    last += kFieldDepthHalfWidth;
    const CameraFieldDepthSample *sample = worldFieldDepthSamples + last;
    if (!(nearest > sample->depth))
        return true;
    --last;
    --sample;
    last -= first;
    while (last-- >= 0) {
        if (!(nearest > sample->depth))
            return true;
        --sample;
    }
    return false;
}

// @verified - d
bool camCtrlCheckDepthBufferPosition(const CameraFieldDepthSample *samples, const Vector3 &position, float radius) {
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&fieldDepthTransform);
    Vector3 point;
    Matrix4x4::TransformPoint(position, point);
    PopCurrentMatrix(1);
    const float depth = -point.z;
    if (!(depth > 0.0f))
        return true;
    const float x = point.x;
    const float nearest = depth - 0.5f * radius;
    std::int32_t first = static_cast<std::int32_t>(((x - radius) / depth) * 128.0f);
    if (first < -kFieldDepthHalfWidth)
        first = -kFieldDepthHalfWidth;
    else if (first > kFieldDepthHalfWidth)
        first = kFieldDepthHalfWidth;
    first += kFieldDepthHalfWidth;
    if (!(nearest > samples[first].depth))
        return true;
    std::int32_t last = static_cast<std::int32_t>(((x + radius) / depth) * 128.0f);
    if (last > kFieldDepthHalfWidth)
        last = kFieldDepthHalfWidth;
    else if (last < -kFieldDepthHalfWidth)
        last = -kFieldDepthHalfWidth;
    last += kFieldDepthHalfWidth;
    const CameraFieldDepthSample *sample = &samples[last];
    if (!(nearest > sample->depth))
        return true;
    std::int32_t count = last - 1;
    --sample;
    count -= first;
    while (count-- >= 0) {
        if (!(nearest > sample->depth))
            return true;
        --sample;
    }
    return false;
}

// @verified - d
bool camCtrlCheckWorldDepthPosition(const Vector3 &position, float radius) {
    return camCtrlCheckDepthBufferPosition(worldFieldDepthSamples, position, radius);
}

// @verified - d
bool camCtrlCheckPosition(const Vector3 &position, float radius) {
    return camCtrlCheckDepthBufferPosition(cameraFieldDepthSamples, position, radius);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool camCtrlIsViewPointDepthVisible(const Vector3 &point, float radius) {
    const float x = point.x;
    const float depth = point.z;
    if (!(depth > 0.0f))
        return true;
    const float nearest = depth - radius;
    std::int32_t first = static_cast<std::int32_t>(((x - radius) / depth) * 128.0f);
    if (first < -kFieldDepthHalfWidth)
        first = -kFieldDepthHalfWidth;
    else if (first > kFieldDepthHalfWidth)
        first = kFieldDepthHalfWidth;
    first += kFieldDepthHalfWidth;
    if (!(nearest > worldFieldDepthSamples[first].depth))
        return true;
    std::int32_t last = static_cast<std::int32_t>(((x + radius) / depth) * 128.0f);
    if (last > kFieldDepthHalfWidth)
        last = kFieldDepthHalfWidth;
    else if (last < -kFieldDepthHalfWidth)
        last = -kFieldDepthHalfWidth;
    last += kFieldDepthHalfWidth;
    const CameraFieldDepthSample *sample = worldFieldDepthSamples + last;
    if (!(nearest > sample->depth))
        return true;
    --last;
    --sample;
    last -= first;
    while (last-- >= 0) {
        if (!(nearest > sample->depth))
            return true;
        --sample;
    }
    return false;
}
#endif

// @verified - d
void SetMapFieldDepthSlotEnabled(std::int32_t enabled, std::int32_t index) {
    if (!g_mapFieldDepthSlots[index].dangerSpheres && !g_mapFieldDepthSlots[index].walls && !g_mapFieldDepthSlots[index].cameraWalls)
        g_mapFieldDepthSlots[index].enabled = 0;
    else
        g_mapFieldDepthSlots[index].enabled = enabled;
}

// @verified - d
std::uint32_t GetMapFieldDepthSlotEnabled(std::int32_t index) {
    return g_mapFieldDepthSlots[index].enabled;
}

// @verified - d
void SetAllMapFieldDepthSlotsEnabled(std::int32_t enabled) {
    for (std::int32_t index = 0; index < 32; ++index)
        SetMapFieldDepthSlotEnabled(enabled, index);
}

// @verified - d
void SetMapFieldDepthCells(MapModelCommandCell *value, std::int32_t index) {
    g_mapFieldDepthSlots[index].dangerCells = value;
}

// @verified - d
MapModelCommandCell *GetMapFieldDepthCells(std::int32_t index) {
    return g_mapFieldDepthSlots[index].dangerCells;
}

// @verified - d
void SetMapFieldDepthCellCount(std::int32_t value, std::int32_t index) {
    g_mapFieldDepthSlots[index].dangerCellCount = value;
}

// @verified - d
std::int32_t GetMapFieldDepthCellCount(std::int32_t index) {
    return g_mapFieldDepthSlots[index].dangerCellCount;
}

// @verified - d
void SetFieldDepthPointCount(std::uint32_t count) {
    fieldDepthPointCount = count;
}

// @verified - d
std::uint32_t GetFieldDepthPointCount() {
    return fieldDepthPointCount;
}

void PublishFieldDepthWall(std::int32_t cameraOnly, const Vector3 *firstPosition, const Vector3 *secondPosition) {
    float depth = camCtrlGetFarDistance();
    Vector3 first, second;
    Matrix4x4::TransformPoint(*firstPosition, first);
    Matrix4x4::TransformPoint(*secondPosition, second);
    first.z = -first.z;
    second.z = -second.z;
    if ((kFieldDepthNearClip > first.z && kFieldDepthNearClip > second.z) || (first.z > depth && second.z > depth))
        return;
    float slope = second.x - first.x;
    slope /= second.z - first.z;
    const float oldX = first.x, oldDepth = first.z;
    if (kFieldDepthNearClip > first.z) {
        first.z = kFieldDepthNearClip;
        first.x = ((slope) * (kFieldDepthNearClip - oldDepth) + (oldX));
    }
    if (first.z > depth) {
        first.z = depth;
        first.x = ((slope) * (depth - oldDepth) + (oldX));
    }
    if (kFieldDepthNearClip > second.z) {
        second.z = kFieldDepthNearClip;
        second.x = ((slope) * (kFieldDepthNearClip - oldDepth) + (oldX));
    }
    if (second.z > depth) {
        second.z = depth;
        second.x = ((slope) * (depth - oldDepth) + (oldX));
    }
    std::int32_t begin = static_cast<std::int32_t>((first.x / first.z) * 128.0f);
    std::int32_t end = static_cast<std::int32_t>((second.x / second.z) * 128.0f);
    if (begin > end) {
        const float savedX = first.x, savedY = first.y, savedZ = first.z;
        const std::int32_t savedIndex = begin;
        first.x = second.x;
        first.y = second.y;
        first.z = second.z;
        begin = end;
        second.x = savedX;
        second.y = savedY;
        second.z = savedZ;
        end = savedIndex;
    }
    if (begin > kFieldDepthHalfWidth || end < -kFieldDepthHalfWidth)
        return;
    if (begin < -kFieldDepthHalfWidth)
        begin = -kFieldDepthHalfWidth;
    if (end > kFieldDepthHalfWidth)
        end = kFieldDepthHalfWidth;
    std::uint32_t id = fieldDepthPointCount;
    const std::int32_t firstSample = begin + kFieldDepthHalfWidth;
    CameraFieldDepthSample **samples;
    if (cameraOnly)
        samples = &cameraFieldDepthSamples;
    else
        samples = &worldFieldDepthSamples;
    CameraFieldDepthSample *sample = *samples + firstSample;
    slope = second.z - first.z;
    slope /= second.x - first.x;
    const float intercept = first.z - slope * first.x;
    for (std::int32_t x = begin; x <= end; ++x, ++sample) {
        depth = intercept;
        depth /= 1.0f - (static_cast<float>(x) * slope) * kFieldDepthPixelScale;
        if (!(0.0f > depth) && depth < sample->depth) {
            sample->id = id++;
            sample->depth = depth;
        }
    }
    fieldDepthPointCount = id;
}

// @verified - d
const Matrix4x4 &GetFieldDepthTransform() {
    return fieldDepthTransform;
}

// @verified - d
Matrix4x4 &GetFieldDepthModelTransform() {
    return fieldDepthModelTransform;
}

} // namespace s2
} // namespace shenmue

#endif
