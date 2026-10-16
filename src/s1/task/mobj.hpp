#pragma once
#include "s1/task/cycle_man.hpp"

namespace shenmue::s1 {


bool SetMOBJTaskPtr();
bool MOBJ_AllocateTaskMemory();
bool MOBJ_SetupTaskPointer();
bool MOBJ_Setup(const char *filename, shen_name areaId);
void MOBJ_Cleanup();
void MOBJ_SetEnabled(bool enabled);
void MOBJ_CoreCallback(HLib::TaskData *taskData);

} // namespace shenmue::s1
