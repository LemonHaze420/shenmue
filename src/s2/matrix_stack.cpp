#include "s2/matrix_stack.hpp"

namespace shenmue::s2 {

// @verified - p
void ApplyShearToPrePassMatrix(float sine, float cosine) {
    const Vector4 first = workTransformMatrix.rows[0];
    const Vector4 third = workTransformMatrix.rows[2];
    workTransformMatrix.rows[0] = {
        first.x * cosine - third.x * sine, first.y * cosine - third.y * sine, first.z * cosine - third.z * sine, first.w * cosine - third.w * sine};
    workTransformMatrix.rows[2] = {
        first.x * sine + third.x * cosine, first.y * sine + third.y * cosine, first.z * sine + third.z * cosine, first.w * sine + third.w * cosine};
}

} // namespace shenmue::s2
