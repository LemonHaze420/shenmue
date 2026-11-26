#pragma once

#include "common/matrix_stack.hpp"

#if SHENMUE_IS_DC

namespace shenmue {
namespace matrix_stack {

#if SHENMUE_S2_GCC295
Matrix4x4 *GetMatrix(Matrix4x4 *memory = 0);
#else
Matrix4x4 *GetMatrix(Matrix4x4 *memory = nullptr);
#endif
#if SHENMUE_IS_S2
void CopyMatrix(Matrix4x4 *destination, const Matrix4x4 *source);
#endif
void GetPreviousMatrix(Matrix4x4 *memory) asm("_shenmue_dc_get_previous_particle_matrix");
#if SHENMUE_S2_GCC295
void LoadMatrix(const Matrix4x4 *memory = 0);
#else
void LoadMatrix(const Matrix4x4 *memory = nullptr);
#endif
// GameJam's second entry point: same null handling, loads the matrix with paired (64-bit) moves
void LoadMatrixPairs(const Matrix4x4 *memory);
float InvertMatrix(Matrix4x4 *matrix);
float FactorizeMatrix(Matrix4x4 *matrix, std::int8_t *permutation);
extern Matrix4x4 g_savedWorkMatrix;
void SaveWorkMatrix();
void RestoreWorkMatrix();
void TranslateVector(const Vector3 *translation);
void TranslateInverseVector(const Vector3 *translation);
void UnitCurrentMatrix();
bool PushUnitMatrix();
void GetTranslation(Vector3 *position);
float InnerProduct4(const Vector4 &first, const Vector4 &second);
extern Matrix4x4 g_worldToScreenMatrix;
extern float g_projectionViewportWidth;
extern float g_projectionViewportHeight;
extern float g_projectionViewportOriginY;
extern float g_projectionViewportOriginX;
void TransformPosition(const Vector3 *input, Vector4 *output);
#if SHENMUE_IS_S2
void TransformVector4(const Vector4 *input, Vector4 *output);
#endif
#if SHENMUE_IS_S2
void RotateXRadians(float angle);
void RotateYRadians(float angle);
#endif
void RotateXSinCos(float sine, float cosine);
void RotateYSinCos(float sine, float cosine);
void RotateZSinCos(float sine, float cosine);
void MultiplyPreviousMatrix();
#if SHENMUE_IS_S2
void UpdateWorldToScreenMatrix(float width, float height);
#else
void UpdateWorldToScreenMatrix();
#endif
void ProjectWorldPointToScreen(const Vector3 *input, Vector2 *output);

} // namespace matrix_stack
} // namespace shenmue

#endif
