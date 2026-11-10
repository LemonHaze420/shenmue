#pragma once

#include "shenmue.h"



#include "s2/hlib.hpp"

#include <cstdint>

namespace shenmue::s2 {



HLib::Task *NAVI_DummyWait_Enqueue(std::uint8_t nextFunctionIndex);



HLib::Task *NVW_CtrlNaviWindow_Enqueue(std::uint8_t nextFunctionIndex = 12);
std::int64_t OpenNaviWindow();
std::int64_t CloseNaviWindow(std::int32_t force);

bool NaviIsDisplayable(bool skipMapItemRequirement);
std::uint32_t NaviFolderEntryMask(std::int32_t page, std::uint16_t excludedFlags = 2);
std::int32_t NaviFolderEntryCount(std::int32_t page, std::uint16_t excludedFlags = 2);
bool IsCurrentMapNaviFolderAvailable();
const char *GetNaviFolderLabel(std::int32_t page);
const char *GetNaviFolderEntryLabel(std::int32_t page, std::int32_t entry);

void ReleaseNavigationTaskOwners();

} // namespace shenmue::s2

