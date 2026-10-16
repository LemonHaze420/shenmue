#include "s1/collision.hpp"

#include "s1/hlib.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/wexe.hpp"
#include "s1/task/dswk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/boot.hpp"
#include "s1/scene_memory.hpp"
#include "s1/player_control.hpp"
#include "s1/motion.hpp"

#include <algorithm>
#include <bit>
#include <array>
#include <functional>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

CharacterCollisionWork *g_characterCollisionHead = nullptr;

std::uint32_t g_skipAuxiliaryCollisionRefresh = 0;
float g_characterGroundDropLimit = 0.07f;
std::int32_t g_navStateNext = 0;
std::int32_t g_navMetricNext = 0;
NavRegion *g_navRegionListHead = nullptr;
std::int32_t g_navigationRegionCount = 0;
const NavQuad *g_currentNavTriangle = nullptr;
void *g_activeMapQuerySet = nullptr;

namespace {

// CEWP point/segment query cache: kind 1 holds a map record, kind 2 a character or auxiliary shape.
bool g_cewpMapTestsEnabled = false;
bool g_cewpCharacterTestsEnabled = false;
bool g_cewpCacheValid = false;
std::int16_t g_cewpCacheKind = 0;
std::int16_t g_cewpCacheControl = 0;
const std::uint32_t *g_cewpCachedRecord = nullptr;
const QueryShape *g_cewpCachedShape = nullptr;

struct MapQuerySet;

struct MapQueryRuntimeEntry {
    std::uint32_t name;
    std::uint32_t reserved_04;
    MapQuerySet *querySet;
};

struct MapInfoSubchunk {
    std::uint32_t token;
    std::int32_t size;
};

struct CollisionSectionEntry {
    std::uint32_t name;
    std::uint32_t nextOffset;
    MapInfoSubchunk margin;
};

struct MapQuerySet {
    MapInfoSubchunk *collision;
    std::uint32_t collisionSize;
    std::uint32_t reserved_0c;
    MapInfoSubchunk *height;
    std::uint32_t heightSize;
    std::uint32_t reserved_1c;
    MapInfoSubchunk *event;
    std::uint32_t eventSize;
    std::uint32_t reserved_2c;
    MapInfoSubchunk *undo;
    std::uint32_t undoSize;
    std::uint32_t reserved_3c;
    MapInfoSubchunk *sound;
    std::uint32_t soundSize;
    std::uint32_t reserved_4c;
    MapInfoSubchunk *property;
    std::uint32_t propertySize;
    std::uint32_t reserved_5c;
    std::uint32_t *sectionOffsetTable;
};

static_assert(sizeof(MapQueryRuntimeEntry) == 0x10);
static_assert(sizeof(MapInfoSubchunk) == 8);
static_assert(sizeof(CollisionSectionEntry) == 0x10);
static_assert(sizeof(MapQuerySet) == 0x68);

std::array<std::uint32_t, 4> g_collisionSelectionState{};
const std::uint32_t *g_selectedGroundFunction = nullptr;
std::int32_t g_groundIntersectionUsesAlternateSurface = 0;
std::int32_t g_collisionReady = 1;

struct CollisionQueryTransform {
    bool active = false;
    Matrix4x4 matrix{};
};

CollisionQueryTransform g_collisionQueryTransform{};
std::array<std::int32_t, 10> g_taskCollisionCache{-1, -1};
CharacterCollisionWork *g_characterCollisionIterator = nullptr;
std::array<AuxiliaryCollisionEntry, 32> g_auxiliaryCollisionEntries{};
std::uint32_t g_auxiliaryCollisionCount = 0;
std::uint32_t g_auxiliaryCollisionCursor = 0;

MapQuerySet *COLS_CreateMapQuerySet(MapInfoSubchunk *subchunk) {
    auto *const query = static_cast<MapQuerySet *>(HLib_AllocHeapMemBlock(sizeof(MapQuerySet), SHEN_NAME(MRGN)));
    if (!query) {
        return nullptr;
    }
    std::memset(query, 0, sizeof(*query));

    while (subchunk->token || subchunk->size) {
        switch (subchunk->token) {
        case SHEN_NAME(COLI):
            query->collision = subchunk;
            query->collisionSize = static_cast<std::uint32_t>(subchunk->size);
            break;
        case SHEN_NAME(HGHT):
            query->height = subchunk;
            query->heightSize = static_cast<std::uint32_t>(subchunk->size);
            break;
        case SHEN_NAME(EVNT):
            query->event = subchunk;
            query->eventSize = static_cast<std::uint32_t>(subchunk->size);
            break;
        case SHEN_NAME(UNDU):
            query->undo = subchunk;
            query->undoSize = static_cast<std::uint32_t>(subchunk->size);
            break;
        case SHEN_NAME(SOND):
            query->sound = subchunk;
            query->soundSize = static_cast<std::uint32_t>(subchunk->size);
            break;
        case SHEN_NAME(PROP):
            query->property = subchunk;
            query->propertySize = static_cast<std::uint32_t>(subchunk->size);
            break;
        default:
            break;
        }
        subchunk = reinterpret_cast<MapInfoSubchunk *>(reinterpret_cast<std::byte *>(subchunk) + subchunk->size + 8);
    }

    query->sectionOffsetTable = reinterpret_cast<std::uint32_t *>(subchunk + 1);
    g_weatherForecastValue = (GetFCAS() >> 1) & 1;
    return query;
}

struct CollisionVector2 {
    float x;
    float y;
};

struct CollisionGridHeader {
    std::byte reserved00[0x0c];
    std::int32_t width;
    std::int32_t height;
    float minX;
    std::byte reserved18[8];
    float zOrigin;
    float spanX;
    float spanZ;
    float cellSizeX;
    float cellSizeZ;
    float borderMargin;
};

struct CollisionGridView {
    const CollisionGridHeader *header = nullptr;
    const std::int32_t *cellOffsets = nullptr;
    const std::byte *candidates = nullptr;
};

struct PrimitiveContact {
    bool hit = false;
    bool startedOverlapping = false;
    bool escapedInitialOverlap = false;
    float fraction = 1.0f;
    CollisionVector2 normal{};
};

constexpr float kCollisionEpsilon = 0.000001f;



bool MapInfoRangeValid(const void *pointer, std::size_t) {
    return pointer != nullptr;
}

template <typename T> bool ReadMapInfo(const void *pointer, T &value) {
    if (!MapInfoRangeValid(pointer, sizeof(T)))
        return false;
    std::memcpy(&value, pointer, sizeof(T));
    return true;
}

std::uint32_t PresentQueryChunkCount(const MapQuerySet &query) {
    return static_cast<std::uint32_t>(query.collision != nullptr) + static_cast<std::uint32_t>(query.height != nullptr) +
        static_cast<std::uint32_t>(query.event != nullptr) + static_cast<std::uint32_t>(query.undo != nullptr) +
        static_cast<std::uint32_t>(query.sound != nullptr) + static_cast<std::uint32_t>(query.property != nullptr);
}



bool BindQueryGrid(const MapQuerySet &query, std::uint32_t queryIndex, CollisionGridView &grid) {
    if (!query.sectionOffsetTable)
        return false;
    const std::uint32_t count = PresentQueryChunkCount(query);
    if (queryIndex >= count)
        return false;

    std::int32_t offset = -1;
    std::memcpy(&offset, query.sectionOffsetTable + queryIndex, sizeof(offset));
    if (offset == -1 || offset % 4 != 0)
        return false;
    const auto *const headerBytes = reinterpret_cast<const std::byte *>(query.sectionOffsetTable + count) + offset;
    std::int32_t cellOffset = 0;
    std::int32_t cellCount = 0;
    std::memcpy(&cellOffset, headerBytes + 4, sizeof(cellOffset));
    std::memcpy(&cellCount, headerBytes + 8, sizeof(cellCount));
    const auto *const cellOffsets = headerBytes + cellOffset;
    const auto *const candidates =
        cellOffsets + static_cast<std::ptrdiff_t>(cellCount) * static_cast<std::ptrdiff_t>(sizeof(std::int32_t)) + sizeof(std::int32_t);

    grid.header = reinterpret_cast<const CollisionGridHeader *>(headerBytes);
    grid.cellOffsets = reinterpret_cast<const std::int32_t *>(cellOffsets);
    grid.candidates = candidates;
    return grid.header->width > 0 && grid.header->height > 0 && grid.header->cellSizeX > 0.0f && grid.header->cellSizeZ > 0.0f;
}

bool BindCollisionGrid(const MapQuerySet &query, CollisionGridView &grid) {
    return query.collision && BindQueryGrid(query, 0, grid);
}

std::int32_t QueryGridIndex(const MapQuerySet &query, const MapInfoSubchunk *chunk) {
    const std::array<const MapInfoSubchunk *, 6> chunks{
        query.collision,
        query.height,
        query.event,
        query.undo,
        query.sound,
        query.property,
    };
    std::int32_t index = 0;
    for (const MapInfoSubchunk *const candidate : chunks) {
        if (!candidate)
            continue;
        if (candidate == chunk)
            return index;
        ++index;
    }
    return -1;
}

float Dot(CollisionVector2 a, CollisionVector2 b) {
    return a.x * b.x + a.y * b.y;
}

CollisionVector2 Subtract(CollisionVector2 a, CollisionVector2 b) {
    return {a.x - b.x, a.y - b.y};
}

CollisionVector2 AddScaled(CollisionVector2 a, CollisionVector2 b, float scale) {
    return {a.x + b.x * scale, a.y + b.y * scale};
}

CollisionVector2 Normalize(CollisionVector2 value, CollisionVector2 fallback) {
    float lengthSquared = Dot(value, value);
    if (lengthSquared <= kCollisionEpsilon * kCollisionEpsilon) {
        value = fallback;
        lengthSquared = Dot(value, value);
        if (lengthSquared <= kCollisionEpsilon * kCollisionEpsilon)
            return {1.0f, 0.0f};
    }
    const float inverseLength = 1.0f / std::sqrt(lengthSquared);
    return {value.x * inverseLength, value.y * inverseLength};
}

void SelectEarlierContact(PrimitiveContact &best, const PrimitiveContact &candidate, CollisionVector2 movement) {
    best.escapedInitialOverlap |= candidate.escapedInitialOverlap;
    if (!candidate.hit)
        return;
    const bool earlier = !best.hit || candidate.fraction < best.fraction - kCollisionEpsilon;
    const bool equal = best.hit && std::fabs(candidate.fraction - best.fraction) <= kCollisionEpsilon;
    if (earlier || (equal && Dot(movement, candidate.normal) < Dot(movement, best.normal))) {
        const bool escaped = best.escapedInitialOverlap;
        best = candidate;
        best.escapedInitialOverlap |= escaped;
    }
}

bool ResolveInitialOverlap(CollisionVector2 movement, CollisionVector2 requestedNormal, PrimitiveContact &result) {
    const CollisionVector2 normal = Normalize(requestedNormal, movement);
    result.startedOverlapping = true;
    if (Dot(movement, normal) >= -kCollisionEpsilon) {
        result.escapedInitialOverlap = true;
        return false;
    }
    result.hit = true;
    result.fraction = 0.0f;
    result.normal = normal;
    return true;
}

PrimitiveContact SweepCircleAgainstPoint(CollisionVector2 start, CollisionVector2 movement, float radius, CollisionVector2 point) {
    PrimitiveContact result{};
    radius = std::max(0.0f, radius);
    const CollisionVector2 fromPoint = Subtract(start, point);
    const float distanceSquared = Dot(fromPoint, fromPoint);
    if (distanceSquared <= radius * radius + kCollisionEpsilon) {
        ResolveInitialOverlap(movement, fromPoint, result);
        return result;
    }
    const float a = Dot(movement, movement);
    if (a <= kCollisionEpsilon * kCollisionEpsilon)
        return result;
    const float b = 2.0f * Dot(fromPoint, movement);
    const float c = distanceSquared - radius * radius;
    const float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f)
        return result;
    const float root = (-b - std::sqrt(std::max(0.0f, discriminant))) / (2.0f * a);
    if (root < -kCollisionEpsilon || root > 1.0f + kCollisionEpsilon)
        return result;
    result.hit = true;
    result.fraction = std::clamp(root, 0.0f, 1.0f);
    result.normal = Normalize(Subtract(AddScaled(start, movement, result.fraction), point), {-movement.x, -movement.y});
    return result;
}

CollisionVector2 ClosestPointOnSegment(CollisionVector2 point, CollisionVector2 a, CollisionVector2 b) {
    const CollisionVector2 edge = Subtract(b, a);
    const float lengthSquared = Dot(edge, edge);
    if (lengthSquared <= kCollisionEpsilon * kCollisionEpsilon)
        return a;
    const float amount = std::clamp(Dot(Subtract(point, a), edge) / lengthSquared, 0.0f, 1.0f);
    return AddScaled(a, edge, amount);
}

PrimitiveContact SweepCircleAgainstSegment(CollisionVector2 start, CollisionVector2 movement, float radius, CollisionVector2 a, CollisionVector2 b) {
    radius = std::max(0.0f, radius);
    const CollisionVector2 startNormal = Subtract(start, ClosestPointOnSegment(start, a, b));
    if (Dot(startNormal, startNormal) <= radius * radius + kCollisionEpsilon) {
        PrimitiveContact overlap{};
        CollisionVector2 normal = startNormal;
        if (Dot(normal, normal) <= kCollisionEpsilon * kCollisionEpsilon) {
            const CollisionVector2 edge = Subtract(b, a);
            normal = {-edge.y, edge.x};
            if (Dot(movement, normal) < 0.0f)
                normal = {-normal.x, -normal.y};
        }
        ResolveInitialOverlap(movement, normal, overlap);
        return overlap;
    }

    PrimitiveContact best{};
    const CollisionVector2 edge = Subtract(b, a);
    const float edgeLengthSquared = Dot(edge, edge);
    if (edgeLengthSquared <= kCollisionEpsilon * kCollisionEpsilon)
        return SweepCircleAgainstPoint(start, movement, radius, a);
    const float edgeLength = std::sqrt(edgeLengthSquared);
    const CollisionVector2 tangent{edge.x / edgeLength, edge.y / edgeLength};
    const CollisionVector2 lineNormal{-tangent.y, tangent.x};
    const float signedStart = Dot(Subtract(start, a), lineNormal);
    const float signedMovement = Dot(movement, lineNormal);
    if (std::fabs(signedMovement) > kCollisionEpsilon) {
        for (const float side : {-1.0f, 1.0f}) {
            const float amount = (side * radius - signedStart) / signedMovement;
            if (amount < -kCollisionEpsilon || amount > 1.0f + kCollisionEpsilon)
                continue;
            const float clamped = std::clamp(amount, 0.0f, 1.0f);
            const CollisionVector2 center = AddScaled(start, movement, clamped);
            const float along = Dot(Subtract(center, a), tangent);
            if (along < -kCollisionEpsilon || along > edgeLength + kCollisionEpsilon)
                continue;
            PrimitiveContact candidate{};
            candidate.hit = true;
            candidate.fraction = clamped;
            candidate.normal = {lineNormal.x * side, lineNormal.y * side};
            SelectEarlierContact(best, candidate, movement);
        }
    }
    SelectEarlierContact(best, SweepCircleAgainstPoint(start, movement, radius, a), movement);
    SelectEarlierContact(best, SweepCircleAgainstPoint(start, movement, radius, b), movement);
    return best;
}

bool PointOnSegment(CollisionVector2 point, CollisionVector2 a, CollisionVector2 b) {
    const float cross = (b.x - a.x) * (point.y - a.y) - (b.y - a.y) * (point.x - a.x);
    return std::fabs(cross) <= 0.00001f && point.x >= std::min(a.x, b.x) - 0.00001f && point.x <= std::max(a.x, b.x) + 0.00001f &&
        point.y >= std::min(a.y, b.y) - 0.00001f && point.y <= std::max(a.y, b.y) + 0.00001f;
}

bool PointInPolygon(CollisionVector2 point, const CollisionVector2 *points, std::size_t count) {
    if (count < 3)
        return false;
    bool inside = false;
    for (std::size_t index = 0, previous = count - 1; index < count; previous = index++) {
        const CollisionVector2 a = points[index];
        const CollisionVector2 b = points[previous];
        if (PointOnSegment(point, a, b))
            return true;
        if (((a.y > point.y) != (b.y > point.y)) && point.x < (b.x - a.x) * (point.y - a.y) / (b.y == a.y ? kCollisionEpsilon : b.y - a.y) + a.x) {
            inside = !inside;
        }
    }
    return inside;
}

PrimitiveContact SweepCircleAgainstPolyline(
    CollisionVector2 start, CollisionVector2 movement, float radius, const CollisionVector2 *points, std::size_t count, bool closed) {
    PrimitiveContact best{};
    if (!count)
        return best;
    if (count == 1)
        return SweepCircleAgainstPoint(start, movement, radius, points[0]);
    if (closed && PointInPolygon(start, points, count)) {
        float closestDistanceSquared = std::numeric_limits<float>::infinity();
        CollisionVector2 closest = start;
        for (std::size_t index = 0; index < count; ++index) {
            const CollisionVector2 candidate = ClosestPointOnSegment(start, points[index], points[(index + 1) % count]);
            const CollisionVector2 delta = Subtract(candidate, start);
            const float distanceSquared = Dot(delta, delta);
            if (distanceSquared < closestDistanceSquared) {
                closestDistanceSquared = distanceSquared;
                closest = candidate;
            }
        }
        ResolveInitialOverlap(movement, Subtract(closest, start), best);
        return best;
    }
    const std::size_t edgeCount = closed ? count : count - 1;
    for (std::size_t index = 0; index < edgeCount; ++index) {
        SelectEarlierContact(best, SweepCircleAgainstSegment(start, movement, radius, points[index], points[(index + 1) % count]), movement);
    }
    return best;
}

bool ReadCollisionPoints(const std::byte *data, std::array<CollisionVector2, 128> &points, std::size_t &count) {
    std::uint32_t storedCount = 0;
    if (!ReadMapInfo(data, storedCount) || !storedCount || storedCount > points.size() ||
        !MapInfoRangeValid(data + sizeof(storedCount), storedCount * sizeof(CollisionVector2))) {
        return false;
    }
    count = storedCount;
    std::memcpy(points.data(), data + sizeof(storedCount), count * sizeof(CollisionVector2));
    return true;
}

PrimitiveContact SweepCircleAgainstRecord(std::uint32_t functionIndex, const std::byte *data, CollisionVector2 start, CollisionVector2 movement, float radius) {
    std::array<CollisionVector2, 128> points{};
    std::size_t count = 0;
    switch (functionIndex) {
    case 0:
        if (!ReadMapInfo(data, points[0]))
            return {};
        return SweepCircleAgainstPoint(start, movement, radius, points[0]);
    case 1:
        if (!MapInfoRangeValid(data, 2 * sizeof(CollisionVector2)))
            return {};
        std::memcpy(points.data(), data, 2 * sizeof(CollisionVector2));
        return SweepCircleAgainstSegment(start, movement, radius, points[0], points[1]);
    case 2:
    case 6:
    case 7:
    case 8:
        if (!ReadCollisionPoints(data, points, count))
            return {};
        return SweepCircleAgainstPolyline(start, movement, radius, points.data(), count, false);
    case 3: {
        float shapeRadius = 0.0f;
        if (!ReadMapInfo(data, shapeRadius) || !ReadMapInfo(data + sizeof(shapeRadius), points[0]))
            return {};
        return SweepCircleAgainstPoint(start, movement, radius + std::max(0.0f, shapeRadius), points[0]);
    }
    case 4:
        count = 3;
        if (!MapInfoRangeValid(data, count * sizeof(CollisionVector2)))
            return {};
        std::memcpy(points.data(), data, count * sizeof(CollisionVector2));
        return SweepCircleAgainstPolyline(start, movement, radius, points.data(), count, true);
    case 5: {
        float values[6]{};
        if (!ReadMapInfo(data, values))
            return {};
        points[0] = {values[0], values[1]};
        points[1] = {values[0] + values[2], values[1] + values[3]};
        points[2] = {points[1].x + values[4], points[1].y + values[5]};
        points[3] = {values[0] + values[4], values[1] + values[5]};
        return SweepCircleAgainstPolyline(start, movement, radius, points.data(), 4, true);
    }
    default:
        return {};
    }
}

int GridCoordinate(float value, float origin, float cellSize, int count) {
    return std::clamp(static_cast<int>((value - origin) / cellSize), 0, count - 1);
}

void *HLib_GetTaskParamPtr(Character *character, std::uint32_t componentId) {
    CharacterComponentNode *node = character ? character->componentTree : nullptr;
    while (node) {
        if (node->componentId == componentId)
            return node + 1;
        node = node->componentId < componentId ? node->greaterChild : node->lessChild;
    }
    return nullptr;
}

CharacterCollisionWork *FindCharacterCollisionWork(Character *character) {
    return static_cast<CharacterCollisionWork *>(HLib_GetTaskParamPtr(character, SHEN_NAME(CCOW)));
}

const CharacterCollisionWork *FindCharacterCollisionWork(const Character *character) {
    return FindCharacterCollisionWork(const_cast<Character *>(character));
}

void shenGetCollidedPos(Character *character, const QueryShape &shape, QueryShape &output) {
    output.mode = shape.mode;
    float startX = shape.packed[0].x;
    float startZ = shape.packed[0].y;
    Vector3 point{startX, 0.0f, startZ};
    CharacterNode *const node = shenGetNodeFromChara(character);
    if (node && node->child) {
        point = WorldToEntityLocalPoint(*node->child, point);
        startX = point.x;
        startZ = point.z;
    }

    float endX = shape.packed[0].x + shape.packed[0].z;
    float endZ = shape.packed[0].y + shape.packed[1].x;
    point = {endX, 0.0f, endZ};
    if (node && node->child) {
        point = WorldToEntityLocalPoint(*node->child, point);
        endX = point.x;
        endZ = point.z;
    }
    output.packed[0] = {startX, startZ, endX - startX};
    output.packed[1].x = endZ - startZ;

    float thirdX = shape.packed[0].x + shape.packed[1].y;
    float thirdZ = shape.packed[0].y + shape.packed[1].z;
    point = {thirdX, 0.0f, thirdZ};
    if (node && node->child) {
        point = WorldToEntityLocalPoint(*node->child, point);
        thirdX = point.x;
        thirdZ = point.z;
    }
    output.packed[1].y = thirdX - startX;
    output.packed[1].z = thirdZ - startZ;
}

bool GetCewpFlag(const Character *character) {
    if (!character || character->entity.name != SHEN_NAME(CEWP))
        return false;
    const std::uint32_t flags = character->flags.value;
    return (flags & 1U) != 0 || ((flags & 2U) != 0 && (character->motionFlags & 1U) != 0);
}

void BuildCharacterCollisionShape(QueryShape &shape, const Vector3 &origin, std::uint16_t yaw, const Vector3 &extents) {
    if (shape.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange) {
        shape.packed[0] = {extents.x, origin.x, origin.z};
        return;
    }
    if (shape.mode != OrientedBox_OrientedRectXZ_IsPointInTriangle)
        return;

    const float sine = sin_fixed16(yaw);
    const float cosine = sin_fixed16(fixed16_cosine_angle(yaw));
    const float edgeAX = (extents.x + extents.x) * cosine;
    const float edgeAZ = extents.x * -2.0f * sine;
    const float edgeBX = (extents.z + extents.z) * sine;
    const float edgeBZ = (extents.z + extents.z) * cosine;
    shape.packed[0] = {origin.x - (edgeBX + edgeAX) * 0.5f, origin.z - (edgeBZ + edgeAZ) * 0.5f, edgeAX};
    shape.packed[1] = {edgeAZ, edgeBX, edgeBZ};
}

bool CircleOverlapsPolyline(CollisionVector2 center, float radius, const CollisionVector2 *points, std::size_t count, bool closed);

bool GridMayContainSweep(const CollisionGridView &grid, const Vector3 &start, const Vector3 &end, float radius) {
    if (grid.header->spanX <= 0.0f || grid.header->spanZ <= 0.0f)
        return true;
    const float margin = std::max(0.25f, grid.header->borderMargin);
    const float gridMinX = grid.header->minX - margin;
    const float gridMaxX = grid.header->minX + grid.header->spanX + margin;
    const float gridMinZ = -grid.header->zOrigin - margin;
    const float gridMaxZ = -grid.header->zOrigin + grid.header->spanZ + margin;
    const float sweepMinX = std::min(start.x, end.x) - radius;
    const float sweepMaxX = std::max(start.x, end.x) + radius;
    const float sweepMinZ = std::min(start.z, end.z) - radius;
    const float sweepMaxZ = std::max(start.z, end.z) + radius;
    return sweepMaxX >= gridMinX && sweepMinX <= gridMaxX && sweepMaxZ >= gridMinZ && sweepMinZ <= gridMaxZ;
}

float DistanceSquaredToSegment(CollisionVector2 point, CollisionVector2 a, CollisionVector2 b) {
    const CollisionVector2 delta = Subtract(point, ClosestPointOnSegment(point, a, b));
    return Dot(delta, delta);
}

bool CircleOverlapsPolyline(CollisionVector2 center, float radius, const CollisionVector2 *points, std::size_t count, bool closed) {
    if (!count)
        return false;

    const float radiusSquared = radius * radius + kCollisionEpsilon;
    if (count == 1)
        return Dot(Subtract(center, points[0]), Subtract(center, points[0])) <= radiusSquared;

    if (closed && count >= 3 && PointInPolygon(center, points, count))
        return true;

    const std::size_t edgeCount = closed ? count : count - 1;
    for (std::size_t index = 0; index < edgeCount; ++index) {
        if (DistanceSquaredToSegment(center, points[index], points[(index + 1) % count]) <= radiusSquared) {
            return true;
        }
    }
    return false;
}

bool CircleOverlapsRecord(std::uint32_t functionIndex, const std::byte *data, CollisionVector2 center, float radius) {
    std::array<CollisionVector2, 128> points{};
    std::size_t count = 0;
    switch (functionIndex) {
    case 0:
        if (!ReadMapInfo(data, points[0]))
            return false;
        return CircleOverlapsPolyline(center, radius, points.data(), 1, false);
    case 1:
        if (!MapInfoRangeValid(data, 2 * sizeof(CollisionVector2)))
            return false;
        std::memcpy(points.data(), data, 2 * sizeof(CollisionVector2));
        return CircleOverlapsPolyline(center, radius, points.data(), 2, false);
    case 2:
    case 6:
    case 7:
    case 8:
        if (!ReadCollisionPoints(data, points, count))
            return false;
        return CircleOverlapsPolyline(center, radius, points.data(), count, false);
    case 3: {
        float shapeRadius = 0.0f;
        if (!ReadMapInfo(data, shapeRadius) || !ReadMapInfo(data + sizeof(shapeRadius), points[0])) {
            return false;
        }
        return CircleOverlapsPolyline(center, radius + std::max(0.0f, shapeRadius), points.data(), 1, false);
    }
    case 4:
        count = 3;
        if (!MapInfoRangeValid(data, count * sizeof(CollisionVector2))) {
            return false;
        }
        std::memcpy(points.data(), data, count * sizeof(CollisionVector2));
        return CircleOverlapsPolyline(center, radius, points.data(), count, true);
    case 5: {
        float values[6]{};
        if (!ReadMapInfo(data, values))
            return false;
        points[0] = {values[0], values[1]};
        points[1] = {values[0] + values[2], values[1] + values[3]};
        points[2] = {points[1].x + values[4], points[1].y + values[5]};
        points[3] = {values[0] + values[4], values[1] + values[5]};
        return CircleOverlapsPolyline(center, radius, points.data(), 4, true);
    }
    default:
        return false;
    }
}

