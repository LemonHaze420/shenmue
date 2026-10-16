#include "s1/navigation.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/char.hpp"
#include "s1/hlib.hpp"
#include "s1/matrix_stack.hpp"

namespace shenmue::s1 {
namespace {

Vector3 RotateNavPoint(const Vector3 &point, std::int32_t yaw) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(fixed16_cosine_angle(static_cast<std::uint16_t>(yaw)));
    return {
        point.x * cosine + point.z * sine,
        point.y,
        point.z * cosine - point.x * sine,
    };
}

bool NavCircleContains(const NavCircle &circle, const Vector3 &point) {
    if (point.y < static_cast<double>(circle.center.y) - 2.0 || point.y > static_cast<double>(circle.center.y) + 2.0)
        return false;
    const float dx = point.x - circle.center.x;
    const float dz = point.z - circle.center.z;
    return dx * dx + dz * dz <= circle.radius * circle.radius;
}
} // namespace

void navRebuildTransformFor(NavRegion *region) {
    const auto *const source = static_cast<const NavAnchorPoint *>(region->anchors.data);
    auto *const destination = region->transformedAnchors;
    for (std::int32_t index = 0; index < region->anchors.count; ++index) {
        const Vector3 rotated = RotateNavPoint({source[index].x, source[index].y, source[index].z}, region->yaw);
        destination[index] = {
            region->origin.x + rotated.x,
            region->origin.y + rotated.y,
            region->origin.z + rotated.z,
            source[index].radius,
        };
    }
}

void navRebuildWorldGeometry(NavRegion *region) {
    if (region->yaw == 0 && region->origin.x == 0.0f && region->origin.y == 0.0f && region->origin.z == 0.0f) {
        region->activeAnchors = static_cast<NavAnchorPoint *>(region->anchors.data);
        return;
    }
    if (region->transformedAnchors) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(region->transformedAnchors), 0);
    }
    region->transformedAnchors =
        static_cast<NavAnchorPoint *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(sizeof(NavAnchorPoint)) * region->anchors.count, 0x20204350));
    region->activeAnchors = region->transformedAnchors;
    navRebuildTransformFor(region);
}

std::int64_t navBindRegionSectionsById(NavRegion *region, std::uint32_t regionId) {
    auto *regionSection = reinterpret_cast<const NavRawSection *>(g_navigationCollisionDataTable);
    while (regionSection->name != regionId) {
        if (!regionSection->nextOffset)
            return 1;
        regionSection = reinterpret_cast<const NavRawSection *>(reinterpret_cast<const std::byte *>(regionSection) + regionSection->nextOffset);
    }

    region->id = regionId;
    const NavRawSection *const sectionList = reinterpret_cast<const NavRawSection *>(reinterpret_cast<const std::byte *>(regionSection) + 8);
    const std::array<NavSectionBinding *, 11> bindings{
        &region->anchors,
        &region->controlPoints,
        &region->namedCircles,
        &region->movementRanges,
        &region->includePolygons,
        &region->includeCircles,
        &region->excludePolygons,
        &region->excludeCircles,
        &region->auxiliarySections[0],
        &region->auxiliarySections[1],
        &region->auxiliarySections[2],
    };
    for (std::size_t index = 0; index < bindings.size(); ++index) {
        const NavRawSection *section = sectionList;
        while (section->name != kNavSectionNames[index] && section->nextOffset) {
            section = reinterpret_cast<const NavRawSection *>(reinterpret_cast<const std::byte *>(section) + section->nextOffset);
        }
        if (section->name == kNavSectionNames[index]) {
            bindings[index]->count = section->count;
            bindings[index]->data = const_cast<std::byte *>(reinterpret_cast<const std::byte *>(section) + 12);
        } else {
            bindings[index]->count = 0;
            bindings[index]->data = nullptr;
        }
    }
    navRebuildWorldGeometry(region);
    return 0;
}

NavRegion *navCreateRegion(std::uint32_t regionId, const Vector3 *origin, std::int32_t yaw) {
    NavRegion **link = &g_navRegionListHead;
    while (*link) {
        NavRegion *const region = *link;
        if (region->id == regionId && origin && region->origin.x == origin->x && region->origin.y == origin->y && region->origin.z == origin->z &&
            region->yaw == yaw) {
            return region;
        }
        link = &region->next;
    }

    auto *const region = static_cast<NavRegion *>(HLib_AllocHeapMemBlock(sizeof(NavRegion), static_cast<std::int32_t>(regionId)));
    region->transformedAnchors = nullptr;
    if (origin) {
        region->origin = *origin;
        region->yaw = yaw;
    } else {
        region->origin = {};
        region->yaw = 0;
    }
    if (navBindRegionSectionsById(region, regionId)) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(region), 0);
        return nullptr;
    }
    ++g_navigationRegionCount;
    *link = region;
    region->next = nullptr;
    region->active = 1;
    return region;
}

