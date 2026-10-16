#include "s2/debug.hpp"

namespace shenmue::s2 {

// @verified - p
void debug_log(const char*, std::int32_t, const char*, std::va_list) {
}

// @verified - p
void EV_debug_log(const char *file, std::int32_t line, const char *format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    debug_log(file, line, format, arguments);
    va_end(arguments);
}

} // namespace shenmue::s2