bool GroundPointEquals(CollisionVector2 point, CollisionVector2 other) {
    return std::fabs(point.x - other.x) < 0.000000001 && std::fabs(point.y - other.y) < 0.000000001;
}

bool GroundPointOnSegment(CollisionVector2 point, CollisionVector2 start, CollisionVector2 end) {
    const CollisionVector2 a = Subtract(start, point);
    const CollisionVector2 b = Subtract(end, point);
    return std::fabs(b.y * a.x - b.x * a.y) < 0.000000001 && b.y * a.y + b.x * a.x <= 0.0f;
}

bool GroundPointInParallelogram(CollisionVector2 point, CollisionVector2 origin, CollisionVector2 u, CollisionVector2 v) {
    float determinant = u.x * v.y - u.y * v.x;
    if (determinant == 0.0f) {
        const CollisionVector2 start = u.x * v.x + u.y * v.y < 0.0f ? CollisionVector2{origin.x + u.x, origin.y + u.y} : origin;
        const CollisionVector2 end =
            u.x * v.x + u.y * v.y < 0.0f ? CollisionVector2{origin.x + v.x, origin.y + v.y} : CollisionVector2{origin.x + u.x + v.x, origin.y + u.y + v.y};
        return GroundPointOnSegment(point, start, end);
    }
    const float x = point.x - origin.x;
    const float y = point.y - origin.y;
    float alongU = x * v.y - y * v.x;
    float alongV = x * u.y - y * u.x;
    if (determinant > 0.0f) {
        alongV = -alongV;
    } else {
        determinant = -determinant;
        alongU = -alongU;
    }
    return alongU >= 0.0f && alongU <= determinant && alongV >= 0.0f && alongV <= determinant;
}

bool GroundPointIntersectsRecord(std::uint32_t functionIndex, const std::byte *data, CollisionVector2 point) {
    if (functionIndex == 0) {
        CollisionVector2 other{};
        return ReadMapInfo(data, other) && GroundPointEquals(point, other);
    }
    if (functionIndex == 1) {
        CollisionVector2 points[2]{};
        return ReadMapInfo(data, points) && GroundPointOnSegment(point, points[0], points[1]);
    }
    if (functionIndex == 2 || functionIndex == 6) {
        std::uint32_t count = 0;
        if (!ReadMapInfo(data, count) || !count || !MapInfoRangeValid(data + sizeof(count), static_cast<std::size_t>(count) * sizeof(CollisionVector2)))
            return false;
        const auto *points = reinterpret_cast<const CollisionVector2 *>(data + sizeof(count));
        if (count == 1)
            return GroundPointEquals(point, points[0]);
        if (count == 2)
            return GroundPointOnSegment(point, points[0], points[1]);
        if (functionIndex == 2) {
            for (std::uint32_t index = 1; index < count; ++index)
                if (GroundPointOnSegment(point, points[index - 1], points[index]))
                    return true;
            return false;
        }
        bool positive = false;
        bool negative = false;
        for (std::uint32_t index = 0; index < count; ++index) {
            const CollisionVector2 a = points[index];
            const CollisionVector2 b = points[index + 1 == count ? 0 : index + 1];
            const float side = (b.y - a.y) * (point.x - a.x) - (b.x - a.x) * (point.y - a.y);
            positive |= side > 0.0f;
            negative |= side < 0.0f;
            if (positive && negative)
                return false;
        }
        if (positive || negative)
            return true;
        bool useX = false;
        for (std::uint32_t index = 1; index < count; ++index)
            useX |= std::fabs(points[index].x - points[0].x) >= 0.000000001;
        CollisionVector2 minimum = points[0];
        CollisionVector2 maximum = points[0];
        for (std::uint32_t index = 1; index < count; ++index) {
            const float coordinate = useX ? points[index].x : points[index].y;
            if (coordinate < (useX ? minimum.x : minimum.y))
                minimum = points[index];
            if (coordinate > (useX ? maximum.x : maximum.y))
                maximum = points[index];
        }
        return GroundPointOnSegment(point, minimum, maximum);
    }
    if (functionIndex == 3) {
        float circle[3]{};
        if (!ReadMapInfo(data, circle))
            return false;
        const float x = point.x - circle[1];
        const float y = point.y - circle[2];
        return circle[0] * circle[0] - (y * y + x * x) >= 0.0f;
    }
    if (functionIndex == 4) {
        CollisionVector2 points[3]{};
        if (!ReadMapInfo(data, points))
            return false;
        const CollisionVector2 u = Subtract(points[1], points[0]);
        const CollisionVector2 v = Subtract(points[2], points[0]);
        float determinant = v.y * u.x - v.x * u.y;
        if (std::fabs(determinant) < 0.000000001) {
            const CollisionVector2 a = Subtract(points[0], point);
            const CollisionVector2 b = Subtract(points[1], point);
            const CollisionVector2 c = Subtract(points[2], point);
            return std::fabs(b.y * a.x - b.x * a.y) < 0.000000001 && std::fabs(c.y * b.x - c.x * b.y) < 0.000000001 &&
                (b.y * a.y + b.x * a.x <= 0.0f || c.y * b.y + c.x * b.x <= 0.0f);
        }
        const CollisionVector2 local = Subtract(point, points[0]);
        float alongU = local.x * v.y - local.y * v.x;
        float alongV = local.y * u.x - local.x * u.y;
        if (determinant < 0.0f) {
            determinant = -determinant;
            alongU = -alongU;
            alongV = -alongV;
        }
        return alongU >= 0.0f && alongV >= 0.0f && alongU + alongV <= determinant;
    }
    if (functionIndex == 5) {
        CollisionVector2 values[3]{};
        return ReadMapInfo(data, values) && GroundPointInParallelogram(point, values[0], values[1], values[2]);
    }
    if (functionIndex == 7 || functionIndex == 8) {
        float values[5]{};
        if (!ReadMapInfo(data, values))
            return false;
        const float dx = values[2] - values[0];
        const float dy = values[3] - values[1];
        const float scale = values[4] / std::sqrt(dx * dx + dy * dy);
        const float normalX = dy * scale;
        const float normalY = -(dx * scale);
        if (GroundPointInParallelogram(point, {values[0] - normalX, values[1] - normalY}, {normalX * 2.0f, normalY * 2.0f}, {dx, dy}))
            return true;
        if (functionIndex == 7)
            return false;
        const float startX = point.x - values[0];
        const float startY = point.y - values[1];
        const float endX = point.x - values[2];
        const float endY = point.y - values[3];
        return values[4] * values[4] - (startY * startY + startX * startX) >= 0.0f || values[4] * values[4] - (endY * endY + endX * endX) >= 0.0f;
    }
    return false;
}

bool CalculateUndoGroundY(const std::byte *record, std::uint32_t functionIndex, const Vector3 &position, float &outY) {
    std::array<float, 4> heights{};
    if (!MapInfoRangeValid(record, 0x14) || !MapInfoRangeValid(record + 0x14, 1)) {
        return false;
    }
    std::memcpy(heights.data(), record, sizeof(heights));

    std::array<CollisionVector2, 3> planePoints{};
    std::uint32_t pointCount = 0;
    const std::byte *const data = record + 0x14;
    if (functionIndex == 4) {
        if (!MapInfoRangeValid(data, planePoints.size() * sizeof(CollisionVector2))) {
            return false;
        }
        std::memcpy(planePoints.data(), data, planePoints.size() * sizeof(CollisionVector2));
        pointCount = 3;
    } else if (functionIndex == 5) {
        float values[6]{};
        if (!ReadMapInfo(data, values))
            return false;
        planePoints[0] = {values[0], values[1]};
        planePoints[1] = {values[0] + values[2], values[1] + values[3]};
        planePoints[2] = {values[0] + values[4], values[1] + values[5]};
        pointCount = 4;
    } else if (functionIndex == 6) {
        if (!ReadMapInfo(data, pointCount) || static_cast<std::int32_t>(pointCount) > 4 ||
            !MapInfoRangeValid(data + sizeof(pointCount), planePoints.size() * sizeof(CollisionVector2))) {
            return false;
        }
        std::memcpy(planePoints.data(), data + sizeof(pointCount), planePoints.size() * sizeof(CollisionVector2));
    } else {
        return false;
    }

    float candidate = 0.0f;
    if (heights[3] > 10000.0f) {
        candidate = (heights[1] * -position.z + heights[0] * position.x) + heights[2];
    } else if (heights[3] < -10000.0f) {
        candidate = heights[0];
    } else {
        const float dx21 = planePoints[2].x - planePoints[1].x;
        const float dx10 = planePoints[1].x - planePoints[0].x;
        const float dz21 = planePoints[2].y - planePoints[1].y;
        const float dz10 = planePoints[1].y - planePoints[0].y;
        const float b = dx21 * dz10 - dx10 * dz21;
        const float c = dx10 * (heights[2] - heights[1]) - dx21 * (heights[1] - heights[0]);
        const float a = (heights[1] - heights[0]) * dz21 - (heights[2] - heights[1]) * dz10;
        const float d = ((a * planePoints[0].x + b * heights[0]) + c * planePoints[0].y) * -1.0f;
        if (static_cast<double>(b) >= 0.01 || static_cast<double>(b) <= -0.01) {
            candidate = (((c * -position.z + a * position.x) + d) * -1.0f) / b;
        } else {
            candidate = std::max(heights[2], std::max(heights[0], heights[1]));
            if (pointCount == 4)
                candidate = std::max(heights[3], candidate);
        }
    }
    if (!(candidate <= position.y + 0.80000001f)) {
        return false;
    }
    outY = candidate;
    return true;
}

int GroundGridCoordinate(float relative, float span, float cellSize, std::int32_t count) {
    if (relative < std::numeric_limits<float>::epsilon())
        return 0;
    if (relative > span)
        return count - 1;
    int coordinate = static_cast<int>(relative / cellSize);
    if (coordinate < 0)
        coordinate = 0;
    if (coordinate >= count)
        --coordinate;
    return coordinate;
}

bool CollisionShapeIntersectsRecord(const QueryShape &query, std::uint32_t mode, const std::byte *data);

const std::uint32_t *SampleGroundPoint(const MapQuerySet &query, const Vector3 &position, bool undo, float &height, float radius = 0.0f) {
    height = 0.0f;
    const MapInfoSubchunk *const chunk = undo ? query.undo : query.height;
    const std::uint32_t payloadSize = undo ? query.undoSize : query.heightSize;
    if (!chunk || !payloadSize)
        return nullptr;
    const std::int32_t index = QueryGridIndex(query, chunk);
    CollisionGridView grid{};
    if (index < 0 || !BindQueryGrid(query, static_cast<std::uint32_t>(index), grid))
        return nullptr;
    const int x = GroundGridCoordinate(position.x - grid.header->minX, grid.header->spanX, grid.header->cellSizeX, grid.header->width);
    const int z = GroundGridCoordinate(position.z + grid.header->zOrigin, grid.header->spanZ, grid.header->cellSizeZ, grid.header->height);
    std::int32_t listOffset = -1;
    if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, listOffset) || listOffset < 0)
        return nullptr;
    const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);
    const auto *const bytes = reinterpret_cast<const std::byte *>(chunk);
    const std::size_t chunkSize = sizeof(MapInfoSubchunk) + static_cast<std::size_t>(payloadSize);
    const std::size_t functionOffset = undo ? 0x10 : 4;
    QueryShape shape{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange};
    shape.packed[0] = {radius, position.x, -position.z};
    const std::uint32_t *selected = nullptr;
    float highest = -10000.0f;
    std::int32_t recordOffset = -1;
    while (ReadMapInfo(list++, recordOffset) && recordOffset != -1) {
        if (recordOffset < 0 || static_cast<std::size_t>(recordOffset) > chunkSize || functionOffset + 4 > chunkSize - static_cast<std::size_t>(recordOffset))
            continue;
        const std::byte *const record = bytes + recordOffset;
        std::uint32_t functionIndex = 0;
        if (!ReadMapInfo(record + functionOffset, functionIndex) ||
            !(radius > 0.0f ? CollisionShapeIntersectsRecord(shape, functionIndex, record + functionOffset + 4) :
                              GroundPointIntersectsRecord(functionIndex, record + functionOffset + 4, {position.x, -position.z})))
            continue;
        float candidate = 0.0f;
        if (undo) {
            if (!CalculateUndoGroundY(record, functionIndex, position, candidate))
                continue;
        } else if (!ReadMapInfo(record, candidate)) {
            continue;
        }
        if (candidate > highest) {
            highest = candidate;
            selected = reinterpret_cast<const std::uint32_t *>(record + functionOffset);
        }
    }
    if (selected)
        height = highest;
    return selected;
}

bool TrySampleGroundChunk(const MapQuerySet &query, const MapInfoSubchunk *chunk, std::uint32_t payloadSize, bool undo, const Vector3 &position, float radius,
    float &outY, bool requireGridOverlap = true) {
    if (!chunk || !payloadSize)
        return false;
    const std::int32_t queryIndex = QueryGridIndex(query, chunk);
    if (queryIndex < 0)
        return false;
    CollisionGridView grid{};
    if (!BindQueryGrid(query, static_cast<std::uint32_t>(queryIndex), grid) || (requireGridOverlap && !GridMayContainSweep(grid, position, position, radius))) {
        return false;
    }

    const float minX = position.x - radius;
    const float maxX = position.x + radius;
    const float minZ = position.z - radius;
    const float maxZ = position.z + radius;
    const int minCellX = GridCoordinate(minX, grid.header->minX, grid.header->cellSizeX, grid.header->width);
    const int maxCellX = GridCoordinate(maxX, grid.header->minX, grid.header->cellSizeX, grid.header->width);
    const float worldZOrigin = -grid.header->zOrigin;
    const int minCellZ = GridCoordinate(minZ, worldZOrigin, grid.header->cellSizeZ, grid.header->height);
    const int maxCellZ = GridCoordinate(maxZ, worldZOrigin, grid.header->cellSizeZ, grid.header->height);

    const auto *const chunkBytes = reinterpret_cast<const std::byte *>(chunk);
    const std::size_t chunkSize = sizeof(MapInfoSubchunk) + static_cast<std::size_t>(payloadSize);
    const std::size_t recordHeaderSize = undo ? 0x14u : 8u;
    const CollisionVector2 center{position.x, -position.z};
    bool found = false;
    float highest = -std::numeric_limits<float>::infinity();
    for (int z = minCellZ; z <= maxCellZ; ++z) {
        for (int x = minCellX; x <= maxCellX; ++x) {
            std::int32_t listOffset = -1;
            if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, listOffset) || listOffset < 0) {
                continue;
            }
            const auto *const list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);
            for (std::size_t index = 0; index < 4096; ++index) {
                std::int32_t recordOffset = -1;
                if (!ReadMapInfo(list + index, recordOffset) || recordOffset == -1) {
                    break;
                }
                if (recordOffset < 0 || static_cast<std::size_t>(recordOffset) > chunkSize ||
                    recordHeaderSize > chunkSize - static_cast<std::size_t>(recordOffset)) {
                    continue;
                }
                const std::byte *const record = chunkBytes + recordOffset;
                std::uint32_t functionIndex = 0;
                float candidate = 0.0f;
                if (undo) {
                    if (!ReadMapInfo(record + 0x10, functionIndex) || !CircleOverlapsRecord(functionIndex, record + 0x14, center, radius) ||
                        !CalculateUndoGroundY(record, functionIndex, position, candidate)) {
                        continue;
                    }
                } else {
                    if (!ReadMapInfo(record, candidate) || !ReadMapInfo(record + 4, functionIndex) || !std::isfinite(candidate) ||
                        !CircleOverlapsRecord(functionIndex, record + 8, center, radius)) {
                        continue;
                    }
                }
                if (candidate > highest) {
                    highest = candidate;
                    found = true;
                }
            }
        }
    }
    if (found)
        outY = highest;
    return found;
}

const MapQuerySet *FindGroundQuerySet(const MapControlData &control, const Vector3 &position) {
    const MapQuerySet *fallback = nullptr;
    for (void *const opaqueQuery : control.mapQuerySetsBySection) {
        const auto *const query = static_cast<const MapQuerySet *>(opaqueQuery);
        if (!query)
            continue;
        if (!fallback)
            fallback = query;
        CollisionGridView grid{};
        if (BindQueryGrid(*query, 0, grid) && GridMayContainSweep(grid, position, position, 0.0f)) {
            return query;
        }
    }
    return fallback;
}

bool TrySampleGroundY(const MapControlData &control, const Vector3 &position, float radius, float &outY) {
    const MapQuerySet *const query = FindGroundQuerySet(control, position);
    if (!query)
        return false;
    float heightY = 0.0f;
    float undoY = 0.0f;
    const bool foundHeight = TrySampleGroundChunk(*query, query->height, query->heightSize, false, position, radius, heightY);
    const bool foundUndo = TrySampleGroundChunk(*query, query->undo, query->undoSize, true, position, radius, undoY);
    if (!foundHeight && !foundUndo)
        return false;
    outY = foundHeight && foundUndo ? std::max(heightY, undoY) : (foundHeight ? heightY : undoY);
    return true;
}

Vector3 TransformCollisionQueryPoint(const Vector3 &point) {
    if (!g_collisionQueryTransform.active)
        return point;
    const Matrix4x4 &matrix = g_collisionQueryTransform.matrix;
    return {
        ((matrix.rows[0].x * point.x + matrix.rows[1].x * point.y) + matrix.rows[2].x * point.z) + matrix.rows[3].x,
        ((matrix.rows[0].y * point.x + matrix.rows[1].y * point.y) + matrix.rows[2].y * point.z) + matrix.rows[3].y,
        ((matrix.rows[0].z * point.x + matrix.rows[1].z * point.y) + matrix.rows[2].z * point.z) + matrix.rows[3].z,
    };
}

struct PackedCollisionLine {
    CollisionVector2 start;
    CollisionVector2 end;
};

struct PackedCollisionBox {
    CollisionVector2 origin;
    CollisionVector2 edgeA;
    CollisionVector2 edgeB;
};

const std::uint32_t *g_selectedCollisionFunction = nullptr;
QueryShape g_collisionPolylineHit{};

float PackedCross(CollisionVector2 a, CollisionVector2 b) {
    return a.x * b.y - a.y * b.x;
}

PackedCollisionBox PackedStripBox(PackedCollisionLine line, float width) {
    const CollisionVector2 delta = Subtract(line.end, line.start);
    const float scale = width / std::sqrt(delta.x * delta.x + delta.y * delta.y);
    const float sideX = delta.y * scale;
    const float sideY = -(delta.x * scale);
    return {{line.start.x - sideX, line.start.y - sideY}, {sideX * 2.0f, sideY * 2.0f}, delta};
}

bool PackedLineContainsPoint(PackedCollisionLine line, CollisionVector2 point) {
    const CollisionVector2 start = Subtract(line.start, point);
    const CollisionVector2 end = Subtract(line.end, point);
    return static_cast<double>(std::fabs(end.y * start.x - end.x * start.y)) < 0.000000001 && end.y * start.y + end.x * start.x <= 0.0f;
}

bool PackedLinesIntersect(PackedCollisionLine a, PackedCollisionLine b) {
    const CollisionVector2 directionA = Subtract(a.end, a.start);
    if (PackedCross(directionA, Subtract(b.end, a.start)) * PackedCross(directionA, Subtract(b.start, a.start)) > 0.0f)
        return false;
    const CollisionVector2 directionB = Subtract(b.end, b.start);
    const CollisionVector2 start = Subtract(a.start, b.start);
    const CollisionVector2 end = Subtract(a.end, b.start);
    const float startSide = PackedCross(directionB, start);
    const float endSide = PackedCross(directionB, end);
    if (startSide * endSide > 0.0f)
        return false;
    if (std::fabs(startSide) < 1.0e-9f && std::fabs(endSide) < 1.0e-9f) {
        const float startDistance = start.y * directionB.y + start.x * directionB.x;
        const float endDistance = end.y * directionB.y + end.x * directionB.x;
        const float length = directionB.y * directionB.y + directionB.x * directionB.x;
        if ((startDistance < 0.0f && endDistance < 0.0f) || (startDistance > length && endDistance > length))
            return false;
    }
    return true;
}

bool PackedCircleIntersectsLine(float radius, CollisionVector2 center, PackedCollisionLine line) {
    if (std::fabs(radius) < 1.0e-9f)
        return PackedLineContainsPoint(line, center);
    const CollisionVector2 start = Subtract(line.start, center);
    const float radiusSquared = radius * radius;
    if (std::fabs(line.start.x - line.end.x) < 1.0e-9f && std::fabs(line.start.y - line.end.y) < 1.0e-9f)
        return radiusSquared - (start.y * start.y + start.x * start.x) >= 0.0f;
    const CollisionVector2 delta = Subtract(line.end, line.start);
    const float distance = start.y * start.y + start.x * start.x - radiusSquared;
    if (distance <= 0.0f)
        return true;
    const float length = delta.y * delta.y + delta.x * delta.x;
    if (std::fabs(length) < 1.0e-9f)
        return false;
    const float projection = delta.y * start.y + delta.x * start.x;
    return ((projection + length) + projection) + distance <= 0.0f ||
        (-projection >= 0.0f && length >= -projection && projection * projection - length * distance >= 0.0f);
}




bool PackedBoxContainsPoint(const PackedCollisionBox &box, CollisionVector2 point) {
    float area = PackedCross(box.edgeA, box.edgeB);
    if (static_cast<double>(std::fabs(area)) < 0.000000001) {
        const PackedCollisionLine diagonal = Dot(box.edgeA, box.edgeB) < 0.0f ?
            PackedCollisionLine{AddScaled(box.origin, box.edgeA, 1.0f), AddScaled(box.origin, box.edgeB, 1.0f)} :
            PackedCollisionLine{box.origin, AddScaled(AddScaled(box.origin, box.edgeA, 1.0f), box.edgeB, 1.0f)};
        return PackedLineContainsPoint(diagonal, point);
    }
    const CollisionVector2 offset = Subtract(point, box.origin);
    float alongA = PackedCross(offset, box.edgeB);
    float alongB = PackedCross(offset, box.edgeA);
    if (area <= 0.0f) {
        alongA = -alongA;
        area = -area;
    } else {
        alongB = -alongB;
    }
    return alongA >= 0.0f && alongA <= area && alongB >= 0.0f && alongB <= area;
}

bool PackedBoxIntersectsLine(const PackedCollisionBox &box, PackedCollisionLine line) {
    const CollisionVector2 direction = Subtract(line.end, line.start);
    PackedCollisionLine diagonal{};
    if (PackedCross(direction, box.edgeA) * PackedCross(direction, box.edgeB) < 0.0f) {
        diagonal = {AddScaled(box.origin, box.edgeA, 1.0f), AddScaled(box.origin, box.edgeB, 1.0f)};
    } else {
        diagonal = {box.origin, AddScaled(AddScaled(box.origin, box.edgeA, 1.0f), box.edgeB, 1.0f)};
    }
    if (PackedCross(direction, Subtract(diagonal.start, line.start)) * PackedCross(direction, Subtract(diagonal.end, line.start)) > 0.0f)
        return false;
    const float area = PackedCross(box.edgeA, box.edgeB);
    if (std::fabs(area) < 1.0e-9f)
        return false;
    const CollisionVector2 start = Subtract(line.start, box.origin);
    const CollisionVector2 end = Subtract(line.end, box.origin);
    const CollisionVector2 edges[]{box.edgeA, box.edgeB};
    for (std::size_t index = 0; index < 2; ++index) {
        float span = index ? -area : area;
        float first = PackedCross(edges[index], start);
        float last = PackedCross(edges[index], end);
        if (span < 0.0f) {
            span = -span;
            first = -first;
            last = -last;
        }
        if ((first < 0.0f && last < 0.0f) || (first > span && last > span))
            return false;
    }
    return true;
}

bool PackedBoxIntersectsCircle(const PackedCollisionBox &box, float radius, CollisionVector2 center) {
    const CollisionVector2 offset = Subtract(center, box.origin);
    const float radiusSquared = radius * radius;
    float area = PackedCross(box.edgeA, box.edgeB);
    const CollisionVector2 edges[]{box.edgeA, box.edgeB};
    bool checkCorners = false;
    for (const CollisionVector2 edge : edges) {
        float span = area;
        float side = PackedCross(edge, offset);
        if (area < 0.0f) {
            span = -span;
            side = -side;
        }
        const float length = edge.y * edge.y + edge.x * edge.x;
        if (side < 0.0f || side > span) {
            float along = edge.y * offset.y + edge.x * offset.x;
            const float outside = side < 0.0f ? side : side - span;
            if (outside * outside > length * radiusSquared)
                return false;
            if (side > span)
                along -= box.edgeB.x * box.edgeA.x + box.edgeA.y * box.edgeB.y;
            if (along >= 0.0f && length >= along)
                return true;
            checkCorners = true;
        }
        area = -area;
    }
    if (!checkCorners)
        return true;
    const CollisionVector2 corners[]{box.origin, AddScaled(box.origin, box.edgeA, 1.0f), AddScaled(box.origin, box.edgeB, 1.0f),
        {box.edgeA.x + box.edgeB.x + box.origin.x, box.edgeA.y + box.edgeB.y + box.origin.y}};
    for (const CollisionVector2 corner : corners) {
        const CollisionVector2 delta = Subtract(center, corner);
        if (radiusSquared > delta.y * delta.y + delta.x * delta.x)
            return true;
    }
    return false;
}

bool PackedBoxAxisAccepts(const PackedCollisionBox &a, const PackedCollisionBox &b, CollisionVector2 axis, float span, bool finalAxis) {
    const CollisionVector2 offset = Subtract(b.origin, a.origin);
    CollisionVector2 first = offset;
    CollisionVector2 last{};
    if (PackedCross(axis, b.edgeA) * PackedCross(axis, b.edgeB) < 0.0f) {
        first = AddScaled(offset, b.edgeA, 1.0f);
        last = AddScaled(offset, b.edgeB, 1.0f);
    } else {
        last = AddScaled(AddScaled(offset, b.edgeA, 1.0f), b.edgeB, 1.0f);
    }
    float firstSide = PackedCross(axis, first);
    float lastSide = PackedCross(axis, last);
    const float product = firstSide * lastSide;
    if (finalAxis ? product <= 0.0f : !(product > 0.0f))
        return true;
    if (span < 0.0f) {
        span = -span;
        firstSide = -firstSide;
        lastSide = -lastSide;
    }
    return firstSide >= 0.0f && (firstSide <= span || lastSide <= span);
}

bool PackedBoxesIntersect(const PackedCollisionBox &a, const PackedCollisionBox &b) {
    const float areaA = PackedCross(a.edgeA, a.edgeB);
    if (std::fabs(areaA) < 1.0e-9f)
        return false;
    const float areaB = PackedCross(b.edgeA, b.edgeB);
    if (std::fabs(areaB) < 1.0e-9f)
        return false;
    return PackedBoxAxisAccepts(a, b, a.edgeA, areaA, false) && PackedBoxAxisAccepts(a, b, a.edgeB, -areaA, false) &&
        PackedBoxAxisAccepts(b, a, b.edgeA, areaB, false) && PackedBoxAxisAccepts(b, a, b.edgeB, -areaB, true);
}

