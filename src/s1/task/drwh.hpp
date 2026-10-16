#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {



HLib::Task *DRWH_Init();
void DRWH_Shutdown();
HLib::Task *DRWH_GetTask();

void CharDrawHuman_DRWH_Task_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
