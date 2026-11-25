#pragma once

#include "shenmue.h"
#include <cstdio>
#include <cstdarg>
#include <cstdint>

namespace shenmue {
namespace s2 {
#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint8_t trapSerialSelectBaud(std::int32_t baud);
void start_trap_info_log(const char *message);
void AM2_assert_trap(const char *file, std::int32_t line);
void EventAssertNullsub(const char *, std::int32_t, ...);
void EventAssertMessageNullsub(const char *, std::int32_t, const char *);
#endif

void debug_log(const char *file, std::int32_t line, const char *format, std::va_list arguments);
void EV_debug_log(const char *file, std::int32_t line, const char *format, ...);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DebugLogToFileRemoved(const char *format, ...);
#endif

} // namespace s2
} // namespace shenmue
