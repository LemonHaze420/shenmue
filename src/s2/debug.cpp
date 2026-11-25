#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/debug.hpp"
#else
#include "s2/debug.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void debug_log(const char*, std::int32_t, const char*, std::va_list) {
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void EV_debug_log(const char *file, std::int32_t line, const char *format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    debug_log(file, line, format, arguments);
    va_end(arguments);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

extern "C" std::uint32_t shenmue_dc_loader_sentinel;

static std::int32_t trapLogStarted = 0;
static char trapLogMessage[0x100];
static const char *trapAssertFile;
static std::int32_t trapAssertLine;

static void trapSerialPutReady(std::uint8_t value) {
    std::int32_t attempt = 0;
    std::int16_t *serialStatus;
    std::int32_t status;
    goto poll;
retry:
    if (++attempt > 0xfff)
        return;
poll:
    serialStatus = reinterpret_cast<std::int16_t *>(0xffe80010);
    do {
        status = *serialStatus;
    } while (0);
    if (!(status & 0x20))
        goto retry;
    *reinterpret_cast<std::uint8_t *>(0xffe8000c) = value;
    *serialStatus = static_cast<std::int16_t>(status & 0x9f);
}

static void trapSerialInit(std::int32_t baud) {
    *reinterpret_cast<std::uint16_t *>(0xffe80008) = 0;
    *reinterpret_cast<std::uint16_t *>(0xffe80018) = 6;
    *reinterpret_cast<std::uint16_t *>(0xffe80000) = 0;
    if (baud == 0x180 || baud == 0x9600)
        *reinterpret_cast<std::uint8_t *>(0xffe80004) = 40;
    else
        asm volatile("trapa #0x20");
    for (volatile std::int32_t delay = 0; delay <= 99999; ++delay) {
    }
    *reinterpret_cast<std::uint16_t *>(0xffe80018) = 0;
    *reinterpret_cast<std::uint16_t *>(0xffe80008) = 0x30;
}

static std::int32_t trapSerialSetupTimer();

std::uint8_t trapSerialSelectBaud(std::int32_t baud) {
    std::int32_t clocks[16];
    clocks[0] = 50000000;
    clocks[1] = 25000000;
    clocks[2] = 44444444;
    clocks[3] = 22222222;
    clocks[4] = 60000000;
    clocks[5] = 52500000;
    clocks[6] = 45000000;
    clocks[7] = 42954000;
    clocks[8] = 37500000;
    clocks[9] = 33000000;
    clocks[10] = 30000000;
    clocks[11] = 24000000;
    clocks[12] = 21477000;
    clocks[13] = 20250000;
    clocks[14] = 15000000;
    clocks[15] = 22222222;
    std::int32_t clock = trapSerialSetupTimer();
    if (clock == 1)
        clock = clocks[*reinterpret_cast<std::uint32_t *>(0xa05f74b0) & 0xfU];
    std::int32_t count = 0;
    for (clock -= baud << 4; clock >= baud << 4; clock -= baud << 4) {
        if (++count > 255) {
            count >>= 1;
            return static_cast<std::uint8_t>(count);
        }
    }
    count >>= 1;
    return static_cast<std::uint8_t>(count);
}

static std::int32_t trapSerialSetupTimer() {
    *reinterpret_cast<std::uint32_t *>(0xffd80020) = 0xffffffffU;
    *reinterpret_cast<std::uint32_t *>(0xffd80024) = 0xffffffffU;
    *reinterpret_cast<std::uint16_t *>(0xffd80028) = 0;
    *reinterpret_cast<std::uint8_t *>(0xffd80004) |= 4;
    const std::uint8_t start = *reinterpret_cast<std::uint8_t *>(0xffc80000);
    std::int32_t ticked = 0;
    for (;;) {
        if (start != *reinterpret_cast<std::uint8_t *>(0xffc80000)) {
            ticked = 1;
            break;
        }
        if (!(*reinterpret_cast<std::uint32_t *>(0xffd80024) > 0xfffc0000U))
            break;
    }
    *reinterpret_cast<std::uint8_t *>(0xffd80004) &= ~4;
    if (!ticked)
        return 1;
    *reinterpret_cast<std::uint32_t *>(0xffd80024) = 0xffffffffU;
    do {
    } while (*reinterpret_cast<std::uint8_t *>(0xffc80000) & 0x40);
    do {
    } while (!(*reinterpret_cast<std::uint8_t *>(0xffc80000) & 0x40));
    *reinterpret_cast<std::uint8_t *>(0xffd80004) |= 4;
    do {
    } while (*reinterpret_cast<std::uint8_t *>(0xffc80000) & 0x40);
    *reinterpret_cast<std::uint8_t *>(0xffd80004) &= ~4;
    return static_cast<std::int32_t>(~*reinterpret_cast<std::uint32_t *>(0xffd80024) << 3);
}

void start_trap_info_log(const char *message) {
    if (!trapLogStarted) {
        trapSerialInit(0x9600);
        trapLogStarted = 1;
        start_trap_info_log("--- start
");
    }
    while (*message)
        trapSerialPutReady(static_cast<std::uint8_t>(*message++));
}

void trap_info_printf(const char *format, ...) {
    if (shenmue_dc_loader_sentinel) {
        std::va_list arguments;
        va_start(arguments, format);
        std::vsprintf(trapLogMessage, format, arguments);
        start_trap_info_log(trapLogMessage);
        va_end(arguments);
    }
}

void AM2_assert_trap(const char *file, std::int32_t line) {
    trapAssertFile = file;
    trapAssertLine = line;
    std::sprintf(trapLogMessage, "ASSERT at %s(%d)
", file, line);
    start_trap_info_log(trapLogMessage);
    asm volatile("trapa #0x30");
}

// @verified - d
void EventAssertNullsub(const char *, std::int32_t, ...) {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void EventAssertMessageNullsub(const char *, std::int32_t, const char *) {
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DebugLogToFileRemoved(const char *, ...) {
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DebugObjectNullsub() {
}
#endif

#endif

} // namespace s2
} // namespace shenmue
