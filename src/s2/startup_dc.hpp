#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"

namespace shenmue {
namespace s2 {

extern shen_name g_currentTaskName;
void InitializeMemoryAndHLib();
void StartupNullsub();
void ExecuteTasks();

} } // namespace shenmue::s2

#endif