bool PackedBoxIntersectsTriangle(const PackedCollisionBox &box, const CollisionVector2 *triangle) {
    const float boxArea = PackedCross(box.edgeA, box.edgeB);
    if (std::fabs(boxArea) < 1.0e-9f)
        return false;
    const CollisionVector2 corners[]{box.origin, AddScaled(box.origin, box.edgeA, 1.0f), AddScaled(AddScaled(box.origin, box.edgeA, 1.0f), box.edgeB, 1.0f),
        AddScaled(box.origin, box.edgeB, 1.0f)};
    const CollisionVector2 edges[]{box.edgeA, box.edgeB, {-box.edgeA.x, -box.edgeA.y}, {-box.edgeB.x, -box.edgeB.y}};
    for (std::size_t edge = 0; edge < 4; ++edge) {
        std::size_t point = 0;
        while (point < 3 && !(PackedCross(edges[edge], Subtract(triangle[point], corners[edge])) * boxArea >= 0.0f))
            ++point;
        if (point == 3)
            return false;
    }
    const float area = PackedCross(Subtract(triangle[1], triangle[0]), Subtract(triangle[2], triangle[0]));
    if (std::fabs(area) < 1.0e-9f)
        return false;
    for (std::size_t edge = 0; edge < 3; ++edge) {
        const CollisionVector2 direction = Subtract(triangle[(edge + 1) % 3], triangle[edge]);
        std::size_t point = 0;
        while (point < 4 && !(PackedCross(direction, Subtract(corners[point], triangle[edge])) * area >= 0.0f))
            ++point;
        if (point == 4)
            return false;
    }
    return true;
}

bool PackedConvexPolygonIntersectsLine(const CollisionVector2 *points, std::uint32_t count, PackedCollisionLine line) {
    if (!count)
        return false;
    if (count == 1)
        return PackedLineContainsPoint(line, points[0]);
    if (count == 2)
        return PackedLinesIntersect(line, {points[0], points[1]});
    const CollisionVector2 direction = Subtract(line.end, line.start);
    const float firstSide = PackedCross(direction, Subtract(points[0], line.start));
    std::uint32_t point = 1;
    while (point < count && !(PackedCross(direction, Subtract(points[point], line.start)) * firstSide <= 0.0f))
        ++point;
    if (point == count)
        return false;
    const float area = PackedCross(Subtract(points[1], points[0]), Subtract(points[2], points[1]));
    for (std::uint32_t index = 0; index < count; ++index) {
        const CollisionVector2 edge = Subtract(points[(index + 1) % count], points[index]);
        if (PackedCross(edge, Subtract(line.start, points[index])) * area < 0.0f && PackedCross(edge, Subtract(line.end, points[index])) * area < 0.0f)
            return false;
    }
    return true;
}

bool PackedQueryIntersectsRecord(PackedCollisionLine line, const PackedCollisionBox *box, std::uint32_t mode, const std::byte *data) {
    switch (mode) {
    case 0: {
        CollisionVector2 point{};
        std::memcpy(&point, data, sizeof(point));
        return box ? PackedBoxContainsPoint(*box, point) : PackedLineContainsPoint(line, point);
    }
    case 1: {
        PackedCollisionLine target{};
        std::memcpy(&target, data, sizeof(target));
        return box ? PackedBoxIntersectsLine(*box, target) : PackedLinesIntersect(line, target);
    }
    case 2:
    case 6: {
        std::uint32_t count = 0;
        std::memcpy(&count, data, sizeof(count));
        if (!count)
            return false;
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if (count == 1)
            return box ? PackedBoxContainsPoint(*box, points[0]) : PackedLineContainsPoint(line, points[0]);
        if (mode == 6) {
            if (!box)
                return PackedConvexPolygonIntersectsLine(points, count, line);
            if (count == 2)
                return PackedBoxIntersectsLine(*box, {points[0], points[1]});
            CollisionVector2 triangle[]{points[2], points[1], points[0]};
            std::uint32_t slot = 1;
            if (PackedBoxIntersectsTriangle(*box, triangle))
                return true;
            for (std::uint32_t index = 3; index < count; ++index) {
                triangle[slot] = points[index];
                slot ^= 1U;
                if (PackedBoxIntersectsTriangle(*box, triangle))
                    return true;
            }
            return false;
        }
        for (std::uint32_t index = 1; index < count; ++index) {
            const PackedCollisionLine segment{points[index - 1], points[index]};
            if (box ? PackedBoxIntersectsLine(*box, segment) : PackedLinesIntersect(line, segment)) {
                g_collisionPolylineHit.mode = CalculateLineIntersection_0;
                std::memcpy(g_collisionPolylineHit.packed, &segment, sizeof(segment));
                return true;
            }
        }
        return false;
    }
    case 3: {
        Vector3 circle{};
        std::memcpy(&circle, data, sizeof(circle));
        return box ? PackedBoxIntersectsCircle(*box, circle.x, {circle.y, circle.z}) : PackedCircleIntersectsLine(circle.x, {circle.y, circle.z}, line);
    }
    case 4: {
        CollisionVector2 triangle[3]{};
        std::memcpy(triangle, data, sizeof(triangle));
        return box ? PackedBoxIntersectsTriangle(*box, triangle) : PackedConvexPolygonIntersectsLine(triangle, 3, line);
    }
    case 5: {
        PackedCollisionBox target{};
        std::memcpy(&target, data, sizeof(target));
        return box ? PackedBoxesIntersect(*box, target) : PackedBoxIntersectsLine(target, line);
    }
    case 7:
    case 8: {
        struct Strip {
            PackedCollisionLine line;
            float width;
        } strip{};
        std::memcpy(&strip, data, sizeof(strip));
        const PackedCollisionBox target = PackedStripBox(strip.line, strip.width);
        if (box ? PackedBoxesIntersect(*box, target) : PackedBoxIntersectsLine(target, line))
            return true;
        if (mode == 7)
            return false;
        return box ? PackedBoxIntersectsCircle(*box, strip.width, strip.line.start) || PackedBoxIntersectsCircle(*box, strip.width, strip.line.end) :
                     PackedCircleIntersectsLine(strip.width, strip.line.start, line) || PackedCircleIntersectsLine(strip.width, strip.line.end, line);
    }
    default:
        return false;
    }
}

struct PackedGridCell {
    std::int32_t x;
    std::int32_t z;
};

std::uint32_t PackedGridCoordinates(const CollisionGridHeader &grid, float x, float z, PackedGridCell &cell) {
    std::uint32_t flags = 0;
    const float relativeX = x - grid.minX;
    if (relativeX < 1.1920929e-7f) {
        cell.x = 0;
        if (relativeX + grid.borderMargin < 1.1920929e-7f)
            flags = 8;
    } else if (relativeX > grid.spanX) {
        cell.x = grid.width - 1;
        if (relativeX - grid.borderMargin > grid.spanX)
            flags = 4;
    } else {
        cell.x = static_cast<std::int32_t>(relativeX / grid.cellSizeX);
        if (cell.x < 0)
            cell.x = 0;
        else if (cell.x >= grid.width)
            --cell.x;
    }
    const float relativeZ = z + grid.zOrigin;
    if (relativeZ < 1.1920929e-7f) {
        cell.z = 0;
        if (relativeZ + grid.borderMargin < 1.1920929e-7f)
            flags |= 2;
    } else if (relativeZ > grid.spanZ) {
        cell.z = grid.height - 1;
        if (relativeZ - grid.borderMargin > grid.spanZ)
            flags |= 1;
    } else {
        cell.z = static_cast<std::int32_t>(relativeZ / grid.cellSizeZ);
        if (cell.z < 0)
            cell.z = 0;
        else if (cell.z >= grid.height)
            --cell.z;
    }
    return flags;
}

bool PackedQueryGridRange(const CollisionGridHeader &grid, CollisionVector2 minimum, CollisionVector2 maximum, PackedGridCell &first, PackedGridCell &last) {
    const float border = grid.borderMargin + grid.borderMargin;
    if (border > maximum.x - minimum.x && border > maximum.y - minimum.y) {
        const std::uint32_t flags = PackedGridCoordinates(grid, (minimum.x + maximum.x) * 0.5f, (minimum.y + maximum.y) * -0.5f, first);
        last = first;
        return flags == 0;
    }
    const std::uint32_t firstFlags = PackedGridCoordinates(grid, minimum.x, -maximum.y, first);
    const std::uint32_t lastFlags = PackedGridCoordinates(grid, maximum.x, -minimum.y, last);
    return !((lastFlags & 12U) && (lastFlags & 12U) == (firstFlags & 12U)) && !((lastFlags & 3U) && (lastFlags & 3U) == (firstFlags & 3U));
}

struct CollisionVectorProbe {
    CollisionVector2 movement;
    CharacterVectorCollisionResult result;
    float radius;
    CollisionVector2 origin;
};

std::int32_t CollisionFoldContactAngle(float y, float x) {
    const std::uint16_t angle = Shenmue_Atan2Fixed(y, x);
    const std::int32_t half = angle <= 0x8000U ? angle : angle - 0x8000;
    return half <= 0x4000 ? half : half - 0x8000;
}

bool CollisionSelectVectorFraction(CollisionVectorProbe &probe, float fraction) {
    const std::uint32_t previous = probe.result.flags & 7U;
    if (!((previous == 1 && fraction >= 0.0f && probe.result.fraction > fraction) || (previous == 0 && fraction >= 0.0f)))
        return false;
    probe.result.fraction = fraction;
    probe.result.flags = (probe.result.flags & ~7U) | 1U;
    return true;
}



// @verified - p
void CollisionVectorCircle(CollisionVectorProbe &probe, CollisionVector2 center, float radius) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 relative = Subtract(probe.origin, center);
    const float combinedRadius = probe.radius + radius;
    const float distance = relative.y * relative.y + relative.x * relative.x - combinedRadius * combinedRadius;
    if (!(distance > 0.0f)) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    const float length = probe.movement.x * probe.movement.x + probe.movement.y * probe.movement.y;
    if (static_cast<double>(std::fabs(length)) < 0.000000001)
        return;
    const float projection = relative.x * probe.movement.x + relative.y * probe.movement.y;
    if (projection > 0.0f)
        return;
    const float discriminant = projection * projection - length * distance;
    if (discriminant < 0.0f)
        return;
    const float fraction = (-projection - std::sqrt(discriminant)) / length;
    if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U)) {
        const CollisionVector2 contact{probe.movement.x * fraction + relative.x, probe.movement.y * fraction + relative.y};
        probe.result.angle =
            CollisionFoldContactAngle(probe.movement.x * contact.y - probe.movement.y * contact.x, probe.movement.y * contact.y + probe.movement.x * contact.x);
    }
}


void CollisionRayPoint(CollisionVectorProbe &probe, CollisionVector2 point) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 relative = Subtract(point, probe.origin);
    if (static_cast<double>(std::fabs(probe.movement.y * relative.x - probe.movement.x * relative.y)) >= 0.000000001)
        return;
    const CollisionVector2 end = AddScaled(probe.origin, probe.movement, 1.0f);
    if (point.x < std::fmin(probe.origin.x, end.x) || point.x > std::fmax(probe.origin.x, end.x) || point.y < std::fmin(probe.origin.y, end.y) ||
        point.y > std::fmax(probe.origin.y, end.y))
        return;
    float fraction = 0.0f;
    if (static_cast<double>(std::fabs(probe.movement.x)) >= 0.000000001) {
        fraction = relative.x / probe.movement.x;
    } else if (static_cast<double>(std::fabs(probe.movement.y)) >= 0.000000001) {
        fraction = relative.y / probe.movement.y;
    } else {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U))
        probe.result.angle = 0;
}


void CollisionRayLine(CollisionVectorProbe &probe, PackedCollisionLine line) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 edge = Subtract(line.end, line.start);
    if (static_cast<double>(std::fabs(edge.x)) < 0.000000001 && static_cast<double>(std::fabs(edge.y)) < 0.000000001) {
        CollisionRayPoint(probe, line.start);
        return;
    }
    const float denominator = probe.movement.x * edge.y - probe.movement.y * edge.x;
    const CollisionVector2 relative = Subtract(probe.origin, line.start);
    if (static_cast<double>(std::fabs(denominator)) < 0.000000001) {
        if (static_cast<double>(std::fabs(relative.x * edge.y - relative.y * edge.x)) >= 0.000000001)
            return;
        const float projection = relative.y * edge.y + relative.x * edge.x;
        const float length = edge.y * edge.y + edge.x * edge.x;
        const float speed = probe.movement.y * edge.y + probe.movement.x * edge.x;
        if (projection >= 0.0f && projection <= length) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
        } else if (static_cast<double>(std::fabs(speed)) >= 0.000000001) {
            const float distance = projection >= 0.0f ? projection - length : -projection;
            if (CollisionSelectVectorFraction(probe, distance / speed) && (probe.result.flags & 8U))
                probe.result.angle = 0x4000;
        }
        return;
    }
    const float edgeParameter = (probe.movement.x * relative.y - probe.movement.y * relative.x) / denominator;
    if (edgeParameter < 0.0f || edgeParameter > 1.0f)
        return;
    const float fraction = -(relative.x * edge.y - relative.y * edge.x) / denominator;
    if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U))
        probe.result.angle =
            CollisionFoldContactAngle(probe.movement.x * -edge.x - probe.movement.y * edge.y, probe.movement.y * -edge.x + probe.movement.x * edge.y);
}

void CollisionVectorBounds(const CollisionVectorProbe &probe, CollisionVector2 &minimum, CollisionVector2 &maximum) {
    minimum = {probe.origin.x - probe.radius, probe.origin.y - probe.radius};
    maximum = {probe.origin.x + probe.radius, probe.origin.y + probe.radius};
    if (probe.movement.x >= 0.0f)
        maximum.x += probe.movement.x;
    else
        minimum.x += probe.movement.x;
    if (probe.movement.y >= 0.0f)
        maximum.y += probe.movement.y;
    else
        minimum.y += probe.movement.y;
}

bool CollisionVectorBoundsOverlap(const CollisionVectorProbe &probe, const CollisionVector2 *points, std::uint32_t count) {
    if (!count)
        return false;
    CollisionVector2 minimum{};
    CollisionVector2 maximum{};
    CollisionVectorBounds(probe, minimum, maximum);
    CollisionVector2 low = points[0];
    CollisionVector2 high = points[0];
    for (std::uint32_t index = 1; index < count; ++index) {
        low.x = std::fmin(low.x, points[index].x);
        low.y = std::fmin(low.y, points[index].y);
        high.x = std::fmax(high.x, points[index].x);
        high.y = std::fmax(high.y, points[index].y);
    }
    return minimum.x <= high.x && low.x <= maximum.x && minimum.y <= high.y && low.y <= maximum.y;
}



// @verified - p
void CollisionVectorLine(CollisionVectorProbe &probe, PackedCollisionLine line) {
    const CollisionVector2 points[]{line.start, line.end};
    if (!CollisionVectorBoundsOverlap(probe, points, 2))
        return;
    CollisionVectorCircle(probe, line.start, 0.0f);
    if ((probe.result.flags & 7U) == 2U || (line.start.x == line.end.x && line.start.y == line.end.y))
        return;
    CollisionVectorCircle(probe, line.end, 0.0f);
    if ((probe.result.flags & 7U) == 2U)
        return;
    const PackedCollisionBox strip = PackedStripBox(line, probe.radius);
    if (PackedBoxContainsPoint(strip, probe.origin)) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    const CollisionVector2 end = AddScaled(strip.origin, strip.edgeB, 1.0f);
    CollisionRayLine(probe, {strip.origin, end});
    CollisionRayLine(probe, {AddScaled(strip.origin, strip.edgeA, 1.0f), AddScaled(end, strip.edgeA, 1.0f)});
}

PackedCollisionLine CollisionCollinearSpan(const CollisionVector2 *points, std::uint32_t count) {
    PackedCollisionLine span{points[0], points[0]};
    bool useY = false;
    std::uint32_t first = 1;
    for (; first < count; ++first) {
        if (std::fabs(points[first].x - points[0].x) >= 1.0e-9f)
            break;
        if (std::fabs(points[first].y - points[0].y) >= 1.0e-9f) {
            useY = true;
            break;
        }
    }
    for (std::uint32_t index = first; index < count; ++index) {
        const float value = useY ? points[index].y : points[index].x;
        if (value < (useY ? span.start.y : span.start.x))
            span.start = points[index];
        if (value > (useY ? span.end.y : span.end.x))
            span.end = points[index];
    }
    return span;
}

bool CollisionCircleInTriangle(const CollisionVector2 *points, float radius, CollisionVector2 center) {
    const float radiusSquared = radius * radius;
    const float orientation = PackedCross(Subtract(points[1], points[0]), Subtract(points[2], points[1]));
    bool outside = false;
    for (std::size_t index = 0; index < 3; ++index) {
        const CollisionVector2 offset = Subtract(center, points[index]);
        if (radiusSquared >= offset.y * offset.y + offset.x * offset.x)
            return true;
        const CollisionVector2 edge = Subtract(points[(index + 1) % 3], points[index]);
        float side = PackedCross(edge, offset);
        if (orientation > 0.0f)
            side = -side;
        if (side <= 0.0f)
            continue;
        outside = true;
        const float length = edge.y * edge.y + edge.x * edge.x;
        if (side * side > length * radiusSquared)
            return false;
        const float projection = edge.y * offset.y + edge.x * offset.x;
        if (projection >= 0.0f && projection <= length)
            return true;
    }
    return !outside;
}

bool CollisionPointInPolygon(const CollisionVector2 *points, std::uint32_t count, CollisionVector2 point) {
    bool positive = false;
    bool negative = false;
    for (std::uint32_t index = 0; index < count; ++index) {
        const CollisionVector2 edge = Subtract(points[(index + 1) % count], points[index]);
        const CollisionVector2 offset = Subtract(point, points[index]);
        const float side = edge.y * offset.x - edge.x * offset.y;
        negative = negative || side < 0.0f;
        positive = positive || side > 0.0f;
        if (positive && negative)
            return false;
    }
    return positive || negative || PackedLineContainsPoint(CollisionCollinearSpan(points, count), point);
}

bool CollisionShapeIntersectsRecord(const QueryShape &query, std::uint32_t mode, const std::byte *data) {
    const bool circleQuery = query.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
    PackedCollisionBox box{};
    if (!circleQuery) {
        if (query.mode != OrientedBox_OrientedRectXZ_IsPointInTriangle)
            return false;
        std::memcpy(&box, query.packed, sizeof(box));
    }
    const float radius = query.packed[0].x;
    const CollisionVector2 center{query.packed[0].y, query.packed[0].z};
    const auto pointHit = [&](CollisionVector2 point, float targetRadius, bool pointOnly = false) {
        if (!circleQuery)
            return pointOnly ? PackedBoxContainsPoint(box, point) : PackedBoxIntersectsCircle(box, targetRadius, point);
        const CollisionVector2 delta = Subtract(point, center);
        const float combinedRadius = radius + targetRadius;
        return combinedRadius * combinedRadius - (delta.y * delta.y + delta.x * delta.x) >= 0.0f;
    };
    const auto lineHit = [&](PackedCollisionLine line) {
        return circleQuery ? PackedCircleIntersectsLine(radius, center, line) : PackedBoxIntersectsLine(box, line);
    };
    const auto triangleHit = [&](const CollisionVector2 *points) {
        return circleQuery ? CollisionCircleInTriangle(points, radius, center) : PackedBoxIntersectsTriangle(box, points);
    };
    switch (mode) {
    case 0:
        return pointHit(*reinterpret_cast<const CollisionVector2 *>(data), 0.0f, true);
    case 1:
        return lineHit(*reinterpret_cast<const PackedCollisionLine *>(data));
    case 2:
    case 6: {
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(data);
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if (!count)
            return false;
        if (count == 1)
            return pointHit(points[0], 0.0f, true);
        if (count == 2)
            return lineHit({points[0], points[1]});
        if (mode == 6) {
            if (circleQuery) {
                const CollisionVectorProbe bounds{{}, {}, radius, center};
                if (!CollisionVectorBoundsOverlap(bounds, points, count))
                    return false;
                if (CollisionPointInPolygon(points, count, center))
                    return true;
                for (std::uint32_t index = 0; index < count; ++index)
                    if (lineHit({points[index], points[index + 1 == count ? 0 : index + 1]}))
                        return true;
            } else {
                CollisionVector2 triangle[]{points[2], points[1], points[0]};
                std::uint32_t slot = 1;
                if (triangleHit(triangle))
                    return true;
                for (std::uint32_t index = 3; index < count; ++index) {
                    triangle[slot] = points[index];
                    slot ^= 1U;
                    if (triangleHit(triangle))
                        return true;
                }
            }
            return false;
        }
        for (std::uint32_t index = 1; index < count; ++index) {
            const PackedCollisionLine line{points[index - 1], points[index]};
            if (lineHit(line)) {
                g_collisionPolylineHit.mode = CalculateLineIntersection_0;
                std::memcpy(g_collisionPolylineHit.packed, &line, sizeof(line));
                return true;
            }
        }
        return false;
    }
    case 3: {
        const Vector3 &circle = *reinterpret_cast<const Vector3 *>(data);
        return pointHit({circle.y, circle.z}, circle.x);
    }
    case 4:
        return triangleHit(reinterpret_cast<const CollisionVector2 *>(data));
    case 5: {
        const auto &target = *reinterpret_cast<const PackedCollisionBox *>(data);
        return circleQuery ? PackedBoxIntersectsCircle(target, radius, center) : PackedBoxesIntersect(box, target);
    }
    case 7:
    case 8: {
        const auto &line = *reinterpret_cast<const PackedCollisionLine *>(data);
        const float width = *reinterpret_cast<const float *>(data + sizeof(line));
        const PackedCollisionBox strip = PackedStripBox(line, width);
        if (circleQuery ? PackedBoxIntersectsCircle(strip, radius, center) : PackedBoxesIntersect(box, strip))
            return true;
        return mode == 8 && (pointHit(line.start, width) || pointHit(line.end, width));
    }
    default:
        return false;
    }
}

void CollisionVectorPolygonEdges(CollisionVectorProbe &probe, const CollisionVector2 *points, std::uint32_t count, std::int32_t *signs, std::int32_t *enabled,
    Vector3 *equations, float *distances) {
    if (probe.movement.x == 0.0f && probe.movement.y == 0.0f)
        return;
    const float normalX = probe.movement.y;
    const float normalY = -probe.movement.x;
    const float intercept = -(normalY * probe.origin.y + normalX * probe.origin.x);
    const float length = normalY * normalY + normalX * normalX;
    const float radiusSquared = probe.radius * probe.radius;
    for (std::uint32_t index = 0; index < count; ++index) {
        const float side = (normalY * points[index].y + normalX * points[index].x) + intercept;
        signs[index] = side < 0.0f ? -1 : side > 0.0f ? 1 : 0;
        distances[index] = side * side / length - radiusSquared;
    }
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::uint32_t next = index + 1 == count ? 0 : index + 1;
        const CollisionVector2 &a = points[index];
        const CollisionVector2 &b = points[next];
        Vector3 &equation = equations[index];
        equation.x = a.y - b.y;
        equation.y = -(a.x - b.x);
        equation.z = -(equation.x * a.x + equation.y * a.y);
        enabled[index] =
            (a.x != b.x || a.y != b.y) && (signs[index] != signs[next] || !signs[index] || !signs[next] || distances[index] <= 0.0f || distances[next] <= 0.0f);
    }
    const CollisionVector2 interior = count <= 3 ? CollisionVector2{(points[2].x + points[1].x) * 0.5f, (points[2].y + points[1].y) * 0.5f} : points[2];
    const float inside = (equations[0].y * interior.y + equations[0].x * interior.x) + equations[0].z;
    for (std::uint32_t index = 0; index < count; ++index) {
        const Vector3 &equation = equations[index];
        const float side = (equation.y * probe.origin.y + equation.x * probe.origin.x) + equation.z;
        if (enabled[index] && !(inside <= 0.0f ? side < 0.0f : side > 0.0f))
            CollisionVectorLine(probe, {points[index], points[index + 1 == count ? 0 : index + 1]});
    }
}

void CollisionVectorTriangle(CollisionVectorProbe &probe, const CollisionVector2 *points) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 first = Subtract(points[1], points[0]);
    const CollisionVector2 second = Subtract(points[2], points[0]);
    if (std::fabs(PackedCross(first, second)) < 1.0e-9f) {
        const PackedCollisionLine line = Dot(first, second) < 0.0f ?
            PackedCollisionLine{points[1], points[2]} :
            PackedCollisionLine{points[0], Dot(first, first) <= Dot(second, second) ? points[2] : points[1]};
        CollisionVectorLine(probe, line);
        return;
    }
    if (CollisionCircleInTriangle(points, probe.radius, probe.origin)) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    std::int32_t signs[3]{};
    std::int32_t enabled[3]{};
    Vector3 equations[3]{};
    float distances[3]{};
    CollisionVectorPolygonEdges(probe, points, 3, signs, enabled, equations, distances);
}

void CollisionVectorBox(CollisionVectorProbe &probe, const PackedCollisionBox &box) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 points[]{box.origin, AddScaled(box.origin, box.edgeA, 1.0f), AddScaled(AddScaled(box.origin, box.edgeA, 1.0f), box.edgeB, 1.0f),
        AddScaled(box.origin, box.edgeB, 1.0f)};
    if (!CollisionVectorBoundsOverlap(probe, points, 4))
        return;
    if (std::fabs(PackedCross(box.edgeA, box.edgeB)) < 1.0e-9f) {
        CollisionVectorLine(probe, Dot(box.edgeA, box.edgeB) < 0.0f ? PackedCollisionLine{points[1], points[3]} : PackedCollisionLine{points[0], points[2]});
        return;
    }
    if (PackedBoxIntersectsCircle(box, probe.radius, probe.origin)) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    std::int32_t signs[4]{};
    std::int32_t enabled[4]{};
    Vector3 equations[4]{};
    float distances[4]{};
    CollisionVectorPolygonEdges(probe, points, 4, signs, enabled, equations, distances);
}

void CollisionVectorPolygon(CollisionVectorProbe &probe, const CollisionVector2 *points, std::uint32_t count) {
    if ((probe.result.flags & 7U) == 2U || !CollisionVectorBoundsOverlap(probe, points, count))
        return;
    if (count == 1) {
        CollisionVectorCircle(probe, points[0], 0.0f);
        return;
    }
    if (count == 2) {
        CollisionVectorLine(probe, {points[0], points[1]});
        return;
    }
    auto *const signs = static_cast<std::int32_t *>(AllocateSceneTransient(4ULL * count));
    auto *const enabled = static_cast<std::int32_t *>(AllocateSceneTransient(4ULL * count));
    auto *const equations = static_cast<Vector3 *>(AllocateSceneTransient(12ULL * count));
    auto *const distances = static_cast<float *>(AllocateSceneTransient(4ULL * count));
    bool collinear = true;
    const CollisionVector2 first = Subtract(points[1], points[0]);
    for (std::uint32_t index = 2; index < count; ++index) {
        if (std::fabs(PackedCross(first, Subtract(points[index], points[0]))) >= 1.0e-9f) {
            collinear = false;
            break;
        }
    }
    if (collinear) {
        CollisionVectorLine(probe, CollisionCollinearSpan(points, count));
    } else {
        bool overlapping = CollisionPointInPolygon(points, count, probe.origin);
        for (std::uint32_t index = 0; index < count && !overlapping; ++index)
            overlapping = PackedCircleIntersectsLine(probe.radius, probe.origin, {points[index], points[index + 1 == count ? 0 : index + 1]});
        if (overlapping)
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
        else
            CollisionVectorPolygonEdges(probe, points, count, signs, enabled, equations, distances);
    }
    for (const void *allocation :
        {static_cast<const void *>(distances), static_cast<const void *>(equations), static_cast<const void *>(enabled), static_cast<const void *>(signs)}) {
        if (allocation)
            g_TotalSceneFileReadBytes = static_cast<const std::byte *>(allocation) - static_cast<const std::byte *>(g_pSTCH) - 4;
    }
}

