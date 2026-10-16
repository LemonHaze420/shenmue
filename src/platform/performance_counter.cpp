#include "platform/performance_counter.hpp"

#include <chrono>

namespace shenmue::platform {

std::int64_t performanceCounterFrequency() noexcept {
    using Period = std::chrono::steady_clock::period;
    return static_cast<std::int64_t>(Period::den / Period::num);
}

std::int64_t performanceCounter() noexcept {
    return std::chrono::steady_clock::now().time_since_epoch().count();
}

} // namespace shenmue::platform
