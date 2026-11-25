#pragma once

#include "s2/motion_work_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

bool mtwkIsRightFootTargetAhead(MotionWork *work);
bool ApplyCharacterMotionToPose(Character *character, Vector3 *position, Vector3i *rotation, float frame, std::uint16_t motion);
bool SampleNativeMotionRoot(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *output);
bool SampleNativeMotionRootAndRotation(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *position, Vector3i *rotation);

} } // namespace shenmue::s2

#endif