void DispatchCollisionVector(CollisionVectorProbe &probe, std::uint32_t mode, const std::byte *data) {
    switch (mode) {
    case 0:
        CollisionVectorCircle(probe, *reinterpret_cast<const CollisionVector2 *>(data), 0.0f);
        break;
    case 1:
        CollisionVectorLine(probe, *reinterpret_cast<const PackedCollisionLine *>(data));
        break;
    case 2: {
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(data);
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if ((probe.result.flags & 7U) == 2U || !count)
            return;
        if (count == 1) {
            CollisionVectorCircle(probe, points[0], 0.0f);
            return;
        }
        if (!CollisionVectorBoundsOverlap(probe, points, count))
            return;
        bool published = false;
        for (std::uint32_t index = 1; index < count; ++index) {
            const PackedCollisionLine line{points[index - 1], points[index]};
            const CollisionVector2 pair[]{line.start, line.end};
            if (!CollisionVectorBoundsOverlap(probe, pair, 2))
                continue;
            CollisionVectorLine(probe, line);
            const std::uint32_t hit = probe.result.flags & 7U;
            if ((hit == 1 && !published) || hit == 2) {
                g_collisionPolylineHit.mode = CalculateLineIntersection_0;
                std::memcpy(g_collisionPolylineHit.packed, &line, sizeof(line));
                published = true;
            }
            if (hit == 2)
                return;
        }
        break;
    }
    case 3: {
        const auto &circle = *reinterpret_cast<const Vector3 *>(data);
        CollisionVectorCircle(probe, {circle.y, circle.z}, circle.x);
        break;
    }
    case 4:
        CollisionVectorTriangle(probe, reinterpret_cast<const CollisionVector2 *>(data));
        break;
    case 5:
        CollisionVectorBox(probe, *reinterpret_cast<const PackedCollisionBox *>(data));
        break;
    case 6:
        CollisionVectorPolygon(probe, reinterpret_cast<const CollisionVector2 *>(data + 4), *reinterpret_cast<const std::uint32_t *>(data));
        break;
    case 7:
    case 8: {
        const auto &line = *reinterpret_cast<const PackedCollisionLine *>(data);
        const float width = *reinterpret_cast<const float *>(data + sizeof(line));
        CollisionVectorBox(probe, PackedStripBox(line, width));
        break;
    }
    }
}

void DispatchCollisionRay(CollisionVectorProbe &probe, std::uint32_t mode, const std::byte *data);

void TraceMapCollisionVector(CollisionVectorProbe &probe, const std::uint8_t *mask, bool swept) {
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!query || !query->collisionSize)
        return;
    CollisionGridView grid{};
    if (!BindCollisionGrid(*query, grid))
        return;
    CollisionVectorProbe world = probe;
    const Vector3 origin = TransformCollisionQueryPoint({probe.origin.x, 0.0f, probe.origin.y});
    world.origin = {origin.x, -origin.z};
    if (g_collisionQueryTransform.active) {
        const Matrix4x4 &matrix = g_collisionQueryTransform.matrix;
        world.movement = {matrix.rows[0].x * probe.movement.x + matrix.rows[2].x * probe.movement.y,
            -(matrix.rows[0].z * probe.movement.x + matrix.rows[2].z * probe.movement.y)};
    } else {
        world.movement.y = -world.movement.y;
    }
    g_selectedCollisionFunction = nullptr;
    CollisionVector2 minimum{};
    CollisionVector2 maximum{};
    CollisionVectorBounds(world, minimum, maximum);
    PackedGridCell first{};
    PackedGridCell last{};
    if (PackedQueryGridRange(*grid.header, minimum, maximum, first, last)) {
        for (std::int32_t z = first.z; z <= last.z; ++z) {
            for (std::int32_t x = first.x; x <= last.x; ++x) {
                std::int32_t offset = -1;
                if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, offset))
                    continue;
                const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + offset);
                std::int32_t recordOffset = -1;
                while (ReadMapInfo(list++, recordOffset) && recordOffset != -1) {
                    const auto *const record = reinterpret_cast<const std::byte *>(query->collision) + recordOffset;
                    std::uint32_t control = 0;
                    std::uint32_t mode = 0;
                    if (!ReadMapInfo(record, control) || !ReadMapInfo(record + 4, mode))
                        continue;
                    if (mask && !(mask[control >> 3] & (1U << (control & 7U))))
                        continue;
                    if (swept)
                        DispatchCollisionVector(world, mode, record + 8);
                    else
                        DispatchCollisionRay(world, mode, record + 8);
                    if ((world.result.flags & 7U) == 2U) {
                        probe.result = world.result;
                        return;
                    }
                }
            }
        }
    }
    probe.result = world.result;
}

bool ExecuteCollisionShapeQuery(const MapQuerySet &querySet, const CollisionGridView &grid, const QueryShape &query, const std::uint8_t *mask) {
    QueryShape world = query;
    CollisionVector2 minimum{};
    CollisionVector2 maximum{};
    if (query.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange) {
        const Vector3 center = TransformCollisionQueryPoint({query.packed[0].y, 0.0f, query.packed[0].z});
        world.packed[0] = {query.packed[0].x, center.x, -center.z};
        minimum = {center.x - query.packed[0].x, -center.z - query.packed[0].x};
        maximum = {center.x + query.packed[0].x, -center.z + query.packed[0].x};
    } else if (query.mode == OrientedBox_OrientedRectXZ_IsPointInTriangle) {
        const Vector3 origin = TransformCollisionQueryPoint({query.packed[0].x, 0.0f, query.packed[0].y});
        PackedCollisionBox box{{origin.x, -origin.z}, {query.packed[0].z, -query.packed[1].x}, {query.packed[1].y, -query.packed[1].z}};
        if (g_collisionQueryTransform.active) {
            const Matrix4x4 &matrix = g_collisionQueryTransform.matrix;
            box.edgeA = {matrix.rows[0].x * query.packed[0].z + matrix.rows[2].x * query.packed[1].x,
                -(matrix.rows[0].z * query.packed[0].z + matrix.rows[2].z * query.packed[1].x)};
            box.edgeB = {matrix.rows[0].x * query.packed[1].y + matrix.rows[2].x * query.packed[1].z,
                -(matrix.rows[0].z * query.packed[1].y + matrix.rows[2].z * query.packed[1].z)};
        }
        std::memcpy(world.packed, &box, sizeof(box));
        minimum = maximum = box.origin;
        for (const CollisionVector2 edge : {box.edgeA, box.edgeB}) {
            if (edge.x >= 0.0f)
                maximum.x += edge.x;
            else
                minimum.x += edge.x;
            if (edge.y >= 0.0f)
                maximum.y += edge.y;
            else
                minimum.y += edge.y;
        }
    } else {
        return false;
    }
    g_selectedCollisionFunction = nullptr;
    PackedGridCell first{};
    PackedGridCell last{};
    if (!PackedQueryGridRange(*grid.header, minimum, maximum, first, last))
        return false;
    for (std::int32_t z = first.z; z <= last.z; ++z) {
        for (std::int32_t x = first.x; x <= last.x; ++x) {
            std::int32_t offset = -1;
            if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, offset))
                continue;
            const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + offset);
            std::int32_t recordOffset = -1;
            while (ReadMapInfo(list++, recordOffset) && recordOffset != -1) {
                const auto *const record = reinterpret_cast<const std::byte *>(querySet.collision) + recordOffset;
                std::int32_t control = 0;
                std::uint32_t mode = 0;
                if (!ReadMapInfo(record, control) || !ReadMapInfo(record + 4, mode))
                    continue;
                if (mask && !(mask[control >> 3] & (1U << (control & 7))))
                    continue;
                if (CollisionShapeIntersectsRecord(world, mode, record + 8)) {
                    g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(record + 4);
                    return true;
                }
            }
        }
    }
    return false;
}

bool ExecutePackedLineQuery(const MapQuerySet &querySet, const CollisionGridView &grid, const QueryShape &query, const std::uint8_t *mask) {
    const Vector3 start = TransformCollisionQueryPoint({query.packed[0].x, 0.0f, query.packed[0].y});
    const Vector3 end = TransformCollisionQueryPoint({query.packed[0].z, 0.0f, query.packed[1].x});
    const PackedCollisionLine line{{start.x, -start.z}, {end.x, -end.z}};
    PackedCollisionBox strip{};
    const PackedCollisionBox *box = nullptr;
    CollisionVector2 minimum{std::min(line.start.x, line.end.x), std::min(line.start.y, line.end.y)};
    CollisionVector2 maximum{std::max(line.start.x, line.end.x), std::max(line.start.y, line.end.y)};
    if (query.mode == IsPointInTriangle_2) {
        strip = PackedStripBox(line, query.packed[1].y);
        box = &strip;
        minimum = strip.origin;
        maximum = strip.origin;
        for (const CollisionVector2 edge : {strip.edgeA, strip.edgeB}) {
            if (edge.x >= 0.0f)
                maximum.x += edge.x;
            else
                minimum.x += edge.x;
            if (edge.y >= 0.0f)
                maximum.y += edge.y;
            else
                minimum.y += edge.y;
        }
    }
    g_selectedCollisionFunction = nullptr;
    PackedGridCell first{};
    PackedGridCell last{};
    if (!PackedQueryGridRange(*grid.header, minimum, maximum, first, last))
        return false;
    for (std::int32_t z = first.z; z <= last.z; ++z) {
        for (std::int32_t x = first.x; x <= last.x; ++x) {
            std::int32_t offset = -1;
            if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, offset))
                continue;
            const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + offset);
            std::int32_t recordOffset = -1;
            while (ReadMapInfo(list++, recordOffset) && recordOffset != -1) {
                const auto *record = reinterpret_cast<const std::byte *>(querySet.collision) + recordOffset;
                std::uint32_t control = 0;
                std::uint32_t primitive = 0;
                if (!ReadMapInfo(record, control) || !ReadMapInfo(record + 4, primitive))
                    continue;
                if (mask && (control >= 256U || !(mask[control >> 3] & (1U << (control & 7U)))))
                    continue;
                if (PackedQueryIntersectsRecord(line, box, primitive, record + 8)) {
                    g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(record + 4);
                    return true;
                }
            }
        }
    }
    return false;
}

void ApplyCollisionQueryAncestor(CharacterNode *node) {
    if (!node)
        return;
    MapQuerySet *embeddedQuery = nullptr;
    // CTRL's 0x2c transform records and character ancestors share this slot.
    std::memcpy(&embeddedQuery, reinterpret_cast<const std::byte *>(node) + 0x24, sizeof(embeddedQuery));
    if (embeddedQuery) {
        g_activeMapQuerySet = embeddedQuery;
        return;
    }
    ApplyCollisionQueryAncestor(node->child);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    g_collisionQueryTransform.active = true;
}

void CaptureCollisionQueryTransform(CharacterNode *node) {
    g_collisionQueryTransform = {};
    Matrix4x4::SetActiveTransformToPrePass();
    ApplyCollisionQueryAncestor(node);
    g_collisionQueryTransform.matrix = workTransformMatrix;
    Matrix4x4::Pop();
}

float HighestCollisionBelow(Character *character, const Vector3 &localPoint) {
    g_selectedCollisionFunction = nullptr;
    if (!SelectCharacterMapQuery(character))
        return 0.0f;
    return FindHighestValidCollisionBelow(localPoint, 0.1f, 0.1f);
}

bool PropertyShapeRangeValid(std::uint32_t functionIndex, const std::byte *data, const std::byte *end) {
    if (!data || data > end)
        return false;
    const std::size_t remaining = static_cast<std::size_t>(end - data);
    switch (functionIndex) {
    case 0:
        return remaining >= sizeof(CollisionVector2);
    case 1:
        return remaining >= 2 * sizeof(CollisionVector2);
    case 2:
    case 6:
    case 7:
    case 8: {
        if (remaining < sizeof(std::uint32_t))
            return false;
        std::uint32_t count = 0;
        std::memcpy(&count, data, sizeof(count));
        return count > 0 && count <= 128 && remaining - sizeof(count) >= static_cast<std::size_t>(count) * sizeof(CollisionVector2);
    }
    case 3:
        return remaining >= sizeof(float) + sizeof(CollisionVector2);
    case 4:
        return remaining >= 3 * sizeof(CollisionVector2);
    case 5:
        return remaining >= 6 * sizeof(float);
    default:
        return false;
    }
}

const std::byte *FindActivePropertyRecord(const Vector3 *point, std::uint32_t token) {
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!query || !point || !query->property || !query->propertySize) {
        return nullptr;
    }

    const std::int32_t queryIndex = QueryGridIndex(*query, query->property);
    if (queryIndex < 0)
        return nullptr;
    CollisionGridView grid{};
    if (!BindQueryGrid(*query, static_cast<std::uint32_t>(queryIndex), grid)) {
        return nullptr;
    }

    const int x = GridCoordinate(point->x, grid.header->minX, grid.header->cellSizeX, grid.header->width);
    const int z = GridCoordinate(point->z, -grid.header->zOrigin, grid.header->cellSizeZ, grid.header->height);
    std::int32_t listOffset = -1;
    if (!ReadMapInfo(grid.cellOffsets + x + z * grid.header->width, listOffset) || listOffset < 0) {
        return nullptr;
    }

    const auto *const list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);
    const auto *const property = reinterpret_cast<const std::byte *>(query->property);
    const std::size_t chunkSize = sizeof(MapInfoSubchunk) + static_cast<std::size_t>(query->propertySize);
    if (!MapInfoRangeValid(property, chunkSize))
        return nullptr;
    const std::byte *const propertyEnd = property + chunkSize;
    const CollisionVector2 center{point->x, -point->z};
    for (std::size_t index = 0; index < 4096; ++index) {
        std::int32_t recordOffset = -1;
        if (!ReadMapInfo(list + index, recordOffset) || recordOffset == -1) {
            break;
        }
        if (recordOffset < 0 || static_cast<std::size_t>(recordOffset) > chunkSize || 0x18u > chunkSize - static_cast<std::size_t>(recordOffset)) {
            continue;
        }

        const std::byte *const record = property + recordOffset;
        std::uint32_t recordToken = 0;
        std::uint32_t rawValueLow = 0;
        std::uint32_t rawValueHigh = 0;
        std::uint32_t functionIndex = 0;
        if (!ReadMapInfo(record, recordToken) || recordToken != token || !ReadMapInfo(record + 4, rawValueLow) || !ReadMapInfo(record + 8, rawValueHigh) ||
            !ReadMapInfo(record + 0x14, functionIndex) || !PropertyShapeRangeValid(functionIndex, record + 0x18, propertyEnd) ||
            !CircleOverlapsRecord(functionIndex, record + 0x18, center, 0.0f)) {
            continue;
        }

        g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(record + 0x14);
        return record;
    }
    return nullptr;
}

} // namespace

bool CheckVectorWithinConditions(const QueryShape &params, const QueryShape &box) {
    return CollisionShapeIntersectsRecord(box, params.mode, reinterpret_cast<const std::byte *>(params.packed));
}

void DispatchVectorOrientationTests(
    const QueryShape &params, CharacterVectorCollisionResult &result, const Vector2 &origin, const Vector2 &movement, float radius) {
    CollisionVectorProbe probe{{movement.x, movement.y}, result, radius, {origin.x, origin.y}};
    DispatchCollisionVector(probe, params.mode, reinterpret_cast<const std::byte *>(params.packed));
    result = probe.result;
}

std::uint16_t COLS_EvaluateWeatherSoundEnvironment(float x, float z) {
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!query || !query->sound || !query->soundSize)
        return 0;

    const Vector3 worldPoint = TransformCollisionQueryPoint({x, 0.0f, z});
    const std::int32_t queryIndex = QueryGridIndex(*query, query->sound);
    CollisionGridView grid{};
    if (queryIndex < 0 || !BindQueryGrid(*query, static_cast<std::uint32_t>(queryIndex), grid))
        return 0;

    const int cellX = GridCoordinate(worldPoint.x, grid.header->minX, grid.header->cellSizeX, grid.header->width);
    const int cellZ = GridCoordinate(worldPoint.z, -grid.header->zOrigin, grid.header->cellSizeZ, grid.header->height);
    std::int32_t listOffset = -1;
    if (!ReadMapInfo(grid.cellOffsets + cellX + cellZ * grid.header->width, listOffset) || listOffset < 0)
        return 0;

    const auto *const list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);
    const auto *const sound = reinterpret_cast<const std::byte *>(query->sound);
    const std::size_t chunkSize = sizeof(MapInfoSubchunk) + static_cast<std::size_t>(query->soundSize);
    if (!MapInfoRangeValid(sound, chunkSize))
        return 0;
    const std::byte *const soundEnd = sound + chunkSize;
    const CollisionVector2 center{worldPoint.x, -worldPoint.z};
    std::uint16_t selected = 0;
    for (std::size_t index = 0; index < 4096; ++index) {
        std::int32_t recordOffset = -1;
        if (!ReadMapInfo(list + index, recordOffset) || recordOffset == -1)
            break;
        if (recordOffset < 0 || static_cast<std::size_t>(recordOffset) > chunkSize || 8u > chunkSize - static_cast<std::size_t>(recordOffset)) {
            continue;
        }

        const std::byte *const record = sound + recordOffset;
        std::uint32_t soundId = 0;
        std::uint32_t functionIndex = 0;
        if (!ReadMapInfo(record, soundId) || !ReadMapInfo(record + 4, functionIndex) || !PropertyShapeRangeValid(functionIndex, record + 8, soundEnd) ||
            !CircleOverlapsRecord(functionIndex, record + 8, center, 0.0f)) {
            continue;
        }

        const auto candidate = static_cast<std::uint16_t>(soundId);
        const std::uint16_t environment = candidate % 100U;
        if (g_weatherForecastValue == 0 && (environment == 15U || environment == 19U || environment == 20U)) {
            continue;
        }
        if (candidate > selected)
            selected = candidate;
    }
    return selected == 0 ? 0 : static_cast<std::uint16_t>(selected % 100U);
}

namespace {



std::uint32_t COLS_UpdateTaskCollisionCache(std::int32_t slot, std::int32_t onlyIfChanged, const Vector3 *point) {
    if (slot < 0 || static_cast<std::size_t>(slot) >= g_taskCollisionCache.size())
        return 0xffffffffU;
    const std::size_t index = static_cast<std::size_t>(slot);
    std::int32_t value4 = 0;
    std::int32_t value8 = 0;
    std::int32_t valueC = 0;
    std::int32_t value10 = 0;
    const CollisionPointQueryOutputs outputs{.value4 = &value4, .value8 = &value8, .valueC = &valueC, .value10 = &value10};
    const std::uint32_t token = 0x00544e43U | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(slot + '0')) << 24U);
    if (!point || !CheckIsPointIntersectWith(*point, &outputs, token)) {
        g_taskCollisionCache[index] = -1;
        return 0xffffffffU;
    }
    if (onlyIfChanged && g_taskCollisionCache[index] == value4)
        return 0xffffffffU;
    g_taskCollisionCache[index] = value4;
    return static_cast<std::uint32_t>(value4);
}

Vector3 ResolvePlayerGround(const MapControlData &control, const CharacterCollisionWork &work, const Vector3 &start, Vector3 wanted, float radius) {
    if ((work.stateFlags & 0x80u) != 0)
        return wanted;

    float startGroundY = start.y;
    float targetGroundY = wanted.y;
    const bool foundStartGround = TrySampleGroundY(control, start, radius, startGroundY);
    if (!TrySampleGroundY(control, wanted, radius, targetGroundY))
        return wanted;

    const bool startGroundMatchesCharacter = foundStartGround && startGroundY >= start.y - 0.30000001f && startGroundY <= start.y + 0.5f;
    const float referenceGroundY = startGroundMatchesCharacter ? startGroundY : start.y;
    if (targetGroundY - referenceGroundY < -0.30000001f) {
        wanted.y = start.y;
    } else {
        wanted.y = targetGroundY;
    }
    return wanted;
}
// HMDL_t's root node: MT5 stores the rotation as integer angles.
struct SearchModelTransform {
    std::uint32_t nodeId;
    std::uint32_t offset;
    Vector3i rotation;
    Vector3 scale;
    Vector3 position;
};

} // namespace




bool COLS_SearchSegmentHitsCycleCharacter(const Character &character, const Vector3 &start, const Vector3 &end) {
    std::uint32_t cycleId = 0;
    const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const scenario = control && control->scenarioTask ? reinterpret_cast<ScenarioTaskData *>(control->scenarioTask->task_data_ptr) : nullptr;
    if (scenario) {
        const std::size_t bucket = reinterpret_cast<std::uintptr_t>(&character) % scenario->characterBuckets.size();
        CharacterIdLookupNode **link = &scenario->characterBuckets[bucket];
        while (*link && (*link)->character != &character)
            link = &(*link)->bucketNext;
        if (CharacterIdLookupNode *const node = *link) {
            *link = node->bucketNext;
            node->bucketNext = scenario->characterBuckets[bucket];
            scenario->characterBuckets[bucket] = node;
            cycleId = node->id;
        }
    }
    const CycleManager::RuntimeState &cm = g_cycleManagerRuntime;
    const CycleManager::ShutRow *row = nullptr;
    if (!cm.shutRows.empty() && CycleManager::CurrentAreaId() == cm.shutAreaID.name) {
        for (const CycleManager::ShutRow &candidate : cm.shutRows) {
            if (!candidate.token.name)
                break;
            if (candidate.token.name == cycleId) {
                row = &candidate;
                break;
            }
        }
    }
    if (!row)
        return false;



    CharacterNode &node = const_cast<CharacterNode &>(character.entity.node);
    const Vector3 entityPosition = GetPositionForEntity(node);
    const Vector3i entityEuler = GetEulerAnglesForEntity(node);
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(entityPosition.x, entityPosition.y, entityPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(entityEuler.y, entityEuler.x, entityEuler.z);
    if (const auto *const model = static_cast<const SearchModelTransform *>(character.model)) {
        Matrix4x4::preTranslate(model->position.x, model->position.y, model->position.z);
        Matrix4x4::SetViewMatrixFromEuler(model->rotation.y, model->rotation.x, model->rotation.z);
    }
    Matrix4x4_Push();
    Matrix4x4::preTranslate(2.5f, 0.0f, 0.0f);
    const Vector3 sideA{workTransformMatrix.rows[3].x, 0.0f, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    Matrix4x4::preTranslate(-2.5f, 0.0f, 0.0f);
    const float blockerX = workTransformMatrix.rows[3].x - sideA.x;
    const float blockerZ = workTransformMatrix.rows[3].z - sideA.z;
    Matrix4x4::Pop(1);

    const float subjectX = end.x - start.x;
    const float subjectZ = end.z - start.z;
    const float denominator = subjectZ * blockerX - subjectX * blockerZ;
    if (std::fabs(denominator) <= 0.000001f)
        return false;
    const float offsetX = sideA.x - start.x;
    const float offsetZ = sideA.z - start.z;
    const float blockerT = (subjectX * offsetZ - subjectZ * offsetX) / denominator;
    if (blockerT <= 0.0f || blockerT >= 1.0f)
        return false;
    const float subjectT = std::fabs(subjectX) > 0.000001f ? (blockerT * blockerX + offsetX) / subjectX :
                                                             (std::fabs(subjectZ) > 0.000001f ? (blockerT * blockerZ + offsetZ) / subjectZ : -1.0f);
    return subjectT > 0.0f && subjectT < 1.0f;
}

namespace {

void CollisionRayPoint(CollisionVectorProbe &probe, CollisionVector2 point, bool outside) {
    const std::uint32_t hit = probe.result.flags & 7U;
    if (outside) {
        if (probe.origin.x != point.x || probe.origin.y != point.y) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
        } else if ((probe.movement.x != 0.0f || probe.movement.y != 0.0f) && ((hit == 1 && probe.result.fraction > 0.0f) || hit == 0)) {
            probe.result.flags = (probe.result.flags & ~7U) | 1U;
            if (probe.result.flags & 8U)
                probe.result.angle = 0;
        }
        return;
    }
    if (hit == 2)
        return;
    const CollisionVector2 delta = Subtract(point, probe.origin);
    if (std::fabs(probe.movement.y * delta.x - probe.movement.x * delta.y) >= 1.0e-9f)
        return;
    const CollisionVector2 end = AddScaled(probe.origin, probe.movement, 1.0f);
    if (point.x < std::min(probe.origin.x, end.x) || point.x > std::max(probe.origin.x, end.x) || point.y < std::min(probe.origin.y, end.y) ||
        point.y > std::max(probe.origin.y, end.y))
        return;
    if (std::fabs(probe.movement.x) < 1.0e-9f && std::fabs(probe.movement.y) < 1.0e-9f) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    const float fraction = std::fabs(probe.movement.x) < 1.0e-9f ? delta.y / probe.movement.y : delta.x / probe.movement.x;
    if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U))
        probe.result.angle = 0;
}

void CollisionRayLineWithExterior(CollisionVectorProbe &probe, PackedCollisionLine line, bool outside = false) {
    if ((probe.result.flags & 7U) == 2U)
        return;
    const CollisionVector2 edge = Subtract(line.end, line.start);
    if (outside && !PackedLineContainsPoint(line, probe.origin)) {
        probe.result.flags = (probe.result.flags & ~7U) | 2U;
        return;
    }
    if (std::fabs(edge.x) < 1.0e-9f && std::fabs(edge.y) < 1.0e-9f) {
        CollisionRayPoint(probe, line.start, outside);
        return;
    }
    const float denominator = probe.movement.x * edge.y - probe.movement.y * edge.x;
    if (outside) {
        if (std::fabs(denominator) < 1.0e-9f) {
            CollisionRayPoint(probe, line.start, false);
            CollisionRayPoint(probe, line.end, false);
            return;
        }
        if (!CollisionSelectVectorFraction(probe, 0.0f))
            return;
    } else {
        const CollisionVector2 delta = Subtract(probe.origin, line.start);
        if (std::fabs(denominator) < 1.0e-9f) {
            if (std::fabs(delta.x * edge.y - delta.y * edge.x) >= 1.0e-9f)
                return;
            const float along = delta.y * edge.y + delta.x * edge.x;
            const float lengthSquared = edge.y * edge.y + edge.x * edge.x;
            if (along >= 0.0f && along <= lengthSquared) {
                probe.result.flags = (probe.result.flags & ~7U) | 2U;
                return;
            }
            const float movement = probe.movement.y * edge.y + probe.movement.x * edge.x;
            if (std::fabs(movement) < 1.0e-9f)
                return;
            const float fraction = (along >= 0.0f ? along - lengthSquared : -along) / movement;
            if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U))
                probe.result.angle = 0x4000;
            return;
        }
        const float segmentFraction = (probe.movement.x * delta.y - probe.movement.y * delta.x) / denominator;
        if (segmentFraction < 0.0f || segmentFraction > 1.0f)
            return;
        const float fraction = -(delta.x * edge.y - delta.y * edge.x) / denominator;
        if (!CollisionSelectVectorFraction(probe, fraction))
            return;
    }
    if (probe.result.flags & 8U)
        probe.result.angle =
            CollisionFoldContactAngle(probe.movement.x * -edge.x - probe.movement.y * edge.y, probe.movement.y * -edge.x + probe.movement.x * edge.y);
}

