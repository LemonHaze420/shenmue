#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/field_collision_dc.hpp"

#include "s2/map_info.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/matrix.hpp"

#include <cstring>
#include <float.h>
#include "s2/field_event_dc.hpp"
#include "s2/field_sphere_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/qron_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cfloat>
#include <cmath>
#include "s2/math.hpp"
#include "s2/qron_map_data_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/elevator.hpp"
#include "s2/lighting.hpp"
#include "s2/fog_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/weather.hpp"
#include "s2/screen_film.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/model_render_dc.hpp"
#include "s2/world_render.hpp"
#endif
#else
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"

#include "s2/map_info.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

using namespace field_collision_data;

std::array<FieldGeneratedVolume, kMaximumGeneratedVolumes> generatedVolumes{};

std::size_t generatedVolumeCount{};

FieldDataSourceRoot *flddRuntimeSpace{};

std::array<std::int16_t, kMaximumGeneratedVolumes * 6U + 2U> generatedPolygonList{};

bool generatedRuntimeAllocated{};

std::size_t lastRadiusPartIndex = std::numeric_limits<std::size_t>::max();

std::vector<FieldDataPart> fieldParts;

std::uint16_t fieldDataFlags = 0x10;

std::uint16_t fieldDataClearFlags = 0;

std::uint16_t fieldDataResetFlags = 0;

std::uint16_t intersectionMask = 0;

bool foundIntersection = false;

FieldIntersection lastIntersection{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

bool TestPointInsidePolygon(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &point, const Vector3 &normal);
bool DecodeCategoryPair(std::uint32_t category, std::int32_t &first, std::int32_t &second);
void ResetQueryFlags();

} // namespace

void transformIntersectionResult(Vector3s &angles);
void TransformAgainstIntersectionResult(Vector3 &point);
std::int32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 &point, float radius);
bool FLDD_AllocateRuntimeBuffers();
bool FLDD_ReadFieldData(const void *chunk, std::size_t chunkSize);
std::int32_t ParseFLDD(std::span<const std::byte> data, shen_name name);
std::int32_t RemoveFieldPart(shen_name name);
bool flddIsValid();
void flddClearAllGeneratedVolumes();
bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::uint16_t yaw);
std::int32_t flddSetPointFlagOnGrid(shen_name partName, std::int32_t flag);
std::int32_t SetOccupancyForThisFieldSegment(shen_name partName, const Vector3 *translation, std::int32_t yaw);
std::int32_t ToggleFlagOnFieldEntity(shen_name entityName, std::int32_t toggle);
void flddToggleFlag(std::int16_t flags);
void flddPushIntersectionFlag(std::int16_t flags);
std::int32_t FindNearestRayIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int32_t RaycastInWorldGrid(Vector3 &output, const Vector3 &origin, float radius);
float shenFindIntersectionWithScene(const Vector3 &position, float maximumY);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
#else
bool GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
#endif
std::int32_t scenFindIntersection(const Vector3 &position, float maximumY);
std::int32_t flddQueryStairOrigin(const Vector3 &position, Vector3 &origin, std::int32_t &yaw);
std::int32_t flddQueryStairRadius(const Vector3 &position, Vector3 &origin, std::int32_t &yaw, float &maximumEdgeLength);
std::uint32_t flddQueryStairConnection(const Vector3 &position, float maximumY, Vector3 &anchor, std::int32_t &yaw, std::int32_t &connectionId);
std::int32_t plymDoIntersectionTest(const Vector3 &position, float maximumY);
std::uint32_t flddTestMapEventIntersection(const Vector3 &position);
std::int32_t MoveObjectToPosFromThisScene(shen_name partName, std::int32_t entryId, Vector3 &position, std::int32_t &yaw);
bool flddHasFoundIntersectionOfType(std::uint16_t flags);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestPointInsidePolygon(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &point, const Vector3 &normal);

void ResetQueryFlags();

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

