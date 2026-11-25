#pragma once

#include "s2/math.hpp"
#include "common/types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

float GetVector3DotProduct(const Vector3 &first, const Vector3 &second);
void GetVector3CrossProduct(const Vector3 &first, const Vector3 &second, Vector3 &output);
bool ComputeAngle(const Vector3 &origin, float radius, std::int32_t yaw, std::int32_t halfAngle, const Vector3 &point);
bool TestCharaPositionWithinRange(shen_name name, const Vector3 *first, const Vector3 *second, shen_name relativeTo);
void scenApplyTransRot(const Vector3 *position, const Vector3i *rotation, Vector3 *point);
void TransformScenarioDirection(const Vector3 *position, const Vector3i *rotation, Vector3 *direction);
void ComputeVectorAlignmentRotation(const Vector3 &first, const Vector3 &second, Vector3i *rotation);
void scenProjectToScreen(const Vector3 *position, Vector2 *screen);

} // namespace s2
} // namespace shenmue

#endif