bool CollisionTriangleContainsPoint(const CollisionVector2 *points, CollisionVector2 point) {
    const CollisionVector2 first = Subtract(points[1], points[0]);
    const CollisionVector2 second = Subtract(points[2], points[0]);
    float area = second.y * first.x - second.x * first.y;
    if (std::fabs(area) < 1.0e-9f) {
        const CollisionVector2 a = Subtract(points[0], point);
        const CollisionVector2 b = Subtract(points[1], point);
        const CollisionVector2 c = Subtract(points[2], point);
        return std::fabs(b.y * a.x - b.x * a.y) < 1.0e-9f && std::fabs(c.y * b.x - c.x * b.y) < 1.0e-9f &&
            (b.y * a.y + b.x * a.x <= 0.0f || c.y * b.y + c.x * b.x <= 0.0f);
    }
    const CollisionVector2 offset = Subtract(point, points[0]);
    float firstWeight = offset.x * second.y - offset.y * second.x;
    float secondWeight = offset.y * first.x - offset.x * first.y;
    if (area < 0.0f) {
        area *= -1.0f;
        firstWeight *= -1.0f;
        secondWeight *= -1.0f;
    }
    return firstWeight >= 0.0f && secondWeight >= 0.0f && secondWeight + firstWeight <= area;
}


bool CollisionRecordContainsPoint(std::uint32_t mode, const std::byte *data, CollisionVector2 point) {
    const auto pointsEqual = [](CollisionVector2 a, CollisionVector2 b) { return std::fabs(a.x - b.x) < 1.0e-9f && std::fabs(a.y - b.y) < 1.0e-9f; };
    const auto circleContains = [&](float radius, CollisionVector2 center) {
        return radius * radius - ((point.y - center.y) * (point.y - center.y) + (point.x - center.x) * (point.x - center.x)) >= 0.0f;
    };
    switch (mode) {
    case 0:
        return pointsEqual(point, *reinterpret_cast<const CollisionVector2 *>(data));
    case 1:
        return PackedLineContainsPoint(*reinterpret_cast<const PackedCollisionLine *>(data), point);
    case 2: {
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(data);
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if (count == 1)
            return pointsEqual(points[0], point);
        for (std::uint32_t index = 0; index + 1 < count; ++index) {
            if (PackedLineContainsPoint({points[index], points[index + 1]}, point))
                return true;
        }
        return false;
    }
    case 3: {
        const auto &circle = *reinterpret_cast<const Vector3 *>(data);
        return circleContains(circle.x, {circle.y, circle.z});
    }
    case 4:
        return CollisionTriangleContainsPoint(reinterpret_cast<const CollisionVector2 *>(data), point);
    case 5:
        return PackedBoxContainsPoint(*reinterpret_cast<const PackedCollisionBox *>(data), point);
    case 6: {
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(data);
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if (!count)
            return false;
        if (count == 1)
            return pointsEqual(points[0], point);
        if (count == 2)
            return PackedLineContainsPoint({points[0], points[1]}, point);
        return CollisionPointInPolygon(points, count, point);
    }
    case 7:
    case 8: {
        const auto &line = *reinterpret_cast<const PackedCollisionLine *>(data);
        const float width = *reinterpret_cast<const float *>(data + sizeof(line));
        if (PackedBoxContainsPoint(PackedStripBox(line, width), point))
            return true;
        return mode == 8 && (circleContains(width, line.start) || circleContains(width, line.end));
    }
    default:
        return false;
    }
}

void DispatchCollisionRay(CollisionVectorProbe &probe, std::uint32_t mode, const std::byte *data) {
    const bool outside = (probe.result.flags & 0x10U) != 0;
    if (mode == 0) {
        CollisionRayPoint(probe, *reinterpret_cast<const CollisionVector2 *>(data), outside);
        return;
    }
    if (mode == 1) {
        CollisionRayLineWithExterior(probe, *reinterpret_cast<const PackedCollisionLine *>(data), outside);
        return;
    }
    if (mode == 3) {
        const Vector3 &circle = *reinterpret_cast<const Vector3 *>(data);
        if (!outside) {
            CollisionVectorCircle(probe, {circle.y, circle.z}, circle.x);
            return;
        }
        const CollisionVector2 relative = Subtract(probe.origin, {circle.y, circle.z});
        const float distance = relative.y * relative.y + relative.x * relative.x - circle.x * circle.x;
        if (distance >= 0.0f) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
            return;
        }
        const float lengthSquared = probe.movement.x * probe.movement.x + probe.movement.y * probe.movement.y;
        if (std::fabs(lengthSquared) < 1.0e-9f)
            return;
        const float projection = relative.x * probe.movement.x + relative.y * probe.movement.y;
        const float fraction = (std::sqrt(std::fmax(0.0f, projection * projection - lengthSquared * distance)) - projection) / lengthSquared;
        if (CollisionSelectVectorFraction(probe, fraction) && (probe.result.flags & 8U)) {
            const CollisionVector2 normal = AddScaled(relative, probe.movement, fraction);
            probe.result.angle =
                CollisionFoldContactAngle(probe.movement.x * normal.y - probe.movement.y * normal.x, probe.movement.y * normal.y + probe.movement.x * normal.x);
        }
        return;
    }
    if ((probe.result.flags & 7U) == 2U)
        return;
    if (mode == 2 || mode == 6) {
        const std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(data);
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data + 4);
        if (!count)
            return;
        if (mode == 2) {
            if (count == 1) {
                CollisionRayPoint(probe, points[0], outside);
                return;
            }
            bool published = false;
            if (!outside && !CollisionVectorBoundsOverlap(probe, points, count))
                return;
            for (std::uint32_t index = 1; index < count; ++index) {
                const PackedCollisionLine line{points[index - 1], points[index]};
                if (outside) {
                    if (!PackedLineContainsPoint(line, probe.origin))
                        continue;
                    g_collisionPolylineHit.mode = CalculateLineIntersection_0;
                    std::memcpy(g_collisionPolylineHit.packed, &line, sizeof(line));
                    CollisionRayLineWithExterior(probe, line, true);
                    return;
                }
                const CollisionVector2 pair[]{line.start, line.end};
                if (!CollisionVectorBoundsOverlap(probe, pair, 2))
                    continue;
                CollisionRayLineWithExterior(probe, line);
                const std::uint32_t hit = probe.result.flags & 7U;
                if ((hit == 1 && !published) || hit == 2) {
                    g_collisionPolylineHit.mode = CalculateLineIntersection_0;
                    std::memcpy(g_collisionPolylineHit.packed, &line, sizeof(line));
                    published = true;
                }
                if (hit == 2)
                    return;
            }
            if (outside)
                probe.result.flags = (probe.result.flags & ~7U) | 2U;
            return;
        }
        if (!outside && !CollisionVectorBoundsOverlap(probe, points, count))
            return;
        if (CollisionPointInPolygon(points, count, probe.origin) != outside) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
            return;
        }
        CollisionVector2 previous = points[count - 1];
        for (std::uint32_t index = 0; index < count; ++index) {
            CollisionRayLineWithExterior(probe, {previous, points[index]});
            previous = points[index];
        }
        return;
    }
    if (mode == 4) {
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(data);
        if (CollisionTriangleContainsPoint(points, probe.origin) != outside) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
            return;
        }
        CollisionRayLineWithExterior(probe, {points[0], points[1]});
        CollisionRayLineWithExterior(probe, {points[0], points[2]});
        CollisionRayLineWithExterior(probe, {points[1], points[2]});
        return;
    }
    if (mode == 5 || mode == 7 || mode == 8) {
        const PackedCollisionBox box = mode == 5 ?
            *reinterpret_cast<const PackedCollisionBox *>(data) :
            PackedStripBox(*reinterpret_cast<const PackedCollisionLine *>(data), *reinterpret_cast<const float *>(data + sizeof(PackedCollisionLine)));
        if (PackedBoxContainsPoint(box, probe.origin) != outside) {
            probe.result.flags = (probe.result.flags & ~7U) | 2U;
            return;
        }
        const CollisionVector2 first = AddScaled(box.origin, box.edgeA, 1.0f);
        const CollisionVector2 second = AddScaled(box.origin, box.edgeB, 1.0f);
        const CollisionVector2 opposite = AddScaled(first, box.edgeB, 1.0f);
        CollisionRayLineWithExterior(probe, {box.origin, first});
        CollisionRayLineWithExterior(probe, {box.origin, second});
        CollisionRayLineWithExterior(probe, {opposite, second});
        CollisionRayLineWithExterior(probe, {opposite, first});
    }
}

#pragma pack(push, 1)
struct CharacterMovementProbe {
    Character *character;
    CharacterCollisionWork *collision;
    std::uint32_t grounded;
    std::uint8_t queryFlags;
    std::byte reserved15[3];
    std::int32_t yawTolerance;
    std::uint32_t yaw;
    float verticalOffset;
    Vector3 basePosition;
    Vector3 movement;
    float movementLength;
    Vector3 intersection;
    Vector3 extents;
    float radialExtent;
    QueryShape shape;
};
struct CharacterMovementCandidate {
    Character *character;
    CharacterCollisionWork *collision;
    float distance;
    std::uint32_t flags;
    std::int32_t yawError;
    float bottom;
    float top;
    float extra;
};
struct CharacterMovementQuery {
    CharacterMovementProbe probe;
    std::byte reserved78[0x0c];
    float bottom;
    float top;
    std::uint8_t count;
    std::uint8_t blocked;
    std::byte reserved8e[2];
    CharacterMovementCandidate candidates[20];
    std::uint8_t indices[20];
    std::uint8_t controlledPlayer;
    std::byte reserved3c5[3];
    Vector3 reposition;
};
#pragma pack(pop)
static_assert(sizeof(CharacterMovementProbe) == 0x78);
static_assert(sizeof(CharacterMovementCandidate) == 0x28);
static_assert(offsetof(CharacterMovementQuery, candidates) == 0x90);
static_assert(offsetof(CharacterMovementQuery, indices) == 0x3b0);
static_assert(offsetof(CharacterMovementQuery, controlledPlayer) == 0x3c4);
static_assert(offsetof(CharacterMovementQuery, reposition) == 0x3c8);
static_assert(sizeof(CharacterMovementQuery) == 0x3d4);

MapControlData *CharacterCollisionMapControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

void BuildCharacterMovementShape(CharacterMovementQuery &state) {
    CharacterMovementProbe &probe = state.probe;
    probe.intersection = {probe.movement.x + probe.basePosition.x, probe.movement.y + probe.basePosition.y, probe.movement.z + probe.basePosition.z};
    state.bottom = probe.intersection.y;
    state.top = probe.extents.y + probe.intersection.y;
    BuildCharacterCollisionShape(probe.shape, probe.intersection, static_cast<std::uint16_t>(probe.yaw), probe.extents);
}

float SampleCharacterMovementGround(const Vector3 &position) {
    const float height = mapUpdateIntersectionStateAtLocalPoint(position);
    g_selectedCollisionFunction = g_selectedGroundFunction;
    return height;
}

CharacterCollisionWork *NextCharacterCollisionShape() {
    MapControlData *const control = CharacterCollisionMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    while (g_characterCollisionIterator) {
        CharacterCollisionWork *const work = g_characterCollisionIterator;
        g_characterCollisionIterator = work->next;
        Character *const character = work->character;
        if (character == ryo || !ryo || !GetCewpFlag(character) || shenGetNodeFromChara(ryo)->child != character->entity.node.child)
            continue;
        float height = work->shapeExtents.y;
        if (std::fabs(height) < 0.00000011920929f)
            height = 0.0f;
        const float ryoY = GetPositionForEntity(ryo->entity.node).y;
        const float y = GetPositionForEntity(character->entity.node).y;
        if (!(ryoY + 1.8f > y && height + y > ryoY))
            continue;
        if (!(character->flags.value & 4U))
            work->publishedFlags &= ~0x100U;
        if ((work->publishedFlags & 0x104U) == 0x104U && (character->motionFlags & 0x3fU) == 0x3fU)
            return work;
    }
    return nullptr;
}

void RefreshAuxiliaryCharacterCollisionShapes(Character &ryo) {
    g_auxiliaryCollisionCount = static_cast<std::uint32_t>(
        CycleManager::CollectAuxiliaryCollisionShapes(g_auxiliaryCollisionEntries.data(), g_auxiliaryCollisionEntries.size(), ryo.eventGroup));
    g_characterCollisionIterator = g_characterCollisionHead;
    while (g_auxiliaryCollisionCount < g_auxiliaryCollisionEntries.size()) {
        CharacterCollisionWork *const work = NextCharacterCollisionShape();
        if (!work)
            break;
        if (work->shape.mode != SimpleRadial_CircleXZ_CalculateDistanceWithinRange || !MOTM_GetWork(work->character))
            continue;
        AuxiliaryCollisionEntry &entry = g_auxiliaryCollisionEntries[g_auxiliaryCollisionCount++];
        std::memset(&entry, 0, offsetof(AuxiliaryCollisionEntry, shape));
        entry.kind = 1;
        entry.shape.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
        entry.shape.packed[0] = work->shape.packed[0];
    }
}

bool CharacterMovementIntersectsAuxiliary(CharacterMovementProbe &probe) {
    if (probe.collision->avoidanceLock == 1)
        return false;
    g_auxiliaryCollisionCursor = 0;
    while (g_auxiliaryCollisionCursor < g_auxiliaryCollisionCount) {
        const QueryShape &shape = g_auxiliaryCollisionEntries[g_auxiliaryCollisionCursor++].shape;
        if (CollisionShapeIntersectsRecord(probe.shape, shape.mode, reinterpret_cast<const std::byte *>(shape.packed))) {
            g_groundIntersectionUsesAlternateSurface = 0;
            g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(&shape);
            return true;
        }
    }
    return false;
}

std::uint32_t AdjustCharacterMovementForCandidate(CharacterMovementQuery &state) {
    CharacterMovementProbe &probe = state.probe;
    CharacterMovementCandidate &candidate = state.candidates[state.count];
    CharacterCollisionWork &work = *probe.collision;
    if (candidate.flags & 0x100U) {
        const float baseY = probe.basePosition.y;
        if (baseY >= candidate.top) {
            const float difference = baseY - candidate.top;
            const float correction = difference + probe.movement.y;
            probe.movement.y = -difference;
            probe.movementLength -= std::fabs(correction);
        } else if (candidate.bottom >= probe.extents.y + baseY) {
            const float difference = candidate.bottom - (probe.extents.y + probe.intersection.y);
            probe.movement.y += difference;
            probe.movementLength -= difference;
        } else if (candidate.flags & 8U) {
            const float remaining = probe.movementLength - probe.movement.y;
            probe.movement.y = candidate.top - baseY;
            probe.movementLength = probe.movement.y + remaining;
        } else {
            state.blocked = 1;
            BuildCharacterMovementShape(state);
            return 0;
        }
        work.lastAdjustMagnitude = 0.0f;
        if (std::fabs(work.lastAdjustMagnitude) >= 0.01088888943195343f) {
            work.lateralDamp *= 0.30000001f;
            work.forwardDamp *= 0.30000001f;
        } else {
            work.lastAdjustMagnitude = 0.0f;
            work.lateralDamp = 0.0f;
            work.forwardDamp = 0.0f;
        }
        BuildCharacterMovementShape(state);
        return 1;
    }
    state.blocked = 1;
    BuildCharacterMovementShape(state);
    return 0;
}

void InitializeCharacterGroundCandidate(CharacterMovementQuery &state, std::int32_t mode) {
    CharacterMovementProbe &probe = state.probe;
    CharacterCollisionWork &work = *probe.collision;
    SelectCharacterMapQuery(probe.character);
    CharacterMovementCandidate &ground = state.candidates[0];
    ground = {};
    std::uint32_t avoidance = 0;
    std::uint32_t adjustment = 0;
    do {
        const float height = SampleCharacterMovementGround(probe.intersection) + work.groundYOffset;
        state.indices[0] = 0;
        ground.top = height;
        ground.bottom = height - 100.0f;
        float difference = height - probe.intersection.y;
        bool geometryBlocked = false;
        if ((work.stateFlags & 0x1402U) == 2U) {
            if (difference > 0.5f) {
                ground.flags = 0x3b1U;
                avoidance = 3U + (g_groundIntersectionUsesAlternateSurface != 0);
                geometryBlocked = true;
            } else {
                MapControlData *const control = CharacterCollisionMapControl();
                if (ExecuteMathFnWrapper(&probe.shape, reinterpret_cast<const std::uint8_t *>(control->cycleMask))) {
                    avoidance = 2;
                    geometryBlocked = true;
                } else if (CharacterMovementIntersectsAuxiliary(probe) && work.avoidanceLock != 1) {
                    avoidance = 7;
                    geometryBlocked = true;
                }
                if (geometryBlocked) {
                    ground.flags = 0x231U;
                    if (difference >= 0.0f) {
                        ground.flags |= 8U;
                        probe.grounded = 1;
                    }
                }
            }
        }
        if (!geometryBlocked) {
            const float dropLimit = work.groundDropLimit == -68342.0f ? g_characterGroundDropLimit : work.groundDropLimit;
            if (difference < -dropLimit || (difference < 0.0f && ((work.stateFlags & 0x2080U) || probe.movement.y > 0.0f))) {
                ground.flags = 0x14U;
            } else {
                if (difference < 0.0f) {
                    difference = 0.0f;
                    probe.movement.y = ground.top - probe.basePosition.y;
                    probe.intersection.y = probe.movement.y + probe.basePosition.y;
                    state.bottom = probe.intersection.y;
                    state.top = probe.extents.y + probe.intersection.y;
                }
                probe.grounded = 1;
                ground.flags = 0x18U;
                if (mode == 1) {
                    if (difference > 0.0f)
                        ground.flags = 0x119U;
                    else
                        work.lateralDamp = work.lastAdjustMagnitude = work.forwardDamp = 0.0f;
                }
            }
        }
        if (!mode || !(ground.flags & 0x21U))
            return;
        adjustment = AdjustCharacterMovementForCandidate(state);
        if (!(adjustment & 1U)) {
            if (avoidance != 2 && avoidance != 7 && !g_selectedCollisionFunction) {
                SampleCharacterMovementGround(probe.intersection);
                avoidance = 5U + (g_groundIntersectionUsesAlternateSurface != 0);
            }
            if (g_selectedCollisionFunction && *g_selectedCollisionFunction == 2)
                g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(&g_collisionPolylineHit);
            work.intersectionContext = g_selectedCollisionFunction;
            work.avoidanceMode = work.intersectionContext ? avoidance : 0;
        }
    } while (adjustment & 2U);
}

bool RecordCharacterMovementCandidate(CharacterMovementQuery &state, Character &character, std::int32_t mode) {
    CharacterMovementProbe &probe = state.probe;
    CharacterCollisionWork &target = *FindCharacterCollisionWork(&character);
    CharacterMovementCandidate &candidate = state.candidates[state.count];
    const float dx = target.targetPosition.x - probe.intersection.x;
    const float dz = target.targetPosition.z - probe.intersection.z;
    const bool separated = std::fabs(dx) >= 0.001f || std::fabs(dz) >= 0.001f;
    std::int16_t yaw = 0;
    bool accepted = true;
    if (probe.queryFlags & 0x40U) {
        candidate.distance = std::sqrt(dz * dz + dx * dx);
        if (!separated) {
            accepted = false;
        } else {
            yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-dx, -dz));
            const std::int16_t difference = static_cast<std::int16_t>(yaw - cpctGetAccumYaw(&character) + 0x8000);
            accepted = candidate.distance <= target.distanceLimit && candidate.distance <= probe.radialExtent &&
                target.maxAngle >= std::abs(static_cast<std::int32_t>(difference));
        }
    } else {
        const float extent = probe.radialExtent + target.radialAabbExtent;
        accepted = std::fabs(dx) <= extent && std::fabs(dz) <= extent &&
            (mode != 1 ||
                (probe.verticalOffset + state.bottom <= target.targetPosition.y + target.shapeExtents.y &&
                    target.targetPosition.y <= probe.verticalOffset + state.top));
        if (accepted) {
            candidate.distance = std::sqrt(dz * dz + dx * dx);
            if (separated)
                yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-dx, -dz));
        }
    }
    if (accepted && character.entity.node.child == probe.character->entity.node.child) {
        while (CollisionShapeIntersectsRecord(probe.shape, target.shape.mode, reinterpret_cast<const std::byte *>(target.shape.packed))) {
            candidate.flags = 0;
            candidate.yawError = separated ? static_cast<std::int16_t>(yaw - static_cast<std::uint16_t>(probe.yaw)) : 0;
            if (probe.yawTolerance != 0x7fff && std::abs(candidate.yawError) >= std::abs(probe.yawTolerance))
                break;
            candidate.character = &character;
            candidate.collision = &target;
            candidate.bottom = target.targetPosition.y;
            candidate.top = candidate.bottom + target.shapeExtents.y;
            if (std::fabs(candidate.top - probe.intersection.y) < 0.00000011920929f &&
                CollisionShapeIntersectsRecord(probe.collision->shape, target.shape.mode, reinterpret_cast<const std::byte *>(target.shape.packed))) {
                if (probe.queryFlags & 0x20U)
                    break;
                candidate.flags |= 8U;
                probe.grounded = 1;
            } else if (probe.verticalOffset + state.bottom <= candidate.top) {
                candidate.flags |= candidate.bottom < probe.verticalOffset + state.top ? 0x341U : 0x242U;
            } else {
                candidate.flags |= 0x244U;
            }
            if (!mode) {
                std::uint8_t index = 0;
                while (index < state.count && state.candidates[state.indices[index]].bottom <= candidate.bottom)
                    ++index;
                for (std::uint8_t slot = state.count; slot > index; --slot)
                    state.indices[slot] = state.indices[slot - 1];
                state.indices[index] = state.count;
            } else if (candidate.flags & 1U) {
                if (probe.collision->stateFlags & 0x400U) {
                    target.stateFlags |= 0x800U;
                    target.avoidanceMode = 1;
                    target.movementCharacter = probe.character;
                    if (probe.movementLength < 0.5f)
                        target.carryDelta = probe.movement;
                    break;
                }
                const std::uint32_t adjustment = AdjustCharacterMovementForCandidate(state);
                if (!(adjustment & 1U)) {
                    probe.collision->avoidanceMode = 1;
                    probe.collision->movementCharacter = &character;
                }
                if (adjustment & 2U)
                    continue;
            }
            ++state.count;
            break;
        }
    }
    return (!mode || probe.movementLength > 0.0f) && state.count < 20;
}

void GatherCharacterMovementCandidates(CharacterMovementQuery &state, std::int32_t mode) {
    state.count = 0;
    InitializeCharacterGroundCandidate(state, mode);
    state.count = 1;
    CharacterCollisionWork &work = *state.probe.collision;
    if (mode && (!(work.stateFlags & 4U) || state.probe.movementLength <= 0.0f || (work.stateFlags & 0x1000U) || work.avoidanceLock == 1))
        return;
    const auto visit = [&](CharacterCollisionWork &candidate) {
        Character *const character = candidate.character;
        const std::uint32_t flags = candidate.publishedFlags;
        if (character == state.probe.character || (mode && (flags & 0x1000U)) || (flags & 0x300U) != 0x300U || !(flags & 4U))
            return true;
        if (!GetCewpFlag(character) || !(character->flags.value & 4U)) {
            candidate.publishedFlags &= ~0x100U;
            return true;
        }
        return RecordCharacterMovementCandidate(state, *character, mode);
    };
    if (!work.candidateObjectCount) {
        for (CharacterCollisionWork *candidate = g_characterCollisionHead; candidate; candidate = candidate->next)
            if (!visit(*candidate))
                return;
        return;
    }
    std::int32_t remaining = work.candidateObjectCount;
    MapControlData *const control = CharacterCollisionMapControl();
    for (const std::uint32_t objectId : work.candidateObjects) {
        if (remaining <= 0)
            return;
        if (!objectId)
            continue;
        Character *const character = CHID_Find(control->scenarioTask, objectId);
        if (CharacterCollisionWork *const candidate = FindCharacterCollisionWork(character))
            if (!visit(*candidate))
                return;
        --remaining;
    }
}

std::uint32_t TestCharacterAvoidanceShape(CharacterMovementQuery &state, const QueryShape &shape) {
    CharacterMovementProbe &probe = state.probe;
    if (probe.shape.mode != SimpleRadial_CircleXZ_CalculateDistanceWithinRange)
        return 0;
    CollisionVectorProbe vector{
        {probe.movement.x, probe.movement.z}, {8U, 9.9999999e36f, 0}, probe.shape.packed[0].x, {probe.basePosition.x, probe.basePosition.z}};
    DispatchCollisionVector(vector, shape.mode, reinterpret_cast<const std::byte *>(shape.packed));
    if ((vector.result.flags & 7U) == 1U) {
        if (vector.result.fraction <= 1.0f)
            probe.collision->steeringAngle = -vector.result.angle;
        else
            vector.result.flags &= ~7U;
    }
    return vector.result.flags & 7U;
}

std::uint32_t TestCharacterAvoidanceContext(CharacterMovementQuery &state) {
    CharacterMovementProbe &probe = state.probe;
    CharacterCollisionWork &work = *probe.collision;
    if (probe.shape.mode != SimpleRadial_CircleXZ_CalculateDistanceWithinRange || !work.intersectionContext)
        return 0;
    const auto *const record = static_cast<const std::uint32_t *>(work.intersectionContext);
    if (work.avoidanceMode == 7)
        return TestCharacterAvoidanceShape(state, *reinterpret_cast<const QueryShape *>(record));
    if (work.avoidanceMode < 2 || work.avoidanceMode > 6)
        return 0;
    if ((work.avoidanceMode == 4 || work.avoidanceMode == 6) &&
        SampleCharacterMovementGround(probe.intersection) + work.groundYOffset - probe.intersection.y <= 0.5f)
        return 0;
    const Vector3 position = TransformCollisionQueryPoint({probe.basePosition.x, 0.0f, probe.basePosition.z});
    CollisionVectorProbe vector{{probe.movement.x, -probe.movement.z}, {work.avoidanceMode >= 5 ? 0x18U : 8U, 9.9999999e36f, 0},
        work.avoidanceMode == 2 ? probe.shape.packed[0].x : 0.0f, {position.x, -position.z}};
    if (g_collisionQueryTransform.active) {
        const Matrix4x4 &matrix = g_collisionQueryTransform.matrix;
        vector.movement = {matrix.rows[0].x * probe.movement.x + matrix.rows[2].x * probe.movement.z,
            -(matrix.rows[0].z * probe.movement.x + matrix.rows[2].z * probe.movement.z)};
    }
    if (work.avoidanceMode == 2)
        DispatchCollisionVector(vector, record[0], reinterpret_cast<const std::byte *>(record + 1));
    else
        DispatchCollisionRay(vector, record[0], reinterpret_cast<const std::byte *>(record + 1));
    if ((vector.result.flags & 7U) == 1U) {
        if (vector.result.fraction <= 1.0f)
            work.steeringAngle = vector.result.angle;
        else
            vector.result.flags &= ~7U;
    }
    return vector.result.flags & 7U;
}

