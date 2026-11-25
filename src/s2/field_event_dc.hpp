#pragma once

#include "s2/field_sphere_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

inline shen_name FieldBasePartName() {
    return shen_name(SHEN_NAME4('0', '0', '0', '0'));
}
#define FIELD_BASE_PART FieldBasePartName()
const std::uint32_t fieldCategoryTerminator = 0xffffff01U;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ToggleFlagOnFieldEntity(shen_name name, std::int32_t enabled);
std::uint32_t plymDoIntersectionTest(const Vector3 *position);
std::uint32_t flddQueryCategory6InRadius(const Vector3 *position);
bool CheckSegmentCylinder(const Vector3 *start, const Vector3 *end, const Vector3 *center, float radius, float height);
#endif
void flddPushIntersectionFlag(std::uint16_t flags);
std::uint32_t GetFieldSoundProperties(const Vector3 *position);
const std::uint32_t *FindFieldCategoryRecord(std::uint32_t category);
std::uint32_t IsWithinRadiusFromVector3PolyIntersectTest(const Vector3 *position, float radius);
void TransformAgainstIntersectionResult(Vector3 *position);
std::int32_t TestFieldPartSphereIntersection(const Vector3 *position, float radius);
std::int32_t GatherFieldSphereVolumeCells(const Vector3 *position, float radius, std::int16_t *cells);
float SignedDistanceToFieldPlane(const Vector3 *position);
void ProjectPointToFieldPlane(Vector3 *output, const Vector3 *position);

} // namespace s2
} // namespace shenmue

#endif
