#include "s2/field_collision.hpp"

#include "s2/map_info.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {

using namespace field_collision_data;
std::array<FieldGeneratedVolume, kMaximumGeneratedVolumes> generatedVolumes{};
std::size_t generatedVolumeCount{};
FieldDataSourceRoot generatedRoot{};
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

template <typename T> const T *SpanAt(std::span<const std::byte> bytes, std::size_t offset, std::size_t count = 1) {
    if (offset > bytes.size() || count > (bytes.size() - offset) / sizeof(T)) {
        return nullptr;
    }
    return reinterpret_cast<const T *>(bytes.data() + offset);
}

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

float Dot(const Vector3 &left, const Vector3 &right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

float SquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float x = left.x - right.x;
    const float y = left.y - right.y;
    const float z = left.z - right.z;
    return x * x + y * y + z * z;
}


float PlanarSquaredDistance(const Vector3 &left, const Vector3 &right) {
    const float z = left.z - right.z;
    const float x = left.x - right.x;
    if (std::fabs(x) < std::numeric_limits<float>::epsilon() && std::fabs(z) < std::numeric_limits<float>::epsilon())
        return 0.0f;
    return z * z + x * x;
}

Vector3 TransformPoint(const Matrix4x4 &matrix, const Vector3 &point) {
    Vector3 output;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrix);
    Matrix4x4::TransformPoint(point, output);
    Matrix4x4::Pop(1);
    return output;
}

Vector3 TransformVector(const Matrix4x4 &matrix, const Vector3 &vector) {
    Vector3 output;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&matrix);
    Matrix4x4::Transform(vector, output);
    Matrix4x4::Pop(1);
    return output;
}

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

bool TestPointInsidePolygon(const FieldDataPart &part, const FieldDataPolygon &polygon, const Vector3 &point, const Vector3 &normal);

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

void ResetQueryFlags();

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

bool flddDecodeCategoryScalar(std::uint32_t category, std::int32_t &output) {
    const std::int32_t *const scalar = FindCategoryRecord(category, 1);
    if (!scalar)
        return false;
    output = *scalar;
    return true;
}

// @verified - p
void TransformIntersectionNormal(Vector3 &normal) {
    if (!foundIntersection)
        return;
    normal = lastIntersection.planeNormal;
    if (lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].transformed)
        normal = TransformVector(fieldParts[lastRadiusPartIndex].localToWorld, normal);
}

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

// @verified - p
void TransformAgainstIntersectionResult(Vector3 &point) {
    if (!foundIntersection)
        return;
    point = lastIntersection.point;
    if (lastRadiusPartIndex < fieldParts.size() && fieldParts[lastRadiusPartIndex].transformed)
        point = TransformPoint(fieldParts[lastRadiusPartIndex].localToWorld, point);
}

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

namespace {

bool DecodeCategoryPair(std::uint32_t category, std::int32_t &first, std::int32_t &second) {
    const std::int32_t *const pair = FindCategoryRecord(category, 2);
    if (!pair)
        return false;
    first = pair[0];
    second = pair[1];
    return true;
}

void ResetQueryFlags() {
    intersectionMask = 0;
    fieldDataFlags = 0x10;
    fieldDataClearFlags = 0;
    fieldDataResetFlags = 0;
}

} // namespace

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
                generated.data = &generatedRoot;
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

std::int32_t RemoveFieldPart(shen_name name) {
    for (std::size_t index = 0; index < fieldParts.size(); ++index) {
        if (fieldParts[index].name.name != name.name)
            continue;
        fieldParts.erase(fieldParts.begin() + static_cast<std::ptrdiff_t>(index));
        return 0;
    }
    return -1;
}

void FLDD_FreeFieldData() {
    flddClearAllGeneratedVolumes();
    fieldParts.clear();
    generatedRuntimeAllocated = false;
    lastRadiusPartIndex = std::numeric_limits<std::size_t>::max();
    foundIntersection = false;
    lastIntersection = {};
    ResetQueryFlags();
}