std::uint32_t AdjustCharacterAvoidanceMovement(CharacterMovementQuery &state) {
    CharacterMovementProbe &probe = state.probe;
    CharacterCollisionWork &work = *probe.collision;
    CharacterCollisionWork *obstacle = nullptr;
    Character *const candidate = work.movementCharacter;
    if (GetCewpFlag(candidate))
        obstacle = FindCharacterCollisionWork(candidate);
    const QueryShape *shape = nullptr;
    std::uint32_t result = 0;
    if (std::fabs(probe.movement.x) >= 0.00000011920929f || std::fabs(probe.movement.z) >= 0.00000011920929f) {
        if (work.avoidanceMode == 1 && obstacle) {
            shape = &obstacle->shape;
            result = TestCharacterAvoidanceShape(state, *shape);
        } else {
            result = TestCharacterAvoidanceContext(state);
        }
    }
    MapControlData *const control = CharacterCollisionMapControl();
    if (!result) {
        work.avoidanceMode = 0;
        work.intersectionContext = nullptr;
    } else if (result == 1) {
        const std::uint16_t tangent = static_cast<std::uint16_t>(work.steeringAngle + (work.steeringAngle <= 0 ? 0x4000 : -0x4000));
        const float length = std::sqrt(probe.movement.x * probe.movement.x + probe.movement.z * probe.movement.z);
        const float cosine = sin_fixed16(fixed16_cosine_angle(tangent));
        const float projectedZ = (cosine * length) * probe.movement.z / length;
        const float projectedX = (cosine * length) * probe.movement.x / length;
        const float sine = sin_fixed16(tangent);
        float z = projectedZ * cosine - projectedX * sine;
        float x = projectedX * cosine + projectedZ * sine;
        if (control->ryo != probe.character || g_pControlPadController->controlMode != 2 || ((g_ryoCurrentAnimationState - 3) & ~4) == 0 ||
            g_ryoIdleTurnInPlaceCounter >= 20) {
            const float damping = std::fabs(sin_fixed16(work.steeringAngle));
            z *= damping;
            x *= damping;
        }
        probe.movement.x = x;
        probe.movement.z = z;
    } else if (result == 2 && shape && shape->mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange &&
        (control->ryo == probe.character || (obstacle && control->ryo == obstacle->movementCharacter))) {
        Vector3 direction{probe.basePosition.x - shape->packed[0].y, 0.0f, probe.basePosition.z - shape->packed[0].z};
        if (std::fabs(direction.x) >= 0.001f || std::fabs(direction.z) >= 0.001f)
            Vector3::NormalizeInPlace(direction);
        probe.movement.x = direction.x * 0.039999999f;
        probe.movement.z = direction.z * 0.039999999f;
        work.avoidanceLock = 1;
    }
    return result;
}

void CharacterCollisionRepresentativePoint(const std::uint32_t *record, CollisionVector2 &point) {
    const auto *const data = reinterpret_cast<const float *>(record + 1);
    switch (record[0]) {
    case 0:
        point = {data[0], data[1]};
        break;
    case 1:
        point = {(data[2] + data[0]) * 0.5f, (data[3] + data[1]) * 0.5f};
        break;
    case 3:
        point = {data[1], data[2]};
        break;
    case 4:
        point = {(data[2] + data[0] + data[4]) / 3.0f, (data[3] + data[1] + data[5]) / 3.0f};
        break;
    case 5: {
        const float oppositeX = data[0] + data[4];
        const float oppositeY = data[1] + data[5];
        point = {((data[0] + (data[0] + data[2]) + oppositeX) + (data[2] + oppositeX)) * 0.25f,
            ((data[1] + (data[1] + data[3]) + oppositeY) + (data[3] + oppositeY)) * 0.25f};
        break;
    }
    case 6: {
        const std::uint32_t count = record[1];
        const auto *const points = reinterpret_cast<const CollisionVector2 *>(record + 2);
        float x = 0.0f;
        float y = 0.0f;
        for (std::uint32_t index = 0; index < count; ++index) {
            x += points[index].x;
            y += points[index].y;
        }
        point = {x / static_cast<float>(count), y / static_cast<float>(count)};
        break;
    }
    }
}

std::int16_t ComputeDirectionAngleDelta(const Vector3 &origin, const Vector3 &target, const Vector3 &movement) {
    const float dx = target.x - origin.x;
    const float dz = target.z - origin.z;
    if ((std::fabs(movement.x) < 0.001f && std::fabs(movement.z) < 0.001f) || (std::fabs(dx) < 0.001f && std::fabs(dz) < 0.001f)) {
        return 0;
    }
    return static_cast<std::int16_t>(Shenmue_Atan2Fixed(movement.x, movement.z) - Shenmue_Atan2Fixed(dx, dz));
}

bool AdjustCharacterMovementAroundAuxiliary(CharacterMovementQuery &state) {
    CharacterMovementProbe &probe = state.probe;
    CharacterCollisionWork &work = *probe.collision;
    CollisionVector2 representative;
    CharacterCollisionRepresentativePoint(static_cast<const std::uint32_t *>(work.intersectionContext), representative);
    const Vector3 representativePosition{representative.x, 0.0f, representative.y};
    const std::int16_t difference = ComputeDirectionAngleDelta(probe.basePosition, representativePosition, probe.movement);
    if (std::abs(static_cast<std::int32_t>(difference)) >= 0x4000) {
        work.avoidanceLock = 1;
        return true;
    }
    const std::uint16_t tangent = static_cast<std::uint16_t>((difference <= 0 ? -0x4000 : 0x4000) - difference);
    const float length = std::sqrt(probe.movement.x * probe.movement.x + probe.movement.z * probe.movement.z);
    const float cosine = sin_fixed16(fixed16_cosine_angle(tangent));
    const float projected = cosine * length;
    if (length <= 0.0f)
        return false;
    const float projectedX = projected * probe.movement.x / length;
    const float projectedZ = projected * probe.movement.z / length;
    const float sine = sin_fixed16(tangent);
    float z = projectedZ * cosine - projectedX * sine;
    float x = projectedZ * sine + projectedX * cosine;
    if (((g_ryoCurrentAnimationState - 3) & ~4) == 0 || g_ryoIdleTurnInPlaceCounter >= 20) {
        const float damping = std::fabs(sin_fixed16(difference));
        x *= damping;
        z *= damping;
    }
    probe.movement.x = x;
    probe.movement.z = z;
    work.avoidanceLock = 0;
    return true;
}

Vector3 CharacterBackwardDisplacement(const Vector3 &origin, std::int16_t yaw, float distance) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY(sin_fixed16(yaw), sin_fixed16(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw))));
    const float x = ((workTransformMatrix.rows[0].x * 0.0f + workTransformMatrix.rows[1].x * 0.0f) + workTransformMatrix.rows[2].x * -distance) +
        workTransformMatrix.rows[3].x;
    const float z = ((workTransformMatrix.rows[0].z * 0.0f + workTransformMatrix.rows[1].z * 0.0f) + workTransformMatrix.rows[2].z * -distance) +
        workTransformMatrix.rows[3].z;
    Matrix4x4::Pop();
    return {x + origin.x, origin.y, z + origin.z};
}

std::uint32_t RepositionCharacterFromFacing(CharacterMovementQuery &state, std::int16_t normalYaw, std::int16_t tangentYaw, float displacement) {
    CharacterCollisionWork &work = *state.probe.collision;
    const float dx = work.lastRepositionPosition.x - work.targetPosition.x;
    const float dz = work.lastRepositionPosition.z - work.targetPosition.z;
    if (std::sqrt(dx * dx + dz * dz) <= 0.005999999888241291f) {
        const std::int16_t difference = static_cast<std::int16_t>(work.lastRepositionYaw - normalYaw);
        if (std::abs(static_cast<std::int32_t>(difference)) >= 30947)
            return 0;
    }
    const Vector3 &movement = state.probe.movement;
    float tangentDistance = 0.0f;
    if (std::fabs(movement.x) >= 0.001f || std::fabs(movement.z) >= 0.001f) {
        const std::uint16_t difference = static_cast<std::uint16_t>(Shenmue_Atan2Fixed(-movement.x, -movement.z) - tangentYaw);
        const float cosine = sin_fixed16(fixed16_cosine_angle(difference));
        tangentDistance = cosine * std::sqrt(movement.x * movement.x + movement.z * movement.z);
    }
    state.reposition = CharacterBackwardDisplacement({}, normalYaw, displacement);
    work.lastRepositionPosition = work.targetPosition;
    work.lastRepositionYaw = normalYaw;
    state.probe.movement = CharacterBackwardDisplacement(state.reposition, tangentYaw, tangentDistance);
    return 2;
}

std::uint32_t RepositionCharacterFromPoint(CharacterMovementQuery &state, const std::uint32_t *record, bool mapCoordinates) {
    CollisionVector2 point;
    CharacterCollisionRepresentativePoint(record, point);
    if (mapCoordinates)
        point.y = -point.y;
    const float dx = point.x - state.probe.collision->targetPosition.x;
    const float dz = point.y - state.probe.collision->targetPosition.z;
    if (std::fabs(dx) < 0.001f && std::fabs(dz) < 0.001f)
        return 0;
    const auto yaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(dx, dz));
    return RepositionCharacterFromFacing(state, yaw, static_cast<std::int16_t>(yaw + 0x4000), state.probe.collision->movementFailTimer);
}

std::uint32_t RepositionCharacterFromLine(CharacterMovementQuery &state, PackedCollisionLine line) {
    const float directionX = line.end.x - line.start.x;
    const float directionZ = -line.end.y + line.start.y;
    if (std::fabs(directionX) < 0.001f && std::fabs(directionZ) < 0.001f)
        return 0;
    const auto tangent = static_cast<std::int16_t>(Shenmue_Atan2Fixed(-directionX, -directionZ));
    const Vector3 &previous = state.probe.collision->targetPosition;
    const float startX = line.start.x - previous.x;
    const float startZ = -line.start.y - previous.z;
    const float endX = line.end.x - previous.x;
    const float endZ = -line.end.y - previous.z;
    if ((std::fabs(startX) < 0.001f && std::fabs(startZ) < 0.001f) || (std::fabs(endX) < 0.001f && std::fabs(endZ) < 0.001f))
        return 0;
    const auto normal = static_cast<std::int16_t>(tangent + (endZ * startX - endX * startZ >= 0.0f ? -0x4000 : 0x4000));
    return RepositionCharacterFromFacing(state, normal, tangent, state.probe.collision->movementFailTimer);
}

std::uint32_t TryCharacterFacingRecovery(CharacterMovementQuery &state) {
    if (!state.controlledPlayer)
        return 0;
    CharacterCollisionWork &work = *state.probe.collision;
    std::uint32_t result = 0;
    if (work.avoidanceMode == 1) {
        Character *const character = work.movementCharacter;
        if (GetCewpFlag(character)) {
            CharacterCollisionWork *const obstacle = FindCharacterCollisionWork(character);
            result = RepositionCharacterFromPoint(state, reinterpret_cast<const std::uint32_t *>(&obstacle->shape), false);
        }
    } else if (work.avoidanceMode == 2 && work.intersectionContext) {
        const auto *const record = static_cast<const std::uint32_t *>(work.intersectionContext);
        result = record[0] == 1 ? RepositionCharacterFromLine(state, *reinterpret_cast<const PackedCollisionLine *>(record + 1)) :
                                  RepositionCharacterFromPoint(state, record, true);
    } else if (work.avoidanceMode == 7 && ((*static_cast<const std::uint32_t *>(work.intersectionContext) - 3U) & ~2U) == 0) {
        MapControlData *const control = CharacterCollisionMapControl();
        if (control->ryo == state.probe.character && g_pControlPadController->controlMode == 2 && AdjustCharacterMovementAroundAuxiliary(state)) {
            state.reposition = {};
            result = 2;
        }
    }
    if (result)
        BuildCharacterMovementShape(state);
    return result;
}
float SolvePathQuadratic(float a, float b, float c) {
    if (a == 0.0f)
        return b > 0.0f ? -(c / (b + b)) : -1.0f;
    const float discriminant = b * b - a * c;
    if (discriminant < 0.0f)
        return -1.0f;
    const float root = std::sqrt(discriminant);
    float result = (root - b) / a;
    if (((a + a) * result + (b + b)) < 0.0f)
        result = (-b - root) / a;
    return result;
}

float ComputeDistanceToPathCurve(const PathConstraintState &path, const Vector2 &curve) {
    const float yDifference = path.position.y - curve.y;
    const float radiusSquared = curve.x * curve.x;
    return SolvePathQuadratic(((path.tangent.y * path.tangent.y + path.velocity.y * path.velocity.y) * radiusSquared) - path.velocity.x * path.velocity.x,
        ((path.tangent.y * path.position.x + path.velocity.y * path.tangent.x) * radiusSquared) - path.velocity.x * yDifference,
        ((path.position.x * path.position.x + path.tangent.x * path.tangent.x) * radiusSquared) - yDifference * yDifference);
}

void ProjectPointOutOfPathSphere(PathConstraintState &path, float verticalSlope, const Vector3 &sphere) {
    const float dx = path.position.x - sphere.y;
    const float dz = path.tangent.x - sphere.z;
    if (dx * dx + dz * dz > sphere.x * sphere.x)
        return;
    const float radialLength = std::sqrt(path.position.x * path.position.x + path.tangent.x * path.tangent.x);
    const float unitX = path.position.x / radialLength;
    const float unitZ = path.tangent.x / radialLength;
    const float a = unitX * unitX + unitZ * unitZ;
    const float b = dx * unitX + dz * unitZ;
    const float c = dx * dx + dz * dz - sphere.x * sphere.x;
    float distance = SolvePathQuadratic(a, b, c);
    distance = static_cast<float>(static_cast<double>(distance) * 1.01);
    path.position.x += distance * unitX;
    path.position.y += distance * verticalSlope;
    path.tangent.x += distance * unitZ;
}

void ApplyPathSegmentConstraints(PathConstraintState &path, const std::array<Vector2, 4> &curves, const std::array<Vector3, 4> &spheres) {
    const float radialLength = std::sqrt(path.tangent.x * path.tangent.x + path.position.x * path.position.x);
    for (const Vector2 &curve : curves) {
        const float permittedRadius = (path.position.y - curve.y) / curve.x;
        if (radialLength > permittedRadius || permittedRadius < 0.0f) {
            const float correction = curve.x * radialLength + curve.y - path.position.y;
            path.position.y += static_cast<float>(static_cast<double>(correction) + 0.0001);
        }
    }
    for (const Vector3 &sphere : spheres)
        ProjectPointOutOfPathSphere(path, curves[3].x, sphere);
}

} // namespace

std::uint32_t mevwCheckEventArea(float x, float z) {
    const Vector3 world = TransformCollisionQueryPoint({x, 0.0f, z});
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    CollisionGridView grid{};
    if (!query || !query->eventSize || !BindQueryGrid(*query, 2, grid))
        return 0;
    const CollisionGridHeader &header = *grid.header;
    const auto cell = [](float offset, float span, float cellSize, std::int32_t count) {
        if (offset < 0.00000011920929f)
            return 0;
        if (offset > span)
            return count - 1;
        const std::int32_t index = static_cast<std::int32_t>(offset / cellSize);
        return index < 0 ? 0 : (index >= count ? index - 1 : index);
    };
    const std::int32_t cellX = cell(world.x - header.minX, header.spanX, header.cellSizeX, header.width);
    const std::int32_t cellZ = cell(world.z + header.zOrigin, header.spanZ, header.cellSizeZ, header.height);
    std::int32_t listOffset = -1;
    if (!ReadMapInfo(grid.cellOffsets + cellX + cellZ * header.width, listOffset))
        return 0;
    const CollisionVector2 point{world.x, -world.z};
    for (const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);; ++list) {
        std::int32_t recordOffset = -1;
        if (!ReadMapInfo(list, recordOffset) || recordOffset == -1)
            return 0;
        const auto *const record = reinterpret_cast<const std::byte *>(query->event) + (recordOffset & ~3);
        std::uint32_t packed = 0;
        std::uint32_t mode = 0;
        if (!ReadMapInfo(record, packed) || !ReadMapInfo(record + 4, mode))
            return 0;
        if (CollisionRecordContainsPoint(mode, record + 8, point))
            return packed;
    }
}

void COLS_DebugForEachRecord(const void *querySet, std::uint32_t chunkKind, float x, float z, float extent,
    const std::function<void(std::uint32_t tag, std::uint32_t mode, const std::byte *data)> &visit) {
    const auto *const query = static_cast<const MapQuerySet *>(querySet);
    if (!query)
        return;
    const std::array<const MapInfoSubchunk *, 6> chunks{query->collision, query->height, query->event, query->undo, query->sound, query->property};
    if (chunkKind >= chunks.size() || !chunks[chunkKind])
        return;
    const std::int32_t gridIndex = QueryGridIndex(*query, chunks[chunkKind]);
    CollisionGridView grid{};
    if (gridIndex < 0 || !BindQueryGrid(*query, static_cast<std::uint32_t>(gridIndex), grid))
        return;
    const CollisionGridHeader &header = *grid.header;
    const int minX = GridCoordinate(x - extent, header.minX, header.cellSizeX, header.width);
    const int maxX = GridCoordinate(x + extent, header.minX, header.cellSizeX, header.width);
    const int minZ = GridCoordinate(z - extent, -header.zOrigin, header.cellSizeZ, header.height);
    const int maxZ = GridCoordinate(z + extent, -header.zOrigin, header.cellSizeZ, header.height);
    std::vector<std::int32_t> seen;
    for (int cellZ = minZ; cellZ <= maxZ; ++cellZ) {
        for (int cellX = minX; cellX <= maxX; ++cellX) {
            std::int32_t listOffset = -1;
            if (!ReadMapInfo(grid.cellOffsets + cellX + cellZ * header.width, listOffset) || listOffset < 0)
                continue;
            for (const auto *list = reinterpret_cast<const std::int32_t *>(grid.candidates + listOffset);; ++list) {
                std::int32_t recordOffset = -1;
                if (!ReadMapInfo(list, recordOffset) || recordOffset == -1)
                    break;
                recordOffset &= ~3;
                bool duplicate = false;
                for (const std::int32_t offset : seen)
                    duplicate = duplicate || offset == recordOffset;
                if (duplicate)
                    continue;
                seen.push_back(recordOffset);
                const auto *const record = reinterpret_cast<const std::byte *>(chunks[chunkKind]) + recordOffset;
                std::uint32_t tag = 0;
                std::uint32_t mode = 0;
                if (ReadMapInfo(record, tag) && ReadMapInfo(record + 4, mode) && MapInfoRangeValid(record + 8, 0x20))
                    visit(tag, mode, record + 8);
            }
        }
    }
}

namespace {

} // namespace


const std::byte *g_navigationCollisionDataTable = nullptr;

bool au_re_CheckIfPointWithinShape(const QueryShape *query, const AuxiliaryCollisionEntry **matchedEntry) {
    g_auxiliaryCollisionCursor = 0;
    if (!g_auxiliaryCollisionCount)
        return false;
    do {
        const AuxiliaryCollisionEntry &entry = g_auxiliaryCollisionEntries[g_auxiliaryCollisionCursor++];
        if (CollisionShapeIntersectsRecord(*query, entry.shape.mode, reinterpret_cast<const std::byte *>(entry.shape.packed))) {
            *matchedEntry = &entry;
            g_cewpCachedShape = &entry.shape;
            g_cewpCacheKind = 2;
            g_cewpCacheValid = true;
            return true;
        }
    } while (g_auxiliaryCollisionCursor < g_auxiliaryCollisionCount);
    return false;
}

namespace {

PackedCollisionLine CollisionQueryLineToWorld(const QueryShape &query) {
    const Vector3 start = TransformCollisionQueryPoint({query.packed[0].x, 0.0f, query.packed[0].y});
    const Vector3 end = TransformCollisionQueryPoint({query.packed[0].z, 0.0f, query.packed[1].x});
    return {{start.x, -start.z}, {end.x, -end.z}};
}

QueryShape CollisionQueryCircleToWorld(const QueryShape &query) {
    const Vector3 center = TransformCollisionQueryPoint({query.packed[0].y, 0.0f, query.packed[0].z});
    QueryShape world = query;
    world.packed[0] = {query.packed[0].x, center.x, -center.z};
    return world;
}

bool CachedMapRecordIntersects(const QueryShape &query) {
    const std::uint32_t mode = g_cewpCachedRecord[0];
    const auto *const data = reinterpret_cast<const std::byte *>(g_cewpCachedRecord + 1);
    if (query.mode == CalculateLineIntersection_0)
        return PackedQueryIntersectsRecord(CollisionQueryLineToWorld(query), nullptr, mode, data);
    return CollisionShapeIntersectsRecord(CollisionQueryCircleToWorld(query), mode, data);
}

bool ShapeIntersectsQuery(const QueryShape &shape, const QueryShape &query) {
    if (query.mode == CalculateLineIntersection_0) {
        const PackedCollisionLine line{{query.packed[0].x, query.packed[0].y}, {query.packed[0].z, query.packed[1].x}};
        return PackedQueryIntersectsRecord(line, nullptr, shape.mode, reinterpret_cast<const std::byte *>(shape.packed));
    }
    return CollisionShapeIntersectsRecord(query, shape.mode, reinterpret_cast<const std::byte *>(shape.packed));
}

// Shared map tail of cewpTestPointWithinRadius and CEWP_MathFuncs_1.
bool CEWP_TestMapAndCache(const QueryShape &query, const std::uint8_t *mask) {
    if (!g_cewpMapTestsEnabled || !ExecuteMathFnWrapper(&query, mask))
        return false;
    const auto control = static_cast<std::int32_t>(g_selectedCollisionFunction[-1]);
    if (control != -1) {
        g_cewpCacheKind = 1;
        g_cewpCacheControl = static_cast<std::int16_t>(control);
        if (*g_selectedCollisionFunction == 2)
            g_selectedCollisionFunction = reinterpret_cast<const std::uint32_t *>(&g_collisionPolylineHit);
        g_cewpCachedRecord = g_selectedCollisionFunction;
        g_cewpCacheValid = true;
    }
    return control >= 0;
}

bool CEWP_TestCachedMapRecord(const QueryShape &query) {
    return g_cewpMapTestsEnabled && g_cewpCacheValid && g_cewpCacheKind == 1 && CachedMapRecordIntersects(query) && g_cewpCacheControl >= 0;
}

const QueryShape *CEWP_FindCharacterShape(const QueryShape &query) {
    g_characterCollisionIterator = g_characterCollisionHead;
    while (CharacterCollisionWork *const work = NextCharacterCollisionShape()) {
        if (ShapeIntersectsQuery(work->shape, query)) {
            g_cewpCachedShape = &work->shape;
            g_cewpCacheKind = 2;
            g_cewpCacheValid = true;
            return &work->shape;
        }
    }
    const AuxiliaryCollisionEntry *entry = nullptr;
    return au_re_CheckIfPointWithinShape(&query, &entry) ? &entry->shape : nullptr;
}

} // namespace


void CEWP_ResetPointTestCache(Character *player) {
    g_cewpCacheValid = false;
    const CharacterCollisionWork *const work = FindCharacterCollisionWork(player);
    g_cewpMapTestsEnabled = work && (work->stateFlags & 2U) != 0;
    g_cewpCharacterTestsEnabled = work && (work->stateFlags & 4U) != 0;
}

bool CEWP_MapTestsEnabled() {
    return g_cewpMapTestsEnabled;
}

bool CEWP_CharacterTestsEnabled() {
    return g_cewpCharacterTestsEnabled;
}

// cewpGetDesiredAngle
std::int32_t cewpGetDesiredAngle(const QueryShape &shape) {
    CollisionVector2 representative{};
    CharacterCollisionRepresentativePoint(reinterpret_cast<const std::uint32_t *>(&shape), representative);
    const Character *const player = g_playerCharacter;
    std::uint16_t yaw = 0;
    Vector3 position{};
    if (player) {
        for (const CharacterNode *node = &player->entity.node; node; node = node->child)
            yaw = static_cast<std::uint16_t>(yaw + static_cast<std::uint16_t>(node->rotation.y));
        position = GetPositionForEntity(const_cast<Character *>(player)->entity.node);
    }
    const float dx = position.x - representative.x;
    const float dz = position.z - representative.y;
    if (std::fabs(dx) < 0.001f && std::fabs(dz) < 0.001f)
        return 0x8000;
    return static_cast<std::int16_t>(Shenmue_Atan2Fixed(dx, dz) - yaw);
}

// cewpTestPointWithinRadius
bool cewpTestPointWithinRadius(const Vector3 &point, float radius, const std::uint8_t *mask) {
    QueryShape query{};
    query.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
    query.packed[0] = {radius, point.x, point.z};
    if (CEWP_TestCachedMapRecord(query))
        return true;
    if (g_cewpCharacterTestsEnabled) {
        const QueryShape *shape = nullptr;
        if (g_cewpCacheValid && g_cewpCacheKind == 2 && ShapeIntersectsQuery(*g_cewpCachedShape, query))
            shape = g_cewpCachedShape;
        else
            shape = CEWP_FindCharacterShape(query);
        if (shape) {
            const std::int32_t limit = shape->mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange ? 0x4000 : 0x6000;
            if (std::abs(cewpGetDesiredAngle(*shape)) < limit)
                return true;
        }
    }
    return CEWP_TestMapAndCache(query, mask);
}

// CEWP_MathFuncs_1
bool CEWP_TestSegment(const Vector3 &start, const Vector3 &end, const std::uint8_t *mask) {
    QueryShape query{};
    query.mode = CalculateLineIntersection_0;
    query.packed[0] = {start.x, start.z, end.x};
    query.packed[1].x = end.z;
    if (CEWP_TestCachedMapRecord(query))
        return true;
    if (!g_cewpCharacterTestsEnabled)
        return CEWP_TestMapAndCache(query, mask);
    if (g_cewpCacheValid && g_cewpCacheKind == 2 && ShapeIntersectsQuery(*g_cewpCachedShape, query))
        return true;
    if (CEWP_FindCharacterShape(query))
        return true;
    return CEWP_TestMapAndCache(query, mask);
}

