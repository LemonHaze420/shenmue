#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {



HLib::Task *DRWO_Init();
void DRWO_Shutdown();
HLib::Task *DRWO_GetTask();

void CharDrawObject_DRWO_Task_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
