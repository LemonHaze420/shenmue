#pragma once

#include "s2/motion_snapshot_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

void WriteMotionSnapshotData(MotionSnapshot *snapshot, Character *character, MotionWork *work);
std::int32_t CaptureMotionSnapshot(shen_name subject);

} } // namespace shenmue::s2

#endif
