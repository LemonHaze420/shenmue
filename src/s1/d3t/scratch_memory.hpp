#pragma once

#include <cstddef>

namespace shenmue::s1 {

void *AllocateMemoryFromGlobalPool(std::size_t size);
void *GetGlobalPoolCheckpoint();
void RestoreGlobalPoolCheckpoint(void *checkpoint);
void ResetGlobalPoolSlices();

} // namespace shenmue::s1
