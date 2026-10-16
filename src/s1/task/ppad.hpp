#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {
extern std::int32_t g_ppadNeedsSetup;

void PPAD_Init();
void PPAD_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
