#include "s2/field_sphere_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"

namespace shenmue {
namespace s2 {

void IntersectFieldPlaneAlongNormal(Vector3 *output, const Vector3 *position, const Vector3 *direction);

// @verified - d
std::uint32_t ResolveFieldPartSphereOverlap(Vector3 *output, const Vector3 *position, float radius) {
    std::int16_t *cell;
    std::uint32_t result = 0;
    Vector3 intersection, point, normal;
    Vector3 *const hit = &intersection, *const current = &point, *const push = &normal;
    if (!g_currentFieldData) {
        *current = *position;
        return 0;
    }
    const std::int16_t flags = g_fieldDataFlags, required = g_fieldDataClearFlags;
    *current = *position;
    cell = g_fieldGridCells;
    GatherFieldSphereGridCells(position, radius, cell);
    const float radiusSquared = radius * radius;
    for (; *cell != -1; ++cell) {
        for (const std::int16_t *polygon = g_currentFieldData->polygonLists[*cell]; *polygon != -1; ++polygon) {
            if (static_cast<std::int16_t>((flags | required) & GetFieldPolygonFlags(*polygon)) != required)
                continue;
            SelectFieldPolygon(*polygon);
            if (!IsPointAboveFieldPlane(current))
                continue;
            push->x = g_currentFieldPlane.normal.x;
            push->y = 0.0f;
            push->z = g_currentFieldPlane.normal.z;
            IntersectFieldPlaneAlongNormal(hit, current, push);
            const float x = hit->x - current->x, z = hit->z - current->z;
            if (!(radiusSquared > x * x + z * z))
                continue;
            if (FieldPolygonContainsPoint(hit)) {
                result = 1;
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(hit);
                am2Vector3NormalizeInPlace(push);
                current->x = ((radius) * (push->x) + (hit->x));
                current->z = ((radius) * (push->z) + (hit->z));
            } else if (ResolveFieldPolygonSphereOverlap(hit, current, radius)) {
                StoreFieldIntersectionPlane(*polygon);
                StoreFieldIntersectionPosition(hit);
                if (hit->y == current->y) {
                    result = 1;
                    current->x = hit->x;
                    current->z = hit->z;
                }
            }
        }
    }
    *output = *current;
    return result;
}

} // namespace s2
} // namespace shenmue

#endif
