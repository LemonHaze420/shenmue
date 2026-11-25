#pragma once

#include "platform/dc/matrix.hpp"
#include "s2/math.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

void SetCurrentMatrixTranslation(const Vector3 *translation);
void AddCurrentMatrix(const Matrix4x4 *matrix);
void SubtractCurrentMatrix(const Matrix4x4 *matrix);
void MultiplyCurrentMatrixFromRight(const Matrix4x4 *matrix);
bool PopCurrentMatrix(std::int32_t count);

} } // namespace shenmue::s2

#endif