bool COLS_UpdatePathWithConstraints(
    PathConstraintState &path, std::int32_t &statusBits, const std::array<Vector2, 4> &curves, const std::array<Vector3, 4> &spheres) {
    std::int32_t hitIndex = -1;
    std::int32_t hitKind = 0;
    float hitTime = path.time;
    for (std::int32_t index = 0; index < 4; ++index) {
        const float distance = ComputeDistanceToPathCurve(path, curves[static_cast<std::size_t>(index)]);
        if (distance > 0.0f && hitTime >= distance) {
            hitTime = distance;
            hitIndex = index;
            hitKind = 1;
        }
    }

    const float tangentVelocity = path.tangent.y;
    const float radialVelocity = path.velocity.y;
    const float radialPosition = path.position.x;
    const float tangentPosition = path.tangent.x;
    for (std::int32_t index = 0; index < 4; ++index) {
        const Vector3 &sphere = spheres[static_cast<std::size_t>(index)];
        const float distance = SolvePathQuadratic(-(radialVelocity * radialVelocity + tangentVelocity * tangentVelocity),
            -(radialVelocity * (tangentPosition - sphere.z) + tangentVelocity * (radialPosition - sphere.y)),
            -(((tangentPosition - sphere.z) * (tangentPosition - sphere.z) + (radialPosition - sphere.y) * (radialPosition - sphere.y)) - sphere.x * sphere.x));
        if (distance > 0.0f && hitTime >= distance) {
            hitTime = distance;
            hitIndex = index;
            hitKind = 2;
        }
    }

    const float verticalVelocity = path.velocity.x;
    if (hitKind != 0) {
        const float hitPositionX = tangentVelocity * hitTime + radialPosition;
        const float hitPositionY = verticalVelocity * hitTime + path.position.y;
        const float hitTangentX = radialVelocity * hitTime + tangentPosition;
        float newTangentVelocity = 0.0f;
        float newVerticalVelocity = 0.0f;
        float newRadialVelocity = 0.0f;
        if (hitKind == 1) {
            const Vector2 &curve = curves[static_cast<std::size_t>(hitIndex)];
            const float radius = std::sqrt(hitTangentX * hitTangentX + hitPositionX * hitPositionX);
            const float normalX = -(curve.x * hitPositionX / radius);
            const float normalZ = -(curve.x * hitTangentX / radius);
            const float normalLength = std::sqrt(normalX * normalX + 1.0f + normalZ * normalZ);
            const float nx = normalX / normalLength;
            const float ny = 1.0f / normalLength;
            const float nz = normalZ / normalLength;
            const float projectedVelocity = tangentVelocity * nx + verticalVelocity * ny + radialVelocity * nz;
            newTangentVelocity = (tangentVelocity - nx * projectedVelocity) - nx * projectedVelocity * 0.5f;
            newVerticalVelocity = (verticalVelocity - ny * projectedVelocity) - ny * projectedVelocity * 0.5f;
            newRadialVelocity = (radialVelocity - nz * projectedVelocity) - nz * projectedVelocity * 0.5f;
        } else {
            const Vector3 &sphere = spheres[static_cast<std::size_t>(hitIndex)];
            const float dx = hitPositionX - sphere.y;
            const float dz = hitTangentX - sphere.z;
            const float radius = std::sqrt(dx * dx + dz * dz);
            const float nx = dx / radius;
            const float nz = dz / radius;
            const float ny = (radius / 10.0f) / radius;
            const float projectedVelocity = tangentVelocity * nx + verticalVelocity * ny + radialVelocity * nz;
            newTangentVelocity = (tangentVelocity - nx * projectedVelocity) - nx * projectedVelocity * 0.34999999f;
            newVerticalVelocity = (verticalVelocity - ny * projectedVelocity) - ny * projectedVelocity * 0.34999999f;
            newRadialVelocity = (radialVelocity - nz * projectedVelocity) - nz * projectedVelocity * 0.34999999f;
            std::int32_t status = static_cast<std::int32_t>(1.0f -
                std::sqrt((verticalVelocity - newVerticalVelocity) * (verticalVelocity - newVerticalVelocity) +
                    (tangentVelocity - newTangentVelocity) * (tangentVelocity - newTangentVelocity) +
                    (radialVelocity - newRadialVelocity) * (radialVelocity - newRadialVelocity)) *
                    -0.5f * 256.0f);
            if (status > 255)
                status = 255;
            if (statusBits < status)
                statusBits = status;
        }

        path.position = {hitPositionX, hitPositionY};
        path.tangent = {hitTangentX, newTangentVelocity};
        path.velocity.x = newVerticalVelocity;
        path.velocity.y = newRadialVelocity;
        path.time -= hitTime;
        ApplyPathSegmentConstraints(path, curves, spheres);
    } else {
        path.position.x += path.time * tangentVelocity;
        path.position.y += path.time * verticalVelocity;
        path.tangent.x += path.time * radialVelocity;
        ApplyPathSegmentConstraints(path, curves, spheres);
        hitTime = path.time;
    }
    path.velocity.x = static_cast<float>(static_cast<double>(path.velocity.x) - static_cast<double>(hitTime) * 4.9);
    return hitKind != 0;
}

float CalculateNavigationCollisionResponse(const Vector3 &position, const Vector3 &direction, std::int32_t *angle, float radius, std::uint32_t owners) {
    const bool swept = radius > 0.0f;
    CollisionVectorProbe probe{
        {direction.x * 10.0f, direction.z * 10.0f}, {angle ? 8U : 0U, 9.9999999e36f, 0}, swept ? radius : 0.0f, {position.x, position.z}};
    MapControlData *const control = CharacterCollisionMapControl();
    float travelFraction = 1.0f;
    std::uint32_t hit = 0;
    g_navStateNext = 0;
    if ((owners & 1U) != 0) {
        TraceMapCollisionVector(probe, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr, swept);
        hit = probe.result.flags & 7U;
        if (hit == 2U) {
            probe.result.fraction = 0.0f;
            travelFraction = 0.0f;
            g_navStateNext = 1;
        }
    }
    if ((owners & 2U) != 0 && travelFraction > 0.0f) {
        float nearestFraction = probe.result.fraction;
        Character *const excluded = control && control->scenarioTask ? CHID_Find(control->scenarioTask, SHEN_NAME(ABAG)) : nullptr;
        g_characterCollisionIterator = g_characterCollisionHead;
        while (CharacterCollisionWork *const candidate = NextCharacterCollisionShape()) {
            if (candidate->character == excluded)
                continue;
            const QueryShape &shape = candidate->shape;
            const auto *const data = reinterpret_cast<const std::byte *>(shape.packed);
            if (swept)
                DispatchCollisionVector(probe, shape.mode, data);
            else
                DispatchCollisionRay(probe, shape.mode, data);
            const std::uint32_t candidateHit = probe.result.flags & 7U;
            if (candidateHit != 2U && (candidateHit != 1U || probe.result.fraction >= nearestFraction))
                continue;
            hit = candidateHit;
            g_navStateNext = shape.mode == SimpleRadial_CircleXZ_CalculateDistanceWithinRange && MOTM_GetWork(candidate->character) ? 2 : 1;
            if (candidateHit == 2U) {
                probe.result.fraction = 0.0f;
                travelFraction = 0.0f;
                break;
            }
            probe.result.angle = -probe.result.angle;
            travelFraction = probe.result.fraction;
            nearestFraction = probe.result.fraction;
        }
    }
    if ((owners & 4U) != 0 && travelFraction > 0.0f) {
        float nearestFraction = probe.result.fraction;
        g_auxiliaryCollisionCursor = 0;
        while (g_auxiliaryCollisionCursor < g_auxiliaryCollisionCount) {
            const QueryShape &shape = g_auxiliaryCollisionEntries[g_auxiliaryCollisionCursor++].shape;
            const auto *const data = reinterpret_cast<const std::byte *>(shape.packed);
            if (swept)
                DispatchCollisionVector(probe, shape.mode, data);
            else
                DispatchCollisionRay(probe, shape.mode, data);
            const std::uint32_t candidateHit = probe.result.flags & 7U;
            if (candidateHit == 1U && probe.result.fraction < nearestFraction) {
                probe.result.angle = -probe.result.angle;
                hit = 1;
                g_navStateNext = 2;
                travelFraction = probe.result.fraction;
                nearestFraction = probe.result.fraction;
            } else if (candidateHit == 2U) {
                hit = 2;
                travelFraction = 0.0f;
                g_navStateNext = 2;
                break;
            }
        }
    }
    g_navMetricNext = static_cast<std::int32_t>(hit);
    if (hit == 2U)
        return 0.0f;
    if (hit != 1U)
        return 10.0f;
    if (angle)
        *angle = probe.result.angle;
    return travelFraction < 0.0f ? 10.0f : std::min(10.0f, travelFraction * 10.0f);
}

void CCOW_ResetState(CharacterCollisionWork *work) {
    work->groundDropLimit = -68342.0f;
    work->maxAngle = 0;
    work->interactionHeight = 0.0f;
    work->lateralDamp = 0.0f;
    work->forwardDamp = 0.0f;
    work->verticalClampLimit = 0.0f;
    work->reserved004 = 0;
    work->shape.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
    work->shapeExtents = {0.40000001f, 1.5f, 0.40000001f};
    work->radialAabbExtent = 0.40000001f;
    work->reserved0b0 = std::bit_cast<std::int32_t>(60.0f);
    work->stateFlags = 7;
    work->candidateObjectCount = 0;
    std::fill(std::begin(work->candidateObjects), std::end(work->candidateObjects), 0U);
    work->proximityCooldown = 0;
    work->proximityObject = 0;
    work->proximityDistance = 0.0f;
    work->proximityFlags = 0;
    work->previousY = 0.0f;
    work->movementFailTimer = 0.0f;
    work->changed = 1;
    work->lastRepositionYaw = 0;
    constexpr float unsetPosition = std::bit_cast<float>(0xc7c34ff3U);
    work->lastRepositionPosition = {unsetPosition, 0.0f, unsetPosition};
    work->groundYOffset = 0.0f;
}

CharacterCollisionWork *CCOW_Allocate(Character *character) {
    if (!character)
        return nullptr;
    auto *const work = static_cast<CharacterCollisionWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CCOW), sizeof(CharacterCollisionWork), CCOW_Callback));
    if (!work)
        return nullptr;
    CCOW_Initialize(character, work);
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CCOW))) {
        component->destroyCallback = CCOW_Cleanup;
    }
    return work;
}

void CCOW_Initialize(Character *character, CharacterCollisionWork *work) {
    CCOW_ResetState(work);
    work->next = g_characterCollisionHead;
    if (work->next)
        work->next->previous = work;
    work->publishedFlags = work->stateFlags;
    work->previous = nullptr;
    g_characterCollisionHead = work;
    work->character = character;
}

void CCOW_Cleanup(Character *, void *payload) {
    auto &work = *static_cast<CharacterCollisionWork *>(payload);
    if (work.next)
        work.next->previous = work.previous;
    if (work.previous)
        work.previous->next = work.next;
    else
        g_characterCollisionHead = work.next;
}

std::size_t CCOW_CollectMobjSteeringShapes(CCOWSteeringShapeRef *outShapes, std::size_t capacity, bool convertToCharacterSpace) {
    if (!outShapes || capacity == 0)
        return 0;

    capacity = std::min<std::size_t>(capacity, 40);
    const MapControlData *const control = CharacterCollisionMapControl();
    Character *const player = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!player)
        return 0;

    const float playerY = GetPositionForEntity(player->entity.node).y;
    std::size_t count = 0;
    for (CharacterCollisionWork *work = g_characterCollisionHead; work && count < capacity; work = work->next) {
        Character *const character = work->character;
        if (character == player || !GetCewpFlag(character))
            continue;

        const std::uint32_t flags = character->flags.value;
        if (shenGetNodeFromChara(player)->child != character->entity.node.child)
            continue;

        const float positionY = GetPositionForEntity(character->entity.node).y;
        float height = work->shapeExtents.y;
        if (std::fabs(height) < 0.00000011920929f)
            height = 0.0f;
        if (playerY + 1.8f <= positionY || positionY + height <= playerY)
            continue;

        if ((flags & 4U) == 0)
            work->stateFlags &= ~0x100U;
        if ((work->stateFlags & 0x104U) != 0x104U || (character->motionFlags & 0x3fU) != 0x3fU)
            continue;

        CCOWSteeringShapeRef &output = outShapes[count++];
        output.character = character;
        if (convertToCharacterSpace) {
            shenGetCollidedPos(character, work->shape, output.transformedShape);
            output.shape = &output.transformedShape;
        } else {
            output.shape = &work->shape;
        }
    }
    return count;
}

void CCOW_Callback(Character *character, void *payload) {
    if (g_bDebugSkipCyclesForCharacters)
        return;
    auto &work = *static_cast<CharacterCollisionWork *>(payload);
    MapControlData *const control = CharacterCollisionMapControl();
    PlayerControlWork *const player = g_pControlPadController;
    const bool controlledPlayer = control->ryo == character && player->controlMode == 2;
    if (controlledPlayer)
        g_ryoFootPositions.z = 0.0f;
    const float initialWorldY = character ? GetPositionForEntity(character->entity.node).y : 0.0f;
    work.changed = initialWorldY != work.previousY;
    if (control->ryo == character) {
        if (g_skipAuxiliaryCollisionRefresh)
            g_skipAuxiliaryCollisionRefresh = 0;
        else
            RefreshAuxiliaryCharacterCollisionShapes(*character);
    }
    work.stateFlags = (work.stateFlags & 0xffffe8ffU) | 0x100U;
    if (work.proximityFlags & 7U) {
        std::uint32_t flags = work.proximityFlags;
        if (work.proximityCooldown) {
            --work.proximityCooldown;
        } else {
            if (flags & 8U)
                work.proximityFlags = flags | 0x10U;
            flags = work.proximityFlags;
            if (flags & 0x10U) {
                const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
                Character *const target = CHID_Find(control->scenarioTask, work.proximityObject);
                if (!target)
                    return;
                const Vector3 targetPosition = GetPositionForEntity(target->entity.node);
                const float dx = position.x - targetPosition.x;
                const float dz = position.z - targetPosition.z;
                const float distance = std::sqrt(dx * dx + dz * dz);
                const std::uint32_t mask = (flags & 6U) | 1U;
                if (work.proximityDistance <= distance)
                    work.stateFlags &= ~mask;
                else
                    work.stateFlags |= mask;
                if (flags & 0x20U)
                    work.proximityCooldown = static_cast<std::uint32_t>(std::fabs(static_cast<double>(distance - work.proximityDistance) * 4.0));
            }
        }
        work.proximityFlags = flags & ~0x10U;
    }
    {
        CharacterMovementQuery state;
        CharacterMovementProbe &probe = state.probe;
        probe.basePosition = character ? character->entity.node.position : Vector3{};
        probe.yaw = charaGetWorldYaw(character);
        probe.movement = character ? character->rootMotionDelta : Vector3{};
        if (controlledPlayer)
            player->resolvedMovementSpeed = std::sqrt(probe.movement.x * probe.movement.x + probe.movement.z * probe.movement.z);
        if (!(work.stateFlags & 7U))
            goto publish;
        if (!(work.publishedFlags & 7U)) {
            work.targetPosition = probe.basePosition;
            work.cachedYaw = probe.yaw;
            BuildCharacterCollisionShape(work.shape, probe.basePosition, static_cast<std::uint16_t>(probe.yaw), work.shapeExtents);
        }
        MotionManagerWork *const manager = MOTM_GetWork(character);
        if (manager && manager->motion.poseEvaluationStage >= 2 && !(character->motionFlags & 0x10U))
            goto publish;
        const Vector3 originalMovement = probe.movement;
        probe.movement.x += work.lateralDamp;
        probe.movement.y += work.lastAdjustMagnitude;
        probe.movement.z += work.forwardDamp;
        work.stateFlags |= 0x200U;
        if (work.stateFlags & 0x800U) {
            probe.basePosition.x += work.carryDelta.x;
            probe.basePosition.y += work.carryDelta.y;
            probe.basePosition.z += work.carryDelta.z;
            probe.movement.x -= work.carryDelta.x;
            probe.movement.y -= work.carryDelta.y;
            probe.movement.z -= work.carryDelta.z;
        }
        if (!controlledPlayer) {
            const bool unchangedPosition =
                work.targetPosition.x == probe.basePosition.x && work.targetPosition.y == probe.basePosition.y && work.targetPosition.z == probe.basePosition.z;
            const bool stationary = std::fabs(probe.movement.x) < 0.001f && std::fabs(probe.movement.y) < 0.001f && std::fabs(probe.movement.z) < 0.001f;
            if (unchangedPosition) {
                if (work.stateByte == 1 && probe.yaw == work.cachedYaw && stationary)
                    goto publish;
            } else if (!(work.stateFlags & 0x800U) && stationary) {
                probe.movement = {
                    probe.basePosition.x - work.targetPosition.x, probe.basePosition.y - work.targetPosition.y, probe.basePosition.z - work.targetPosition.z};
                probe.basePosition = work.targetPosition;
                work.stateFlags |= 0x400U;
            }
        }
        probe.movementLength = std::sqrt((probe.movement.y * probe.movement.y + probe.movement.x * probe.movement.x) + probe.movement.z * probe.movement.z);
        probe.extents = work.shapeExtents;
        probe.radialExtent = work.radialAabbExtent;
        probe.grounded = 0;
        probe.queryFlags = 0x10;
        std::memset(probe.reserved15, 0, sizeof(probe.reserved15));
        probe.character = character;
        probe.collision = &work;
        probe.yawTolerance = 0x7fff;
        probe.verticalOffset = 0.0f;
        probe.shape.mode = work.shape.mode;
        state.controlledPlayer = controlledPlayer;
        BuildCharacterMovementShape(state);
        work.avoidanceLock = 0;
        work.avoidanceResult = 0;
        if (work.avoidanceMode) {
            work.avoidanceResult = AdjustCharacterAvoidanceMovement(state);
            if (work.stateFlags & 0x800U) {
                probe.basePosition.x -= work.carryDelta.x;
                probe.basePosition.y -= work.carryDelta.y;
                probe.basePosition.z -= work.carryDelta.z;
                probe.movement.x += work.carryDelta.x;
                probe.movement.y += work.carryDelta.y;
                probe.movement.z += work.carryDelta.z;
            }
            BuildCharacterMovementShape(state);
        }
        state.blocked = 0;
        GatherCharacterMovementCandidates(state, 1);
        if (state.blocked) {
            if (work.movementFailTimer < 0.0049999999f)
                work.movementFailTimer += 0.001f;
            bool resolved = false;
            if (controlledPlayer) {
                AdjustCharacterAvoidanceMovement(state);
                BuildCharacterMovementShape(state);
                state.blocked = 0;
                GatherCharacterMovementCandidates(state, 1);
                resolved = state.blocked == 0;
                if (!resolved)
                    probe.movement = originalMovement;
            }
            if (!resolved) {
                const std::uint32_t facing = TryCharacterFacingRecovery(state);
                if (facing == 1 || facing == 2) {
                    state.blocked = 0;
                    GatherCharacterMovementCandidates(state, 1);
                    if (facing == 2 && state.blocked) {
                        probe.movement = state.reposition;
                        state.blocked = 0;
                    }
                }
                resolved = state.blocked == 0;
            }
            if (!resolved) {
                work.lateralDamp = 0.0f;
                work.lastAdjustMagnitude = 0.0f;
                work.forwardDamp = 0.0f;
                probe.movement = {};
                probe.movementLength = 0.0f;
                work.avoidanceResult = 2;
                work.movementFailTimer = 0.0f;
            }
        } else {
            work.movementFailTimer = 0.0f;
        }
        if (work.stateFlags & 0x80U) {
            probe.movement.y -= work.lastAdjustMagnitude;
            work.lastAdjustMagnitude = 0.0f;
        } else if (!probe.grounded) {
            const float delta = SampleCharacterMovementGround(probe.intersection) + work.groundYOffset - probe.basePosition.y;
            work.lastAdjustMagnitude = delta;
            probe.movement.y = delta;
            if (delta < -0.3f) {
                probe.movement.y = 0.0f;
                work.lastAdjustMagnitude = 0.0f;
            }
        }
        work.stateByte = 1;
        if (controlledPlayer && g_ryoCurrentAnimationState == 10)
            probe.movement.y = 0.0f;
        if (work.stateFlags & 0x400U) {
            probe.movement = originalMovement;
        } else {
            if (character)
                character->rootMotionDelta = probe.movement;
            work.previousY = probe.movement.y + initialWorldY;
            const Vector3 correction{probe.movement.x - originalMovement.x, probe.movement.y - originalMovement.y, probe.movement.z - originalMovement.z};
            if (manager &&
                (std::fabs(correction.x) >= 0.00000011920929f || std::fabs(correction.y) >= 0.00000011920929f ||
                    std::fabs(correction.z) >= 0.00000011920929f)) {
                g_ryoFootPositions.z = correction.y;
                MOTI_ApplyPositionOffset(manager->motion, correction);
            }
            if (controlledPlayer)
                player->resolvedMovementSpeed = std::sqrt(probe.movement.x * probe.movement.x + probe.movement.z * probe.movement.z);
        }
        const float previousDeltaY = probe.movement.y;
        const float previousFootOffset = g_ryoFootPositions.z;
        const float magnitudeY = std::fabs(probe.movement.y);
        if (magnitudeY >= 0.00000011920929f && !(work.stateFlags & 0x2000U) && magnitudeY < 0.5f) {
            work.verticalClampAccel += 0.0020000001f;
            work.verticalClampLimit += work.verticalClampAccel;
            if (probe.movement.y > work.verticalClampLimit) {
                probe.movement.y = work.verticalClampLimit;
            } else if (probe.movement.y < -work.verticalClampLimit) {
                probe.movement.y = -work.verticalClampLimit;
            } else {
                work.verticalClampLimit -= 0.0020000001f;
                if (work.verticalClampLimit < 0.0f)
                    work.verticalClampLimit = 0.0f;
                work.verticalClampAccel = 0.0f;
            }
            if (character)
                character->rootMotionDelta = probe.movement;
            const float correction = probe.movement.y - previousDeltaY;
            work.previousY = probe.movement.y + initialWorldY;
            if (manager && std::fabs(correction) >= 0.00000011920929f) {
                MOTI_ApplyPositionOffset(manager->motion, {0.0f, correction, 0.0f});
                g_ryoFootPositions.z = previousFootOffset + correction;
            }
        } else {
            if (magnitudeY >= 0.5f && !(work.stateFlags & 0x2000U))
                work.verticalClampLimit = 0.0f;
            work.verticalClampLimit -= 0.0049999999f;
            if (work.verticalClampLimit < 0.0f)
                work.verticalClampLimit = 0.0f;
            work.verticalClampAccel = 0.0f;
        }
        work.targetPosition = probe.intersection;
        work.cachedYaw = probe.yaw;
        BuildCharacterCollisionShape(work.shape, probe.intersection, static_cast<std::uint16_t>(probe.yaw), work.shapeExtents);
    }
publish:
    work.stateFlags &= ~0x800U;
    work.publishedFlags = work.stateFlags;
    work.groundYOffset = 0.0f;
}

void CCOW_RefreshAuthoredShape(Character *character) {
    CharacterCollisionWork *const work = FindCharacterCollisionWork(character);
    if (!work)
        return;
    work->targetPosition = character->entity.node.position;
    work->cachedYaw = charaGetWorldYaw(character);
    BuildCharacterCollisionShape(work->shape, work->targetPosition, static_cast<std::uint16_t>(work->cachedYaw), work->shapeExtents);
}

namespace {

bool CheckPointConditions(float startX, float startZ, float endX, float endZ) {
    QueryShape line{.mode = CalculateLineIntersection_0};
    line.packed[0] = {startX, startZ, endX};
    line.packed[1].x = endZ;
    g_characterCollisionIterator = g_characterCollisionHead;
    while (CharacterCollisionWork *const work = NextCharacterCollisionShape()) {
        if (ShapeIntersectsQuery(work->shape, line))
            return true;
    }
    return false;
}

bool CheckLineIntersectionWithVector(float startX, float startZ, float endX, float endZ) {
    QueryShape line{.mode = CalculateLineIntersection_0};
    line.packed[0] = {startX, startZ, endX};
    line.packed[1].x = endZ;
    g_auxiliaryCollisionCursor = 0;
    if (!g_auxiliaryCollisionCount)
        return false;
    g_auxiliaryCollisionCursor = 1;
    for (std::uint32_t index = 0;;) {
        const QueryShape &shape = g_auxiliaryCollisionEntries[index].shape;
        if (shape.mode == CalculateLineIntersection_0 && ShapeIntersectsQuery(shape, line))
            return true;
        if (g_auxiliaryCollisionCursor >= g_auxiliaryCollisionCount)
            return false;
        index = g_auxiliaryCollisionCursor++;
    }
}

bool ResolveRyoLocalSegment(const Vector3 &start, const Vector3 &end, float &startX, float &startZ, float &endX, float &endZ) {
    MapControlData *const control = CharacterCollisionMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo)
        return false;
    SelectCharacterMapQuery(ryo);
    const Vector3 localEnd = ryo->entity.node.child ? WorldToEntityLocalPoint(*ryo->entity.node.child, end) : end;
    const Vector3 localStart = ryo->entity.node.child ? WorldToEntityLocalPoint(*ryo->entity.node.child, start) : start;
    endX = localEnd.x;
    endZ = localEnd.z;
    startX = localStart.x;
    startZ = localStart.z;
    return true;
}

} // namespace

std::int32_t ctrlCheckColi(const Vector3 &start, const Vector3 &end, float radius, const std::uint8_t *mask) {
    float startX = 0.0f;
    float startZ = 0.0f;
    float endX = 0.0f;
    float endZ = 0.0f;
    if (!ResolveRyoLocalSegment(start, end, startX, startZ, endX, endZ))
        return 0;
    QueryShape query{.mode = CalculateLineIntersection_0};
    if (radius <= 0.001f) {
        if (std::fabs(startX - endX) < 0.001 && std::fabs(startZ - endZ) < 0.001)
            return 0;
    } else {
        Vector3 direction{endX - startX, 0.0f, endZ - startZ};
        if (std::fabs(endX - startX) < 0.001 && std::fabs(endZ - startZ) < 0.001)
            return 0;
        const float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        if (magnitude > 0.0f) {
            const float inverse = 1.0f / magnitude;
            direction = {direction.x * inverse, direction.y * inverse, direction.z * inverse};
        }
        const float offsetX = direction.x * radius;
        const float offsetZ = direction.z * radius;
        query.packed[0] = {endX - offsetZ, offsetX + endZ, startX - offsetZ};
        query.packed[1].x = offsetX + startZ;
        if (CheckPointConditions(endX - offsetZ, offsetX + endZ, startX - offsetZ, offsetX + startZ) ||
            CheckLineIntersectionWithVector(endX - offsetZ, offsetX + endZ, startX - offsetZ, offsetX + startZ) || ExecuteMathFnWrapper(&query, mask)) {
            return 1;
        }
        query.mode = CalculateLineIntersection_0;
        endZ -= offsetX;
        startZ -= offsetX;
        endX += offsetZ;
        startX += offsetZ;
    }
    query.packed[0] = {endX, endZ, startX};
    query.packed[1].x = startZ;
    if (CheckPointConditions(endX, endZ, startX, startZ) || CheckLineIntersectionWithVector(endX, endZ, startX, startZ))
        return 1;
    return ExecuteMathFnWrapper(&query, mask) != 0 ? 1 : 0;
}

std::int32_t CalculateVectorDistanceAndExecuteMathFn(const Vector3 &start, const Vector3 &end, float radius, const std::uint8_t *mask) {
    float startX = 0.0f;
    float startZ = 0.0f;
    float endX = 0.0f;
    float endZ = 0.0f;
    if (!ResolveRyoLocalSegment(start, end, startX, startZ, endX, endZ))
        return 0;
    QueryShape query{.mode = CalculateLineIntersection_0};
    if (radius <= 0.001f) {
        if (std::fabs(startX - endX) < 0.001 && std::fabs(startZ - endZ) < 0.001)
            return 0;
    } else {
        Vector3 direction{endX - startX, 0.0f, endZ - startZ};
        if (std::fabs(endX - startX) < 0.001 && std::fabs(endZ - startZ) < 0.001)
            return 0;
        const float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        if (magnitude > 0.0f) {
            const float inverse = 1.0f / magnitude;
            direction = {direction.x * inverse, direction.y * inverse, direction.z * inverse};
        }
        const float offsetX = direction.x * radius;
        const float offsetZ = direction.z * radius;
        query.packed[0] = {endX - offsetZ, offsetX + endZ, startX - offsetZ};
        query.packed[1].x = offsetX + startZ;
        if (ExecuteMathFnWrapper(&query, mask))
            return 1;
        query.mode = CalculateLineIntersection_0;
        endZ -= offsetX;
        startZ -= offsetX;
        endX += offsetZ;
        startX += offsetZ;
    }
    query.packed[0] = {endX, endZ, startX};
    query.packed[1].x = startZ;
    return ExecuteMathFnWrapper(&query, mask) != 0 ? 1 : 0;
}

