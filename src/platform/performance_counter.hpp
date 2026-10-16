#pragma once

#include <cstdint>

namespace shenmue::platform {

std::int64_t performanceCounterFrequency() noexcept;
std::int64_t performanceCounter() noexcept;

} // namespace shenmue::platform
