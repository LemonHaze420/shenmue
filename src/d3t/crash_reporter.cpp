#include "d3t/crash_reporter.hpp"

#if defined(_WIN32)
#include "d3t/dx11_renderer.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>

namespace shenmue::d3t {

void CollectSymbols() {
    const HANDLE process = GetCurrentProcess();
    SymInitialize(process, nullptr, TRUE);
    void *backTrace[16];
    const std::int16_t frameCount = static_cast<std::int16_t>(RtlCaptureStackBackTrace(1, 16, backTrace, nullptr) - 3);
    SYMBOL_INFO *const symbol = static_cast<SYMBOL_INFO *>(std::calloc(sizeof(SYMBOL_INFO) + 256, 1));
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for (std::int16_t frame = 0; frame < frameCount; ++frame)
        SymFromAddr(process, reinterpret_cast<DWORD64>(backTrace[frame]), nullptr, symbol);
    std::free(symbol);
}

std::int32_t CrashReporter(_EXCEPTION_POINTERS *exception) {
    CollectSymbols();
    if (IsDebuggerPresent())
        return EXCEPTION_CONTINUE_SEARCH;
    char dumpPath[261] = "Unknown";
    char executablePath[261]{};
    if (static_cast<std::int32_t>(GetModuleFileNameA(nullptr, executablePath, sizeof(executablePath))) >= 0)
        _splitpath_s(executablePath, nullptr, 0, nullptr, 0, dumpPath, sizeof(dumpPath), nullptr, 0);
    strcat_s(dumpPath, sizeof(dumpPath), ".dmp");
    const HANDLE dumpFile = CreateFileA(dumpPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, 0, nullptr);
    MINIDUMP_EXCEPTION_INFORMATION information{.ThreadId = GetCurrentThreadId(), .ExceptionPointers = exception, .ClientPointers = TRUE};
    const BOOL dumped = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, MiniDumpWithDataSegs, &information, nullptr, nullptr);
    char screenshotPath[260];
    strcpy_s(screenshotPath, sizeof(screenshotPath), dumpPath);
    std::memcpy(screenshotPath + std::strlen(dumpPath) - 4, ".png", 4);
    MapAndTakeScreenshot(screenshotPath);
    char commandLine[512];
    sprintf_s(commandLine, sizeof(commandLine), "%s %s", executablePath, dumpPath);
    STARTUPINFOA startup{.cb = sizeof(STARTUPINFOA)};
    FILE *reporter = nullptr;
    fopen_s(&reporter, "CrashReporter.exe", "rb");
    if (dumped) {
        PROCESS_INFORMATION process{};
        if (CreateProcessA("CrashReporter.exe", commandLine, nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startup, &process)) {
            WaitForSingleObject(process.hProcess, INFINITE);
            CloseHandle(process.hProcess);
            CloseHandle(process.hThread);
        }
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

} // namespace shenmue::d3t

#endif
