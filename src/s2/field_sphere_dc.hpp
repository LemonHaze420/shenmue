#pragma once

#include "s2/field_query_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t TestFieldLineSphere(const Vector3 *origin, const Vector3 *end, const Vector3 *centre, float radius);
std::int32_t FindFieldGridCell(const Vector3 *position);
std::int32_t GatherFieldSegmentGridCells(const Vector3 *start, const Vector3 *end, std::int16_t *cells);
void OffsetPointFromFieldPlane(Vector3 *output, const Vector3 *position, float amount);
std::int32_t FindFieldPartEndpointIntersection(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int32_t TestFieldPartSegment(const Vector3 *start, const Vector3 *end);
#endif
std::int32_t GatherFieldSphereGridCells(const Vector3 *position, float radius, std::int16_t *cells);
bool ResolveFieldPolygonSphereOverlap(Vector3 *output, const Vector3 *position, float radius);
std::int32_t GetFieldPolygonFlags(std::int16_t index);
void SelectFieldPolygon(std::int16_t index);
bool IsPointAboveFieldPlane(const Vector3 *position);
bool FieldPolygonContainsPoint(const Vector3 *position);
float IntersectFieldPlane(Vector3 *output, const Vector3 *start, const Vector3 *end);
void StoreFieldIntersectionPlane(std::int16_t polygon);
void StoreFieldIntersectionPosition(const Vector3 *position);

} // namespace s2
} // namespace shenmue

#endif
