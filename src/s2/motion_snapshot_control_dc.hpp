#pragma once

#include "s2/motion_registry_dc.hpp"
#include "s2/motion_work_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

std::uint32_t RestoreMotionSnapshot(shen_name subject, std::uint32_t index);
std::uint32_t RestoreMotionSnapshotPlacement(shen_name subject, std::uint32_t index);
std::uint32_t ReleaseMotionSnapshot(std::uint32_t index);
void mtwkApplySnapshot(MotionWork *work, const MotionSnapshot *snapshot);

} } // namespace shenmue::s2

#endif