template <typename T> const T *SpanAt(std::span<const std::byte> bytes, std::size_t offset, std::size_t count = 1) {
    if (offset > bytes.size() || count > (bytes.size() - offset) / sizeof(T)) {
        return nullptr;
    }
    return reinterpret_cast<const T *>(bytes.data() + offset);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t DerivePolygonFlags(const FieldDataSourcePolygon &source, std::span<const Vector3> vertices, std::span<const Vector3> normals) {
    std::uint16_t flags = source.flags;
    unsigned duplicatePairs = 0;
    for (std::size_t left = 0; left < source.vertexIndices.size(); ++left) {
        for (std::size_t right = left + 1; right < source.vertexIndices.size(); ++right) {
            if (source.vertexIndices[left] == source.vertexIndices[right]) {
                ++duplicatePairs;
            }
        }
    }
    if ((source.vertexIndices[0] == source.vertexIndices[3] && duplicatePairs >= 2) ||
        (source.vertexIndices[0] != source.vertexIndices[3] && duplicatePairs >= 1)) {
        flags |= 0x10U;
    }

    if ((flags & 9U) == 0 && source.normalIndex < normals.size() && std::fabs(normals[source.normalIndex].y) <= 0.5f) {
        float minimumY = std::numeric_limits<float>::max();
        float maximumY = -std::numeric_limits<float>::max();
        for (const std::uint16_t index : source.vertexIndices) {
            if (index >= vertices.size())
                return flags;

            minimumY = std::min(minimumY, vertices[index].y);
            maximumY = std::max(maximumY, vertices[index].y);
        }
        flags |= (maximumY - minimumY >= 0.45f) ? 1U : 0x400U;
    }
    return flags;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ReadPart(std::span<const std::byte> source, shen_name name, FieldDataPart &output) {
    const auto *const rootOffset = SpanAt<std::uint32_t>(source, 0);
    if (!rootOffset)
        return false;

    const auto *const root = SpanAt<FieldDataSourceRoot>(source, *rootOffset);
    if (!root)
        return false;

    const auto &offsets = root->segmentOffsets;
    for (const std::uint32_t offset : offsets) {
        if (offset > source.size())
            return false;
    }

    if (offsets[0] > offsets[1] || offsets[1] > offsets[2])
        return false;

    const std::size_t vertexBytes = offsets[1] - offsets[0];
    const std::size_t normalBytes = offsets[2] - offsets[1];
    if (offsets[2] > offsets[3])
        return false;

    const std::size_t planeDistanceBytes = offsets[3] - offsets[2];
    if (vertexBytes % sizeof(Vector3) || normalBytes % sizeof(Vector3)) {
        return false;
    }
    if (planeDistanceBytes % sizeof(float))
        return false;
    const auto *const vertices = SpanAt<Vector3>(source, offsets[0], vertexBytes / sizeof(Vector3));
    const auto *const normals = SpanAt<Vector3>(source, offsets[1], normalBytes / sizeof(Vector3));
    const auto *const planeDistances = SpanAt<float>(source, offsets[2], planeDistanceBytes / sizeof(float));
    const auto *const metadata = SpanAt<std::uint32_t>(source, offsets[6], 2);
    const auto *const grid = SpanAt<FieldDataGridConfig>(source, offsets[4]);
    if ((!vertices && vertexBytes) || (!normals && normalBytes) || (!planeDistances && planeDistanceBytes) || !metadata || !grid) {
        return false;
    }

    float gridMode{};
    std::memcpy(&gridMode, metadata, sizeof(gridMode));
    const std::int32_t polygonCount = static_cast<std::int32_t>(metadata[1]);
    if (!std::isfinite(gridMode) || polygonCount < 0)
        return false;
    const std::int32_t partClass = gridMode > 0.0f ? 0 : static_cast<std::int32_t>(-gridMode);
    const auto *const polygons = SpanAt<FieldDataSourcePolygon>(source, offsets[3], static_cast<std::size_t>(polygonCount));
    if (!polygons && polygonCount)
        return false;

    output = {};
    output.name = name;
    output.data = root;
    output.enabled = 1;
    output.gridMode = gridMode;
    output.partClass = partClass;
    output.grid = *grid;
    output.segmentOffsets = offsets;
    output.sourceData = source;
    if (vertexBytes) {
        output.vertices.assign(vertices, vertices + vertexBytes / sizeof(Vector3));
    }
    if (normalBytes) {
        output.normals.assign(normals, normals + normalBytes / sizeof(Vector3));
    }
    if (planeDistanceBytes) {
        output.planeDistances.assign(planeDistances, planeDistances + planeDistanceBytes / sizeof(float));
    }
    output.polygons.reserve(static_cast<std::size_t>(polygonCount));
    for (std::int32_t index = 0; index < polygonCount; ++index) {
        const FieldDataSourcePolygon &sourcePolygon = polygons[index];
        if (sourcePolygon.normalIndex >= output.normals.size())
            return false;
        if (sourcePolygon.field06 >= output.planeDistances.size())
            return false;
        for (const std::uint16_t vertex : sourcePolygon.vertexIndices) {
            if (vertex >= output.vertices.size())
                return false;
        }
        output.polygons.push_back(FieldDataPolygon{
            partClass >= 2 ? DerivePolygonFlags(sourcePolygon, output.vertices, output.normals) : sourcePolygon.flags,
            sourcePolygon.dataGroup,
            sourcePolygon.normalIndex,
            sourcePolygon.field06,
            sourcePolygon.vertexIndices,
        });
    }
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float Dot(const Vector3 &left, const Vector3 &right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float SquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    return x * x + y * y + z * z;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native Vector3::PlanarSquaredDistance: the XZ squared distance, or zero when both deltas are below FLT_EPSILON.
float PlanarSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    if (std::fabs(x) < std::numeric_limits<float>::epsilon() && std::fabs(z) < std::numeric_limits<float>::epsilon())
        return 0.0f;
    return z * z + x * x;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 TransformPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    Vector3 output;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrix);
    Matrix4x4::TransformPoint(point, output);
    Matrix4x4::Pop(1);
    return output;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

Vector3 TransformVector(const Matrix4x4 &matrix, const Vector3 &vector) {
    Vector3 output;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrix);
    Matrix4x4::Transform(vector, output);
    Matrix4x4::Pop(1);
    return output;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void BuildPartTransforms(FieldDataRuntimePart &part) {
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(part.translation.x, part.translation.y, part.translation.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(part.yaw));
    Matrix4x4::GetTransformMatrix(&part.localToWorld);
    for (std::size_t row = 0; row < 3; ++row) {
        Vector4 &basis = workTransformMatrix.rows[row];
        const float length = std::sqrt((basis.x * basis.x + basis.y * basis.y) + basis.z * basis.z);
        if (length > 0.0f) {
            const float inverse = 1.0f / length;
            basis.x *= inverse;
            basis.y *= inverse;
            basis.z *= inverse;
        }
    }
    Matrix4x4::InvertRigidTransform();
    Matrix4x4::GetTransformMatrix(&part.worldToLocal);
    Matrix4x4::Pop(1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestLineIntersection(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &start, const Vector3 &end, Vector3 &intersection) {
    const Vector3 &normal = part.normals[polygon.normalIndex];
    const float planeDistance = part.planeDistances[polygon.field06];
    if (Dot(normal, end) - planeDistance >= 0.0f)
        return false;

    const float startDistance = planeDistance - Dot(normal, start);
    if (startDistance > 0.0f)
        return false;

    const Vector3 direction{end.x - start.x, end.y - start.y, end.z - start.z};
    const float denominator = Dot(direction, normal);
    const float factor = startDistance / denominator;
    intersection = Vector3{
        start.x + direction.x * factor,
        start.y + direction.y * factor,
        start.z + direction.z * factor,
    };

    return TestPointInsidePolygon(part, polygon, intersection, normal);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestPointInsidePolygon(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &point, const Vector3 &normal) {
    const std::size_t edgeCount = polygon.vertexIndices[0] == polygon.vertexIndices[3] ? 3U : 4U;
    for (std::size_t edge = 0; edge < edgeCount; ++edge) {
        const Vector3 &current = part.vertices[polygon.vertexIndices[edge]];
        const Vector3 &next = part.vertices[polygon.vertexIndices[(edge + 1U) & 3U]];
        const Vector3 segment{next.x - current.x, next.y - current.y, next.z - current.z};
        const Vector3 toPoint{
            point.x - current.x,
            point.y - current.y,
            point.z - current.z,
        };
        const Vector3 cross{
            segment.y * toPoint.z - segment.z * toPoint.y,
            segment.z * toPoint.x - segment.x * toPoint.z,
            segment.x * toPoint.y - segment.y * toPoint.x,
        };
        if (static_cast<double>(Dot(cross, normal)) < -0.008726)
            return false;
    }
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool FindClosestEdgeWithinRadius(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &point, float radius, Vector3 &closestPoint) {
    const float radiusSquared = radius * radius;
    const std::size_t edgeCount = polygon.vertexIndices[0] == polygon.vertexIndices[3] ? 3U : 4U;
    for (std::size_t edge = 0; edge < edgeCount; ++edge) {
        const Vector3 &current = part.vertices[polygon.vertexIndices[edge]];
        const Vector3 &next = part.vertices[polygon.vertexIndices[(edge + 1U) & 3U]];
        const Vector3 segment{next.x - current.x, next.y - current.y, next.z - current.z};
        const Vector3 fromPoint{
            current.x - point.x,
            current.y - point.y,
            current.z - point.z,
        };

        const float segmentLengthSquared = Dot(segment, segment);
        if (segmentLengthSquared == 0.0f)
            continue;

        const float factor = -Dot(fromPoint, segment) / segmentLengthSquared;
        if (factor < 0.0f || factor > 1.0f)
            continue;

        const Vector3 candidate{
            current.x + segment.x * factor,
            current.y + segment.y * factor,
            current.z + segment.z * factor,
        };
        const Vector3 separation{
            fromPoint.x + segment.x * factor,
            fromPoint.y + segment.y * factor,
            fromPoint.z + segment.z * factor,
        };
        if (radiusSquared > Dot(separation, separation)) {
            closestPoint = candidate;
            return true;
        }
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// The native voxel buffer and polygon lists contain signed 16-bit entries.
// Keep the narrowing before lookup, including the -1 terminator; do not
// substitute a polygon scan when the source grid/list is absent.
template <typename Visitor> bool VisitVoxelPolygons(const FieldDataPart &part, std::int16_t voxel, Visitor &&visit) {
    std::int64_t listOffset = part.segmentOffsets[5];
    if (part.segmentOffsets[4] != 0) {
        const std::int64_t tableOffset = listOffset + static_cast<std::int64_t>(voxel) * 4;
        if (tableOffset < 0)
            return false;
        const auto *offset = SpanAt<std::int32_t>(part.sourceData, static_cast<std::size_t>(tableOffset));
        if (!offset || *offset <= 0)
            return false;
        listOffset = *offset;
    }

    if (listOffset == 0)
        return false;

    for (std::size_t cursor = static_cast<std::size_t>(listOffset);; cursor += sizeof(std::int16_t)) {
        const auto *entry = SpanAt<std::int16_t>(part.sourceData, cursor);
        if (!entry || *entry == -1)
            return false;

        if (*entry < 0 || static_cast<std::size_t>(*entry) >= part.polygons.size())
            return false;
        if (visit(static_cast<std::uint16_t>(*entry)))
            return true;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// SetNewIntersection saves LOCAL plane/point/owner context.
// The outer native queries transform only their returned heights, not this
// publication. A later owner can overwrite it without winning the height.
void PublishPartIntersection(const FieldDataPart &part, std::size_t partIndex, std::uint16_t polygonIndex, const Vector3 &point) {
    const FieldDataPolygon &polygon = part.polygons[polygonIndex];
    foundIntersection = true;
    lastRadiusPartIndex = partIndex;
    lastIntersection = {
        point,
        part.normals[polygon.normalIndex],
        part.planeDistances[polygon.field06],
        polygon.flags,
        polygonIndex,
        part.name,
    };
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// ClipPointToPlaneRegion. The lower bound is calculated in
// double precision, unlike the single-precision starting plane distance.
bool ClipPointToPlaneRegion(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &input, Vector3 &clipped) {
    const Vector3 &normal = part.normals[polygon.normalIndex];
    const float distance = (normal.z * input.z + normal.x * input.x) - part.planeDistances[polygon.field06];
    if (!(distance >= -(input.y * normal.y)) || !((static_cast<double>(input.y) - 9999.9) * static_cast<double>(-normal.y) > distance)) {
        return false;
    }
    clipped = {
        input.x,
        input.y - (distance + normal.y * input.y) / normal.y,
        input.z,
    };
    return TestPointInsidePolygon(part, polygon, clipped, normal);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// MapPointToVoxels descends from clamped Y; MapPointOnGrid
// ascends from clamped Y. Both truncate coordinates toward zero.
// Stream the native word sequence rather than allocating a temporary buffer.
template <typename Visitor> void VisitVerticalVoxels(const FieldDataPart &part, const Vector3 &point, bool ascending, Visitor &&visit) {
    if (part.segmentOffsets[4] == 0) {
        visit(std::int16_t{0});
        return;
    }
    const auto &grid = part.grid;
    const auto width = static_cast<std::int32_t>(grid.width);
    const auto height = static_cast<std::int32_t>(grid.height);
    const auto depth = static_cast<std::int32_t>(grid.depth);
    const auto x = static_cast<std::int32_t>((point.x - grid.minimumX) * grid.reciprocalCellSize);
    auto y = static_cast<std::int32_t>((point.y - grid.minimumY) * grid.reciprocalCellSize);
    const auto z = static_cast<std::int32_t>((point.z - grid.minimumZ) * grid.reciprocalCellSize);
    if (x < 0 || x >= width || z < 0 || z >= depth || (ascending ? y >= height : y < 0)) {
        return;
    }
    y = ascending ? std::max(y, 0) : std::min(y, height - 1);
    const auto stride = static_cast<std::int32_t>(static_cast<std::uint32_t>(width) * depth);
    const auto limit = static_cast<std::int32_t>(static_cast<std::uint32_t>(stride) * height);
    auto index = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + static_cast<std::uint32_t>(width) * z + static_cast<std::uint32_t>(stride) * y);
    while (ascending ? index < limit : index >= 0) {
        const auto voxel = static_cast<std::int16_t>(index);
        if (voxel == -1 || visit(voxel))
            return;
        index = static_cast<std::int32_t>(ascending ? static_cast<std::uint32_t>(index) + stride : static_cast<std::uint32_t>(index) - stride);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float FindVerticalIntersection(const FieldDataPart &part, std::size_t partIndex, const Vector3 &input, bool minimum) {
    if (!part.data)
        return input.y;
    float height = minimum ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max();
    Vector3 voxelPoint = input;
    // ClipPointToPlaneRegionOnGrid uses the rounded float
    // constant for grid mapping but the original point for polygon clipping.
    if (minimum)
        voxelPoint.y -= 9999.900390625f;
    const auto activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    VisitVerticalVoxels(part, voxelPoint, minimum, [&](std::int16_t voxel) {
        bool hit = false;
        VisitVoxelPolygons(part, voxel, [&](std::uint16_t polygonIndex) {
            const auto &polygon = part.polygons[polygonIndex];
            Vector3 clipped{};
            if ((polygon.flags & activeFlags) == intersectionMask && ClipPointToPlaneRegion(part, polygon, input, clipped) &&
                (minimum ? clipped.y < height : clipped.y > height)) {
                height = clipped.y;
                PublishPartIntersection(part, partIndex, polygonIndex, clipped);
                hit = true;
            }
            return false;
        });
        // First voxel containing a hit wins; ties keep the first polygon.
        return hit;
    });
    return height;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// RaycastThrough3DGrid_DDA. Native uses signed cell coordinates
// and 32-bit step-budget products, then narrows each emitted index to int16.
// Negative first boundary times adjust the next boundary without moving the
// cell. Equal boundary times choose Z, then Y, then X (not a diagonal step).
template <typename Visitor> void VisitSegmentVoxels(const FieldDataPart &part, const Vector3 &start, const Vector3 &end, Visitor &&visit) {
    if (part.segmentOffsets[4] == 0) {
        visit(std::int16_t{0});
        return;
    }
    const auto &grid = part.grid;
    const std::array<float, 3> minimum{grid.minimumX, grid.minimumY, grid.minimumZ};
    const std::array<float, 3> origin{start.x, start.y, start.z};
    const std::array<float, 3> destination{end.x, end.y, end.z};
    const std::array<std::int32_t, 3> dimensions{
        static_cast<std::int32_t>(grid.width), static_cast<std::int32_t>(grid.height), static_cast<std::int32_t>(grid.depth)};
    std::array<std::int32_t, 3> cell{}, last{}, step{}, next{};
    std::array<float, 3> reciprocalDirection{}, boundaryTime{};
    std::uint32_t stepProduct = 1;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        cell[axis] = static_cast<std::int32_t>((origin[axis] - minimum[axis]) * grid.reciprocalCellSize);
        last[axis] = static_cast<std::int32_t>((destination[axis] - minimum[axis]) * grid.reciprocalCellSize);
        const auto delta = static_cast<std::int32_t>(static_cast<std::uint32_t>(last[axis]) - cell[axis]);
        step[axis] = (delta > 0) - (delta < 0);
        const auto magnitude = delta < 0 ? 0U - static_cast<std::uint32_t>(delta) : static_cast<std::uint32_t>(delta);
        stepProduct *= magnitude + 1U;
        const float direction = destination[axis] - origin[axis];
        reciprocalDirection[axis] = std::fabs(direction) >= std::numeric_limits<float>::epsilon() ? 1.0f / direction : 100000.0f;
        boundaryTime[axis] = std::numeric_limits<float>::max();
    }
    const auto outsideTogether = [&] {
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if ((cell[axis] < 0 && last[axis] < 0) || (cell[axis] >= dimensions[axis] && last[axis] >= dimensions[axis])) {
                return true;
            }
        }
        return false;
    };
    if (outsideTogether())
        return;

    const auto gridBudget = static_cast<std::int32_t>(2U * dimensions[0] * dimensions[1] * dimensions[2]);
    auto remaining = static_cast<std::int32_t>(static_cast<std::uint32_t>(std::min(static_cast<std::int32_t>(stepProduct), gridBudget)) + 8U);
    const float cellSize = 1.0f / grid.reciprocalCellSize;
    std::int32_t previousIndex = -1;
    while (remaining > 0) {
        --remaining;
        if (cell[0] >= 0 && cell[0] < dimensions[0] && cell[1] >= 0 && cell[1] < dimensions[1] && cell[2] >= 0 && cell[2] < dimensions[2]) {
            const auto index = static_cast<std::int32_t>(static_cast<std::uint32_t>(cell[0]) +
                static_cast<std::uint32_t>(dimensions[0]) * (static_cast<std::uint32_t>(dimensions[2]) * cell[1] + cell[2]));
            if (index != previousIndex) {
                if (visit(static_cast<std::int16_t>(index)))
                    return;
                previousIndex = index;
            }
        }
        if (cell == last || outsideTogether())
            return;
        for (std::size_t axis = 0; axis < 3; ++axis) {
            if (step[axis] == 0)
                continue;
            const auto boundary = static_cast<std::int32_t>(static_cast<std::uint32_t>(next[axis]) + cell[axis]);
            boundaryTime[axis] = ((static_cast<float>(boundary) * cellSize + minimum[axis]) - origin[axis]) * reciprocalDirection[axis];
        }
        const std::size_t axis =
            boundaryTime[1] > boundaryTime[0] ? (boundaryTime[2] > boundaryTime[0] ? 0U : 2U) : (boundaryTime[2] <= boundaryTime[1] ? 2U : 1U);
        if (boundaryTime[axis] >= 0.0f) {
            cell[axis] = static_cast<std::int32_t>(static_cast<std::uint32_t>(cell[axis]) + step[axis]);
            next[axis] = step[axis];
        } else {
            next[axis] = static_cast<std::int32_t>(static_cast<std::uint32_t>(next[axis]) + step[axis]);
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool FindSegmentIntersection(const FieldDataPart &part, std::size_t partIndex, const Vector3 &start, const Vector3 &end, bool withinRadius, float radius) {
    if (!part.data)
        return false;
    const auto activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    bool hit = false;
    std::int16_t previousVoxel = -1;
    VisitSegmentVoxels(part, start, end, [&](std::int16_t voxel) {
        if (voxel == -1) {
            previousVoxel = voxel;
            return false;
        }
        // FindIntersections stops at adjacent repeated word indices;
        // FindRayIntersectionInGridWithinRadius visits the full DDA count.
        if (!withinRadius && voxel == previousVoxel)
            return true;
        previousVoxel = voxel;
        hit = VisitVoxelPolygons(part, voxel, [&](std::uint16_t polygonIndex) {
            const auto &polygon = part.polygons[polygonIndex];
            if ((polygon.flags & activeFlags) != intersectionMask) {
                return false;
            }
            const auto &normal = part.normals[polygon.normalIndex];
            const float distance = part.planeDistances[polygon.field06];
            const float startDot = Dot(normal, start);
            if ((Dot(normal, end) - distance >= 0.0f) == (startDot - distance >= 0.0f)) {
                return false;
            }
            const Vector3 direction{end.x - start.x, end.y - start.y, end.z - start.z};
            const float factor = (distance - startDot) / Dot(normal, direction);
            Vector3 point{
                start.x + direction.x * factor,
                start.y + direction.y * factor,
                start.z + direction.z * factor,
            };
            if (!TestPointInsidePolygon(part, polygon, point, normal) && (!withinRadius || !FindClosestEdgeWithinRadius(part, polygon, point, radius, point))) {
                return false;
            }
            PublishPartIntersection(part, partIndex, polygonIndex, point);
            return true;
        });
        return hit;
    });
    return hit;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool FindClosestIntersectionInGrid(const FieldDataPart &part, std::size_t partIndex, Vector3 &output, const Vector3 &destination, const Vector3 &origin) {
    output = destination;
    if (!part.data)
        return true;
    const auto activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    float closestDistance = std::numeric_limits<float>::max();
    bool hit = false;
    std::int16_t previousVoxel = -1;
    VisitSegmentVoxels(part, origin, destination, [&](std::int16_t voxel) {
        if (voxel == -1) {
            previousVoxel = voxel;
            return false;
        }

        if (voxel == previousVoxel)
            return true;
        previousVoxel = voxel;
        VisitVoxelPolygons(part, voxel, [&](std::uint16_t polygonIndex) {
            const auto &polygon = part.polygons[polygonIndex];
            auto flags = polygon.flags;
            if ((flags & fieldDataClearFlags) != 0) {
                flags = static_cast<std::uint16_t>(flags & ~fieldDataResetFlags);
            }
            if ((flags & activeFlags) != intersectionMask)
                return false;

            Vector3 point{};
            if (!TestLineIntersection(part, polygon, origin, destination, point))
                return false;

            const float distance = SquaredDistance(point, origin);
            if (!(distance < closestDistance))
                return false;
            closestDistance = distance;
            hit = true;
            PublishPartIntersection(part, partIndex, polygonIndex, point);
            const auto &normal = part.normals[polygon.normalIndex];
            output = {
                point.x + normal.x * 0.001f,
                point.y + normal.y * 0.001f,
                point.z + normal.z * 0.001f,
            };
            return false;
        });
        // Unlike the height queries, a hit does not stop voxel traversal.
        return false;
    });
    return hit;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Unlike the DDA query, FindIntersection visits only the two
// endpoint cells and does not apply the conditional face-flag remapping.
std::int32_t FindEndpointIntersection(const FieldDataPart &part, std::size_t partIndex, Vector3 &output, const Vector3 &destination, const Vector3 &origin) {
    output = destination;
    if (!part.data)
        return -1;
    const auto voxelFor = [&part](const Vector3 &point) -> std::int16_t {
        if (part.segmentOffsets[4] == 0)
            return 0;
        const auto x = static_cast<std::int32_t>((point.x - part.grid.minimumX) * part.grid.reciprocalCellSize);
        const auto y = static_cast<std::int32_t>((point.y - part.grid.minimumY) * part.grid.reciprocalCellSize);
        const auto z = static_cast<std::int32_t>((point.z - part.grid.minimumZ) * part.grid.reciprocalCellSize);
        const auto width = static_cast<std::int32_t>(part.grid.width);
        const auto height = static_cast<std::int32_t>(part.grid.height);
        const auto depth = static_cast<std::int32_t>(part.grid.depth);
        if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
            return -1;
        return static_cast<std::int16_t>(x + width * (z + depth * y));
    };
    const std::int16_t voxels[2]{voxelFor(origin), voxelFor(destination)};
    const auto activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    float closestDistance = std::numeric_limits<float>::max();
    bool hit = false;
    for (std::uint8_t i = 0; i < 2; ++i) {
        if (voxels[i] == -1)
            continue;
        VisitVoxelPolygons(part, voxels[i], [&](std::uint16_t polygonIndex) {
            const auto &polygon = part.polygons[polygonIndex];
            if ((polygon.flags & activeFlags) != intersectionMask)
                return false;
            Vector3 point{};
            if (!TestLineIntersection(part, polygon, origin, destination, point))
                return false;
            const float distance = std::sqrt(SquaredDistance(point, origin));
            if (!(distance < closestDistance))
                return false;
            closestDistance = distance;
            hit = true;
            PublishPartIntersection(part, partIndex, polygonIndex, point);
            const auto &normal = part.normals[polygon.normalIndex];
            output = {point.x + normal.x * 0.001f, point.y + normal.y * 0.001f, point.z + normal.z * 0.001f};
            return false;
        });
        if (voxels[0] == voxels[1])
            break;
    }
    return hit;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestRadiusPolygon(const FieldDataPart &part, std::size_t partIndex, std::uint16_t polygonIndex, const Vector3 &point, float radius) {
    if (polygonIndex >= part.polygons.size())
        return false;
    const FieldDataPolygon &polygon = part.polygons[polygonIndex];
    const std::uint16_t activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    if ((polygon.flags & activeFlags) != intersectionMask)
        return false;

    const Vector3 &normal = part.normals[polygon.normalIndex];
    const float planeDistance = part.planeDistances[polygon.field06];
    const float signedDistance = Dot(normal, point) - planeDistance;
    if (signedDistance < 0.0f || radius <= signedDistance)
        return false;

    const Vector3 projected{
        point.x - signedDistance * normal.x,
        point.y - signedDistance * normal.y,
        point.z - signedDistance * normal.z,
    };
    Vector3 closest{};
    if (!TestPointInsidePolygon(part, polygon, projected, normal) && !FindClosestEdgeWithinRadius(part, polygon, point, radius, closest)) {
        return false;
    }
    PublishPartIntersection(part, partIndex, polygonIndex, projected);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestRadiusPolygonList(const FieldDataPart &part, std::size_t partIndex, std::size_t listOffset, const Vector3 &point, float radius) {
    for (std::size_t cursor = listOffset;; cursor += sizeof(std::int16_t)) {
        const auto *const entry = SpanAt<std::int16_t>(part.sourceData, cursor);
        if (!entry || *entry == -1)
            return false;

        if (*entry < 0)
            return false;

        if (TestRadiusPolygon(part, partIndex, static_cast<std::uint16_t>(*entry), point, radius))
            return true;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestRadiusVoxel(const FieldDataPart &part, std::size_t partIndex, std::int32_t voxelIndex, const Vector3 &point, float radius) {
    std::size_t listOffset = part.segmentOffsets[5];
    if (part.segmentOffsets[4] != 0) {
        if (voxelIndex < 0)
            return false;

        const std::size_t tableIndex = static_cast<std::size_t>(voxelIndex);
        if (tableIndex > (std::numeric_limits<std::size_t>::max() - listOffset) / sizeof(std::int32_t))
            return false;

        const auto *const offset = SpanAt<std::int32_t>(part.sourceData, listOffset + tableIndex * sizeof(std::int32_t));
        if (!offset || *offset <= 0)
            return false;

        listOffset = static_cast<std::size_t>(*offset);
    }
    return TestRadiusPolygonList(part, partIndex, listOffset, point, radius);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool TestRadiusInPart(const FieldDataPart &part, std::size_t partIndex, const Vector3 &point, float radius) {
    if (!part.data)
        return false;
    if (part.segmentOffsets[4] == 0) {
        return TestRadiusVoxel(part, partIndex, 0, point, radius);
    }

    const int width = static_cast<int>(part.grid.width);
    const int height = static_cast<int>(part.grid.height);
    const int depth = static_cast<int>(part.grid.depth);
    const int xFirst = static_cast<int>(((point.x - radius) - part.grid.minimumX) * part.grid.reciprocalCellSize);
    const int xLast = static_cast<int>(((point.x + radius) - part.grid.minimumX) * part.grid.reciprocalCellSize);
    const int zFirst = static_cast<int>(((point.z - radius) - part.grid.minimumZ) * part.grid.reciprocalCellSize);
    const int zLast = static_cast<int>(((point.z + radius) - part.grid.minimumZ) * part.grid.reciprocalCellSize);
    const int yFirst = static_cast<int>(((point.y - radius) - part.grid.minimumY) * part.grid.reciprocalCellSize);
    const int yLast = static_cast<int>(((point.y + radius) - part.grid.minimumY) * part.grid.reciprocalCellSize);

    const int zStride = width;
    const int yStride = width * depth;
    for (int y = yFirst; y <= yLast; ++y) {
        if (y < 0 || y >= height)
            continue;

        for (int z = zFirst; z <= zLast; ++z) {
            if (z < 0 || z >= depth)
                continue;

            for (int x = xFirst; x <= xLast; ++x) {
                if (x < 0 || x >= width)
                    continue;

                const std::int16_t voxelIndex = static_cast<std::int16_t>(x + z * zStride + y * yStride);
                if (voxelIndex == -1)
                    return false;
                if (TestRadiusVoxel(part, partIndex, voxelIndex, point, radius)) {
                    return true;
                }
            }
        }
    }
    return false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ResolveHorizontalRadiusPolygon(const FieldDataPart &part, std::size_t partIndex, std::uint16_t polygonIndex, Vector3 &point, float radius) {
    if (polygonIndex >= part.polygons.size())
        return false;

    const FieldDataPolygon &polygon = part.polygons[polygonIndex];
    const std::uint16_t activeFlags = static_cast<std::uint16_t>(intersectionMask | fieldDataFlags);
    if ((polygon.flags & activeFlags) != intersectionMask)
        return false;

    const Vector3 &normal = part.normals[polygon.normalIndex];
    const float planeDistance = part.planeDistances[polygon.field06];
    const float signedDistance = Dot(normal, point) - planeDistance;
    if (signedDistance < 0.0f)
        return false;

    const float horizontalNormalSquared = normal.x * normal.x + normal.z * normal.z;

    const float projection = -signedDistance / horizontalNormalSquared;
    if (!(radius * radius > (horizontalNormalSquared * projection) * projection)) {
        return false;
    }

    const Vector3 projected{
        point.x + projection * normal.x,
        point.y,
        point.z + projection * normal.z,
    };
    if (TestPointInsidePolygon(part, polygon, projected, normal)) {
        PublishPartIntersection(part, partIndex, polygonIndex, projected);
        point.x = projected.x + radius * normal.x;
        point.z = projected.z + radius * normal.z;
        return true;
    }
    Vector3 closest{};
    if (!FindClosestEdgeWithinRadius(part, polygon, point, radius, closest))
        return false;

    const Vector3 separation{closest.x - point.x, closest.y - point.y, closest.z - point.z};
    const float length = std::sqrt(Dot(separation, separation));
    const float inverseLength = length > 0.0f ? 1.0f / length : 0.0f;
    const Vector3 corrected{
        closest.x - separation.x * inverseLength * radius,
        closest.y - separation.y * inverseLength * radius,
        closest.z - separation.z * inverseLength * radius,
    };
    // The edge hit publishes its scratch even when the slope rejects correction.
    PublishPartIntersection(part, partIndex, polygonIndex, closest);
    const float horizontalDistance = std::sqrt((closest.x - corrected.x) * (closest.x - corrected.x) + (closest.z - corrected.z) * (closest.z - corrected.z));
    const std::int16_t angle = static_cast<std::int16_t>(FixedAtan2(closest.y - corrected.y, horizontalDistance));
    const std::uint16_t magnitude = angle < 0 ? static_cast<std::uint16_t>(-static_cast<std::int32_t>(angle)) : static_cast<std::uint16_t>(angle);
    if (magnitude >= 910U)
        return false;
    point.x = corrected.x;
    point.z = corrected.z;
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ResolveHorizontalRadiusVoxel(const FieldDataPart &part, std::size_t partIndex, std::int16_t voxelIndex, Vector3 &point, float radius) {
    std::size_t listOffset = part.segmentOffsets[5];
    if (part.segmentOffsets[4] != 0) {
        if (voxelIndex < 0)
            return false;

        const auto *const offset = SpanAt<std::int32_t>(part.sourceData, listOffset + static_cast<std::size_t>(voxelIndex) * sizeof(std::int32_t));
        if (!offset || *offset <= 0)
            return false;

        listOffset = static_cast<std::size_t>(*offset);
    }

    bool hit = false;
    for (std::size_t cursor = listOffset;; cursor += sizeof(std::int16_t)) {
        const auto *const entry = SpanAt<std::int16_t>(part.sourceData, cursor);
        if (!entry || *entry == -1)
            return hit;

        if (*entry < 0)
            return hit;

        hit = ResolveHorizontalRadiusPolygon(part, partIndex, static_cast<std::uint16_t>(*entry), point, radius) || hit;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool ResolveHorizontalRadiusInPart(const FieldDataPart &part, std::size_t partIndex, Vector3 &point, float radius) {
    if (!part.data)
        return false;
    if (part.segmentOffsets[4] == 0) {
        return ResolveHorizontalRadiusVoxel(part, partIndex, 0, point, radius);
    }

    const int width = static_cast<int>(part.grid.width);
    const int height = static_cast<int>(part.grid.height);
    const int depth = static_cast<int>(part.grid.depth);
    if (width <= 0 || height <= 0 || depth <= 0)
        return false;

    const int y = static_cast<int>((point.y - part.grid.minimumY) * part.grid.reciprocalCellSize);
    if (y < 0 || y >= height)
        return false;

    const int xFirst = static_cast<int>(((point.x - radius) - part.grid.minimumX) * part.grid.reciprocalCellSize);
    const int xLast = static_cast<int>(((point.x + radius) - part.grid.minimumX) * part.grid.reciprocalCellSize);
    const int zFirst = static_cast<int>(((point.z - radius) - part.grid.minimumZ) * part.grid.reciprocalCellSize);
    const int zLast = static_cast<int>(((point.z + radius) - part.grid.minimumZ) * part.grid.reciprocalCellSize);
    const int zStride = width;
    const int yOffset = width * depth * y;
    bool hit = false;
    for (int z = zFirst; z <= zLast; ++z) {
        if (z < 0 || z >= depth)
            continue;

        for (int x = xFirst; x <= xLast; ++x) {
            if (x < 0 || x >= width)
                continue;
            const std::int16_t voxel = static_cast<std::int16_t>(x + z * zStride + static_cast<std::int16_t>(yOffset));
            if (voxel == -1)
                return hit;
            hit = ResolveHorizontalRadiusVoxel(part, partIndex, voxel, point, radius) || hit;
        }
    }
    return hit;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool DecodeCategory5Metadata(FieldRadiusMetadata &output) {
    constexpr std::uint32_t category = 5;
    if (!foundIntersection || lastRadiusPartIndex >= fieldParts.size()) {
        return false;
    }
    const FieldDataPart &part = fieldParts[lastRadiusPartIndex];
    if (lastIntersection.polygonIndex >= part.polygons.size())
        return false;
    const FieldDataPolygon &polygon = part.polygons[lastIntersection.polygonIndex];
    if ((polygon.flags & (1U << category)) == 0 || polygon.dataGroup == 0xffffU || part.segmentOffsets[7] == 0) {
        return false;
    }

    const std::size_t tableOffset = part.segmentOffsets[7];
    const std::size_t dataGroup = polygon.dataGroup;
    if (dataGroup > (std::numeric_limits<std::size_t>::max() - tableOffset) / sizeof(std::int32_t))
        return false;

    const auto *const recordOffset = SpanAt<std::int32_t>(part.sourceData, tableOffset + dataGroup * sizeof(std::int32_t));
    if (!recordOffset || *recordOffset <= 0)
        return false;

    std::size_t cursor = static_cast<std::size_t>(*recordOffset);
    for (;;) {
        const auto *const value = SpanAt<std::int32_t>(part.sourceData, cursor);
        if (!value || *value == -255)
            return false;

        cursor += sizeof(std::int32_t);
        if (*value != ~static_cast<std::int32_t>(category))
            continue;

        const auto *const words = SpanAt<std::uint32_t>(part.sourceData, cursor, 6);
        if (!words)
            return false;

        FieldRadiusMetadata decoded{};
        decoded.stepCount = words[0];
        std::memcpy(&decoded.anchor, part.sourceData.data() + cursor + 8, sizeof(decoded.anchor));
        if (decoded.stepCount) {
            const Vector3 &edge0 = part.vertices[polygon.vertexIndices[0]];
            const Vector3 &edge1 = part.vertices[polygon.vertexIndices[1]];
            const Vector3 &edge2 = part.vertices[polygon.vertexIndices[2]];
            decoded.maximumEdgeLength = std::sqrt(std::fmax(PlanarSquaredDistance(edge1, edge2), std::fmax(PlanarSquaredDistance(edge0, edge1), 0.0f)));
        }
        float yawDegrees{};
        std::memcpy(&yawDegrees, part.sourceData.data() + cursor + 20, sizeof(yawDegrees));
        decoded.wallYaw = static_cast<std::uint16_t>(static_cast<std::int32_t>((yawDegrees * 8192.0f) / 45.0f));
        if (part.transformed) {
            decoded.anchor = TransformPoint(part.localToWorld, decoded.anchor);
            decoded.wallYaw = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(decoded.wallYaw) + static_cast<std::uint32_t>(part.yaw));
        }
        output = decoded;
        return true;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

const std::int32_t *FindCategoryRecord(std::uint32_t category, std::size_t valueCount) {
    if (!foundIntersection || lastRadiusPartIndex >= fieldParts.size()) {
        return nullptr;
    }
    const FieldDataPart &part = fieldParts[lastRadiusPartIndex];
    if (lastIntersection.polygonIndex >= part.polygons.size())
        return nullptr;

    const FieldDataPolygon &polygon = part.polygons[lastIntersection.polygonIndex];
    if ((polygon.flags & (1U << category)) == 0 || polygon.dataGroup == 0xffffU || part.segmentOffsets[7] == 0)
        return nullptr;

    const std::size_t tableOffset = part.segmentOffsets[7];
    const std::size_t dataGroup = polygon.dataGroup;
    if (dataGroup > (std::numeric_limits<std::size_t>::max() - tableOffset) / sizeof(std::int32_t))
        return nullptr;

    const auto *const recordOffset = SpanAt<std::int32_t>(part.sourceData, tableOffset + dataGroup * sizeof(std::int32_t));
    if (!recordOffset || *recordOffset <= 0)
        return nullptr;

    std::size_t cursor = static_cast<std::size_t>(*recordOffset);
    for (;;) {
        const auto *const value = SpanAt<std::int32_t>(part.sourceData, cursor);
        if (!value || *value == -255)
            return nullptr;

        cursor += sizeof(std::int32_t);
        if (*value != ~static_cast<std::int32_t>(category))
            continue;
        return SpanAt<std::int32_t>(part.sourceData, cursor, valueCount);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddDecodeCategoryScalar(std::uint32_t category, std::int32_t &output) {
    const std::int32_t *const scalar = FindCategoryRecord(category, 1);
    if (!scalar)
        return false;
    output = *scalar;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void TransformIntersectionNormal(Vector3 &normal) {
    if (!foundIntersection)
        return;
    normal = lastIntersection.planeNormal;
    if (lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].transformed)
        normal = TransformVector(fieldParts[lastRadiusPartIndex].localToWorld, normal);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ProjectPointToFieldPlane(Vector3 *output, const Vector3 *position);

std::int32_t FindFieldGridCell(const Vector3 *position);

std::int32_t GatherFieldSegmentGridCells(const Vector3 *start, const Vector3 *end, std::int16_t *cells);

bool IsPointAboveFieldPlane(const Vector3 *position);

bool FieldPolygonContainsPoint(const Vector3 *position);

bool ResolveFieldPolygonSphereOverlap(Vector3 *output, const Vector3 *position, float radius);

void OffsetPointFromFieldPlane(Vector3 *output, const Vector3 *position, float amount);

float IntersectFieldPlane(Vector3 *output, const Vector3 *start, const Vector3 *end);

static std::uint32_t ResolveFieldPartSphereVolume(Vector3 *output, const Vector3 *position, float radius);

static std::uint32_t TestFieldPartSphereRegion(Vector3 *output, const Vector3 *position, float radius);

static std::int32_t FindFieldPartSegmentOffsetIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin);

static std::int32_t GatherFieldSphereRegionCells(const Vector3 *position, float radius, std::int16_t *cells);

void IntersectFieldPlaneAlongNormal(Vector3 *output, const Vector3 *position, const Vector3 *direction);

float SignedDistanceToFieldPlane(const Vector3 *position);

std::int32_t GetFieldPolygonFlags(std::int16_t index);

void SelectFieldPolygon(std::int16_t index);

void StoreFieldIntersectionPlane(std::int16_t index);

void StoreFieldIntersectionPosition(const Vector3 *position);

FieldDataRuntimePart *FindFieldDataPart(shen_name name);

float FindFieldIntersectionHeight(const Vector3 *position);

std::uint32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 *position, float radius);

const std::uint32_t *FindFieldCategoryRecord(std::uint32_t category);

bool InitializeGeneratedFieldData();

FieldDataRoot *g_generatedFieldData = 0;

std::int32_t g_generatedFieldVolumeCount = 0;

FieldDataRuntimePart g_fieldParts[8];

std::int32_t g_fieldPartCount;

FieldDataRuntimePart *g_currentFieldPart;

FieldDataRoot *g_currentFieldData = 0;

std::uint16_t g_fieldDataFlags;

std::uint16_t g_fieldDataClearFlags;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint16_t g_fieldDataResetFlags;
#endif

FieldPlane g_currentFieldPlane;

FieldIntersectionState g_fieldIntersection;

std::uint16_t &g_fieldIntersectionFound = g_fieldIntersection.found;

std::int16_t g_fieldGridCells[128];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_fieldRayUsesGrid;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const Vector3 generatedBoxVertices[8] = {
    {-0.5f, 0.0f, -0.5f},
    {-0.5f, 0.0f, 0.5f},
    {-0.5f, 1.0f, 0.5f},
    {-0.5f, 1.0f, -0.5f},
    {0.5f, 0.0f, 0.5f},
    {0.5f, 1.0f, 0.5f},
    {0.5f, 0.0f, -0.5f},
    {0.5f, 1.0f, -0.5f},
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const Vector3 generatedBoxNormals[6] = {
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const FieldDataPolygon generatedBoxPolygons[6] = {
    {1, 65535, 0, 0, {0, 1, 2, 3}},
    {1, 65535, 1, 1, {1, 4, 5, 2}},
    {1, 65535, 2, 2, {4, 6, 7, 5}},
    {1, 65535, 3, 3, {6, 0, 3, 7}},
    {8, 65535, 4, 4, {6, 4, 1, 0}},
    {8, 65535, 5, 5, {3, 2, 5, 7}},
};
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawFixedShadowMesh(std::uint32_t index, float alpha) {
    void *const mesh = GetFixedShadowMesh(index);
    if (!mesh)
        return;
    if (::fabsf(alpha - 1.0f) < FLT_EPSILON)
        dc::sdk::DrawSingleLightMT7Mesh(mesh);
    else if (!(::fabsf(alpha) < FLT_EPSILON))
        dc::sdk::DrawSingleLightMT7MeshAlpha(mesh, alpha);
}
#endif

// @verified - d
void FieldDebugNullsub() {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float SignedDistanceToFieldPlane(const Vector3 *position) {
    const float *normal = &g_currentFieldPlane.normal.x;
    const float x = *normal++ * position->x;
    const float y = *normal++ * position->y;
    const float z = *normal++ * position->z;
    return x + y + z - *normal;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool CheckSegmentCylinder(const Vector3 *start, const Vector3 *end, const Vector3 *center, float radius, float height) {
    radius *= radius;
    const float bottom = center->y;
    const float top = bottom + height;
    float distance = start->x - center->x;
    distance = distance * distance + (start->z - center->z) * (start->z - center->z);
    if (!(distance > radius) && !(bottom > start->y) && !(start->y > top))
        return true;
    Vector3 delta;
    am2Vector3Subtract(&delta, end, start);
    if (std::fabs(delta.x) < 0.00000011920929f && std::fabs(delta.z) < 0.00000011920929f)
        return false;
    distance = end->x - center->x;
    do {
        distance = distance * distance + (end->z - center->z) * (end->z - center->z);
        if (!(distance > radius) && !(bottom > end->y) && !(end->y > top))
            return true;
        float amount = (center->x - start->x) * delta.x + delta.z * (center->z - start->z);
        amount /= delta.x * delta.x + delta.z * delta.z;
        if (0.0f > amount || amount > 1.0f)
            return false;
        const float x = start->x + amount * delta.x;
        const float y = start->y + amount * delta.y;
        const float z = amount * delta.z + start->z;
        distance = x - center->x;
        distance = distance * distance + (z - center->z) * (z - center->z);
        if (distance > radius || bottom > y || y > top)
            return false;
    } while (0);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static std::int32_t ClipSegmentToCylinderLocal(const Vector3 *start, const Vector3 *end, const Vector3 *base, float height, float radius, Vector3 *output) {
    Vector3 first;
    Vector3 last;
    Vector3 delta;
    am2Vector3Subtract(&first, start, base);
    am2Vector3Subtract(&last, end, base);
    am2Vector3Subtract(&delta, &last, &first);
    const float a = delta.x * delta.x + delta.z * delta.z;
    const float b = (first.x * delta.x + first.z * delta.z) * 2.0f;
    const float c = first.x * first.x + first.z * first.z - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    if (0.00000011920929f > std::fabs(a) || 0.0f > discriminant)
        return 0;
    discriminant = std::sqrt(discriminant);
    float near = (-b + discriminant) / (a + a), far = (-b - discriminant) / (a + a);
    if ((0.0f > near || near > 1.0f) && (0.0f > far || far > 1.0f))
        return 0;
    near = (near < 0.0f ? 0.0f : near) < 1.0f ? (near < 0.0f ? 0.0f : near) : 1.0f;
    far = (far < 0.0f ? 0.0f : far) < 1.0f ? (far < 0.0f ? 0.0f : far) : 1.0f;
    const float nearY = first.y + near * delta.y, farY = first.y + far * delta.y;
    if ((0.0f > nearY && 0.0f > farY) || (nearY > height && farY > height))
        return 0;
    float bottom = -FLT_MAX, top = FLT_MAX;
    if (!(0.00000011920929f > std::fabs(delta.y))) {
        bottom = (0.0f - first.y) / delta.y;
        top = (height - first.y) / delta.y;
        if (bottom > top) {
            const float swap = bottom;
            bottom = top;
            top = swap;
        }
    }
    near = (near < bottom ? bottom : near) < top ? (near < bottom ? bottom : near) : top;
    far = (far < bottom ? bottom : far) < top ? (far < bottom ? bottom : far) : top;
    const float amount = near < far ? near : far;
    if (output) {
        output->x = first.x + amount * delta.x + base->x;
        output->y = first.y + amount * delta.y + base->y;
        output->z = first.z + amount * delta.z + base->z;
    }
    return 1;
}

// @verified - d
std::int32_t TestPointInOrientedCylinder(const Vector3 *start, const Vector3 *end, const Vector3 *base, float radius, const Vector3 *top, Vector3 *output) {
    Vector3 local[2];
    Vector3 axis;
    Vector3 up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    const Vector3 origin = {0.0f, 0.0f, 0.0f};
    am2Vector3Subtract(&axis, top, base);
    const float height = am2Vector3NormalizeInPlace(&axis);
    matrix_stack::PushUnitMatrix();
    Matrix4x4_BuildRotationMatrix(axis, up);
    matrix_stack::TranslateInverseVector(base);
    Matrix4x4::TransformPoint(*start, local[0]);
    Matrix4x4::TransformPoint(*end, local[1]);
    PopCurrentMatrix(1);
    const std::int32_t result = ClipSegmentToCylinderLocal(&local[0], &local[1], &origin, height, radius, output);
    if (result && output) {
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(base);
        Matrix4x4_BuildRotationMatrix(up, axis);
        Matrix4x4::TransformPoint(*output, *output);
        PopCurrentMatrix(1);
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindFieldPartEndpointIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    std::int16_t *cell;
    std::int32_t found;
    std::int32_t index;
    std::int16_t flags, required;
    float nearest, distance;
    Vector3 intersection, result;
    Vector3 *const point = &intersection;
    Vector3 *const offset = &result;
    const std::int16_t *polygon;
    if (!g_currentFieldData) {
        *output = *destination;
        return -1;
    }
    flags = static_cast<std::int16_t>(g_fieldDataFlags);
    required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    *offset = *destination;
    nearest = FLT_MAX;
    found = 0;
    cell = g_fieldGridCells;
    cell[0] = FindFieldGridCell(origin);
    cell[1] = FindFieldGridCell(destination);
    for (index = 0; index < 2; ++index, ++cell) {
        if (*cell == -1)
            continue;
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (IsPointAboveFieldPlane(destination) || !IsPointAboveFieldPlane(origin))
                continue;
            IntersectFieldPlane(point, origin, destination);
            if (!FieldPolygonContainsPoint(point))
                continue;
            {
                const float x = point->x - origin->x, y = point->y - origin->y, z = point->z - origin->z;
                distance = x * x + y * y + z * z;
            }
            if (!(nearest > distance))
                continue;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(point);
            OffsetPointFromFieldPlane(offset, point, 0.001f);
            nearest = distance;
            found = 1;
        }
        if (cell[0] == cell[1])
            break;
    }
    *output = *offset;
    return found;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t FindFieldPartSegmentOffsetIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    std::int16_t *cell;
    std::int32_t found;
    std::int32_t index, count;
    std::int16_t flags, required;
    float nearest, distance;
    Vector3 intersection, result;
    Vector3 *const point = &intersection;
    Vector3 *const offset = &result;
    const std::int16_t *polygon;
    if (!g_currentFieldData) {
        *output = *destination;
        return -1;
    }
    flags = static_cast<std::int16_t>(g_fieldDataFlags);
    required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    *offset = *destination;
    nearest = FLT_MAX;
    found = 0;
    count = GatherFieldSegmentGridCells(origin, destination, g_fieldGridCells);
    cell = g_fieldGridCells;
    for (index = 0; index < count; ++index, ++cell) {
        if (*cell == -1)
            continue;
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (IsPointAboveFieldPlane(destination) || !IsPointAboveFieldPlane(origin))
                continue;
            IntersectFieldPlane(point, origin, destination);
            if (!FieldPolygonContainsPoint(point))
                continue;
            {
                const float x = point->x - origin->x, y = point->y - origin->y, z = point->z - origin->z;
                distance = x * x + y * y + z * z;
            }
            if (!(nearest > distance))
                continue;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(point);
            OffsetPointFromFieldPlane(offset, point, 0.001f);
            nearest = distance;
            found = 1;
        }
        if (cell[0] == cell[1])
            break;
    }
    *output = *offset;
    return found;
}
#else
static std::int32_t FindFieldPartSegmentOffsetIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    if (!g_currentFieldData) {
        *output = *destination;
        return -1;
    }
    const std::uint16_t flags = g_fieldDataFlags, required = g_fieldDataClearFlags;
    Vector3 result = *destination;
    float nearest = FLT_MAX;
    std::int32_t found = 0;
    const std::int32_t count = GatherFieldSegmentGridCells(origin, destination, g_fieldGridCells);
    const std::int16_t *cell = g_fieldGridCells;
    for (std::int32_t index = 0; index < count; ++index, ++cell) {
        if (*cell == -1)
            continue;
        for (const std::int16_t *polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != static_cast<std::int16_t>(required))
                continue;
            SelectFieldPolygon(*polygon);
            if (IsPointAboveFieldPlane(destination) || !IsPointAboveFieldPlane(origin))
                continue;
            Vector3 intersection;
            IntersectFieldPlane(&intersection, origin, destination);
            if (!FieldPolygonContainsPoint(&intersection))
                continue;
            const float x = intersection.x - origin->x, y = intersection.y - origin->y, z = intersection.z - origin->z;
            const float distance = y * y + x * x + z * z;
            if (!(nearest > distance))
                continue;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(&intersection);
            OffsetPointFromFieldPlane(&result, &intersection, 0.001f);
            nearest = distance;
            found = 1;
        }
        if (cell[0] == cell[1])
            break;
    }
    *output = result;
    return found;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t TestFieldPartSegment(const Vector3 *start, const Vector3 *end) {
    std::int16_t *cell;
    std::int32_t index, count;
    std::int16_t flags, required;
    Vector3 intersection;
    Vector3 *const point = &intersection;
    const std::int16_t *polygon;
    if (!g_currentFieldData)
        return 0;
    if (0) {
    found:
        StoreFieldIntersectionPlane(*polygon);
        StoreFieldIntersectionPosition(point);
        return 1;
    }
    flags = static_cast<std::int16_t>(g_fieldDataFlags);
    required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    cell = g_fieldGridCells;
    count = GatherFieldSegmentGridCells(start, end, cell);
    for (index = 0; index < count; ++index, ++cell) {
        if (*cell == -1)
            continue;
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            const bool endAbove = IsPointAboveFieldPlane(end);
            if (endAbove == IsPointAboveFieldPlane(start))
                continue;
            IntersectFieldPlane(point, start, end);
            if (FieldPolygonContainsPoint(point))
                goto found;
        }
        if (cell[0] == cell[1])
            break;
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t IsPointWithinSphere(const Vector3 *point, const Vector3 *centre, float radius) {
    float distance = point->x - centre->x;
    distance = distance * distance + (point->y - centre->y) * (point->y - centre->y);
    distance += (point->z - centre->z) * (point->z - centre->z);
    if (distance > radius * radius)
        return 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t TestFieldLineSphere(const Vector3 *origin, const Vector3 *end, const Vector3 *centre, float radius) {
    const float radiusSquared = radius * radius;
    float distance = origin->x - centre->x;
    distance = distance * distance + (origin->y - centre->y) * (origin->y - centre->y);
    distance += (origin->z - centre->z) * (origin->z - centre->z);
    if (!(distance > radiusSquared))
        return 1;
    Vector3 direction;
    am2Vector3Subtract(&direction, end, origin);
    if (__builtin_fabsf(direction.x) < 1.1920928955078125e-07f && __builtin_fabsf(direction.z) < 1.1920928955078125e-07f)
        return 0;
    const float amount = ((centre->x - origin->x) * direction.x + (centre->y - origin->y) * direction.y + (centre->z - origin->z) * direction.z) /
        (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    const float x = origin->x + amount * direction.x;
    const float y = origin->y + amount * direction.y;
    const float z = origin->z + amount * direction.z;
    distance = x - centre->x;
    distance = distance * distance + (y - centre->y) * (y - centre->y);
    distance += (z - centre->z) * (z - centre->z);
    if (distance > radiusSquared)
        return 0;
    return 1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t TestSegmentWithinSphere(const Vector3 *start, const Vector3 *end, const Vector3 *center, float radius) {
    const float squared = radius * radius;
    float distance = start->x - center->x;
    distance = distance * distance + (start->y - center->y) * (start->y - center->y);
    distance += (start->z - center->z) * (start->z - center->z);
    if (!(distance > squared))
        return 1;
    Vector3 delta;
    am2Vector3Subtract(&delta, end, start);
    if (std::fabs(delta.x) < 0.00000011920929f && std::fabs(delta.z) < 0.00000011920929f)
        return 0;
    distance = end->x - center->x;
    distance = distance * distance + (end->y - center->y) * (end->y - center->y);
    distance += (end->z - center->z) * (end->z - center->z);
    if (!(distance > squared))
        return 1;
    float amount = delta.x * (center->x - start->x) + delta.y * (center->y - start->y) + delta.z * (center->z - start->z);
    amount /= delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    if (0.0f > amount || amount > 1.0f)
        return 0;
    const float x = start->x + amount * delta.x;
    const float y = start->y + amount * delta.y;
    const float z = start->z + amount * delta.z;
    distance = x - center->x;
    distance = distance * distance + (y - center->y) * (y - center->y);
    distance += (z - center->z) * (z - center->z);
    if (distance > squared)
        return 0;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t TestFieldPartSphereIntersection(const Vector3 *position, float radius) {
    Vector3 projected;
    const std::int16_t *polygon;
    if (!g_currentFieldData)
        return 0;
    if (0) {
    found:
        StoreFieldIntersectionPlane(*polygon);
        StoreFieldIntersectionPosition(&projected);
        return 1;
    }
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    std::int16_t *cell = g_fieldGridCells;
    GatherFieldSphereVolumeCells(position, radius, cell);
    for (; *cell != -1; ++cell) {
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            const float distance = SignedDistanceToFieldPlane(position);
            if (distance < 0.0f || !(radius > distance))
                continue;
            ProjectPointToFieldPlane(&projected, position);
            if (FieldPolygonContainsPoint(&projected) || ResolveFieldPolygonSphereOverlap(&projected, position, radius))
                goto found;
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ProjectPointToFieldPlane(Vector3 *output, const Vector3 *position) {
    const float nx = g_currentFieldPlane.normal.x;
    const float ny = g_currentFieldPlane.normal.y;
    const float nz = g_currentFieldPlane.normal.z;
    const float x = nx * position->x;
    const float y = ny * position->y;
    const float z = nz * position->z;
    const float amount = g_currentFieldPlane.distance - (x + y + z);
    output->x = nx * amount + position->x;
    output->y = ny * amount + position->y;
    output->z = nz * amount + position->z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindFieldGridCell(const Vector3 *position) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid)
        return 0;
    const float reciprocal = grid->reciprocalCellSize;
    const std::int32_t width = static_cast<std::int32_t>(grid->width), height = static_cast<std::int32_t>(grid->height),
                       depth = static_cast<std::int32_t>(grid->depth);
    const std::int32_t x = static_cast<std::int32_t>((position->x - grid->minimum.x) * reciprocal);
    const std::int32_t y = static_cast<std::int32_t>((position->y - grid->minimum.y) * reciprocal);
    const std::int32_t z = static_cast<std::int32_t>((position->z - grid->minimum.z) * reciprocal);
    if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
        return -1;
    return x + z * width + y * width * depth;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GatherFieldSegmentGridCells(const Vector3 *start, const Vector3 *end, std::int16_t *cells) {
    std::int32_t count, stepX, stepY, stepZ, nextX, nextY, nextZ, previous, crossX, crossY, crossZ;
    std::int32_t spanX, spanY, spanZ;
    std::int32_t cellX, cellY, cellZ, lastX, lastY, lastZ, width, height, depth, budget, remaining, index;
    float deltaX, deltaY, deltaZ, reciprocalX, reciprocalY, reciprocalZ, scale, minimumX, minimumY, minimumZ, cellSize;
    float originX, originY, originZ, timeX, timeY, timeZ;
    const FieldDataGridConfig *grid = g_currentFieldData->grid;
    if (!grid) {
        *cells++ = 0;
        *cells = -1;
        return 1;
    }
    deltaX = end->x - start->x;
    deltaY = end->y - start->y;
    deltaZ = end->z - start->z;
    reciprocalX = reciprocalY = reciprocalZ = 100000.0f;
    if (!(__builtin_fabsf(deltaX) < FLT_EPSILON))
        reciprocalX = 1.0f / deltaX;
    if (!(__builtin_fabsf(deltaY) < FLT_EPSILON))
        reciprocalY = 1.0f / deltaY;
    if (!(__builtin_fabsf(deltaZ) < FLT_EPSILON))
        reciprocalZ = 1.0f / deltaZ;
    scale = grid->reciprocalCellSize;
    minimumX = grid->minimum.x;
    minimumY = grid->minimum.y;
    minimumZ = grid->minimum.z;
    width = static_cast<std::int32_t>(grid->width);
    height = static_cast<std::int32_t>(grid->height);
    depth = static_cast<std::int32_t>(grid->depth);
    cellSize = 1.0f / scale;
    cellX = static_cast<std::int32_t>((start->x - minimumX) * scale);
    cellY = static_cast<std::int32_t>((start->y - minimumY) * scale);
    cellZ = static_cast<std::int32_t>((start->z - minimumZ) * scale);
    lastX = static_cast<std::int32_t>((end->x - minimumX) * scale);
    lastY = static_cast<std::int32_t>((end->y - minimumY) * scale);
    lastZ = static_cast<std::int32_t>((end->z - minimumZ) * scale);
    if ((cellX < 0 && lastX < 0) || (cellX >= width && lastX >= width) || (cellY < 0 && lastY < 0) || (cellY >= height && lastY >= height) ||
        (cellZ < 0 && lastZ < 0) || (cellZ >= depth && lastZ >= depth)) {
        *cells = -1;
        return 0;
    }
    spanX = lastX - cellX;
    spanY = lastY - cellY;
    spanZ = lastZ - cellZ;
    stepX = stepY = stepZ = 0;
    if (spanX > 0)
        stepX = 1;
    else if (spanX < 0)
        stepX = -1;
    if (spanY > 0)
        stepY = 1;
    else if (spanY < 0)
        stepY = -1;
    if (spanZ > 0)
        stepZ = 1;
    else if (spanZ < 0)
        stepZ = -1;
    originX = start->x;
    originY = start->y;
    originZ = start->z;
    timeX = timeY = timeZ = FLT_MAX;
    count = 0;
    previous = -1;
    crossX = 0;
    crossY = 0;
    crossZ = 0;
    remaining = (__builtin_abs(spanX) + 1) * (__builtin_abs(spanY) + 1) * (__builtin_abs(spanZ) + 1);
    budget = width * height * depth * 2;
    if (budget > remaining)
        budget = remaining;
    remaining = budget + 8;
    nextX = nextY = nextZ = 0;
    while (remaining-- > 0) {
        if (cellX >= 0 && cellX < width && cellY >= 0 && cellY < height && cellZ >= 0 && cellZ < depth) {
            index = cellX + width * cellZ + width * cellY * depth;
            if (index != previous) {
                *cells++ = index;
                previous = index;
                ++count;
            }
        }
        if (cellX == lastX && cellY == lastY && cellZ == lastZ)
            break;
        if ((cellX < 0 && lastX < 0) || (cellX >= width && lastX >= width) || (cellY < 0 && lastY < 0) || (cellY >= height && lastY >= height) ||
            (cellZ < 0 && lastZ < 0) || (cellZ >= depth && lastZ >= depth))
            break;
        if (stepX)
            timeX = ((cellX + nextX) * cellSize + minimumX - originX) * reciprocalX;
        if (stepY)
            timeY = ((cellY + nextY) * cellSize + minimumY - originY) * reciprocalY;
        if (stepZ)
            timeZ = ((cellZ + nextZ) * cellSize + minimumZ - originZ) * reciprocalZ;
        if (timeY > timeX) {
            if (timeZ > timeX)
                crossX = 1;
            else
                crossZ = 1;
        } else if (timeZ > timeY)
            crossY = 1;
        else
            crossZ = 1;
        if (crossX) {
            crossX = 0;
            if (timeX < 0.0f)
                nextX += stepX;
            else {
                cellX += stepX;
                nextX = stepX;
            }
        }
        if (crossY) {
            crossY = 0;
            if (timeY < 0.0f)
                nextY += stepY;
            else {
                cellY += stepY;
                nextY = stepY;
            }
        }
        if (crossZ) {
            crossZ = 0;
            if (timeZ < 0.0f)
                nextZ += stepZ;
            else {
                cellZ += stepZ;
                nextZ = stepZ;
            }
        }
    }
    *cells = -1;
    return count;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool IsPointAboveFieldPlane(const Vector3 *position) {
    const float *normal = &g_currentFieldPlane.normal.x;
    const float x = *normal++ * position->x;
    const float y = *normal++ * position->y;
    const float z = *normal++ * position->z;
    const float distance = x + y + z - *normal;
    if (distance >= 0.0f)
        return true;
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool FieldPolygonContainsPoint(const Vector3 *position) {
    Vector3 edgeStorage;
    Vector3 offsetStorage;
    Vector3 *const edge = &edgeStorage;
    Vector3 *const offset = &offsetStorage;
    const FieldPlane *plane = &g_currentFieldPlane;
    const Vector3 *const vertices = g_currentFieldData->vertices;
    const std::int16_t *const indices = plane->vertices;
    std::int32_t count;
    const float normalX = plane->normal.x, normalY = plane->normal.y, normalZ = plane->normal.z;
    const float pointX = position->x, pointY = position->y, pointZ = position->z;
    if (indices[0] == indices[3])
        count = 3;
    else
        count = 4;
    for (std::int32_t index = 0; index < count; ++index) {
        const Vector3 *const first = &vertices[indices[index]];
        const Vector3 *const second = &vertices[indices[(index + 1) & 3]];
        const float x = first->x, y = first->y, z = first->z;
        edge->x = second->x - x;
        edge->y = second->y - y;
        edge->z = second->z - z;
        offset->x = pointX - x;
        offset->y = pointY - y;
        offset->z = pointZ - z;
        am2Vector3Cross(edge, offset, edge);
        const float side = normalX * edge->x + normalY * edge->y + normalZ * edge->z;
        if (side < kFieldPolygonTolerance)
            return false;
    }
    return true;
}
#else
// @verified - d
bool FieldPolygonContainsPoint(const Vector3 *position) {
    Vector3 edgeStorage;
    Vector3 offsetStorage;
    Vector3 *const edge = &edgeStorage;
    Vector3 *const offset = &offsetStorage;
    const Vector3 *const vertices = g_currentFieldData->vertices;
    const std::int16_t *const indices = g_currentFieldPlane.vertices;
    std::int32_t count;
    const float normalX = g_currentFieldPlane.normal.x, normalY = g_currentFieldPlane.normal.y, normalZ = g_currentFieldPlane.normal.z;
    const float pointX = position->x, pointY = position->y, pointZ = position->z;
    if (indices[0] == indices[3])
        count = 3;
    else
        count = 4;
    for (std::int32_t index = 0; index < count; ++index) {
        const Vector3 *const first = &vertices[indices[index]];
        const Vector3 *const second = &vertices[indices[(index + 1) & 3]];
        const float x = first->x, y = first->y, z = first->z;
        edge->x = second->x - x;
        edge->y = second->y - y;
        edge->z = second->z - z;
        offset->x = pointX - x;
        offset->y = pointY - y;
        offset->z = pointZ - z;
        am2Vector3Cross(edge, offset, edge);
        const float side = normalX * edge->x + normalY * edge->y + normalZ * edge->z;
        if (side < kFieldPolygonTolerance)
            return false;
    }
    return true;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool ResolveFieldPolygonSphereOverlap(Vector3 *output, const Vector3 *position, float radius) {
    const Vector3 *const vertices = g_currentFieldData->vertices;
    const std::int16_t *const indices = g_currentFieldPlane.vertices;
    std::int32_t count = 4;
    if (indices[0] == indices[3])
        count = 3;
    const float radiusSquared = radius * radius;
    for (std::int32_t index = 0; index < count; ++index) {
        const Vector3 *const first = &vertices[indices[index]];
        const Vector3 *const second = &vertices[indices[(index + 1) & 3]];
        float x = first->x, y = first->y, z = first->z;
        float separationX, separationY, separationZ;
        float edgeX = second->x - x, edgeY = second->y - y, edgeZ = second->z - z;
        separationX = x - position->x;
        separationY = y - position->y;
        separationZ = z - position->z;
        const float projection = edgeX * separationX + edgeY * separationY + edgeZ * separationZ;
        const float amount = -projection / (edgeX * edgeX + edgeY * edgeY + edgeZ * edgeZ);
        if (0.0f > amount || amount > 1.0f)
            continue;
        const float offsetX = edgeX * amount;
        separationX += offsetX;
        const float offsetY = edgeY * amount;
        separationY += offsetY;
        const float offsetZ = edgeZ * amount;
        separationZ += offsetZ;
        if (!(radiusSquared > separationX * separationX + separationY * separationY + separationZ * separationZ))
            continue;
        x += offsetX;
        y += offsetY;
        z += offsetZ;
        Vector3 direction;
        direction.x = separationX;
        direction.y = separationY;
        direction.z = separationZ;
        am2Vector3NormalizeInPlace(&direction);
        output->x = x - direction.x * radius;
        output->y = y - direction.y * radius;
        output->z = z - direction.z * radius;
        return true;
    }
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void OffsetPointFromFieldPlane(Vector3 *output, const Vector3 *position, float amount) {
    const float *normal = &g_currentFieldPlane.normal.x;
    output->x = amount * *normal++ + position->x;
    output->y = amount * *normal++ + position->y;
    output->z = amount * *normal + position->z;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void IntersectFieldPlaneAlongNormal(Vector3 *output, const Vector3 *position, const Vector3 *direction) {
    const float x = g_currentFieldPlane.normal.x * position->x;
    const float y = g_currentFieldPlane.normal.y * position->y;
    const float z = g_currentFieldPlane.normal.z * position->z;
    const float numerator = g_currentFieldPlane.distance - (x + y + z);
    const float dx = g_currentFieldPlane.normal.x * direction->x;
    const float dy = direction->y * g_currentFieldPlane.normal.y;
    const float dz = g_currentFieldPlane.normal.z * direction->z;
    const float amount = numerator / (dx + dy + dz);
    output->x = amount * direction->x + position->x;
    output->y = amount * direction->y + position->y;
    output->z = amount * direction->z + position->z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float IntersectFieldPlane(Vector3 *output, const Vector3 *start, const Vector3 *end) {
    const float nx = g_currentFieldPlane.normal.x;
    const float ny = g_currentFieldPlane.normal.y;
    const float nz = g_currentFieldPlane.normal.z;
    Vector3 delta;
    delta.x = end->x - start->x;
    delta.y = end->y - start->y;
    delta.z = end->z - start->z;
    const float x = nx * start->x;
    const float y = ny * start->y;
    const float z = nz * start->z;
    float amount = g_currentFieldPlane.distance - (x + y + z);
    const float dx = nx * delta.x;
    const float dy = ny * delta.y;
    const float dz = nz * delta.z;
    amount /= dx + dy + dz;
    output->x = amount * delta.x + start->x;
    output->y = amount * delta.y + start->y;
    output->z = amount * delta.z + start->z;
    return amount;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetFieldPolygonFlags(std::int16_t index) {
    return static_cast<std::int16_t>(g_currentFieldData->polygons[index].flags);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SelectFieldPolygon(std::int16_t index) {
    float *target = &g_currentFieldPlane.normal.x;
    FieldDataRoot *data = g_currentFieldData;
    const FieldDataPolygon *polygon = data->polygons;
    const Vector3 *normals = data->normals;
    const float *distances = data->planeDistances;
    polygon += index;
    const float *normal = reinterpret_cast<const float *>(
        reinterpret_cast<const std::byte *>(normals) + static_cast<std::int16_t>(polygon->normalIndex) * static_cast<std::int16_t>(sizeof(Vector3)));
    const std::int32_t planeOffset = static_cast<std::int16_t>(polygon->planeIndex) * 4;
    *target++ = *normal++;
    *target++ = *normal++;
    *target++ = *normal;
    *target = *reinterpret_cast<const float *>(reinterpret_cast<const std::byte *>(distances) + planeOffset);
    *reinterpret_cast<const std::int16_t **>(target + 1) = polygon->vertexIndices;
}
#else
// @verified - d
void SelectFieldPolygon(std::int16_t index) {
    const FieldDataPolygon &polygon = g_currentFieldData->polygons[index];
    g_currentFieldPlane.normal = g_currentFieldData->normals[polygon.normalIndex];
    g_currentFieldPlane.distance = g_currentFieldData->planeDistances[polygon.planeIndex];
    g_currentFieldPlane.vertices = polygon.vertexIndices;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetFieldDataFlags() {
    g_fieldDataClearFlags = 0;
    g_fieldDataFlags = 0x10;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void flddPushIntersectionFlag(std::uint16_t flags) {
    g_fieldDataClearFlags |= flags;
}

#else

// @verified - p
void flddPushIntersectionFlag(std::int16_t flags) {
    intersectionMask |= static_cast<std::uint16_t>(flags);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool RaycastForCollision(Vector3 *intersectionPoint, const Vector3 *destination, const Vector3 *origin) {
    if (!intersectionPoint || !destination || !origin) {
        ResetQueryFlags();
        return false;
    }
    *intersectionPoint = *destination;
    foundIntersection = false;
    bool hit = false;
    // Native RaycastForCollision visits each enabled registered field owner.
    // Each receives the preceding owner's corrected destination; the saved
    // intersection remains local while the corrected point returns to world.

    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;

        if (part.transformed) {
            const Vector3 end = TransformPoint(part.worldToLocal, *intersectionPoint);
            const Vector3 start = TransformPoint(part.worldToLocal, *origin);
            Vector3 corrected{};
            hit = FindClosestIntersectionInGrid(part, partIndex, corrected, end, start) || hit;
            *intersectionPoint = TransformPoint(part.localToWorld, corrected);
        } else {
            const Vector3 end = *intersectionPoint;
            hit = FindClosestIntersectionInGrid(part, partIndex, *intersectionPoint, end, *origin) || hit;
        }
    }
    ResetQueryFlags();
    return hit;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ClearFieldIntersection() {
    reinterpret_cast<std::uint8_t *>(&g_fieldIntersection.found)[0] = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void StoreFieldIntersectionPlane(std::int16_t index) {
    g_fieldIntersection.polygon = index;
    g_fieldIntersection.plane = g_currentFieldPlane;
    reinterpret_cast<std::uint8_t *>(&g_fieldIntersection.found)[0] = 1;
    g_fieldIntersection.part = g_currentFieldPart;
    g_fieldIntersection.flags = static_cast<std::int16_t>(GetFieldPolygonFlags(index));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void StoreFieldIntersectionPosition(const Vector3 *position) {
    FieldIntersectionState *state = &g_fieldIntersection;
    state->position = *position;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t GatherFieldSphereRegionCells(const Vector3 *position, float radius, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        *cells++ = 0;
        *cells = -1;
        return 1;
    }
    const float positionX = position->x, lowY = position->y - radius, positionZ = position->z, highY = radius + position->y;
    const float reciprocal = grid->reciprocalCellSize;
    const float minimumX = grid->minimum.x, minimumY = grid->minimum.y, minimumZ = grid->minimum.z;
    const std::int32_t width = static_cast<std::int32_t>(grid->width), height = static_cast<std::int32_t>(grid->height),
                       depth = static_cast<std::int32_t>(grid->depth);
    std::int32_t x = static_cast<std::int32_t>((positionX - minimumX) * reciprocal);
    const std::int32_t lowest = static_cast<std::int32_t>((lowY - minimumY) * reciprocal);
    std::int32_t z = static_cast<std::int32_t>((positionZ - minimumZ) * reciprocal);
    if (x < 0 || x >= width || z < 0 || z >= depth) {
        cells[0] = -1;
        return 0;
    }
    const std::int32_t highest = static_cast<std::int32_t>((highY - minimumY) * reciprocal);
    x += z * width;
    z = width * depth;
    std::int32_t count = 0;
    for (std::int32_t y = lowest; y <= highest; ++y) {
        if (y >= 0 && y < height) {
            *cells++ = static_cast<std::int16_t>(y * z + x);
            ++count;
        }
    }
    *cells = -1;
    return count;
}

#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GatherFieldGridCells(const Vector3 *position, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        *cells++ = 0;
        *cells = -1;
        return 1;
    }
    const float reciprocal = grid->reciprocalCellSize;
    const std::int32_t width = static_cast<std::int32_t>(grid->width), height = static_cast<std::int32_t>(grid->height),
                       depth = static_cast<std::int32_t>(grid->depth);
    const std::int32_t x = static_cast<std::int32_t>((position->x - grid->minimum.x) * reciprocal);
    std::int32_t y = static_cast<std::int32_t>((position->y - grid->minimum.y) * reciprocal);
    const std::int32_t z = static_cast<std::int32_t>((position->z - grid->minimum.z) * reciprocal);
    if (x < 0 || x >= width || y < 0 || z < 0 || z >= depth) {
        *cells = -1;
        return 0;
    }
    if (y >= height)
        y = height - 1;
    const std::int32_t plane = width * depth;
    std::int32_t cell = x + y * plane + z * width;
    std::int32_t count = 0;
    for (; cell >= 0; cell -= plane) {
        *cells++ = static_cast<std::int16_t>(cell);
        ++count;
    }
    *cells = -1;
    return count;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::uint32_t ResolveFieldPartSphereVolume(Vector3 *output, const Vector3 *position, float radius) {
    std::uint32_t result = 0;
    Vector3 projected, point;
    Vector3 *const proj = &projected;
    Vector3 *const pt = &point;
    std::int16_t *cell;
    const std::int16_t *polygon;
    if (!g_currentFieldData) {
        *pt = *position;
        return 0;
    }
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    *pt = *position;
    cell = g_fieldGridCells;
    GatherFieldSphereVolumeCells(position, radius, cell);
    for (; *cell != -1; ++cell) {
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            const float distance = SignedDistanceToFieldPlane(pt);
            if (0.0f > distance || !(radius > distance))
                continue;
            ProjectPointToFieldPlane(proj, pt);
            if (FieldPolygonContainsPoint(proj)) {
                result = 1;
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(proj);
                OffsetPointFromFieldPlane(pt, proj, radius);
            } else if (ResolveFieldPolygonSphereOverlap(proj, pt, radius)) {
                result = 1;
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(pt);
                pt->x = proj->x;
                pt->y = proj->y;
                pt->z = proj->z;
            }
        }
    }
    *output = *pt;
    return result;
}
#else
static std::uint32_t ResolveFieldPartSphereVolume(Vector3 *output, const Vector3 *position, float radius) {
    std::uint32_t result = 0;
    Vector3 projected, point;
    if (!g_currentFieldData) {
        point = *position;
        return 0;
    }
    const std::uint16_t flags = g_fieldDataFlags, required = g_fieldDataClearFlags;
    point = *position;
    GatherFieldSphereVolumeCells(position, radius, g_fieldGridCells);
    for (const std::int16_t *cell = g_fieldGridCells; *cell != -1; ++cell) {
        for (const std::int16_t *polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != static_cast<std::int16_t>(required))
                continue;
            SelectFieldPolygon(*polygon);
            const float distance = SignedDistanceToFieldPlane(&point);
            if (0.0f > distance || !(radius > distance))
                continue;
            ProjectPointToFieldPlane(&projected, &point);
            if (FieldPolygonContainsPoint(&projected)) {
                result = 1;
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(&projected);
                OffsetPointFromFieldPlane(&point, &projected, radius);
            } else if (ResolveFieldPolygonSphereOverlap(&projected, &point, radius)) {
                result = 1;
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(&point);
                point.x = projected.x;
                point.y = projected.y;
                point.z = projected.z;
            }
        }
    }
    *output = point;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::uint32_t TestFieldPartSphereRegion(Vector3 *output, const Vector3 *position, float radius) {
    std::int16_t *cell;
    std::uint32_t result = 0;
    Vector3 intersection, point, direction;
    Vector3 *const hit = &intersection;
    Vector3 *const pt = &point;
    Vector3 *const dir = &direction;
    const std::int16_t *polygon;
    if (!g_currentFieldData) {
        *pt = *position;
        return 0;
    }
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    *pt = *position;
    cell = g_fieldGridCells;
    GatherFieldSphereRegionCells(position, radius, cell);
    for (; *cell != -1; ++cell) {
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(pt))
                continue;
            dir->x = 0.0f;
            dir->y = g_currentFieldPlane.normal.y;
            dir->z = 0.0f;
            IntersectFieldPlaneAlongNormal(hit, pt, dir);
            const float distance = pt->y - hit->y;
            if (!((0.0f > distance ? -distance : pt->y - hit->y) < radius))
                continue;
            if (!FieldPolygonContainsPoint(hit))
                continue;
            result = 1;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(hit);
            if (!(0.0f > dir->y))
                pt->y = hit->y + radius;
            else
                pt->y = hit->y - radius;
        }
    }
    *output = *pt;
    return result;
}
#else
static std::uint32_t TestFieldPartSphereRegion(Vector3 *output, const Vector3 *position, float radius) {
    std::uint32_t result = 0;
    Vector3 intersection, point, direction;
    if (!g_currentFieldData) {
        point = *position;
        return 0;
    }
    const std::uint16_t flags = g_fieldDataFlags, required = g_fieldDataClearFlags;
    point = *position;
    GatherFieldSphereRegionCells(position, radius, g_fieldGridCells);
    for (const std::int16_t *cell = g_fieldGridCells; *cell != -1; ++cell) {
        for (const std::int16_t *polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != static_cast<std::int16_t>(required))
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(&point))
                continue;
            direction.x = 0.0f;
            direction.y = g_currentFieldPlane.normal.y;
            direction.z = 0.0f;
            IntersectFieldPlaneAlongNormal(&intersection, &point, &direction);
            const float distance = point.y - intersection.y;
            if (0.0f > distance) {
                if (!(radius > -distance))
                    continue;
            } else if (!(radius > point.y - intersection.y))
                continue;
            if (!FieldPolygonContainsPoint(&intersection))
                continue;
            result = 1;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(&intersection);
            point.y = 0.0f > direction.y ? intersection.y - radius : intersection.y + radius;
        }
    }
    *output = point;
    return result;
}
#endif

#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float FindFieldPartIntersectionHeight(const Vector3 *position) {
    std::int16_t *cell;
    std::int32_t selected;
    float height;
    Vector3 lower, intersection;
    Vector3 *const low = &lower;
    Vector3 *const hit = &intersection;
    const std::int16_t *polygon;
    if (!g_currentFieldData)
        return position->y;
    selected = -1;
    height = -FLT_MAX;
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    low->x = position->x;
    low->y = position->y - 9999.9f;
    low->z = position->z;
    cell = g_fieldGridCells;
    GatherFieldGridCells(position, cell);
    while (*cell != -1) {
        for (polygon = g_currentFieldData->polygonLists[*cell++]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(position) || IsPointAboveFieldPlane(low))
                continue;
            IntersectFieldPlane(hit, position, low);
            if (!FieldPolygonContainsPoint(hit))
                continue;
            if (!(hit->y > height))
                continue;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(hit);
            height = hit->y;
            selected = *polygon;
        }
        if (selected != -1)
            break;
    }
    return height;
}
#else
float FindFieldPartIntersectionHeight(const Vector3 *position) {
    if (!g_currentFieldData)
        return position->y;
    std::int32_t selected = -1;
    float height = -FLT_MAX;
    const std::uint16_t flags = g_fieldDataFlags;
    const std::uint16_t requiredFlags = g_fieldDataClearFlags;
    const Vector3 lower = {SHEN_FIELD(x) position->x, SHEN_FIELD(y) position->y - 9999.9f, SHEN_FIELD(z) position->z};
    GatherFieldGridCells(position, g_fieldGridCells);
    const std::int16_t *cell = g_fieldGridCells;
    while (*cell != -1) {
        const std::int16_t *const polygons = g_currentFieldData->polygonLists[*cell++];
        for (const std::int16_t *polygon = polygons; *polygon != -1; ++polygon) {
            const std::uint16_t polygonFlags = static_cast<std::uint16_t>(GetFieldPolygonFlags(*polygon));
            if (((flags | requiredFlags) & polygonFlags) != requiredFlags)
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(position) || IsPointAboveFieldPlane(&lower))
                continue;
            Vector3 intersection;
            IntersectFieldPlane(&intersection, position, &lower);
            if (FieldPolygonContainsPoint(&intersection) && intersection.y > height) {
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(&intersection);
                height = intersection.y;
                selected = *polygon;
            }
        }
        if (selected != -1)
            break;
    }
    return height;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GatherFieldGridCellsUpward(const Vector3 *position, std::int16_t *cells) {
    const FieldDataGridConfig *const grid = g_currentFieldData->grid;
    if (!grid) {
        *cells++ = 0;
        *cells = -1;
        return 1;
    }
    const float reciprocal = grid->reciprocalCellSize;
    const std::int32_t width = static_cast<std::int32_t>(grid->width), height = static_cast<std::int32_t>(grid->height),
                       depth = static_cast<std::int32_t>(grid->depth);
    const std::int32_t x = static_cast<std::int32_t>((position->x - grid->minimum.x) * reciprocal);
    std::int32_t y = static_cast<std::int32_t>((position->y - grid->minimum.y) * reciprocal);
    const std::int32_t z = static_cast<std::int32_t>((position->z - grid->minimum.z) * reciprocal);
    if (x < 0 || x >= width || y >= height || z < 0 || z >= depth) {
        *cells = -1;
        return 0;
    }
    if (y < 0)
        y = 0;
    y = x + z * width + y * (width * depth);
    const std::int32_t limit = width * height * depth;
    std::int32_t count = 0;
    for (; y < limit; y += width * depth) {
        *cells++ = static_cast<std::int16_t>(y);
        ++count;
    }
    *cells = -1;
    return count;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float FindFieldPartMinimumHeight(const Vector3 *position) {
    std::int16_t *cell;
    std::int32_t selected;
    float height;
    Vector3 lower, intersection;
    Vector3 *const low = &lower;
    Vector3 *const hit = &intersection;
    const std::int16_t *polygon;
    if (!g_currentFieldData)
        return position->y;
    selected = -1;
    height = FLT_MAX;
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    low->x = position->x;
    low->y = position->y - 9999.9f;
    low->z = position->z;
    cell = g_fieldGridCells;
    GatherFieldGridCellsUpward(low, cell);
    while (*cell != -1) {
        for (polygon = g_currentFieldData->polygonLists[*cell++]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(position) || IsPointAboveFieldPlane(low))
                continue;
            IntersectFieldPlane(hit, position, low);
            if (!FieldPolygonContainsPoint(hit))
                continue;
            if (!(hit->y < height))
                continue;
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(hit);
            height = hit->y;
            selected = *polygon;
        }
        if (selected != -1)
            break;
    }
    return height;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
FieldDataRoot *RelocateFieldData(void *data) {
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(data);
    FieldDataRoot *const root = reinterpret_cast<FieldDataRoot *>(base + *reinterpret_cast<const std::uint32_t *>(data));
    root->vertices = reinterpret_cast<Vector3 *>(base + reinterpret_cast<std::uintptr_t>(root->vertices));
    root->normals = reinterpret_cast<Vector3 *>(base + reinterpret_cast<std::uintptr_t>(root->normals));
    root->planeDistances = reinterpret_cast<float *>(base + reinterpret_cast<std::uintptr_t>(root->planeDistances));
    root->polygons = reinterpret_cast<FieldDataPolygon *>(base + reinterpret_cast<std::uintptr_t>(root->polygons));
    root->polygonLists = reinterpret_cast<std::int16_t **>(base + reinterpret_cast<std::uintptr_t>(root->polygonLists));
    root->metadata = reinterpret_cast<FieldDataMetadata *>(base + reinterpret_cast<std::uintptr_t>(root->metadata));
    if (!root->grid) {
        root->polygonLists[0] = reinterpret_cast<std::int16_t *>(base + reinterpret_cast<std::uintptr_t>(root->polygonLists[0]));
    } else {
        root->grid = reinterpret_cast<FieldDataGridConfig *>(base + reinterpret_cast<std::uintptr_t>(root->grid));
        const std::uint32_t width = static_cast<std::uint32_t>(root->grid->width);
        const std::uint32_t height = static_cast<std::uint32_t>(root->grid->height);
        const std::uint32_t depth = static_cast<std::uint32_t>(root->grid->depth);
        std::int16_t **list = root->polygonLists;
        for (std::int32_t index = 0; index < static_cast<std::int32_t>(width * height * depth); ++index) {
            *list = reinterpret_cast<std::int16_t *>(base + reinterpret_cast<std::uintptr_t>(*list));
            ++list;
        }
    }
    if (root->auxiliaryLists) {
        root->auxiliaryLists = reinterpret_cast<void **>(base + reinterpret_cast<std::uintptr_t>(root->auxiliaryLists));
        for (void **entry = root->auxiliaryLists; *entry; ++entry)
            *entry = reinterpret_cast<void *>(base + reinterpret_cast<std::uintptr_t>(*entry));
    }
    return root;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t RemoveFieldPart(shen_name name) {
    FieldDataRuntimePart *part = FindFieldDataPart(name);
    if (!part)
        return -1;
    for (; part < &g_fieldParts[7]; ++part)
        std::memcpy(part, part + 1, sizeof(*part));
    std::memset(&g_fieldParts[7], 0, sizeof(g_fieldParts[7]));
    --g_fieldPartCount;
    return 0;
}

#else

std::int32_t RemoveFieldPart(shen_name name) {
    for (std::size_t index = 0; index < fieldParts.size(); ++index) {
        if (fieldParts[index].name.name != name.name)
            continue;
        fieldParts.erase(fieldParts.begin() + static_cast<std::ptrdiff_t>(index));
        return 0;
    }
    return -1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void FLDD_FreeFieldData() {
    flddClearAllGeneratedVolumes();
    fieldParts.clear();
    generatedRuntimeAllocated = false;
    lastRadiusPartIndex = std::numeric_limits<std::size_t>::max();
    foundIntersection = false;
    lastIntersection = {};
    ResetQueryFlags();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ResolveFightFieldProbe(FightFieldProbe *, MapControlData *) {
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindRayIntersectionInGridWithinRadius(const Vector3 *origin, const Vector3 *destination, float radius) {
    std::int16_t *cell;
    std::int32_t index, count;
    Vector3 intersection, edge;
    const std::int16_t *polygon;
    if (!g_currentFieldData)
        return 0;
    const std::int16_t flags = static_cast<std::int16_t>(g_fieldDataFlags), required = static_cast<std::int16_t>(g_fieldDataClearFlags);
    cell = g_fieldGridCells;
    if (g_fieldRayUsesGrid) {
        count = GatherFieldSegmentGridCells(origin, destination, cell);
    } else {
        if (0) {
        found:
            StoreFieldIntersectionPlane(*polygon);
            StoreFieldIntersectionPosition(&intersection);
            return 1;
        }
        cell[0] = static_cast<std::int16_t>(FindFieldGridCell(origin));
        cell[1] = static_cast<std::int16_t>(FindFieldGridCell(destination));
        count = 2;
    }
    for (index = 0; index < count; ++index, ++cell) {
        if (*cell == -1)
            continue;
        for (polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            const bool destinationAbove = IsPointAboveFieldPlane(destination);
            if (destinationAbove == IsPointAboveFieldPlane(origin))
                continue;
            IntersectFieldPlane(&intersection, origin, destination);
            if (FieldPolygonContainsPoint(&intersection) || ResolveFieldPolygonSphereOverlap(&edge, &intersection, radius))
                goto found;
        }
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetOccupancyForThisFieldSegment(shen_name name, const Vector3 *position, std::int32_t yaw) {
    if (!position)
        return -1;
    FieldDataRuntimePart *const part = FindFieldDataPart(name);
    if (!part)
        return -1;
    part->position = *position;
    part->yaw = yaw;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(position->x, position->y, position->z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    matrix_stack::GetMatrix(&part->transform);
    Matrix4x4::NormalizeAndSetTransformMatrix();
    matrix_stack::GetMatrix(&part->inverseTransform);
    PopCurrentMatrix(1);
    if (__builtin_fabsf(position->x) < 1.1920928955078125e-07f && __builtin_fabsf(position->y) < 1.1920928955078125e-07f &&
        __builtin_fabsf(position->z) < 1.1920928955078125e-07f && yaw == 0)
        part->transformed = 0;
    else
        part->transformed = 1;
    return 0;
}
#endif

#else

// @verified - p
std::int32_t SetOccupancyForThisFieldSegment(shen_name partName, const Vector3 *translation, std::int32_t yaw) {
    if (!translation)
        return -1;
    const auto part =
        std::find_if(fieldParts.begin(), fieldParts.end(), [partName](const FieldDataPart &candidate) { return candidate.name.name == partName.name; });
    if (part == fieldParts.end())
        return -1;

    part->translation = *translation;
    part->yaw = yaw;
    BuildPartTransforms(*part);
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    part->transformed = !(std::fabs(translation->x) < epsilon && std::fabs(translation->y) < epsilon && std::fabs(translation->z) < epsilon) || yaw != 0;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t flddQueryStairConnection(const Vector3 *position, Vector3 *anchor, std::int32_t *yaw, std::int32_t *connectionId) {
    flddPushIntersectionFlag(0x20);
    float distance = FindFieldIntersectionHeight(position) - position->y;
    if (distance < 0.0f)
        distance = -distance;
    if (distance > 2.0f)
        return 0;
    const std::uint32_t *record = FindFieldCategoryRecord(5);
    if (!record)
        return 0;
    const std::uint32_t steps = *record++;
    *connectionId = static_cast<std::int32_t>(*record++);
    anchor->x = *reinterpret_cast<const float *>(record++);
    anchor->y = *reinterpret_cast<const float *>(record++);
    anchor->z = *reinterpret_cast<const float *>(record++);
    *yaw = static_cast<std::uint16_t>(*reinterpret_cast<const float *>(record) * 8192.0f / 45.0f);
    if (g_fieldIntersection.part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&g_fieldIntersection.part->transform);
        Matrix4x4::TransformPoint(*anchor, *anchor);
        PopCurrentMatrix(1);
        *yaw = static_cast<std::int32_t>(static_cast<std::uint32_t>(*yaw) + static_cast<std::uint32_t>(g_fieldIntersection.part->yaw));
    }
    return steps;
}
#endif

#else

// @verified - p
std::uint32_t flddQueryStairConnection(const Vector3 &position, float maximumY, Vector3 &anchor, std::int32_t &yaw, std::int32_t &connectionId) {
    flddPushIntersectionFlag(0x20);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (std::fabs(height - position.y) > 2.0f)
        return 0;

    const std::int32_t *const record = FindCategoryRecord(5U, 6);
    if (!record)
        return 0;

    connectionId = record[1];
    std::memcpy(&anchor, record + 2, sizeof(anchor));
    float yawDegrees{};
    std::memcpy(&yawDegrees, record + 5, sizeof(yawDegrees));
    yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>((yawDegrees * 8192.0f) / 45.0f));
    const FieldDataPart &part = fieldParts[lastRadiusPartIndex];
    if (part.transformed) {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&part.localToWorld);
        Matrix4x4::TransformPoint(anchor, anchor);
        Matrix4x4::Pop(1);
        yaw = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) + static_cast<std::uint32_t>(part.yaw));
    }
    return static_cast<std::uint32_t>(record[0]);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t scenFindIntersection(const Vector3 *position) {
    flddPushIntersectionFlag(0x80);
    float distance = FindFieldIntersectionHeight(position) - position->y;
    if (distance < 0.0f)
        distance = -distance;
    if (distance > 2.0f)
        return -1;
    const std::uint32_t *const record = FindFieldCategoryRecord(7);
    if (!record)
        return -1;
    return static_cast<std::int32_t>(*record);
}
#endif

#else

std::int32_t scenFindIntersection(const Vector3 &position, float maximumY) {
    flddPushIntersectionFlag(0x80);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (!(std::fabs(height - position.y) <= 2.0f)) {
        return -1;
    }
    std::int32_t scalar{};
    return flddDecodeCategoryScalar(7U, scalar) ? scalar : -1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t QueryGroundMaterialToken(const Vector3 &position, float maximumY) {
    // Native QueryGroundMaterialToken: category 2 packs the two complete 32-bit words.
    flddPushIntersectionFlag(4);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (!(std::fabs(height - position.y) <= 2.0f))
        return 0;

    std::int32_t first{};
    std::int32_t second{};
    if (!DecodeCategoryPair(2U, first, second))
        return 0;

    return static_cast<std::uint32_t>(first) | (static_cast<std::uint32_t>(second) << 16U);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t flddQueryCameraIntersection(const Vector3 &position) {
    // camCtrlIntersectionTest has no ground-distance gate.
    flddPushIntersectionFlag(4);
    if (shenFindIntersectionWithScene(position, position.y) == -std::numeric_limits<float>::max())
        return 0;

    std::int32_t first{};
    std::int32_t second{};
    if (!DecodeCategoryPair(2U, first, second))
        return 0;
    return static_cast<std::uint32_t>(first) | (static_cast<std::uint32_t>(second) << 16U);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t flddQueryStairOrigin(const Vector3 *position, Vector3 *origin, std::int32_t *yaw) {
    Vector3 local = *position;
    flddPushIntersectionFlag(0x20);
    local.y += 0.2f;
    if (!IsWithinRadiusFromVector3PolyIntersectTest(&local, 0.4f))
        return 0;
    const std::uint32_t *record = FindFieldCategoryRecord(5);
    if (!record)
        return 0;
    const std::uint32_t steps = *record++;
    record++;
    origin->x = *reinterpret_cast<const float *>(record++);
    origin->y = *reinterpret_cast<const float *>(record++);
    origin->z = *reinterpret_cast<const float *>(record++);
    *yaw = static_cast<std::uint16_t>(*reinterpret_cast<const float *>(record) * 8192.0f / 45.0f);
    if (g_fieldIntersection.part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&g_fieldIntersection.part->transform);
        Matrix4x4::TransformPoint(*origin, *origin);
        PopCurrentMatrix(1);
        *yaw = static_cast<std::int32_t>(static_cast<std::uint32_t>(*yaw) + static_cast<std::uint32_t>(g_fieldIntersection.part->yaw));
    }
    return steps;
}
#endif

#else

// @verified - p
std::int32_t flddQueryStairOrigin(const Vector3 &position, Vector3 &origin, std::int32_t &yaw) {
    // Native flddQueryStairOrigin: retain the full transformed yaw, without the
    // category-5 edge-length work performed by flddQueryStairRadius.
    Vector3 query = position;
    query.y += 0.2f;
    flddPushIntersectionFlag(0x20);
    if (!IsWithinRadiusFromVector3PolyIntersectTest(query, 0.40000001f))
        return 0;

    const std::int32_t *const record = FindCategoryRecord(5U, 6);
    if (!record)
        return 0;

    std::memcpy(&origin, record + 2, sizeof(origin));
    float degrees{};
    std::memcpy(&degrees, record + 5, sizeof(degrees));
    yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>((degrees * 8192.0f) / 45.0f));
    const FieldDataPart &part = fieldParts[lastRadiusPartIndex];
    if (part.transformed) {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&part.localToWorld);
        Matrix4x4::TransformPoint(origin, origin);
        Matrix4x4::Pop(1);
        yaw = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) + static_cast<std::uint32_t>(part.yaw));
    }
    return record[0];
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int32_t flddQueryStairRadius(const Vector3 &position, Vector3 &origin, std::int32_t &yaw, float &maximumEdgeLength) {
    const std::int32_t count = flddQueryStairOrigin(position, origin, yaw);
    maximumEdgeLength = 0.0f;
    if (count != 0) {
        const FieldDataPart &part = fieldParts[lastRadiusPartIndex];
        const FieldDataPolygon &polygon = part.polygons[lastIntersection.polygonIndex];
        const Vector3 &first = part.vertices[polygon.vertexIndices[0]];
        const Vector3 &second = part.vertices[polygon.vertexIndices[1]];
        const Vector3 &third = part.vertices[polygon.vertexIndices[2]];
        maximumEdgeLength = std::sqrt(std::fmax(PlanarSquaredDistance(second, third), std::fmax(PlanarSquaredDistance(first, second), 0.0f)));
    }
    return count;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t flddSetPointFlagOnGrid(shen_name name, std::int32_t enabled) {
    FieldDataRuntimePart *const part = FindFieldDataPart(name);
    if (!part)
        return -1;
    part->enabled = enabled;
    return 0;
}

#else

// @verified - p
std::int32_t flddSetPointFlagOnGrid(shen_name partName, std::int32_t flag) {
    for (FieldDataPart &part : fieldParts) {
        if (part.name.name != partName.name)
            continue;
        part.enabled = flag;
        return 0;
    }
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
FieldDataRuntimePart *FindFieldDataPart(shen_name name) {
    std::int32_t index = 0;
    FieldDataRuntimePart *part = g_fieldParts;
    for (; index < g_fieldPartCount; ++index, ++part) {
        if (part->name == name)
            return part;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t ParseFieldData(void *data, shen_name name, std::int32_t relocate) {
    std::int32_t index = g_fieldPartCount;
    if (!data || index >= 8 || FindFieldDataPart(name))
        return -1;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    FieldDataRoot *root;
    if (relocate)
        root = RelocateFieldData(data);
    else
        root = static_cast<FieldDataRoot *>(data);
    std::int32_t partClass;
    if (root->metadata->gridMode > 0.0f)
        partClass = 0;
    else
        partClass = static_cast<std::int32_t>(-root->metadata->gridMode);
    if (partClass > 1) {
        FieldDataPolygon *polygon = root->polygons;
        for (std::int32_t polygonIndex = 0; polygonIndex < root->metadata->polygonCount; ++polygonIndex, ++polygon) {
            std::int32_t duplicatePairs = 0;
            for (std::int32_t left = 0; left < 4; ++left)
                for (std::int32_t right = 0; right < 4; ++right)
                    if (left < right && polygon->vertexIndices[left] == polygon->vertexIndices[right])
                        ++duplicatePairs;
            if ((polygon->vertexIndices[0] == polygon->vertexIndices[3] && duplicatePairs > 1) ||
                (polygon->vertexIndices[0] != polygon->vertexIndices[3] && duplicatePairs > 0))
                polygon->flags |= 0x10;
            if (polygon->flags & 9)
                continue;
            const Vector3 *const normal = &root->normals[polygon->normalIndex];
            if ((normal->y < 0.0f ? -normal->y : normal->y) <= 0.5f) {
                float minimum = FLT_MAX, maximum = -FLT_MAX;
                for (std::int32_t vertex = 0; vertex < 4; ++vertex) {
                    const Vector3 *const position = &root->vertices[polygon->vertexIndices[vertex]];
                    minimum = position->y > minimum ? minimum : position->y;
                    maximum = maximum > position->y ? maximum : position->y;
                }
                if ((maximum - minimum < 0.0f ? -(maximum - minimum) : maximum - minimum) < 0.45f)
                    polygon->flags |= 0x400;
                else
                    polygon->flags |= 1;
            }
        }
    }
#else
    FieldDataRoot *const root = relocate ? RelocateFieldData(data) : static_cast<FieldDataRoot *>(data);
    const float gridMode = root->metadata->gridMode;
    const std::int32_t partClass = gridMode > 0.0f ? 0 : static_cast<std::int32_t>(-gridMode);
    if (partClass > 1) {
        FieldDataPolygon *polygon = root->polygons;
        for (std::int32_t polygonIndex = 0; polygonIndex < root->metadata->polygonCount; ++polygonIndex, ++polygon) {
            std::int32_t duplicatePairs = 0;
            for (std::int32_t left = 0; left < 4; ++left) {
                for (std::int32_t right = 0; right < 4; ++right) {
                    if (left < right && polygon->vertexIndices[left] == polygon->vertexIndices[right])
                        ++duplicatePairs;
                }
            }
            if ((polygon->vertexIndices[0] == polygon->vertexIndices[3] && duplicatePairs > 1) ||
                (polygon->vertexIndices[0] != polygon->vertexIndices[3] && duplicatePairs > 0))
                polygon->flags |= 0x10;
            if (!(polygon->flags & 9U)) {
                const float normalY = root->normals[polygon->normalIndex].y;
                if (normalY < 0.0f) {
                    if (-normalY > 0.5f)
                        continue;
                } else if (normalY > 0.5f) {
                    continue;
                }
                float minimum = FLT_MAX;
                float maximum = -FLT_MAX;
                for (std::int32_t vertex = 0; vertex < 4; ++vertex) {
                    const float y = root->vertices[polygon->vertexIndices[vertex]].y;
                    minimum = y > minimum ? minimum : y;
                    maximum = maximum > y ? maximum : y;
                }
                const float difference = maximum - minimum;
                const float height = difference < 0.0f ? -difference : difference;
                polygon->flags |= height < 0.45f ? 0x400 : 1;
            }
        }
    }
#endif
    g_fieldParts[index].data = root;
    g_fieldParts[index].enabled = 1;
    g_fieldParts[index].name = name;
    g_fieldParts[index].partClass = partClass;
    ++index;
    g_fieldPartCount = index;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ParseFLDD(void *data, shen_name name) {
    return ParseFieldData(data, name, 1);
}

#else

std::int32_t ParseFLDD(std::span<const std::byte> data, shen_name name) {
    if (data.empty() || fieldParts.size() >= kMaximumFieldParts)
        return -1;
    for (const FieldDataPart &existing : fieldParts)
        if (existing.name.name == name.name)
            return -1;
    FieldDataPart part{};
    if (!ReadPart(data, name, part))
        return -1;
    fieldParts.push_back(std::move(part));
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t FLDD_ReadFieldData(void *chunk) {
    g_currentFieldData = 0;
    g_fieldPartCount = 0;
    std::memset(g_fieldParts, 0, sizeof(g_fieldParts));
    g_currentFieldPart = 0;
    ClearFieldIntersection();
    ResetFieldDataFlags();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    g_fieldRayUsesGrid = 0;
#else
    g_fieldDataResetFlags = 0;
#endif
    if (!chunk)
        return -1;
    MapInfoChunk *part = static_cast<MapInfoChunk *>(chunk) + 1;
    while (part->nextByteOffset) {
        if (reinterpret_cast<std::uintptr_t>(part) == UINT32_MAX - 7U)
            return -1;
        ParseFLDD(part + 1, part->name);
        part = reinterpret_cast<MapInfoChunk *>(reinterpret_cast<std::byte *>(part) + part->nextByteOffset);
    }
    InitializeGeneratedFieldData();
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t FieldDataNullsub() {
    return 0;
}
#endif

#else

bool FLDD_ReadFieldData(const void *chunk, std::size_t chunkSize) {
    FLDD_FreeFieldData();
    if (!chunk || chunkSize < sizeof(MapInfoChunk) + 8)
        return false;
    fieldParts.reserve(kMaximumFieldParts);

    const auto bytes = std::span<const std::byte>(static_cast<const std::byte *>(chunk), chunkSize);
    std::size_t cursor = sizeof(MapInfoChunk);
    while (cursor + 8 <= bytes.size()) {
        const auto *const header = SpanAt<MapInfoChunk>(bytes, cursor);
        if (!header)
            return false;

        if (header->nextByteOffset == 0) {
            if (!FLDD_AllocateRuntimeBuffers())
                return true;
            generatedRuntimeAllocated = true;
            generatedPolygonList = {};
            generatedPolygonList[1] = -1;
            bool generatedNamePresent = false;
            for (const FieldDataPart &part : fieldParts)
                if (part.name.name == kGeneratedVolumeName.name)
                    generatedNamePresent = true;
            if (!generatedNamePresent && fieldParts.size() < kMaximumFieldParts) {
                FieldDataPart generated{};
                generated.name = kGeneratedVolumeName;
                generated.partClass = 2;
                generated.gridMode = -2.0f;
                generated.data = flddRuntimeSpace;
                generated.segmentOffsets[5] = sizeof(std::int16_t);
                generated.sourceData = std::as_bytes(std::span{generatedPolygonList});
                generated.vertices.reserve(kMaximumGeneratedVolumes * 8U);
                generated.normals.reserve(kMaximumGeneratedVolumes * 6U);
                generated.planeDistances.reserve(kMaximumGeneratedVolumes * 6U);
                generated.polygons.reserve(kMaximumGeneratedVolumes * 6U);
                fieldParts.push_back(std::move(generated));
            }
            return true;
        }

        if (header->nextByteOffset < 12 || header->nextByteOffset > bytes.size() - cursor) {
            FLDD_FreeFieldData();
            return false;
        }

        bool duplicate = false;
        for (const FieldDataPart &existing : fieldParts)
            if (existing.name.name == header->name.name)
                duplicate = true;
        if (duplicate || fieldParts.size() >= kMaximumFieldParts) {
            cursor += header->nextByteOffset;
            continue;
        }
        FieldDataPart part{};
        const auto partData = bytes.subspan(cursor + 8, header->nextByteOffset - 8);
        if (!ReadPart(partData, header->name, part)) {
            FLDD_FreeFieldData();
            return false;
        }

        fieldParts.push_back(std::move(part));
        cursor += header->nextByteOffset;
    }
    FLDD_FreeFieldData();
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float FindFieldIntersectionHeight(const Vector3 *position) {
    float height = -FLT_MAX;
    flddPushIntersectionFlag(8);
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 local = *position;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        local.y = FindFieldPartIntersectionHeight(&local);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(local, local);
            PopCurrentMatrix(1);
        }
        if (local.y > height)
            height = local.y;
    }
    ResetFieldDataFlags();
    return height;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float FindFieldIntersectionHeightAbove(const Vector3 *position) {
    Vector3 probe;
    probe.x = position->x;
    probe.y = position->y + 0.5f;
    probe.z = position->z;
    return FindFieldIntersectionHeight(&probe);
}
#endif

float FindFieldMinimumHeightAbove(const Vector3 *position) {
    Vector3 probe;
    probe.x = position->x;
    probe.y = position->y + 0.5f;
    probe.z = position->z;
    float height = FindFieldIntersectionHeight(&probe);
    if (height == -FLT_MAX)
        height = position->y;
    return height;
}

float FindFieldMinimumHeightInWorld(const Vector3 *position) {
    float minimum = FLT_MAX;
    flddPushIntersectionFlag(8);
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 local = *position;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        local.y = FindFieldPartMinimumHeight(&local);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(local, local);
            PopCurrentMatrix(1);
        }
        if (minimum > local.y)
            minimum = local.y;
    }
    ResetFieldDataFlags();
    return minimum;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float TransformClippedPoint(const Vector3 *position) {
    float minimum = FLT_MAX;
    flddPushIntersectionFlag(8);
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 local = *position;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        local.y = 5000.0f;
        local.y = FindFieldPartMinimumHeight(&local);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(local, local);
            PopCurrentMatrix(1);
        }
        if (minimum > local.y)
            minimum = local.y;
    }
    ResetFieldDataFlags();
    return minimum;
}
#endif

std::uint32_t ResolveFieldSphereVolumeInWorld(Vector3 *output, const Vector3 *position, float radius) {
    Vector3 local = *position;
    std::uint32_t result = 0;
    ClearFieldIntersection();
    *output = *position;
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        result |= ResolveFieldPartSphereVolume(output, &local, radius);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(*output, *output);
            PopCurrentMatrix(1);
        }
        local = *output;
    }
    ResetFieldDataFlags();
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindNearestRayIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    Vector3 localEnd = *destination;
    std::int32_t result = 0;
    ClearFieldIntersection();
    *output = *destination;
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 localStart = *origin;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(localEnd, localEnd);
            Matrix4x4::TransformPoint(localStart, localStart);
        }
        result |= FindFieldPartEndpointIntersection(output, &localEnd, &localStart);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(*output, *output);
            PopCurrentMatrix(1);
        }
        localEnd = *output;
    }
    ResetFieldDataFlags();
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    Vector3 localEnd = *destination;
    std::int32_t result = 0;
    ClearFieldIntersection();
    *output = *destination;
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 localStart = *origin;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(localEnd, localEnd);
            Matrix4x4::TransformPoint(localStart, localStart);
        }
        result |= FindFieldPartSegmentOffsetIntersection(output, &localEnd, &localStart);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(*output, *output);
            PopCurrentMatrix(1);
        }
        localEnd = *output;
    }
    ResetFieldDataFlags();
    return result;
}

#endif

#else

// @verified - p
std::int32_t FindNearestRayIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin) {
    if (!output || !destination || !origin) {
        ResetQueryFlags();
        return false;
    }
    *output = *destination;
    foundIntersection = false;
    std::int32_t hit = 0;
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;
        if (part.transformed) {
            const Vector3 end = TransformPoint(part.worldToLocal, *output);
            const Vector3 start = TransformPoint(part.worldToLocal, *origin);
            Vector3 corrected{};
            hit |= FindEndpointIntersection(part, partIndex, corrected, end, start);
            *output = TransformPoint(part.localToWorld, corrected);
        } else {
            const Vector3 end = *output;
            hit |= FindEndpointIntersection(part, partIndex, *output, end, *origin);
        }
    }
    ResetQueryFlags();
    return hit;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t TestFieldWorldSegment(const Vector3 *start, const Vector3 *end) {
    std::int32_t result = 0;
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 localStart = *start, localEnd = *end;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(localStart, localStart);
            Matrix4x4::TransformPoint(localEnd, localEnd);
            PopCurrentMatrix(1);
        }
        result |= TestFieldPartSegment(&localStart, &localEnd);
        if (result)
            break;
    }
    ResetFieldDataFlags();
    return result;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius) {
    std::uint32_t result = 0;
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 start = origin, end = destination;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(start, start);
            Matrix4x4::TransformPoint(end, end);
            PopCurrentMatrix(1);
        }
        g_fieldRayUsesGrid = 1;
        result |= FindRayIntersectionInGridWithinRadius(&start, &end, radius);
        if (result)
            break;
    }
    ResetFieldDataFlags();
    g_fieldRayUsesGrid = 0;
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t GlobalCapsuleIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius) {
    std::uint32_t result = 0;
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 start = origin, end = destination;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(start, start);
            Matrix4x4::TransformPoint(end, end);
            PopCurrentMatrix(1);
        }
        result |= static_cast<std::uint32_t>(TestFieldPartSphereIntersection(&start, radius));
        if (result)
            break;
        result = static_cast<std::uint32_t>(TestFieldPartSphereIntersection(&end, radius));
        if (result)
            break;
        g_fieldRayUsesGrid = 1;
        result = FindRayIntersectionInGridWithinRadius(&start, &end, radius);
        if (result)
            break;
    }
    ResetFieldDataFlags();
    g_fieldRayUsesGrid = 0;
    return result;
}
#endif

#else

bool GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius) {
    foundIntersection = false;
    bool hit = false;
    // GlobalRayIntersectionTest sets FieldDataFlag2=1, selecting full DDA rather than the
    // endpoint-only mode of FindRayIntersectionInGridWithinRadius. This dedicated path needs no global
    // mode flag to leak or restore. Radius expands polygon edges, not voxels.
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;

        const Vector3 start = part.transformed ? TransformPoint(part.worldToLocal, origin) : origin;
        const Vector3 end = part.transformed ? TransformPoint(part.worldToLocal, destination) : destination;
        hit = FindSegmentIntersection(part, partIndex, start, end, true, radius);
        if (hit)
            break;
    }
    ResetQueryFlags();
    return hit;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddFindGround(const Vector3 &position, float maximumY, std::uint16_t requiredFlags, FieldGroundHit &output) {
    foundIntersection = false;
    lastRadiusPartIndex = std::numeric_limits<std::size_t>::max();
    float highestY = -std::numeric_limits<float>::max();
    const Vector3 worldStart{position.x, maximumY, position.z};
    const std::uint16_t queryMask = static_cast<std::uint16_t>(requiredFlags | intersectionMask);

    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const FieldDataPart &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;

        const Vector3 localStart = part.transformed ? TransformPoint(part.worldToLocal, worldStart) : worldStart;

        for (std::size_t polygonIndex = 0; polygonIndex < part.polygons.size(); ++polygonIndex) {
            const FieldDataPolygon &polygon = part.polygons[polygonIndex];
            std::uint16_t processedFlags = polygon.flags;
            if (processedFlags & fieldDataClearFlags)
                processedFlags = static_cast<std::uint16_t>(processedFlags & ~fieldDataResetFlags);

            const std::uint16_t activeFlags = static_cast<std::uint16_t>(queryMask | fieldDataFlags);
            if ((processedFlags & activeFlags) != queryMask)
                continue;

            const Vector3 &normal = part.normals[polygon.normalIndex];
            if (normal.y <= std::numeric_limits<float>::epsilon())
                continue;

            const float planeDistance = part.planeDistances[polygon.field06];
            const float localY = (planeDistance - normal.x * localStart.x - normal.z * localStart.z) / normal.y;
            if (localY > localStart.y)
                continue;

            const Vector3 localHit{localStart.x, localY, localStart.z};
            if (!TestPointInsidePolygon(part, polygon, localHit, normal))
                continue;

            const Vector3 worldHit = part.transformed ? TransformPoint(part.localToWorld, localHit) : localHit;
            if (worldHit.y > maximumY || worldHit.y <= highestY)
                continue;

            highestY = worldHit.y;
            PublishPartIntersection(part, partIndex, static_cast<std::uint16_t>(polygonIndex), localHit);
        }
    }
    ResetQueryFlags();
    if (!foundIntersection)
        return false;

    Vector3 worldPoint = lastIntersection.point;
    TransformAgainstIntersectionResult(worldPoint);
    const FieldDataPart &owner = fieldParts[lastRadiusPartIndex];
    const Vector3 worldNormal = owner.transformed ? TransformVector(owner.localToWorld, lastIntersection.planeNormal) : lastIntersection.planeNormal;
    output = {worldPoint, worldNormal, owner.transformed ? Dot(worldNormal, worldPoint) : lastIntersection.planeDistance, lastIntersection.polygonFlags};
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 *position, float radius) {
    std::uint32_t result = 0;
    ClearFieldIntersection();
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        Vector3 local = *position;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
            PopCurrentMatrix(1);
        }
        result |= static_cast<std::uint32_t>(TestFieldPartSphereIntersection(&local, radius));
        if (result)
            break;
    }
    ResetFieldDataFlags();
    return result;
}

#else

// @verified - p
std::int32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 &point, float radius) {
    foundIntersection = false;
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const FieldDataPart &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;
        const Vector3 localPoint = part.transformed ? TransformPoint(part.worldToLocal, point) : point;
        if (TestRadiusInPart(part, partIndex, localPoint, radius)) {
            ResetQueryFlags();
            return 1;
        }
    }
    ResetQueryFlags();
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void TransformIntersectionNormal(Vector3 *normal) {
    FieldDataRuntimePart *const part = g_fieldIntersection.part;
    if (!static_cast<std::uint8_t>(g_fieldIntersection.found))
        return;
    *normal = g_fieldIntersection.plane.normal;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::Transform(*normal, *normal);
        PopCurrentMatrix(1);
    }
}

float GetFieldIntersectionNormalY() {
    float y;
    if (static_cast<std::uint8_t>(g_fieldIntersection.found))
        y = g_fieldIntersection.plane.normal.y;
    else
        y = FLT_MAX;
    return y;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool DecodeCategoryPair(std::uint32_t category, std::int32_t &first, std::int32_t &second) {
    const std::int32_t *const pair = FindCategoryRecord(category, 2);
    if (!pair)
        return false;
    first = pair[0];
    second = pair[1];
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ResetQueryFlags() {
    intersectionMask = 0;
    fieldDataFlags = 0x10;
    fieldDataClearFlags = 0;
    fieldDataResetFlags = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool StoreFieldRuntimeSegment(FieldDataSourceRoot &root, std::uint32_t index, std::size_t size) {
    void *const block = HLib::AllocateTaskData(size, FIELD_DATA_MEMORY);
    root.segmentOffsets[index] = block ? static_cast<std::uint32_t>(HLib::GetDynamicHeapOffset(block)) : 0U;
    return root.segmentOffsets[index] != 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native FLDD_AllocateRuntimeBuffers: the generated-volume part's source root and its segments come from the field
// owner's task data. A failed allocation leaves the generated runtime unset; the blocks retire with their owner task.
bool FLDD_AllocateRuntimeBuffers() {
    flddRuntimeSpace = nullptr;
    FieldDataSourceRoot *const root = static_cast<FieldDataSourceRoot *>(HLib::AllocateTaskData(sizeof(FieldDataSourceRoot), FIELD_DATA_MEMORY));
    if (!root)
        return false;
    constexpr std::array<std::size_t, 4> segmentSizes{0x12c0, 0xe10, 0x4b0, 0x12c0};
    for (std::uint32_t index = 0; index < segmentSizes.size(); ++index)
        if (!StoreFieldRuntimeSegment(*root, index, segmentSizes[index]))
            return false;
    root->segmentOffsets[4] = 0;
    if (!StoreFieldRuntimeSegment(*root, 5, 8) || !StoreFieldRuntimeSegment(*root, 5, 0x25a) || !StoreFieldRuntimeSegment(*root, 6, 0xc))
        return false;
    (void)StoreFieldRuntimeSegment(*root, 7, 8);
    for (std::uint32_t index = 0; index < segmentSizes.size(); ++index)
        std::memset(HLib::GetPointerFromDynamicHeapOffsetBase(static_cast<std::int32_t>(root->segmentOffsets[index])), 0, segmentSizes[index]);
    std::memset(HLib::GetPointerFromDynamicHeapOffsetBase(static_cast<std::int32_t>(root->segmentOffsets[5])), 0, 0x25a);
    std::uint32_t *const grid = static_cast<std::uint32_t *>(HLib::GetPointerFromDynamicHeapOffsetBase(static_cast<std::int32_t>(root->segmentOffsets[6])));
    grid[0] = 0xc0000000U;
    grid[1] = 0;
    grid[2] = 0;
    root->segmentOffsets[7] = 0;
    flddRuntimeSpace = root;
    generatedVolumeCount = 0;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TransformAgainstIntersectionResult(Vector3 *position) {
    FieldDataRuntimePart *const part = g_fieldIntersection.part;
    if (!static_cast<std::uint8_t>(g_fieldIntersection.found))
        return;
    *position = g_fieldIntersection.position;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::TransformPoint(*position, *position);
        PopCurrentMatrix(1);
    }
}

#else

// @verified - p
void TransformAgainstIntersectionResult(Vector3 &point) {
    if (!foundIntersection)
        return;
    point = lastIntersection.point;
    if (lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].transformed)
        point = TransformPoint(fieldParts[lastRadiusPartIndex].localToWorld, point);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void transformIntersectionResult(Vector2s *angles) {
    FieldDataRuntimePart *const part = g_fieldIntersection.part;
    if (static_cast<std::uint8_t>(g_fieldIntersection.found) == 0)
        return;
    Vector3 normal = g_fieldIntersection.plane.normal;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::Transform(normal, normal);
        PopCurrentMatrix(1);
    }
    Vector3_ToFixed16EulerAngles(normal, reinterpret_cast<Vector3s &>(*angles));
    angles->x = 0x4000 - angles->x;
    angles->y += 0x8000;
}
#else
// @verified - d
void transformIntersectionResult(Vector2s *angles) {
    if (static_cast<std::uint8_t>(g_fieldIntersection.found) == 0)
        return;
    Vector3 normal = g_fieldIntersection.plane.normal;
    if (g_fieldIntersection.part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&g_fieldIntersection.part->transform);
        Matrix4x4::Transform(normal, normal);
        PopCurrentMatrix(1);
    }
    Vector3s rotation = {SHEN_FIELD(x) angles->x, SHEN_FIELD(y) angles->y, SHEN_FIELD(z) 0};
    Vector3_ToFixed16EulerAngles(normal, rotation);
    angles->x = static_cast<std::int16_t>(static_cast<std::uint16_t>(0x4000 - rotation.x));
    angles->y = static_cast<std::int16_t>(static_cast<std::uint16_t>(rotation.y - 0x8000));
}
#endif

#else

// @verified - p
void transformIntersectionResult(Vector3s &angles) {
    if (!foundIntersection)
        return;
    Vector3 normal = lastIntersection.planeNormal;
    if (lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].transformed)
        normal = TransformVector(fieldParts[lastRadiusPartIndex].localToWorld, normal);
    constexpr float epsilon = 0.00000011920929f;
    if (!(std::fabs(normal.x) < epsilon && std::fabs(normal.y) < epsilon && std::fabs(normal.z) < epsilon)) {
        angles.x = static_cast<std::int16_t>(FixedAtan2(normal.y, std::sqrt(normal.x * normal.x + normal.z * normal.z)));
        angles.y = static_cast<std::int16_t>(FixedAtan2(normal.x, normal.z) + 0x8000);
    }
    angles.x = static_cast<std::int16_t>(0x4000 - angles.x);
    angles.y = static_cast<std::int16_t>(angles.y + 0x8000);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t RaycastInWorldGrid(Vector3 *output, const Vector3 *position, float radius) {
    Vector3 local = *position;
    std::uint32_t result = 0;
    flddPushIntersectionFlag(1);
    ClearFieldIntersection();
    *output = *position;
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        result |= ResolveFieldPartSphereOverlap(output, &local, radius);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(*output, *output);
            PopCurrentMatrix(1);
        }
        local = *output;
    }
    ResetFieldDataFlags();
    return result;
}

#else

// @verified - p
std::int32_t RaycastInWorldGrid(Vector3 &output, const Vector3 &origin, float radius) {
    output = origin;
    foundIntersection = false;
    intersectionMask |= 1U;
    bool found = false;
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const FieldDataPart &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;

        Vector3 local = part.transformed ? TransformPoint(part.worldToLocal, output) : output;
        const bool partHit = ResolveHorizontalRadiusInPart(part, partIndex, local, radius);
        output = part.transformed ? TransformPoint(part.localToWorld, local) : local;
        found = partHit || found;
    }
    ResetQueryFlags();
    return found;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint32_t TestFieldSphereRegionInWorld(Vector3 *output, const Vector3 *position, float radius) {
    Vector3 local = *position;
    std::uint32_t result = 0;
    flddPushIntersectionFlag(8);
    ClearFieldIntersection();
    *output = *position;
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (!g_currentFieldPart->enabled)
            continue;
        g_currentFieldData = g_currentFieldPart->data;
        if (g_currentFieldPart->transformed) {
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->inverseTransform);
            Matrix4x4::TransformPoint(local, local);
        }
        result |= TestFieldPartSphereRegion(output, &local, radius);
        if (g_currentFieldPart->transformed) {
            matrix_stack::LoadMatrixPairs(&g_currentFieldPart->transform);
            Matrix4x4::TransformPoint(*output, *output);
            PopCurrentMatrix(1);
        }
        local = *output;
    }
    ResetFieldDataFlags();
    return result;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
float shenFindIntersectionWithScene(const Vector3 &position, float maximumY) {
    (void)maximumY;
    intersectionMask |= 8U;
    foundIntersection = false;
    float height = -std::numeric_limits<float>::max();
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;
        Vector3 point = part.transformed ? TransformPoint(part.worldToLocal, position) : position;
        // FindClosestIntersection_0 starts at its function entry.
        point.y = FindVerticalIntersection(part, partIndex, point, false);
        if (part.transformed)
            point = TransformPoint(part.localToWorld, point);

        if (point.y > height)
            height = point.y;
    }
    ResetQueryFlags();
    return height;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

float TransformClippedPoint(const Vector3 &position, float maximumY) {
    (void)maximumY;
    intersectionMask |= 8U;
    foundIntersection = false;
    float height = std::numeric_limits<float>::max();


    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;
        Vector3 point = part.transformed ? TransformPoint(part.worldToLocal, position) : position;
        point.y = 5000.0f;
        point.y = FindVerticalIntersection(part, partIndex, point, true);
        if (part.transformed)
            point = TransformPoint(part.localToWorld, point);
        if (point.y < height)
            height = point.y;
    }
    ResetQueryFlags();
    return height;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddFindIntersections(const Vector3 &origin, const Vector3 &destination) {
    foundIntersection = false;
    bool hit = false;
    // flddFindIntersections / FindIntersections: first polygon hit in
    // DDA/list order, then first enabled field owner. Never correct endpoints.
    for (std::size_t partIndex = 0; partIndex < fieldParts.size(); ++partIndex) {
        const auto &part = fieldParts[partIndex];
        if (!part.enabled)
            continue;
        const Vector3 start = part.transformed ? TransformPoint(part.worldToLocal, origin) : origin;
        const Vector3 end = part.transformed ? TransformPoint(part.worldToLocal, destination) : destination;
        hit = FindSegmentIntersection(part, partIndex, start, end, false, 0.0f);
        if (hit)
            break;
    }
    ResetQueryFlags();
    return hit;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool MoveObjectToPosFromThisScene(shen_name partName, std::int32_t entryId, Vector3 &position, std::int32_t &yaw) {
    FieldDataRuntimePart *const part = FindFieldDataPart(partName);
    if (!part || !part->enabled)
        return false;
    FieldDataRoot *const data = part->data;
    if (!data->auxiliaryLists)
        return false;
    for (void *const *list = data->auxiliaryLists; *list; ++list) {
        for (const std::int32_t *entry = reinterpret_cast<const std::int32_t *>(*list); *entry != -0xff; ++entry) {
            if (entry[0] != -2 || entry[1] != entryId || entry[2] != 0x1d)
                continue;
            const float *coordinates = reinterpret_cast<const float *>(entry + 3);
            position.x = coordinates[0];
            position.y = coordinates[1];
            position.z = coordinates[2];
            yaw = entry[6];
            if (part->transformed) {
                matrix_stack::Matrix4x4_Push(0);
                matrix_stack::LoadMatrixPairs(&part->transform);
                Matrix4x4::TransformPoint(position, position);
                PopCurrentMatrix(1);
                yaw = static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) + static_cast<std::uint32_t>(part->yaw));
            }
            return true;
        }
    }
    return false;
}

#else

// @verified - p
std::int32_t MoveObjectToPosFromThisScene(shen_name partName, std::int32_t entryId, Vector3 &position, std::int32_t &yaw) {
    const auto found = std::find_if(fieldParts.begin(), fieldParts.end(), [partName](const FieldDataPart &part) { return part.name.name == partName.name; });
    if (found == fieldParts.end() || !found->enabled || found->segmentOffsets[7] == 0) {
        return false;
    }

    const FieldDataPart &part = *found;
    for (std::size_t slot = part.segmentOffsets[7];; slot += sizeof(std::int32_t)) {
        const auto *const offset = SpanAt<std::int32_t>(part.sourceData, slot);
        if (!offset || *offset <= 0)
            return false;

        for (std::size_t cursor = static_cast<std::size_t>(*offset);; cursor += sizeof(std::int32_t)) {
            const auto *const marker = SpanAt<std::int32_t>(part.sourceData, cursor);
            if (!marker)
                return false;

            if (*marker == -255)
                break;

            if (*marker != -2)
                continue;

            const auto *const record = SpanAt<std::int32_t>(part.sourceData, cursor, 7);
            if (!record)
                return false;

            if (record[1] != entryId || record[2] != 29)
                continue;

            std::memcpy(&position, record + 3, sizeof(position));
            yaw = record[6];
            if (part.transformed) {
                position = TransformPoint(part.localToWorld, position);
                yaw += part.yaw;
            }
            return true;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddDecodeLastCategory5Metadata(FieldRadiusMetadata &output) {
    return DecodeCategory5Metadata(output);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddQueryRadiusMetadata(const Vector3 &intersection, FieldRadiusMetadata &output) {
    intersectionMask |= 0x20U;
    Vector3 radiusPoint = intersection;
    radiusPoint.y += 0.2f;
    if (!IsWithinRadiusFromVector3PolyIntersectTest(radiusPoint, 0.40000001f)) {
        return false;
    }
    return DecodeCategory5Metadata(output);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool flddHasFoundIntersection() {
    return foundIntersection;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int16_t Vector3_GetFixedYaw(const Vector3 *direction);

// @verified - d
std::int16_t GetFieldIntersectionNormalYaw() {
    FieldIntersectionState *state = &g_fieldIntersection;
    FieldDataRuntimePart *const part = state->part;
    if (!static_cast<std::uint8_t>(state->found))
        return 0;
    Vector3 normal = state->plane.normal;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::Transform(normal, normal);
        PopCurrentMatrix(1);
    }
    return static_cast<std::int16_t>(Vector3_GetFixedYaw(&normal) - 0x8000);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool flddHasFoundIntersectionOfType(std::int16_t flags) {
    if (!*reinterpret_cast<std::uint8_t *>(&g_fieldIntersection.found))
        return 0;
    if (g_fieldIntersection.flags & flags)
        return 1;
    return 0;
}

#else

bool flddHasFoundIntersectionOfType(std::uint16_t flags) {
    return foundIntersection && (lastIntersection.polygonFlags & flags) != 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

struct FieldIntersectionReference {
    shen_name part;
    std::int16_t polygon;
};

std::int32_t flddGetIntersectionReference(FieldIntersectionReference *reference) {
    if (!static_cast<std::uint8_t>(g_fieldIntersection.found))
        return 0;
    reference->part = g_fieldIntersection.part->name;
    reference->polygon = g_fieldIntersection.polygon;
    return 1;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t flddGetIntersectionNormal(Vector3 *normal, const FieldIntersectionReference *reference) {
    FieldDataRuntimePart *const part = FindFieldDataPart(reference->part);
    if (!part)
        return 0;
    const FieldDataRoot *const data = part->data;
    const FieldDataPolygon &polygon = data->polygons[reference->polygon];
    const Vector3 *const source = &data->normals[polygon.normalIndex];
    *normal = *source;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::Transform(*normal, *normal);
        PopCurrentMatrix(1);
    }
    return 1;
}

std::int32_t flddGetIntersectionNormalAngles(Vector3s *angles, const FieldIntersectionReference *reference) {
    Vector3 normal;
    FieldDataRuntimePart *const part = FindFieldDataPart(reference->part);
    if (!part)
        return 0;
    const FieldDataRoot *const data = part->data;
    const FieldDataPolygon &polygon = data->polygons[reference->polygon];
    const Vector3 *const source = &data->normals[polygon.normalIndex];
    normal = *source;
    if (part->transformed) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrixPairs(&part->transform);
        Matrix4x4::Transform(normal, normal);
        PopCurrentMatrix(1);
    }
    Vector3_ToFixed16EulerAngles(normal, *angles);
    angles->x = 0x4000 - angles->x;
    angles->y += 0x8000;
    return 1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

const std::uint32_t *flddGetCategoryRecord(std::uint32_t category) {
    return FindFieldCategoryRecord(category);
}

std::int32_t flddHasActiveIntersection() {
    if (!*reinterpret_cast<std::uint8_t *>(&g_fieldIntersection.found))
        return 0;
    if (g_fieldIntersection.flags & 1)
        return 0;
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

const FieldIntersection &flddGetLastIntersection() {
    return lastIntersection;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool flddIsValid() {
    g_currentFieldPart = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++g_currentFieldPart) {
        if (g_currentFieldPart->enabled)
            return true;
    }
    return false;
}

#else

bool flddIsValid() {
    return generatedVolumeCount != 0 || std::any_of(fieldParts.begin(), fieldParts.end(), [](const FieldDataPart &part) { return part.enabled != 0; });
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void flddToggleFlag(std::int16_t flags) {
    g_fieldDataFlags |= flags;
}

#else

// @verified - p
void flddToggleFlag(std::int16_t flags) {
    fieldDataFlags |= static_cast<std::uint16_t>(flags);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void FieldQueryNullsub() {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const std::uint32_t *FindFieldCategoryRecord(std::uint32_t category) {
    if (!static_cast<std::uint8_t>(g_fieldIntersection.found))
        return 0;
    const FieldDataRoot *const data = g_fieldIntersection.part->data;
    const FieldDataPolygon *const polygon = &data->polygons[g_fieldIntersection.polygon];
    if (((static_cast<std::int16_t>(polygon->flags) >> category) & 1) != 1)
        return 0;
    if (static_cast<std::int16_t>(polygon->dataGroup) == -1)
        return 0;
    const std::uint32_t *record = reinterpret_cast<const std::uint32_t *>(data->auxiliaryLists[static_cast<std::int16_t>(polygon->dataGroup)]);
    while (*record != fieldCategoryTerminator) {
        if (*record == ~category)
            return record + 1;
        ++record;
    }
    return 0;
}
#else
// @verified - d
const std::uint32_t *FindFieldCategoryRecord(std::uint32_t category) {
    if (!static_cast<std::uint8_t>(g_fieldIntersection.found))
        return 0;
    const FieldDataRoot *const data = g_fieldIntersection.part->data;
    const FieldDataPolygon &polygon = data->polygons[g_fieldIntersection.polygon];
    std::uint32_t flags = static_cast<std::uint32_t>(static_cast<std::int16_t>(polygon.flags));
    const std::int32_t shift = static_cast<std::int32_t>(0U - category);
    if (shift >= 0)
        flags <<= static_cast<std::uint32_t>(shift) & 31U;
    else if (!(static_cast<std::uint32_t>(shift) & 31U))
        flags = static_cast<std::int32_t>(flags) < 0 ? 0xffffffffU : 0U;
    else
        flags = static_cast<std::uint32_t>(static_cast<std::int32_t>(flags) >> ((0U - static_cast<std::uint32_t>(shift)) & 31U));
    if (!(flags & 1U))
        return 0;
    const std::int16_t group = static_cast<std::int16_t>(polygon.dataGroup);
    if (group == -1)
        return 0;
    const std::uint32_t *record = reinterpret_cast<const std::uint32_t *>(data->auxiliaryLists[group]);
    while (*record != fieldCategoryTerminator) {
        if (*record++ == ~category)
            return record;
    }
    return 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t flddTestMapEventIntersection(const Vector3 *position) {
    Vector3 query = *position;
    flddPushIntersectionFlag(2);
    query.y += 0.05f;
    const std::uint32_t result = IsWithinRadiusFromVector3PolyIntersectTest(&query, 0.1f);
    query.y -= 0.05f;
    if (!result) {
        flddPushIntersectionFlag(2);
        query.y += 0.5f;
        float distance = FindFieldIntersectionHeight(&query) - query.y;
        if (distance < 0.0f)
            distance = -distance;
        if (distance > 1.0f)
            return 0;
    }
    const std::uint32_t *const record = FindFieldCategoryRecord(1);
    if (!record)
        return 0;
    const std::uint32_t region = record[0];
    std::uint32_t secondary = record[1];
    if (secondary == 29)
        secondary = 0;
    const FieldDataRuntimePart *const base = FindFieldDataPart(FIELD_BASE_PART);
    if (!secondary && g_fieldIntersection.part != base)
        return 0;
    return (secondary << 16U) | region;
}

#else

std::uint32_t flddTestMapEventIntersection(const Vector3 &position) {
    Vector3 query = position;
    query.y += 0.050000001f;
    flddPushIntersectionFlag(2);
    const bool radiusHit = IsWithinRadiusFromVector3PolyIntersectTest(query, 0.1f);
    query.y += -0.050000001f;
    if (!radiusHit) {
        query.y += 0.5f;
        flddPushIntersectionFlag(2);
        const float height = shenFindIntersectionWithScene(query, query.y);
        if (std::fabs(height - query.y) > 1.0f) {
            return 0;
        }
    }

    std::int32_t region{};
    std::int32_t secondary{};
    if (!DecodeCategoryPair(1U, region, secondary))
        return 0;
    if (secondary == 29)
        secondary = 0;

    const bool basePart = lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].name.name == SHEN_NAME(0000);
    if (secondary == 0 && !basePart)
        return 0;
    return static_cast<std::uint16_t>(region) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>(secondary)) << 16U);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t GetFieldSoundProperties(const Vector3 *position) {
    flddPushIntersectionFlag(4);
    float distance = FindFieldIntersectionHeight(position) - position->y;
    if (distance < 0.0f)
        distance = -distance;
    if (distance > 2.0f)
        return 0;
    const std::uint32_t *const record = FindFieldCategoryRecord(2);
    std::uint32_t value;
    if (record) {
        const std::uint32_t low = record[0];
        value = (record[1] << 16U) | low;
    } else
        value = 0;
    return value;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint32_t flddQueryCategory6InRadius(const Vector3 *position) {
    Vector3 probe = *position;
    flddPushIntersectionFlag(0x40);
    probe.y += 0.8f;
    if (!IsWithinRadiusFromVector3PolyIntersectTest(&probe, 0.8f))
        return 0;
    const std::uint32_t *const record = FindFieldCategoryRecord(6);
    if (!record)
        return 0;
    return *record;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t plymDoIntersectionTest(const Vector3 *position) {
    flddPushIntersectionFlag(0x200U);
    float distance = FindFieldIntersectionHeight(position) - position->y;
    if (distance < 0.0f)
        distance = -distance;
    if (!(distance <= 1.0f))
        return 0;
    const std::uint32_t *const record = FindFieldCategoryRecord(9);
    if (!record)
        return 0;
    return record[0];
}
#endif

#else

std::int32_t plymDoIntersectionTest(const Vector3 &position, float maximumY) {
    flddPushIntersectionFlag(0x200);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (!(std::fabs(height - position.y) <= 1.0f)) {
        return 0;
    }
    std::int32_t scalar{};
    return flddDecodeCategoryScalar(9U, scalar) ? scalar : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t flddQueryDoorIntersection(const Vector3 &position) {

    Vector3 query = position;
    query.y += 0.80000001f;
    flddPushIntersectionFlag(0x40);
    if (!IsWithinRadiusFromVector3PolyIntersectTest(query, 0.80000001f)) {
        return 0;
    }
    std::int32_t scalar{};
    return flddDecodeCategoryScalar(6U, scalar) ? static_cast<std::uint32_t>(scalar) : 0U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ToggleFlagOnFieldEntity(shen_name name, std::int32_t enabled) {
    std::int32_t result = 0;
    std::uint32_t category = 8;
    if (*reinterpret_cast<const char *>(&name) == 'R')
        category = 6;
    FieldDataRuntimePart *part = g_fieldParts;
    for (std::int32_t index = 0; index < g_fieldPartCount; ++index, ++part) {
        if (!part->enabled || !part->partClass)
            continue;
        FieldDataRoot *const data = part->data;
        void *const *groups = data->auxiliaryLists;
        for (std::int32_t group = 0; *groups; ++group, ++groups) {
            const std::uint32_t *record = reinterpret_cast<const std::uint32_t *>(*groups);
            while (*record != fieldCategoryTerminator) {
                if (*record == ~category && record[1] == name) {
                    FieldDataPolygon *entry = data->polygons;
                    for (std::int32_t polygon = 0; polygon < data->metadata->polygonCount; ++polygon, ++entry) {
                        if (static_cast<std::int16_t>(entry->dataGroup) != group)
                            continue;
                        if (enabled)
                            entry->flags &= ~0x10U;
                        else
                            entry->flags |= 0x10U;
                        result = 1;
                    }
                }
                ++record;
            }
        }
    }
    return result;
}
#endif

#else

// @verified - p
std::int32_t ToggleFlagOnFieldEntity(shen_name entityName, std::int32_t toggle) {
    // ToggleFlagOnFieldEntity. Record ordinals, not the object
    // name or polygon number, select the signed face dataGroup.
    const std::int32_t selector = ~(static_cast<std::uint8_t>(entityName.name) == 'R' ? 6 : 8);
    bool matched = false;
    for (FieldDataPart &part : fieldParts) {
        if (!part.enabled || part.partClass == 0 || part.segmentOffsets[7] == 0)
            continue;

        std::int32_t ordinal = 0;
        for (std::size_t table = part.segmentOffsets[7];; table += sizeof(std::int32_t), ++ordinal) {
            const auto *offset = SpanAt<std::int32_t>(part.sourceData, table);
            if (!offset || *offset <= 0)
                break;

            for (std::size_t cursor = static_cast<std::size_t>(*offset);; cursor += sizeof(std::int32_t)) {
                const auto *marker = SpanAt<std::int32_t>(part.sourceData, cursor);
                if (!marker || *marker == -255)
                    break;

                if (*marker != selector)
                    continue;

                const auto *name = SpanAt<std::uint32_t>(part.sourceData, cursor + sizeof(std::int32_t));
                if (!name || *name != entityName.name)
                    continue;

                for (FieldDataPolygon &polygon : part.polygons) {
                    if (static_cast<std::int16_t>(polygon.dataGroup) != ordinal)
                        continue;

                    polygon.flags = static_cast<std::uint16_t>(toggle != 0 ? polygon.flags & ~0x10U : polygon.flags | 0x10U);
                    // Native reports a matching face even for an idempotent
                    // write; it does not report whether the bit changed.
                    matched = true;
                }
            }
        }
    }
    return matched;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetFieldDataFlags(std::int16_t flags) {
    const auto value = static_cast<std::uint16_t>(flags);
    fieldDataResetFlags |= value;
    fieldDataFlags |= value;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetFieldDataFlag(std::int16_t flags) {
    fieldDataClearFlags |= static_cast<std::uint16_t>(flags);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool InitializeGeneratedFieldData() {
    g_generatedFieldData = 0;
    FieldDataRoot *const root = static_cast<FieldDataRoot *>(HLib::AllocateTaskData(0x20, FIELD_DATA_STORAGE));
    if (!root)
        return false;
    root->vertices = static_cast<Vector3 *>(HLib::AllocateTaskData(0x12c0, FIELD_DATA_STORAGE));
    if (!root->vertices)
        return false;
    root->normals = static_cast<Vector3 *>(HLib::AllocateTaskData(0xe10, FIELD_DATA_STORAGE));
    if (!root->normals)
        return false;
    root->planeDistances = static_cast<float *>(HLib::AllocateTaskData(0x4b0, FIELD_DATA_STORAGE));
    if (!root->planeDistances)
        return false;
    root->polygons = static_cast<FieldDataPolygon *>(HLib::AllocateTaskData(0x12c0, FIELD_DATA_STORAGE));
    if (!root->polygons)
        return false;
    root->grid = 0;
    root->polygonLists = static_cast<std::int16_t **>(HLib::AllocateTaskData(4, FIELD_DATA_STORAGE));
    if (!root->polygonLists)
        return false;
    root->polygonLists[0] = static_cast<std::int16_t *>(HLib::AllocateTaskData(0x25a, FIELD_DATA_STORAGE));
    if (!root->polygonLists[0])
        return false;
    root->metadata = static_cast<FieldDataMetadata *>(HLib::AllocateTaskData(0x0c, FIELD_DATA_STORAGE));
    if (!root->metadata)
        return false;
    root->auxiliaryLists = static_cast<void **>(HLib::AllocateTaskData(4, FIELD_DATA_STORAGE));
    std::memset(root->vertices, 0, 0x12c0);
    std::memset(root->normals, 0, 0xe10);
    std::memset(root->planeDistances, 0, 0x4b0);
    std::memset(root->polygons, 0, 0x12c0);
    std::memset(root->polygonLists[0], 0, 0x25a);
    root->metadata->gridMode = -2.0f;
    root->metadata->polygonCount = 0;
    root->metadata->retained = 0.0f;
    root->auxiliaryLists[0] = 0;
    g_generatedFieldData = root;
    g_generatedFieldVolumeCount = 0;
    ParseFieldData(root, GENERATED_FIELD_NAME, 0);
    flddSetPointFlagOnGrid(GENERATED_FIELD_NAME, 0);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::int32_t yaw) {
    FieldDataRoot *const root = g_generatedFieldData;
    const std::int32_t index = g_generatedFieldVolumeCount;
    if (!root || index > 49)
        return false;
    Vector3 *const vertices = root->vertices + index * 8;
    Vector3 *const normals = root->normals + index * 6;
    float *const planes = root->planeDistances + index * 6;
    FieldDataPolygon *const polygons = root->polygons + index * 6;
    std::int16_t *const list = root->polygonLists[0] + index * 6;
    std::memcpy(vertices, generatedBoxVertices, sizeof(generatedBoxVertices));
    std::memcpy(normals, generatedBoxNormals, sizeof(generatedBoxNormals));
    planes[0] = planes[1] = planes[2] = planes[3] = 0.5f;
    planes[4] = 0;
    planes[5] = 1.0f;
    std::memcpy(polygons, generatedBoxPolygons, sizeof(generatedBoxPolygons));
    for (std::int32_t face = 0; face < 6; ++face)
        list[face] = static_cast<std::int16_t>(face);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    for (std::int32_t vertex = 0; vertex < 8; ++vertex)
        Matrix4x4::TransformPoint(vertices[vertex], vertices[vertex]);
    PopCurrentMatrix(1);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    if (mode == 1)
        Matrix4x4::Scale(-1, -1, -1);
    for (std::int32_t normal = 0; normal < 6; ++normal)
        Matrix4x4::Transform(normals[normal], normals[normal]);
    PopCurrentMatrix(1);
    for (std::int32_t face = 0; face < 6; ++face) {
        FieldDataPolygon &polygon = polygons[face];
        polygon.normalIndex = static_cast<std::int16_t>(polygon.normalIndex + index * 6);
        polygon.planeIndex = static_cast<std::int16_t>(polygon.planeIndex + index * 6);
        for (std::int32_t vertex = 0; vertex < 4; ++vertex)
            polygon.vertexIndices[vertex] = static_cast<std::int16_t>(polygon.vertexIndices[vertex] + index * 8);
        if (mode == 1) {
            const std::int16_t second = polygon.vertexIndices[1];
            polygon.vertexIndices[1] = polygon.vertexIndices[3];
            polygon.vertexIndices[3] = second;
        }
        list[face] = static_cast<std::int16_t>(list[face] + index * 6);
    }
    list[6] = -1;
    for (std::int32_t face = 0; face < 6; ++face) {
        const FieldDataPolygon &polygon = polygons[face];
        const Vector3 &point = root->vertices[polygon.vertexIndices[0]];
        const Vector3 &normal = root->normals[polygon.normalIndex];
        planes[face] = (point.x * normal.x + point.y * normal.y) + point.z * normal.z;
    }
    root->metadata->polygonCount += 6;
    ++g_generatedFieldVolumeCount;
    flddSetPointFlagOnGrid(GENERATED_FIELD_NAME, 1);
    return true;
}
#endif

#else

bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::uint16_t yaw) {
    if (!generatedRuntimeAllocated || generatedVolumeCount >= generatedVolumes.size())
        return false;

    FieldGeneratedVolume &volume = generatedVolumes[generatedVolumeCount++];
    volume = {};
    volume.mode = mode;
    volume.position = position;
    volume.scale = scale;
    volume.yaw = yaw;
    volume.polygons = kGeneratedBoxPolygons;

    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    for (std::size_t index = 0; index < volume.vertices.size(); ++index)
        Matrix4x4::TransformPoint(kGeneratedBoxVertices[index], volume.vertices[index]);
    Matrix4x4::Pop(1);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    if (mode == 1)
        Matrix4x4::Scale(-1.0f, -1.0f, -1.0f);
    for (std::size_t index = 0; index < volume.normals.size(); ++index)
        Matrix4x4::Transform(kGeneratedBoxNormals[index], volume.normals[index]);
    Matrix4x4::Pop(1);
    for (std::size_t index = 0; index < volume.normals.size(); ++index) {

        const FieldDataPolygon &polygon = volume.polygons[index];
        volume.planeDistances[index] = Dot(volume.normals[index], volume.vertices[polygon.vertexIndices[0]]);
        if (mode == 1) {
            std::swap(volume.polygons[index].vertexIndices[2], volume.polygons[index].vertexIndices[3]);
        }
    }
    if (mode == 2)
        volume.polygons[5].flags |= 0x10U;
    for (FieldDataPart &part : fieldParts) {
        if (part.name.name != kGeneratedVolumeName.name)
            continue;
        part.enabled = 1;
        if (part.data != flddRuntimeSpace)
            break;
        const std::uint16_t vertexBase = static_cast<std::uint16_t>(part.vertices.size());
        const std::uint16_t normalBase = static_cast<std::uint16_t>(part.normals.size());
        part.vertices.insert(part.vertices.end(), volume.vertices.begin(), volume.vertices.end());
        part.normals.insert(part.normals.end(), volume.normals.begin(), volume.normals.end());
        part.planeDistances.insert(part.planeDistances.end(), volume.planeDistances.begin(), volume.planeDistances.end());
        for (const FieldDataPolygon &source : volume.polygons) {
            FieldDataPolygon polygon = source;
            polygon.normalIndex += normalBase;
            polygon.field06 += normalBase;
            for (std::uint16_t &vertex : polygon.vertexIndices)
                vertex += vertexBase;
            generatedPolygonList[1U + part.polygons.size()] = static_cast<std::int16_t>(part.polygons.size());
            part.polygons.push_back(polygon);
        }
        generatedPolygonList[1U + part.polygons.size()] = -1;
        part.enabled = 1;
        break;
    }
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const FieldGeneratedVolume> flddGetGeneratedVolumes() {
    return {generatedVolumes.data(), generatedVolumeCount};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::span<const FieldDataPart> flddGetParts() {
    return fieldParts;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool flddClearAllGeneratedVolumes() {
    FieldDataRoot *const root = g_generatedFieldData;
    if (!root)
        return false;
    std::memset(root->vertices, 0, 0x12c0);
    std::memset(root->normals, 0, 0xe10);
    std::memset(root->planeDistances, 0, 0x4b0);
    std::memset(root->polygons, 0, 0x12c0);
    std::memset(root->polygonLists[0], 0, 0x25a);
    root->metadata->gridMode = -2.0f;
    root->metadata->polygonCount = 0;
    root->metadata->retained = 0;
    g_generatedFieldVolumeCount = 0;
    flddSetPointFlagOnGrid(GENERATED_FIELD_NAME, 0);
    return true;
}
#endif

#else

void flddClearAllGeneratedVolumes() {
    generatedVolumes = {};
    generatedVolumeCount = 0;
    generatedPolygonList = {};
    generatedPolygonList[1] = -1;
    for (FieldDataPart &part : fieldParts) {
        if (part.name.name != kGeneratedVolumeName.name)
            continue;
        part.enabled = 0;
        if (part.data != flddRuntimeSpace)
            break;
        part.vertices.clear();
        part.normals.clear();
        part.planeDistances.clear();
        part.polygons.clear();
    }
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_ToggleFlagOnFieldEntity(std::uint32_t name, std::int32_t enabled) {
    return shenmue::s2::ToggleFlagOnFieldEntity(shenmue::shen_name(name), enabled);
}
#endif

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_IsWithinRadiusFromVector3PolyIntersectTest(const shenmue::Vector3 *position, float radius) {
    return shenmue::s2::IsWithinRadiusFromVector3PolyIntersectTest(position, radius);
}
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_FindNearestRayIntersection(
    shenmue::Vector3 *output, const shenmue::Vector3 *destination, const shenmue::Vector3 *origin) {
    return shenmue::s2::FindNearestRayIntersection(output, destination, origin);
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetOccupancyForThisFieldSegment(
    std::uint32_t name, const shenmue::Vector3 *position, std::int32_t yaw) {
    return shenmue::s2::SetOccupancyForThisFieldSegment(shenmue::shen_name(name), position, yaw);
}
#endif
#else
#endif