std::int32_t coliTestPlane(const Vector3 &start, const Vector3 &end, const float radius, Vector3 &output, const std::uint8_t *mask) {
    auto *const control = CharacterCollisionMapControl();
    auto *const ryo = static_cast<Character *>(control->ryo);
    SelectCharacterMapQuery(ryo);

    const Matrix4x4 matrix = g_collisionQueryTransform.matrix;
    const auto toLocal = [&matrix](const Vector3 &point) {
        if (!g_collisionQueryTransform.active)
            return point;
        const Vector3 translated{point.x - matrix.rows[3].x, point.y - matrix.rows[3].y, point.z - matrix.rows[3].z};
        return Vector3{matrix.rows[0].x * translated.x + matrix.rows[0].y * translated.y + matrix.rows[0].z * translated.z,
            matrix.rows[1].x * translated.x + matrix.rows[1].y * translated.y + matrix.rows[1].z * translated.z,
            matrix.rows[2].x * translated.x + matrix.rows[2].y * translated.y + matrix.rows[2].z * translated.z};
    };

    const Vector3 localStart = toLocal(start);
    const Vector3 localEnd = toLocal(end);
    const Vector3 delta{localEnd.x - localStart.x, localEnd.y - localStart.y, localEnd.z - localStart.z};
    if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z < 0.000001f) {
        output = end;
        return 0;
    }

    CharacterVectorCollisionResult result{};
    COLS_TraceMapVectorWithMask(localStart, localEnd, radius, result, mask);
    const std::uint32_t hit = result.flags & 7U;
    if (hit == 1U && result.fraction >= 0.0f && result.fraction <= 1.0f) {
        const Vector3 localHit{localStart.x + delta.x * result.fraction, localStart.y, localStart.z + delta.z * result.fraction};
        output = TransformCollisionQueryPoint(localHit);
        return 1;
    }
    if (hit == 2U) {
        output = start;
        return 1;
    }
    output = end;
    return 0;
}

std::int32_t COLS_TraceCharacterVector(
    const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, Character *ignoredA, Character *ignoredB) {
    result.flags &= ~0x1fU;
    result.fraction = 9.9999999e36f;
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, radius, {start.x, start.z}};
    MapControlData *const control = CharacterCollisionMapControl();
    TraceMapCollisionVector(probe, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr, true);
    std::uint32_t hit = probe.result.flags & 7U;
    g_characterCollisionIterator = g_characterCollisionHead;
    std::int32_t returnValue = 0;
    while (CharacterCollisionWork *const work = NextCharacterCollisionShape()) {
        if (work->character == ignoredA || work->character == ignoredB)
            continue;
        probe.result.flags &= ~7U;
        DispatchCollisionVector(probe, work->shape.mode, reinterpret_cast<const std::byte *>(work->shape.packed));
        const std::uint32_t candidateHit = probe.result.flags & 7U;
        if (candidateHit == 1)
            hit = 1;
        else if (candidateHit == 2) {
            hit = 2;
            returnValue = 1;
            break;
        }
    }
    probe.result.flags = (probe.result.flags & ~7U) | hit;
    result = probe.result;
    return returnValue;
}

void COLS_TraceMapRay(const Vector3 &start, const Vector3 &end, CharacterVectorCollisionResult &result) {
    result.flags &= ~0x1fU;
    result.fraction = 9.9999999e36f;
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, 0.0f, {start.x, start.z}};
    MapControlData *const control = CharacterCollisionMapControl();
    TraceMapCollisionVector(probe, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr, false);
    result = probe.result;
}

void COLS_TraceMapVector(const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result) {
    result.flags &= ~0x1fU;
    result.fraction = 9.9999999e36f;
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, radius, {start.x, start.z}};
    MapControlData *const control = CharacterCollisionMapControl();
    TraceMapCollisionVector(probe, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr, true);
    result = probe.result;
}

void COLS_TraceMapVectorWithMask(const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, const std::uint8_t *mask) {
    result.flags &= ~0x1fU;
    result.fraction = 9.9999999e36f;
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, radius, {start.x, start.z}};
    TraceMapCollisionVector(probe, mask, true);
    result = probe.result;
}

namespace {

QueryShape g_cameraCollidedShape{};



const QueryShape *RetrieveCameraCollisionShape(Character *&character) {
    MapControlData *const control = CharacterCollisionMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    const float ryoY = ryo ? GetPositionForEntity(ryo->entity.node).y : 0.0f;
    const bool heightClamp = g_EnableCameraHeightClamp != 0;
    character = nullptr;
    const QueryShape *shape = nullptr;
    while (g_characterCollisionIterator && !shape) {
        CharacterCollisionWork *const work = g_characterCollisionIterator;
        float height = work->shapeExtents.y;
        if (std::fabs(height) < 0.00000011920929f)
            height = 0.0f;
        Character *const candidate = work->character;
        if (ryo && candidate != ryo && GetCewpFlag(candidate)) {
            const float y = GetPositionForEntity(candidate->entity.node).y;
            const bool sharedRoot = heightClamp || shenGetNodeFromChara(ryo)->child == candidate->entity.node.child;
            if (sharedRoot && static_cast<double>(ryoY) + 1.8 > static_cast<double>(y) && height + y > ryoY) {
                if (!(candidate->flags.value & 4U))
                    work->publishedFlags &= ~0x100U;
                if ((work->publishedFlags & 0x104U) == 0x104U) {
                    character = candidate;
                    shape = &work->shape;
                    if (heightClamp && shape->mode == OrientedBox_OrientedRectXZ_IsPointInTriangle) {
                        shenGetCollidedPos(ryo, work->shape, g_cameraCollidedShape);
                        shape = &g_cameraCollidedShape;
                    }
                }
            }
        }
        g_characterCollisionIterator = work->next;
    }
    return shape;
}

void DispatchCameraCollisionShape(std::uint32_t requestMode, CollisionVectorProbe &probe, const QueryShape &shape) {
    if (requestMode == 9U)
        DispatchCollisionRay(probe, shape.mode, reinterpret_cast<const std::byte *>(shape.packed));
    else if (requestMode == 10U)
        DispatchCollisionVector(probe, shape.mode, reinterpret_cast<const std::byte *>(shape.packed));
}

} // namespace

std::int32_t coliTestCharacterShapes(
    std::uint32_t requestMode, const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, std::uint32_t &hitKind) {
    MapControlData *const control = CharacterCollisionMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, radius, {start.x, start.z}};
    float nearest = probe.result.fraction;
    std::int32_t hit = 0;
    g_characterCollisionIterator = g_characterCollisionHead;
    Character *character = nullptr;
    for (const QueryShape *shape = RetrieveCameraCollisionShape(character); shape; shape = RetrieveCameraCollisionShape(character)) {
        if (character == ryo)
            continue;
        DispatchCameraCollisionShape(requestMode, probe, *shape);
        const std::uint32_t kind = probe.result.flags & 7U;
        if (kind == 2U) {
            probe.result.fraction = 0.0f;
            hitKind = 2;
            result = probe.result;
            return 1;
        }
        if (kind == 1U && !(probe.result.fraction >= nearest)) {
            hitKind = 1;
            hit = 1;
            nearest = probe.result.fraction;
        }
    }
    result = probe.result;
    return hit;
}

std::int32_t coliTestAuxiliaryShapes(
    std::uint32_t requestMode, const Vector3 &start, const Vector3 &end, float radius, CharacterVectorCollisionResult &result, std::uint32_t &hitKind) {
    CollisionVectorProbe probe{{end.x - start.x, end.z - start.z}, result, radius, {start.x, start.z}};
    float nearest = probe.result.fraction;
    std::int32_t hit = 0;
    const bool heightClamp = g_EnableCameraHeightClamp != 0;
    g_auxiliaryCollisionCursor = 0;
    while (g_auxiliaryCollisionCursor < g_auxiliaryCollisionCount) {
        const QueryShape &shape = g_auxiliaryCollisionEntries[g_auxiliaryCollisionCursor++].shape;
        if (shape.mode != OrientedBox_OrientedRectXZ_IsPointInTriangle)
            continue;
        if (heightClamp) {
            MapControlData *const control = CharacterCollisionMapControl();
            QueryShape collided{};
            shenGetCollidedPos(control ? static_cast<Character *>(control->ryo) : nullptr, shape, collided);
            DispatchCameraCollisionShape(requestMode, probe, collided);
        } else {
            DispatchCameraCollisionShape(requestMode, probe, shape);
        }
        const std::uint32_t kind = probe.result.flags & 7U;
        if (kind == 2U) {
            probe.result.fraction = 0.0f;
            hitKind = 2;
            result = probe.result;
            return 1;
        }
        if (kind == 1U && !(probe.result.fraction >= nearest)) {
            hitKind = 1;
            hit = 1;
            nearest = probe.result.fraction;
        }
    }
    result = probe.result;
    return hit;
}

float COLS_QueryCombatMoveClearance(const Vector3 &origin, std::int16_t directionDelta, float maxDistance, std::int32_t *hitAngle) {
    const std::uint16_t direction = static_cast<std::uint16_t>(directionDelta + 0x4000);
    const std::int16_t foldedDirection =
        direction <= 0x8000U ? static_cast<std::int16_t>(direction) : static_cast<std::int16_t>(-static_cast<std::int32_t>(direction));
    const Vector3 movement{
        -sin_fixed16(static_cast<std::int16_t>(direction)) * maxDistance, 0.0f, sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedDirection)) * maxDistance};
    CharacterVectorCollisionResult result{8U, 9.9999999e36f, 0};
    CollisionVectorProbe probe{{movement.x, movement.z}, result, 0.6f, {origin.x, origin.z}};
    MapControlData *const control = CharacterCollisionMapControl();
    TraceMapCollisionVector(probe, control ? reinterpret_cast<const std::uint8_t *>(control->cycleMask) : nullptr, true);

    const std::uint32_t hit = probe.result.flags & 7U;
    if (hit == 2U)
        return 0.0f;
    float distance = maxDistance;
    if (hit == 1U && probe.result.fraction < 1.0f) {
        if (probe.result.fraction < 0.0f)
            return 0.0f;
        if (hitAngle)
            *hitAngle = probe.result.angle;
        distance *= probe.result.fraction;
    }
    return distance > 0.0f ? distance - 0.3f : distance;
}

float ryoComputeNavAngleDeltaAtYaw(const Vector3 &position, std::uint16_t yaw, std::int32_t *angle, float radius) {
    const CharacterCollisionWork *const work = FindCharacterCollisionWork(g_playerCharacter);
    const bool mapEnabled = work && (work->stateFlags & 2U) != 0;
    const bool charactersEnabled = work && (work->stateFlags & 4U) != 0;
    if (!mapEnabled && !charactersEnabled)
        return 10.0f;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), sin_fixed16(fixed16_cosine_angle(yaw)));
    const Vector3 direction{
        ((workTransformMatrix.rows[0].x * 0.0f + workTransformMatrix.rows[1].x * 0.0f) + workTransformMatrix.rows[2].x) + workTransformMatrix.rows[3].x, 0.0f,
        ((workTransformMatrix.rows[0].z * 0.0f + workTransformMatrix.rows[1].z * 0.0f) + workTransformMatrix.rows[2].z) + workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    const std::uint32_t owners = mapEnabled ? (charactersEnabled ? 7U : 1U) : 6U;
    return CalculateNavigationCollisionResponse(position, direction, angle, radius, owners);
}

bool SelectCharacterMapQuery(Character *character) {
    g_activeMapQuerySet = nullptr;
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control || !character || character->eventGroup >= control->mapQuerySetsBySection.size()) {
        return false;
    }

    auto *const query = static_cast<MapQuerySet *>(control->mapQuerySetsBySection[character->eventGroup]);
    if (!query)
        return false;
    g_activeMapQuerySet = query;
    CaptureCollisionQueryTransform(character->entity.node.child);
    return true;
}

bool Matrix4x4_TranslateAndRotate(const CharacterIdTransform &transform) {
    g_activeMapQuerySet = nullptr;
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control || transform.layer >= control->mapQuerySetsBySection.size())
        return false;
    auto *const query = static_cast<MapQuerySet *>(control->mapQuerySetsBySection[transform.layer]);
    if (!query)
        return false;
    g_activeMapQuerySet = query;
    CaptureCollisionQueryTransform(transform.node.child);
    return true;
}

float FindHighestValidCollisionBelow(const Vector3 &localPoint, float heightRadius, float undoRadius) {
    g_selectedCollisionFunction = nullptr;
    Vector3 worldPoint = localPoint;
    if (g_collisionQueryTransform.active) {
        Matrix4x4_Push(&g_collisionQueryTransform.matrix);
        Matrix4x4::TransformPoint(worldPoint, worldPoint);
        Matrix4x4::Pop();
    }
    const MapQuerySet *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!query)
        return 0.0f;
    float heightY = 0.0f;
    float undoY = 0.0f;
    const std::uint32_t *const heightFunction = SampleGroundPoint(*query, worldPoint, false, heightY, heightRadius);
    const std::uint32_t *const undoFunction = SampleGroundPoint(*query, worldPoint, true, undoY, undoRadius);
    if (undoFunction && (!heightFunction || heightY < undoY)) {
        g_selectedCollisionFunction = undoFunction;
        return undoY;
    }
    g_selectedCollisionFunction = heightFunction;
    return heightY;
}

float mapUpdateIntersectionStateAtLocalPoint(const Vector3 &localPoint) {
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    g_selectedGroundFunction = nullptr;
    if (!query)
        return 0.0f;
    const Vector3 worldOrigin = TransformCollisionQueryPoint({localPoint.x, 0.0f, localPoint.z});
    float height = 0.0f;
    const std::uint32_t *const heightFunction = SampleGroundPoint(*query, worldOrigin, false, height);
    Vector3 undoPoint = worldOrigin;
    undoPoint.y += localPoint.y;
    float undoHeight = 0.0f;
    g_selectedGroundFunction = SampleGroundPoint(*query, undoPoint, true, undoHeight);
    const float relativeUndoHeight = undoHeight - worldOrigin.y;
    if (!g_selectedGroundFunction || (heightFunction && height > relativeUndoHeight)) {
        g_selectedGroundFunction = heightFunction;
        return height;
    }
    g_groundIntersectionUsesAlternateSurface = 1;
    return relativeUndoHeight;
}

float MapProbe_UpdateIntersectionStateAtPoint(const Vector3 &worldPoint) {
    const MapQuerySet *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    g_selectedGroundFunction = nullptr;
    if (!query)
        return 0.0f;
    const Vector3 origin{worldPoint.x, 0.0f, worldPoint.z};
    float height = 0.0f;
    const std::uint32_t *const heightFunction = SampleGroundPoint(*query, origin, false, height);
    float undoHeight = 0.0f;
    g_selectedGroundFunction = SampleGroundPoint(*query, worldPoint, true, undoHeight);
    if (!g_selectedGroundFunction || (heightFunction && height > undoHeight)) {
        g_selectedGroundFunction = heightFunction;
        return height;
    }
    g_groundIntersectionUsesAlternateSurface = 1;
    return undoHeight;
}

namespace {
const std::uint32_t *FindHighestIKMotionProbe(const Vector3 &point, float radius, bool undo, float &highest) {
    const Vector3 probes[5]{point, {point.x + radius, point.y, point.z + radius}, {point.x + radius, point.y, point.z - radius},
        {point.x - radius, point.y, point.z + radius}, {point.x - radius, point.y, point.z - radius}};
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    const std::uint32_t *selected = nullptr;
    const std::uint32_t *previous = nullptr;
    for (const Vector3 &probe : probes) {
        float height = 0.0f;
        const std::uint32_t *const function = query ? SampleGroundPoint(*query, TransformCollisionQueryPoint(probe), undo, height) : nullptr;
        if (function && (!previous || height > highest)) {
            highest = height;
            selected = function;
        }
        previous = function;
    }
    return selected;
}
} // namespace

std::int32_t FindHighestPointBelow100ForIKMotion(const Vector3 &point, float heightRadius, float undoRadius, float &outY) {
    float heightY = 0.0f;
    float undoY = 0.0f;
    const std::uint32_t *const heightFunction = FindHighestIKMotionProbe(point, heightRadius, false, heightY);
    const std::uint32_t *const undoFunction = FindHighestIKMotionProbe(point, undoRadius, true, undoY);
    if (heightFunction && (!undoFunction || undoY < heightY)) {
        outY = heightY;
        g_selectedGroundFunction = heightFunction;
        return 1;
    }
    if (undoFunction) {
        outY = undoY;
        g_selectedGroundFunction = undoFunction;
        return 2;
    }
    outY = 0.0f;
    g_selectedGroundFunction = nullptr;
    return 0;
}

bool CheckIsPointIntersectWith(const Vector3 &point, const CollisionPointQueryOutputs *outputs, std::uint32_t token) {
    const std::byte *const record = FindActivePropertyRecord(&point, token);
    if (!record)
        return false;
    if (outputs) {
        std::memcpy(outputs->value4, record + 0x04, sizeof(std::uint32_t));
        std::memcpy(outputs->value8, record + 0x08, sizeof(std::uint32_t));
        std::memcpy(outputs->valueC, record + 0x0c, sizeof(std::uint32_t));
        std::memcpy(outputs->value10, record + 0x10, sizeof(std::uint32_t));
    }
    return true;
}

const std::uint32_t *COLS_ResolveSelectedCollisionFunction() {
    static constexpr std::uint32_t kFunctionTwoReplacement[7]{1U, 0U, 0U, 0U, 0U, 0U, 0U};
    if (g_selectedCollisionFunction && *g_selectedCollisionFunction == 2U)
        g_selectedCollisionFunction = kFunctionTwoReplacement;
    return g_selectedCollisionFunction;
}

std::int32_t COLS_GetSelectedCollisionRecordValue() {
    return static_cast<std::int32_t>(g_selectedCollisionFunction[-1]);
}


std::int32_t coneCheck(const Vector3 &end, const Vector3 &point) {
    MapControlData *const control = CharacterCollisionMapControl();
    float startX = 0.0f;
    float startZ = 0.0f;
    float endX = 0.0f;
    float endZ = 0.0f;
    if (!control || !ResolveRyoLocalSegment(point, end, startX, startZ, endX, endZ))
        return 0;
    std::array<std::uint8_t, sizeof(control->cycleMask)> mask{};
    std::memcpy(mask.data(), control->cycleMask, mask.size());
    mask[1] &= 0xfdU;
    mask[0] &= 0x7fU;
    QueryShape query{.mode = CalculateLineIntersection_0};
    query.packed[0] = {startX, startZ, endX};
    query.packed[1].x = endZ;
    return ExecuteMathFnWrapper(&query, mask.data()) != 0 ? 1 : 0;
}

void COLS_ResetQueryState() {
    g_activeMapQuerySet = nullptr;
    g_collisionSelectionState.fill(0);
    g_collisionQueryTransform = {};
    g_taskCollisionCache[0] = -1;
    g_taskCollisionCache[1] = -1;
    g_selectedGroundFunction = nullptr;
    g_selectedCollisionFunction = nullptr;
    g_collisionPolylineHit = {};
    g_groundIntersectionUsesAlternateSurface = 0;
    g_collisionReady = 1;
}

void COLS_Free() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control)
        return;
    auto *entries = static_cast<MapQueryRuntimeEntry *>(control->mapQueryRuntimeTable);
    if (!entries) {
        return;
    }

    for (;;) {
        if (entries->querySet) {
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entries->querySet), 0);
        } else if (entries->name == 0) {
            break;
        }
        ++entries;
    }

    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(control->mapQueryRuntimeTable), 0);
    control->mapQueryRuntimeTable = nullptr;
    control->mapQuerySetsBySection.fill(nullptr);
    COLS_ResetQueryState();
}

void COLS_Process(void *data) {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    if (!data) {
        control->mapQueryRuntimeTable = nullptr;
        return;
    }

    auto *section = reinterpret_cast<CollisionSectionEntry *>(static_cast<std::byte *>(data) + 8);
    CollisionSectionEntry *cursor = section;
    std::int32_t sectionCount = 0;
    std::uint32_t nextOffset = cursor->nextOffset;
    while (nextOffset) {
        cursor = reinterpret_cast<CollisionSectionEntry *>(reinterpret_cast<std::byte *>(cursor) + nextOffset);
        ++sectionCount;
        nextOffset = cursor->nextOffset;
    }

    auto *entries = static_cast<MapQueryRuntimeEntry *>(HLib_AllocHeapMemBlock(sizeof(MapQueryRuntimeEntry) * (sectionCount + 1), SHEN_NAME(iCLT)));
    control->mapQueryRuntimeTable = entries;

    for (std::int32_t index = 0; index < sectionCount; ++index) {
        entries[index].name = section->name;
        entries[index].querySet = COLS_CreateMapQuerySet(&section->margin);
        section = reinterpret_cast<CollisionSectionEntry *>(reinterpret_cast<std::byte *>(section) + section->nextOffset);
    }
    entries[sectionCount].name = 0;
    entries[sectionCount].querySet = nullptr;

    for (std::int32_t sectionIndex = 0; sectionIndex < 32; ++sectionIndex) {
        char name[8];
        std::snprintf(name, sizeof(name), "%04d", sectionIndex);
        std::uint32_t packedName;
        std::memcpy(&packedName, name, sizeof(packedName));
        for (std::int32_t index = 0; index < sectionCount; ++index) {
            if (entries[index].name == packedName) {
                control->mapQuerySetsBySection[sectionIndex] = entries[index].querySet;
                break;
            }
        }
    }
}

void *COLS_FindQuerySet(std::uint32_t name) {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *entry = control ? static_cast<MapQueryRuntimeEntry *>(control->mapQueryRuntimeTable) : nullptr;
    while (entry && entry->name) {
        if (entry->name == name)
            return entry->querySet;
        ++entry;
    }
    return nullptr;
}

bool mapQuerySelectActiveGridSection(std::uint8_t sectionIndex) {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    auto *const query = static_cast<MapQuerySet *>(control->mapQuerySetsBySection[sectionIndex]);
    g_activeMapQuerySet = query;
    if (!query || !query->sectionOffsetTable)
        return false;
    const std::uint32_t count = PresentQueryChunkCount(*query);
    for (std::uint32_t index = 0; index < count; ++index) {
        std::int32_t offset = -1;
        std::memcpy(&offset, query->sectionOffsetTable + index, sizeof(offset));
        if (offset != -1 && (offset & 3) != 0)
            return false;
    }
    return true;
}

std::int64_t ExecuteMathFnWrapper(const QueryShape *query, const std::uint8_t *cm) {
    if (!g_activeMapQuerySet || !query)
        return 0;
    CollisionGridView grid{};
    auto *const activeQuery = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!BindCollisionGrid(*activeQuery, grid))
        return 0;
    if (query->mode == CalculateLineIntersection_0 || query->mode == IsPointInTriangle_2)
        return ExecutePackedLineQuery(*activeQuery, grid, *query, cm) ? 1 : 0;

    return ExecuteCollisionShapeQuery(*activeQuery, grid, *query, cm) ? 1 : 0;
}

bool COLS_SignalBestInteractionTarget(Character *character) {
    CharacterCollisionWork *const source = FindCharacterCollisionWork(character);
    if (!source)
        return false;
    CharacterMovementQuery state;
    CharacterMovementProbe &probe = state.probe;
    probe.character = character;
    probe.collision = source;
    probe.grounded = 0;
    probe.queryFlags = 0x40;
    std::memset(probe.reserved15, 0, sizeof(probe.reserved15));
    probe.yaw = static_cast<std::int16_t>(cpctGetAccumYaw(character));
    probe.basePosition = character->entity.node.position;
    probe.verticalOffset = 0.0f;
    probe.shape.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange;
    probe.radialExtent = source->distanceLimit;
    probe.extents = {source->distanceLimit, source->shapeExtents.y, source->distanceLimit};
    probe.movement = {};
    probe.movementLength = 0.0f;
    BuildCharacterMovementShape(state);
    state.bottom = probe.basePosition.y;
    state.top = probe.basePosition.y + source->interactionHeight;
    probe.yawTolerance = source->maxAngle;
    GatherCharacterMovementCandidates(state, 0);
    CharacterCollisionWork *best = nullptr;
    float bestScore = 0.0f;
    const float angleWeight = (static_cast<float>(source->maxAngle) * 360.0f) * 0.000015258789f;
    for (std::uint8_t index = 0; index < state.count; ++index) {
        const CharacterMovementCandidate &candidate = state.candidates[index];
        if (candidate.flags & 0x10U)
            continue;
        const float height = std::fabs(candidate.bottom - probe.basePosition.y);
        const std::uint32_t angularRatio = static_cast<std::uint32_t>(std::abs(candidate.yawError)) / static_cast<std::uint32_t>(probe.yawTolerance);
        const float score = (1.0f - height / 3.0f) * (source->interactionHeight * 10.0f) +
            ((1.0f - candidate.distance / (probe.extents.z + probe.extents.x)) * (source->distanceLimit * 10.0f) +
                (1.0f - static_cast<float>(angularRatio)) * (angleWeight * 10.0f));
        if (score > bestScore) {
            best = candidate.collision;
            bestScore = score;
        }
    }
    if (!best)
        return false;
    best->interactionState |= 1U;
    return true;
}

void Script_GetHighestIntersectionAtLocalPoint(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = 0;
    if (!rawParams || !g_pCurrentTaskData)
        return;
    const std::int32_t offset = *static_cast<const std::int32_t *>(rawParams);
    if (!offset)
        return;
    const Vector3 localPoint = *reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + offset);
    context->stack->accumulator = std::bit_cast<std::uint32_t>(mapUpdateIntersectionStateAtLocalPoint(localPoint));
}

void Script_GetHighestCollisionBelow(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = 0;
    if (!context->task || !context->task->task || !rawParams || !g_pCurrentTaskData) {
        return;
    }
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    if (!params[0] || !params[1])
        return;
    Character *const character = CHID_Find(context->task->task, params[0]);
    if (!character)
        return;
    const Vector3 localPoint = *reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + static_cast<std::int32_t>(params[1]));
    context->stack->accumulator = std::bit_cast<std::uint32_t>(HighestCollisionBelow(character, localPoint));
}

void Script_UpdateTaskCollisionCache(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack)
        return;
    context->stack->accumulator = 0xffffffffU;
    if (!rawParams)
        return;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    const Vector3 *point = nullptr;
    if (params[2] && g_pCurrentTaskData) {
        point = reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + static_cast<std::int32_t>(params[2]));
    }
    context->stack->accumulator = COLS_UpdateTaskCollisionCache(static_cast<std::int32_t>(params[0]), static_cast<std::int32_t>(params[1]), point);
}

} // namespace shenmue::s1
