#pragma once

#include <cstdarg>
#include <cstdint>

namespace shenmue::s2 {

void debug_log(const char* file, std::int32_t line, const char* format, std::va_list arguments);
void EV_debug_log(const char *file, std::int32_t line, const char *format, ...);

} // namespace shenmue::s2
