#pragma once

#include <cstdint>

struct _EXCEPTION_POINTERS;

namespace shenmue::d3t {

void CollectSymbols();
std::int32_t CrashReporter(_EXCEPTION_POINTERS *exception);

} // namespace shenmue::d3t
