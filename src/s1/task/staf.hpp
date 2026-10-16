#pragma once

#include "s1/hlib.hpp"
#include <cstdint>
namespace shenmue::s1 {
HLib::Task *STAF_GetTask() noexcept;
bool STAF_HasOwnedResources() noexcept;

void Module_STAF(HLib::TaskData *taskData);

} // namespace shenmue::s1
