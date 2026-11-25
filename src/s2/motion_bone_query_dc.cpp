#include "s2/motion_work_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

bool mtwkGetControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position);

std::int32_t ParentBoneId(std::uint32_t selector);

bool mtwkLatchBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position);

bool mtwkGetBonePosition(MotionWork *work, MotionBoneId bone, Vector3 *position);

std::int32_t ParentBoneId(std::uint32_t selector);

bool mtwkLatchBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position);

bool mtwkGetControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position);

bool mtwkGetBonePosition(MotionWork *work, MotionBoneId bone, Vector3 *position);

// @verified - d
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output) {
    const std::byte *const storage = reinterpret_cast<const std::byte *>(&head);
    matrix_stack::Matrix4x4_Push(reinterpret_cast<const Matrix4x4 *>(storage + kMotionHeadWorldMatrixOffset));
    Matrix4x4::preTranslate(
        *reinterpret_cast<const float *>(storage + kMotionHeadEyeOffset), 0.0f, *reinterpret_cast<const float *>(storage + kMotionHeadEyeOffset + 8));
    matrix_stack::GetMatrix(output);
    PopCurrentMatrix(1);
}

} // namespace s2
} // namespace shenmue

#endif
