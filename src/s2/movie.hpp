#pragma once

#include <cstdint>

namespace shenmue::s2 {
void MovieTask(void *taskData);
void FreeMovieResources();
std::int32_t GetIsRunningMovieRenderer() noexcept;
void ResetRunningMovieRender() noexcept;
} // namespace shenmue::s2
