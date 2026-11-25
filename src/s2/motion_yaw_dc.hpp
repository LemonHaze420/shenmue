#pragma once
#include "s2/motion_work_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *matrix);
std::uint32_t mtwkGetBoneYaw(const MotionWork *work, std::int32_t bone);
} } // namespace shenmue::s2
#endif
