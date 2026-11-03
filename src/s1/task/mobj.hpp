#pragma once
#include "shenmue.h"
#include "s1/task/cycle_man.hpp"

namespace shenmue::s1 {

inline constexpr shen_name TASK_MOBJ = SHEN_NAME(MOBJ);
bool SetMOBJTaskPtr();
bool MOBJ_AllocateTaskMemory();
bool MOBJ_SetupTaskPointer();
bool MOBJ_Setup(const char *filename, shen_name areaId);
#if SHENMUE_IS_DEMO
bool MOBJ_Setup(const char *directory, const char *filename, shen_name areaId);
#endif
void MOBJ_Cleanup();
void MOBJ_SetEnabled(bool enabled);
void MOBJ_CoreCallback(HLib::TaskData *taskData);

} // namespace shenmue::s1
