#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {

HLib::Task *METK_Init();
void METK_Callback(HLib::TaskData *taskData);


bool EnqueueMETK();
void DestroyMETK();

HLib::Task *METK_GetTask();

} // namespace shenmue::s1
