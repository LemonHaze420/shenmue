#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include <cstdint>

namespace shenmue { namespace s2 {

extern std::uint32_t objectDrawLightEnabled;
extern std::int32_t objectDrawLightKind;
extern Vector4 objectDrawLightColour;
extern Vector3 objectDrawLightDirection, objectDrawLightPosition;
extern float objectDrawLightRange, objectDrawLightSceneIntensity, objectDrawLightFalloff;
void SetObjectDrawLightEnabled(std::uint32_t enabled);
void SetObjectDrawLightKind(std::int32_t kind);
void SetObjectDrawLightColour(const Vector4 *colour);
void SetObjectDrawLightSceneIntensity(float intensity);
void SetObjectDrawLightDirection(const Vector3 *direction);
void SetObjectDrawLightPosition(const Vector3 *position);
void SetObjectDrawLightRange(float range);
void SetObjectDrawLightFalloff(float falloff);
void ResetObjectDrawLight();
void SetupRefboard();

} } // namespace shenmue::s2

#endif