bool flddIsValid() {
    return generatedVolumeCount != 0 || std::any_of(fieldParts.begin(), fieldParts.end(), [](const FieldDataPart &part) { return part.enabled != 0; });
}

void flddClearAllGeneratedVolumes() {
    generatedVolumes = {};
    generatedVolumeCount = 0;
    generatedPolygonList = {};
    generatedPolygonList[1] = -1;
    for (FieldDataPart &part : fieldParts) {
        if (part.name.name != kGeneratedVolumeName.name)
            continue;
        part.enabled = 0;
        if (part.data != &generatedRoot)
            break;
        part.vertices.clear();
        part.normals.clear();
        part.planeDistances.clear();
        part.polygons.clear();
    }
}

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
        if (part.data != &generatedRoot)
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

std::span<const FieldGeneratedVolume> flddGetGeneratedVolumes() {
    return {generatedVolumes.data(), generatedVolumeCount};
}

std::span<const FieldDataPart> flddGetParts() {
    return fieldParts;
}
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


                    matched = true;
                }
            }
        }
    }
    return matched;
}

void SetFieldDataFlags(std::int16_t flags) {
    const auto value = static_cast<std::uint16_t>(flags);
    fieldDataResetFlags |= value;
    fieldDataFlags |= value;
}

void SetFieldDataFlag(std::int16_t flags) {
    fieldDataClearFlags |= static_cast<std::uint16_t>(flags);
}

// @verified - p
void flddToggleFlag(std::int16_t flags) {
    fieldDataFlags |= static_cast<std::uint16_t>(flags);
}

// @verified - p
void flddPushIntersectionFlag(std::int16_t flags) {
    intersectionMask |= static_cast<std::uint16_t>(flags);
}

bool RaycastForCollision(Vector3 *intersectionPoint, const Vector3 *destination, const Vector3 *origin) {
    if (!intersectionPoint || !destination || !origin) {
        ResetQueryFlags();
        return false;
    }
    *intersectionPoint = *destination;
    foundIntersection = false;
    bool hit = false;




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

std::int32_t scenFindIntersection(const Vector3 &position, float maximumY) {
    flddPushIntersectionFlag(0x80);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (!(std::fabs(height - position.y) <= 2.0f)) {
        return -1;
    }
    std::int32_t scalar{};
    return flddDecodeCategoryScalar(7U, scalar) ? scalar : -1;
}

std::uint32_t QueryGroundMaterialToken(const Vector3 &position, float maximumY) {

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

// @verified - p
std::int32_t flddQueryStairOrigin(const Vector3 &position, Vector3 &origin, std::int32_t &yaw) {


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

std::int32_t plymDoIntersectionTest(const Vector3 &position, float maximumY) {
    flddPushIntersectionFlag(0x200);
    const float height = shenFindIntersectionWithScene(position, maximumY);
    if (!(std::fabs(height - position.y) <= 1.0f)) {
        return 0;
    }
    std::int32_t scalar{};
    return flddDecodeCategoryScalar(9U, scalar) ? scalar : 0;
}

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

bool flddDecodeLastCategory5Metadata(FieldRadiusMetadata &output) {
    return DecodeCategory5Metadata(output);
}

bool flddQueryRadiusMetadata(const Vector3 &intersection, FieldRadiusMetadata &output) {
    intersectionMask |= 0x20U;
    Vector3 radiusPoint = intersection;
    radiusPoint.y += 0.2f;
    if (!IsWithinRadiusFromVector3PolyIntersectTest(radiusPoint, 0.40000001f)) {
        return false;
    }
    return DecodeCategory5Metadata(output);
}

bool flddHasFoundIntersection() {
    return foundIntersection;
}

bool flddHasFoundIntersectionOfType(std::uint16_t flags) {
    return foundIntersection && (lastIntersection.polygonFlags & flags) != 0;
}

const FieldIntersection &flddGetLastIntersection() {
    return lastIntersection;
}

} // namespace shenmue::s2
