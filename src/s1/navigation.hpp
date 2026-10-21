#pragma once
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct NavAnchorPoint {
    float x;
    float y;
    float z;
    float radius;
};
static_assert(sizeof(NavAnchorPoint) == 0x10);

struct NavRegionControlPoint {
    Vector3 position;
    float radius;
    std::uint32_t regionId;
    std::uint32_t flags;
    Vector3 origin;
    std::int32_t yaw;
};
static_assert(sizeof(NavRegionControlPoint) == 0x28);

struct NavQuad {
    std::array<Vector2, 4> vertices;
};
static_assert(sizeof(NavQuad) == 0x20);

struct NavTriangle {
    float centerY;
    NavQuad quad;
};
static_assert(sizeof(NavTriangle) == 0x24);

struct NavCircle {
    Vector3 center;
    float radius;
};
static_assert(sizeof(NavCircle) == 0x10);

struct NavSectionBinding {
    std::int32_t count;
    std::uint32_t reserved;
    void *data;
};
static_assert(sizeof(NavSectionBinding) == 0x10);

struct NavRegion {
    std::uint32_t id;
    std::uint32_t active;
    Vector3 origin;
    std::int32_t yaw;
    NavSectionBinding anchors;
    NavSectionBinding controlPoints;
    NavSectionBinding namedCircles;
    NavSectionBinding movementRanges;
    NavSectionBinding includePolygons;
    NavSectionBinding includeCircles;
    NavSectionBinding excludePolygons;
    NavSectionBinding excludeCircles;
    std::array<NavSectionBinding, 3> auxiliarySections;
    NavAnchorPoint *activeAnchors;
    NavAnchorPoint *transformedAnchors;
    NavRegion *next;
    std::uint64_t reservedTail;
};
static_assert(offsetof(NavRegion, anchors) == 0x18);
static_assert(offsetof(NavRegion, activeAnchors) == 0xc8);
static_assert(offsetof(NavRegion, next) == 0xd8);
static_assert(sizeof(NavRegion) == 0xe8);

struct NavRawSection {
    std::uint32_t name;
    std::uint32_t nextOffset;
    std::int32_t count;
};

constexpr std::array<std::uint32_t, 11> kNavSectionNames{SHEN_NAME(PCCP), SHEN_NAME(PCCN), SHEN_NAME(PCPP), SHEN_NAME(PCTG), SHEN_NAME(PCMR), SHEN_NAME(PCMC),
    SHEN_NAME(PCNR), SHEN_NAME(PCNC), SHEN_NAME(PCCC), SHEN_NAME(PCWC), SHEN_NAME(PCWR)};
extern NavRegion *g_navRegionListHead;
extern const std::byte *g_navigationCollisionDataTable;
extern std::int32_t g_navigationRegionCount;
extern const NavQuad *g_currentNavTriangle;

void navRebuildTransformFor(NavRegion *region);
void navRebuildWorldGeometry(NavRegion *region);
std::int64_t navBindRegionSectionsById(NavRegion *region, std::uint32_t regionId);
NavRegion *navCreateRegion(std::uint32_t regionId, const Vector3 *origin, std::int32_t yaw);
void navUpsertRegionTransform(std::uint32_t previousRegionId, std::uint32_t regionId, const Vector3 *origin, std::int32_t yaw, std::uint32_t flags);
void navUpdateRegionsFromControlPoints();
bool IsPointInsideTriXZ_YBand(const Vector3 *point, const NavTriangle *triangle);
bool navIsPointInsideRegion(const NavRegion *region, const Vector3 *point);

} // namespace shenmue::s1
