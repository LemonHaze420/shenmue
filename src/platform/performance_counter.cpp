#include "platform/performance_counter.hpp"
#include "api/platform.h"

namespace shenmue::platform {

std::int64_t performanceCounterFrequency() noexcept {
    return shen_platform_counter_frequency();
}

std::int64_t performanceCounter() noexcept {
    return shen_platform_counter();
}

} // namespace shenmue::platform
