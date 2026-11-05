#include "api/platform.h"

#include <chrono>
#include <thread>

extern "C" {

int64_t shen_platform_counter_frequency(void) {
    typedef std::chrono::steady_clock::period period;
    return static_cast<int64_t>(period::den / period::num);
}

int64_t shen_platform_counter(void) {
    return std::chrono::steady_clock::now().time_since_epoch().count();
}

void shen_platform_sleep_nanoseconds(int64_t nanoseconds) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
}
}
