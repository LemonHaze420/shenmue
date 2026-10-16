#pragma once

#include "s2/hlib.hpp"

#include <cstdint>

namespace shenmue::s2 {



HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex);



HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex = 12);
std::int64_t OpenNaviWindow();
std::int64_t CloseNaviWindow(std::int32_t force);

bool NaviIsDisplayable(bool skipMapItemRequirement);

void ReleaseNavigationTaskOwners();

} // namespace shenmue::s2
