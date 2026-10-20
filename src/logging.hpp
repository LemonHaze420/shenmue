#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <mutex>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <unistd.h>
#endif
inline std::int32_t g_d3tDebugLogEnabled{};
inline std::array<char, 1024> g_d3tDebugLogBuffer{};

inline const char *D3T_LogModeLabel(const int mode) {
    if (mode == 0xff)
        return "[DBG]";
    constexpr std::array<const char *, 7> labels{"[DBG]", "[VER]", "[SIL]", "[IMP]", "[ERR]", "[WAR]", "[BUILD_ERROR]"};
    return mode >= 0 && static_cast<std::size_t>(mode) < labels.size() ? labels[static_cast<std::size_t>(mode)] : "[DBG]";
}

inline std::FILE *D3T_OpenDebugLogFile() {
    static std::FILE *file = [] {
        std::array<char, 4096> executable{};
#if defined(_WIN32)
        const DWORD length = GetModuleFileNameA(nullptr, executable.data(), static_cast<DWORD>(executable.size()));
        if (length == 0 || length >= executable.size())
            return static_cast<std::FILE *>(nullptr);
#else
        const ssize_t length = readlink("/proc/self/exe", executable.data(), executable.size() - 1);
        if (length <= 0 || static_cast<std::size_t>(length) >= executable.size())
            return static_cast<std::FILE *>(nullptr);
        executable[static_cast<std::size_t>(length)] = '\0';
#endif
        char *leaf = std::max(std::strrchr(executable.data(), '/'), std::strrchr(executable.data(), '\\'));
        leaf = leaf ? leaf + 1 : executable.data();
        if (char *extension = std::strrchr(leaf, '.'))
            *extension = '\0';
        std::string filename{leaf};
        filename += "-DBGLOG.txt";
        return std::fopen(filename.c_str(), "w");
    }();
    return file;
}

inline int AppendLogLineToFile(const char *line) {
    if (std::FILE *file = D3T_OpenDebugLogFile())
        return std::fputs(line, file);
    return 0;
}

inline int FormatAndEmitLogLine(const int mode, const char *source, const int line, const char *format, ...) {
    if (mode < 1)
        return 0;

    static const auto start = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

    const char *basename = std::max(std::strrchr(source, '/'), std::strrchr(source, '\\'));
    basename = basename ? basename + 1 : source;
    std::array<char, 19> shortened{};
    if (std::strlen(basename) > 18) {
        std::memcpy(shortened.data(), basename, 15);
        std::memcpy(shortened.data() + 15, "...", 4);
        basename = shortened.data();
    }

    std::array<char, 2048> message{};
    const int prefixLength =
        std::sprintf(message.data(), "[%-8lli] %s %-18.18s (%-5i): ", static_cast<long long>(elapsed), D3T_LogModeLabel(mode), basename, line);
    std::va_list arguments;
    va_start(arguments, format);
    std::vsprintf(message.data() + prefixLength, format, arguments);
    va_end(arguments);
    std::strcat(message.data(), "\n");

#if defined(_WIN32)
    OutputDebugStringA(message.data());
#endif
    std::fputs(message.data(), stdout);
    std::fflush(stdout);
    return AppendLogLineToFile(message.data());
}

namespace shenmue::d3t {
inline int debug_log(const char *source, const int line, const char *format, ...) {
    std::va_list arguments;
    va_start(arguments, format);
    std::vsprintf(g_d3tDebugLogBuffer.data(), format, arguments);
    va_end(arguments);
    return FormatAndEmitLogLine(0xff, source, line, g_d3tDebugLogBuffer.data());
}

} // namespace shenmue::d3t
inline void D3T_DebugLog(const char *format, ...) {
    if (!g_d3tDebugLogEnabled)
        return;
    std::va_list arguments;
    va_start(arguments, format);
    std::vsprintf(g_d3tDebugLogBuffer.data(), format, arguments);
    va_end(arguments);
}

inline int Shenmue_LogPrintf(std::FILE *stream, const char *format, ...) {
    if (!stream || !format)
        return -1;

    static std::mutex mutex;
    const std::lock_guard<std::mutex> lock(mutex);

    const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm local{};
#if defined(_WIN32)
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    std::array<char, 32> timestamp{};
    std::strftime(timestamp.data(), timestamp.size(), "%I:%M %p %d/%m/%Y", &local);

    int written = std::fprintf(stream, "[inf|%s] ", timestamp.data());
    std::va_list arguments;
    va_start(arguments, format);
    const int messageLength = std::vfprintf(stream, format, arguments);
    va_end(arguments);
    std::fflush(stream);
    if (written < 0 || messageLength < 0)
        return -1;
    return written + messageLength;
}