void navUpsertRegionTransform(std::uint32_t previousRegionId, std::uint32_t regionId, const Vector3 *origin, std::int32_t yaw, std::uint32_t flags) {
    NavRegion *reusable = nullptr;
    for (NavRegion *region = g_navRegionListHead; region; region = region->next) {
        if (region->id == regionId && region->origin.x == origin->x && region->origin.y == origin->y && region->origin.z == origin->z && region->yaw == yaw) {
            return;
        }
        if (flags & 1U) {
            if (region->id == previousRegionId) {
                reusable = region;
                break;
            }
        } else if (region->id != previousRegionId && region->active) {
            reusable = region;
        }
    }
    if (!reusable) {
        navCreateRegion(regionId, origin, yaw);
        return;
    }
    reusable->origin = *origin;
    reusable->yaw = yaw;
    navBindRegionSectionsById(reusable, regionId);
}

void navUpdateRegionsFromControlPoints() {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    const Vector3 playerPosition = control->ryo ? GetPositionForEntity(static_cast<Character *>(control->ryo)->entity.node) : Vector3{};
    for (NavRegion *region = g_navRegionListHead; region; region = region->next) {
        const auto *const points = static_cast<const NavRegionControlPoint *>(region->controlPoints.data);
        for (std::int32_t index = 0; index < region->controlPoints.count; ++index) {
            const NavRegionControlPoint &point = points[index];
            if (playerPosition.y < point.position.y - 2.0f || playerPosition.y > point.position.y + 2.0f)
                continue;
            const float dx = playerPosition.x - point.position.x;
            const float dz = playerPosition.z - point.position.z;
            if (dx * dx + dz * dz <= point.radius * point.radius) {
                navUpsertRegionTransform(region->id, point.regionId, &point.origin, point.yaw, point.flags);
            }
        }
    }
}

bool IsPointInsideTriXZ_YBand(const Vector3 *point, const NavTriangle *triangle) {
    if (point->y < static_cast<double>(triangle->centerY) - 2.0 || point->y > static_cast<double>(triangle->centerY) + 2.0)
        return false;
    g_currentNavTriangle = &triangle->quad;
    const Vector2 &last = triangle->quad.vertices[3];
    const Vector2 &head = triangle->quad.vertices[0];
    const float first = (point->x - last.y) * (head.x - last.x) - (head.y - last.y) * (point->z - last.x);
    if (!(first > 0.0f || first < 0.0f))
        return true;
    for (std::int32_t index = 3; index > 0; --index) {
        const Vector2 &start = triangle->quad.vertices[static_cast<std::size_t>(index - 1)];
        const Vector2 &end = triangle->quad.vertices[static_cast<std::size_t>(index)];
        const float side = (end.x - start.x) * (point->x - start.y) - (end.y - start.y) * (point->z - start.x);
        if (first > 0.0f ? side < 0.0f : side > 0.0f)
            return false;
    }
    return true;
}

bool navIsPointInsideRegion(const NavRegion *region, const Vector3 *point) {
    Vector3 local = *point;
    if (region->yaw != 0 || region->origin.x != 0.0f || region->origin.y != 0.0f || region->origin.z != 0.0f) {
        workTransformMatrix = transformMatrix_prePass;
        const std::uint16_t inverseYaw = static_cast<std::uint16_t>(-static_cast<std::int64_t>(region->yaw));
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(inverseYaw)), sin_fixed16(fixed16_cosine_angle(inverseYaw)));
        Matrix4x4::preTranslate(-region->origin.x, -region->origin.y, -region->origin.z);
        Matrix4x4::TransformPoint(*point, local);
    }

    bool included = false;
    const auto *const includePolygons = static_cast<const NavTriangle *>(region->includePolygons.data);
    for (std::int32_t index = 0; index < region->includePolygons.count; ++index) {
        if (IsPointInsideTriXZ_YBand(&local, &includePolygons[index])) {
            included = true;
            break;
        }
    }
    if (!included) {
        const auto *const includeCircles = static_cast<const NavCircle *>(region->includeCircles.data);
        for (std::int32_t index = 0; index < region->includeCircles.count; ++index) {
            if (NavCircleContains(includeCircles[index], local)) {
                included = true;
                break;
            }
        }
    }
    if (!included)
        return false;

    const auto *const excludePolygons = static_cast<const NavTriangle *>(region->excludePolygons.data);
    for (std::int32_t index = 0; index < region->excludePolygons.count; ++index) {
        if (IsPointInsideTriXZ_YBand(&local, &excludePolygons[index]))
            return false;
    }
    const auto *const excludeCircles = static_cast<const NavCircle *>(region->excludeCircles.data);
    for (std::int32_t index = 0; index < region->excludeCircles.count; ++index) {
        if (NavCircleContains(excludeCircles[index], local))
            return false;
    }
    return true;
}
} // namespace shenmue::s1
