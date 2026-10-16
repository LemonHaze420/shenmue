#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace shenmue::s2 {

#pragma pack(push, 1)
struct FieldDataSourceRoot {
    std::array<std::uint32_t, 8> segmentOffsets;
};

struct FieldDataSourcePolygon {
    std::uint16_t flags;
    std::uint16_t dataGroup;
    std::uint16_t normalIndex;
    std::uint16_t field06;
    std::array<std::uint16_t, 4> vertexIndices;
};

struct FieldDataGridConfig {
    float minimumX;
    float minimumY;
    float minimumZ;
    float reciprocalCellSize;
    float width;
    float height;
    float depth;
};
#pragma pack(pop)

static_assert(sizeof(FieldDataSourceRoot) == 0x20);
static_assert(sizeof(FieldDataSourcePolygon) == 0x10);
static_assert(sizeof(FieldDataGridConfig) == 0x1c);
struct alignas(16) FieldDataRuntimePart {
    shen_name name;
    std::int32_t partClass;
    const void *data;
    std::int32_t enabled;
    std::int32_t transformed;
    std::int32_t reserved18;
    std::int32_t yaw;
    Vector3 translation;
    Matrix4x4 localToWorld{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
    Matrix4x4 worldToLocal{{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    }};
};

static_assert(sizeof(FieldDataRuntimePart) == 0xb0);
static_assert(offsetof(FieldDataRuntimePart, data) == 0x08);
static_assert(offsetof(FieldDataRuntimePart, enabled) == 0x10);
static_assert(offsetof(FieldDataRuntimePart, transformed) == 0x14);
static_assert(offsetof(FieldDataRuntimePart, yaw) == 0x1c);
static_assert(offsetof(FieldDataRuntimePart, translation) == 0x20);
static_assert(offsetof(FieldDataRuntimePart, localToWorld) == 0x2c);
static_assert(offsetof(FieldDataRuntimePart, worldToLocal) == 0x6c);

struct FieldDataPolygon {
    std::uint16_t flags;
    std::uint16_t dataGroup;
    std::uint16_t normalIndex;
    std::uint16_t field06;
    std::array<std::uint16_t, 4> vertexIndices;
};

namespace field_collision_data {
inline constexpr std::size_t kMaximumFieldParts = 8;
inline constexpr std::size_t kMaximumGeneratedVolumes = 50;
inline constexpr shen_name kGeneratedVolumeName{make_shen_name('@', '@', '@', '@')};
inline constexpr std::array<Vector3, 8> kGeneratedBoxVertices{{
    {-0.5f, 0.0f, -0.5f}, {-0.5f, 0.0f, 0.5f}, {-0.5f, 1.0f, 0.5f}, {-0.5f, 1.0f, -0.5f},
    {0.5f, 0.0f, 0.5f}, {0.5f, 1.0f, 0.5f}, {0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, -0.5f},
}};
inline constexpr std::array<Vector3, 6> kGeneratedBoxNormals{{
    {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
}};
inline constexpr std::array<FieldDataPolygon, 6> kGeneratedBoxPolygons{{
    {1U, 0xffffU, 0U, 0U, {0U, 1U, 2U, 3U}}, {1U, 0xffffU, 1U, 1U, {1U, 4U, 5U, 2U}},
    {1U, 0xffffU, 2U, 2U, {4U, 6U, 7U, 5U}}, {1U, 0xffffU, 3U, 3U, {6U, 0U, 3U, 7U}},
    {8U, 0xffffU, 4U, 4U, {6U, 4U, 1U, 0U}}, {8U, 0xffffU, 5U, 5U, {3U, 2U, 5U, 7U}},
}};
} // namespace field_collision_data



struct FieldGeneratedVolume {
    std::int32_t mode;
    Vector3 position;
    Vector3 scale;
    std::uint16_t yaw;
    std::uint16_t reserved;
    std::array<Vector3, 8> vertices;
    std::array<Vector3, 6> normals;
    std::array<float, 6> planeDistances;
    std::array<FieldDataPolygon, 6> polygons;
};

struct FieldDataPart : FieldDataRuntimePart {
    float gridMode;
    FieldDataGridConfig grid{};
    std::array<std::uint32_t, 8> segmentOffsets{};
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<float> planeDistances;
    std::vector<FieldDataPolygon> polygons;
    std::span<const std::byte> sourceData;
};

bool FLDD_ReadFieldData(const void *chunk, std::size_t chunkSize);
void FLDD_FreeFieldData();

std::int32_t ParseFLDD(std::span<const std::byte> data, shen_name name);

std::int32_t RemoveFieldPart(shen_name name);
bool flddIsValid();
std::span<const FieldDataPart> flddGetParts();
void flddClearAllGeneratedVolumes();
bool flddAssignGeneratedBox(std::int32_t mode, const Vector3 &position, const Vector3 &scale, std::uint16_t yaw);
std::span<const FieldGeneratedVolume> flddGetGeneratedVolumes();

struct FieldIntersection {
    Vector3 point;
    Vector3 planeNormal;
    float planeDistance;
    std::uint16_t polygonFlags;
    std::uint16_t polygonIndex;
    shen_name partName;
};

struct FieldGroundHit {
    Vector3 point;
    Vector3 planeNormal;
    float planeDistance;
    std::uint16_t polygonFlags;
};

struct FieldRadiusMetadata {
    std::uint32_t stepCount;
    Vector3 anchor;
    float maximumEdgeLength;
    std::int32_t wallYaw;
};
// Raw signed owner enable flag; missing name returns -1, successful write 0.
std::int32_t flddSetPointFlagOnGrid(shen_name partName, std::int32_t flag);
std::int32_t SetOccupancyForThisFieldSegment(shen_name partName, const Vector3 *translation, std::int32_t yaw);
// A nonzero toggle clears disabled bit 0x10; zero sets it. Returns 0/1 for a matched face.
std::int32_t ToggleFlagOnFieldEntity(shen_name entityName, std::int32_t toggle);
void SetFieldDataFlags(std::int16_t flags);
void SetFieldDataFlag(std::int16_t flags);
void flddToggleFlag(std::int16_t flags);
void flddPushIntersectionFlag(std::int16_t flags);


bool RaycastForCollision(Vector3 *intersectionPoint, const Vector3 *destination, const Vector3 *origin);
// FindNearestRayIntersection visits the origin/destination cells only, without DDA or
// conditional face-flag remapping; later owners receive the corrected endpoint.
std::int32_t FindNearestRayIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
// Radius correction visits each enabled owner, including the registered @@@@ part.
// Saved intersections remain local; the corrected output returns in world space.
std::int32_t RaycastInWorldGrid(Vector3 &output, const Vector3 &origin, float radius);
std::int32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 &point, float radius);
// Leaves output unchanged on no hit; reads the current owner's live transform.
void TransformAgainstIntersectionResult(Vector3 &point);


float shenFindIntersectionWithScene(const Vector3 &position, float maximumY);
float TransformClippedPoint(const Vector3 &position, float maximumY);
// Uncorrected, two-sided segment tests in enabled owner order.
bool flddFindIntersections(const Vector3 &origin, const Vector3 &destination);
bool GlobalRayIntersectionTest(const Vector3 &origin, const Vector3 &destination, float radius);
bool flddFindGround(const Vector3 &position, float maximumY, std::uint16_t requiredFlags, FieldGroundHit &output);
std::int32_t scenFindIntersection(const Vector3 &position, float maximumY);
std::uint32_t QueryGroundMaterialToken(const Vector3 &position, float maximumY);
std::uint32_t flddQueryCameraIntersection(const Vector3 &position);
std::int32_t flddQueryStairOrigin(const Vector3 &position, Vector3 &origin, std::int32_t &yaw);
std::int32_t flddQueryStairRadius(const Vector3 &position, Vector3 &origin, std::int32_t &yaw, float &maximumEdgeLength);
std::uint32_t flddQueryStairConnection(const Vector3 &position, float maximumY, Vector3 &anchor, std::int32_t &yaw, std::int32_t &connectionId);
std::int32_t plymDoIntersectionTest(const Vector3 &position, float maximumY);
std::uint32_t flddQueryDoorIntersection(const Vector3 &position);
std::uint32_t flddTestMapEventIntersection(const Vector3 &position);
std::int32_t MoveObjectToPosFromThisScene(shen_name partName, std::int32_t entryId, Vector3 &position, std::int32_t &yaw);
bool flddDecodeLastCategory5Metadata(FieldRadiusMetadata &output);
bool flddQueryRadiusMetadata(const Vector3 &intersection, FieldRadiusMetadata &output);
bool flddHasFoundIntersection();
bool flddHasFoundIntersectionOfType(std::uint16_t flags);
const FieldIntersection &flddGetLastIntersection();
void TransformIntersectionNormal(Vector3 &normal);
// The first value of the last intersected polygon's category record.
bool flddDecodeCategoryScalar(std::uint32_t category, std::int32_t &output);


void transformIntersectionResult(Vector3s &angles);

} // namespace shenmue::s2
