#include "d3t/steam.hpp"
#if SHENMUE_IS_S1
#include "s1/boot.hpp"
#else
#include "s2/initialization.hpp"
#endif

#include "d3t/threads.hpp"
#include "common/archive_data.hpp"

#include <atomic>
#include <cstring>
#include <cwchar>
#include <dxgi.h>
#include <exception>
#include <windows.h>

namespace shenmue::d3t {
namespace {

std::byte *executableBase;
HMODULE gameModule;
DWORD steamThread;
bool steamInitialized;
std::atomic_flag initializationAttempted = ATOMIC_FLAG_INIT;
using ContextInit = void *(*)(void *);
ContextInit contextInit;
void **originalSteamContext;
void (*runCallbacks)();

template<class T> T &OriginalGlobal(std::uint32_t rva) {
    return *reinterpret_cast<T *>(executableBase + rva);
}

bool QuitPending(HWND window) {
    MSG message{};
    if (OriginalGlobal<std::uint8_t>(ExeInfo.quitRequested) || PeekMessageW(&message, nullptr, WM_QUIT, WM_QUIT, PM_NOREMOVE))
        return true;
    if (PeekMessageW(&message, window, WM_CLOSE, WM_CLOSE, PM_NOREMOVE))
        return true;
    if (PeekMessageW(&message, window, WM_SYSCOMMAND, WM_SYSCOMMAND, PM_NOREMOVE))
        return message.message == WM_QUIT || (message.wParam & 0xfff0U) == SC_CLOSE;
    return false;
}

void ShutdownOriginalRenderer() {
    HWND window = OriginalGlobal<HWND>(ExeInfo.window);
    if (!window || GetWindowThreadProcessId(window, nullptr) != GetCurrentThreadId() || QuitPending(window))
        ExitOriginalExecutable(0);
    reinterpret_cast<void (*)()>(executableBase + ExeInfo.drainWorkers)();
    reinterpret_cast<void (*)()>(executableBase + ExeInfo.joinWorkers)();
    IDXGISwapChain *const swapChain = OriginalGlobal<IDXGISwapChain *>(ExeInfo.swapChain);
    if (!swapChain)
        throw std::exception{};
    BOOL fullscreen = TRUE;
    HRESULT result = swapChain->SetFullscreenState(FALSE, nullptr);
    if (result == S_OK)
        result = swapChain->GetFullscreenState(&fullscreen, nullptr);
    if (result != S_OK || fullscreen)
        throw std::exception{};
    KillTimer(window, 50);
    MSG message{};
    while (PeekMessageW(&message, window, WM_TIMER, WM_TIMER, PM_REMOVE)) {
        if (message.message == WM_QUIT)
            ExitOriginalExecutable(static_cast<int>(message.wParam));
    }
    if (GetCapture() == window)
        ReleaseCapture();
    ClipCursor(nullptr);
    if (QuitPending(window))
        ExitOriginalExecutable(0);
    if (!DestroyWindow(window))
        throw std::exception{};
    const BOOL hasQuitMessage = PeekMessageW(&message, nullptr, WM_QUIT, WM_QUIT, PM_NOREMOVE);
    const std::uint8_t closeRequested = OriginalGlobal<std::uint8_t>(ExeInfo.quitRequested);
    if (closeRequested)
        ExitOriginalExecutable(0);
    if (hasQuitMessage) {
        if (message.wParam != 0)
            ExitOriginalExecutable(static_cast<int>(message.wParam));
        if (PeekMessageW(&message, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE) && message.wParam != 0)
            ExitOriginalExecutable(static_cast<int>(message.wParam));
    }
    if (OriginalGlobal<std::uint8_t>(ExeInfo.quitRequested))
        ExitOriginalExecutable(0);
}

[[noreturn]] void RunGame() {
    int result = 1;
    try {
        reinterpret_cast<void (*)()>(executableBase + ExeInfo.initializeController)();
        HMODULE const steam = GetModuleHandleW(L"steam_api64.dll");
        if (!steam)
            throw std::exception{};
        contextInit = reinterpret_cast<ContextInit>(GetProcAddress(steam, "SteamInternal_ContextInit"));
        runCallbacks = reinterpret_cast<void (*)()>(GetProcAddress(steam, "SteamAPI_RunCallbacks"));
        if (!contextInit || !runCallbacks)
            throw std::exception{};
        steamThread = GetCurrentThreadId();
        originalSteamContext = static_cast<void **>(contextInit(executableBase + ExeInfo.contextDescriptor));
        void **const context = originalSteamContext;
        if (!context || !context[1] || !context[5] || !context[9] ||
            !OriginalGlobal<void *>(ExeInfo.platformOwner) || !OriginalGlobal<std::uint8_t>(ExeInfo.callbacksEnabled) ||
            !OriginalSteamStats() || !OriginalSteamAchievements())
            throw std::exception{};
        const std::int32_t expectedStats = SHENMUE_IS_S1 ? 29 : 28;
        if (OriginalGlobal<std::int32_t>(ExeInfo.statCount) != expectedStats ||
            OriginalGlobal<std::int32_t>(ExeInfo.achievementCount) != expectedStats - 1)
            throw std::exception{};
        HRSRC const resource = FindResourceW(gameModule, MAKEINTRESOURCEW(101), MAKEINTRESOURCEW(10));
        HGLOBAL const resourceData = resource ? LoadResource(gameModule, resource) : nullptr;
        const char *const names = resourceData ? static_cast<const char *>(LockResource(resourceData)) : nullptr;
        const DWORD nameSize = resource ? SizeofResource(gameModule, resource) : 0;
        if (!names || !nameSize)
            throw std::exception{};
        archive_data::ArchiveData_SetEmbeddedNameDatabase(names, nameSize);
        steamInitialized = true;
        ShutdownOriginalRenderer();
        if (!SetCurrentDirectoryW(GetExecutableDirectory().c_str()))
            throw std::exception{};
#if SHENMUE_IS_S1
        result = s1::Shenmue_Main(0, nullptr);
#else
        result = s2::Shenmue2_Main(0, nullptr);
#endif
    } catch (...) {
    }
    try {
        ShutdownSteamStorage();
        ShutdownThreads();
    } catch (...) {
        result = 1;
    }
    ExitOriginalExecutable(result);
}

bool HookSteamInitialization() {
    SYSTEM_INFO system{};
    GetSystemInfo(&system);
    const std::uintptr_t site = reinterpret_cast<std::uintptr_t>(executableBase + ExeInfo.controllerCall);
    const std::uintptr_t aligned = site & ~(static_cast<std::uintptr_t>(system.dwAllocationGranularity) - 1);
    void *relay = nullptr;
    for (std::uintptr_t distance = system.dwAllocationGranularity; distance < 0x7fff0000ULL && !relay; distance += system.dwAllocationGranularity) {
        if (aligned > distance)
            relay = VirtualAlloc(reinterpret_cast<void *>(aligned - distance), system.dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!relay)
            relay = VirtualAlloc(reinterpret_cast<void *>(aligned + distance), system.dwPageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    }
    if (!relay)
        return false;
    std::uint8_t jump[14]{0xff, 0x25};
    const std::uintptr_t destination = reinterpret_cast<std::uintptr_t>(&RunGame);
    std::memcpy(jump + 6, &destination, sizeof(destination));
    std::memcpy(relay, jump, sizeof(jump));
    DWORD oldProtection{};
    if (!VirtualProtect(relay, system.dwPageSize, PAGE_EXECUTE_READ, &oldProtection)) {
        VirtualFree(relay, 0, MEM_RELEASE);
        return false;
    }
    FlushInstructionCache(GetCurrentProcess(), relay, sizeof(jump));
    const std::int64_t relative = reinterpret_cast<std::intptr_t>(relay) - static_cast<std::intptr_t>(site + 5);
    if (relative < INT32_MIN || relative > INT32_MAX || !VirtualProtect(reinterpret_cast<void *>(site), 5, PAGE_EXECUTE_READWRITE, &oldProtection)) {
        VirtualFree(relay, 0, MEM_RELEASE);
        return false;
    }
    const std::int32_t displacement = static_cast<std::int32_t>(relative);
    std::uint8_t call[5]{0xe8};
    std::memcpy(call + 1, &displacement, sizeof(displacement));
    std::memcpy(reinterpret_cast<void *>(site), call, sizeof(call));
    DWORD ignored{};
    VirtualProtect(reinterpret_cast<void *>(site), 5, oldProtection, &ignored);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void *>(site), sizeof(call));
    return true;
}

} // namespace

bool InstallSteam() noexcept {
    if (initializationAttempted.test_and_set())
        return false;
    executableBase = reinterpret_cast<std::byte *>(GetModuleHandleW(nullptr));
    wchar_t path[MAX_PATH]{};
    const DWORD length = GetModuleFileNameW(nullptr, path, MAX_PATH);
    const wchar_t *const separator = std::wcsrchr(path, L'\\');
    if (!length || length >= MAX_PATH || _wcsicmp(separator ? separator + 1 : path, ExeInfo.executable))
        return false;
    const IMAGE_DOS_HEADER *const dos = reinterpret_cast<const IMAGE_DOS_HEADER *>(executableBase);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew < sizeof(IMAGE_DOS_HEADER) || dos->e_lfanew > 0x10000)
        return false;
    const IMAGE_NT_HEADERS64 *const pe = reinterpret_cast<const IMAGE_NT_HEADERS64 *>(executableBase + dos->e_lfanew);
    if (pe->Signature != IMAGE_NT_SIGNATURE || pe->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 ||
        pe->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC || pe->FileHeader.TimeDateStamp != ExeInfo.timestamp ||
        pe->OptionalHeader.SizeOfImage < ExeInfo.minimumImageSize)
        return false;
    const std::byte *const site = executableBase + ExeInfo.controllerCall;
    if (std::memcmp(site, ExeInfo.controllerCallBytes.data(), ExeInfo.controllerCallBytes.size()) ||
        std::memcmp(executableBase + ExeInfo.initializeController, ExeInfo.initializeControllerBytes.data(),
            ExeInfo.initializeControllerBytes.size()) || OriginalGlobal<void *>(ExeInfo.platformOwner))
        return false;
    std::int32_t displacement{};
    std::memcpy(&displacement, site + 1, sizeof(displacement));
    if (site + 5 + displacement != executableBase + ExeInfo.initializeController)
        return false;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
            reinterpret_cast<LPCWSTR>(&RunGame), &gameModule))
        return false;
    return HookSteamInitialization();
}

bool IsSteamInitialized() noexcept { return steamInitialized; }

void RequireSteamInitialized() {
    if (!steamInitialized)
        throw std::exception{};
}

std::filesystem::path GetExecutableDirectory() {
    wchar_t path[32768]{};
    const DWORD length = GetModuleFileNameW(nullptr, path, static_cast<DWORD>(std::size(path)));
    if (!length || length >= std::size(path))
        throw std::exception{};
    return std::filesystem::path(path).parent_path();
}

[[noreturn]] void ExitOriginalExecutable(int status) {
    reinterpret_cast<void (*)(int)>(executableBase + ExeInfo.exitProcess)(status);
#if defined(_MSC_VER)
    __assume(0);
#else
    __builtin_unreachable();
#endif
}

void *OriginalSteamStats() { return OriginalGlobal<void *>(ExeInfo.stats); }
void *OriginalSteamAchievements() { return OriginalGlobal<void *>(ExeInfo.achievements); }
void *OriginalSteamCapsuleStatistics() { return ExeInfo.capsuleStatistics ? OriginalGlobal<void *>(ExeInfo.capsuleStatistics) : nullptr; }
std::int32_t OriginalSteamCapsuleCount() { return ExeInfo.capsuleCount ? OriginalGlobal<std::int32_t>(ExeInfo.capsuleCount) : 0; }

void **OriginalSteamContext() {
    RequireSteamInitialized();
    return originalSteamContext;
}

bool IsSteamThread() { return steamThread == GetCurrentThreadId(); }
void RunOriginalSteamCallbacks() { if (steamInitialized && IsSteamThread()) runCallbacks(); }
void *OriginalExecutableFunction(std::uint32_t rva) { return executableBase + rva; }

std::int64_t PollSteamInput(std::uint32_t mode, void *output) {
    RequireSteamInitialized();
    std::memset(output, 0, sizeof(SteamControllerInput));
    if (mode >= 9 || OriginalGlobal<std::int32_t>(ExeInfo.inputReady) != 1)
        return 0;
    void *const controller = OriginalSteamContext()[12];
    void **const vtable = *static_cast<void ***>(controller);
    std::uint64_t controllers[16]{};
    reinterpret_cast<int (*)(void *, std::uint64_t *)>(vtable[3])(controller, controllers);
    using Handler = std::int64_t (*)(void *, const std::uint64_t *);
    Handler *const handlers = reinterpret_cast<Handler *>(executableBase + ExeInfo.inputHandlers);
    Handler &current = OriginalGlobal<Handler>(ExeInfo.currentInputHandler);
    const std::uint64_t actionSet = reinterpret_cast<std::uint64_t *>(executableBase + ExeInfo.actionSets)[mode];
    if (current != handlers[mode] && actionSet) {
        for (unsigned index = 0; index < 16 && controllers[index]; ++index)
            reinterpret_cast<void (*)(void *, std::uint64_t, std::uint64_t)>(vtable[6])(controller, controllers[index], actionSet);
        current = handlers[mode];
    }
    reinterpret_cast<void (*)(void *)>(vtable[2])(controller);
    return current(output, controllers);
}

#if SHENMUE_IS_S1
std::int32_t OriginalSteamGameInputLayout() {
    RequireSteamInitialized();
    void *const platform = OriginalGlobal<void *>(ExeInfo.platformOwner);
    return reinterpret_cast<std::int32_t (*)(void *)>((*static_cast<void ***>(platform))[13])(platform);
}
#endif

std::uint64_t GetSteamUserId() {
    void *const user = OriginalSteamContext()[1];
    std::uint64_t id{};
    reinterpret_cast<void *(*)(void *, std::uint64_t *)>((*static_cast<void ***>(user))[2])(user, &id);
    return id;
}

void RefreshSteamContext() {
    RequireSteamInitialized();
    if (contextInit(executableBase + ExeInfo.contextDescriptor) != originalSteamContext)
        throw std::exception{};
}

bool IndicateSteamAchievementProgress(const char *name, std::uint32_t current, std::uint32_t maximum) {
    void *const stats = OriginalSteamContext()[5];
    return reinterpret_cast<bool (*)(void *, const char *, std::uint32_t, std::uint32_t)>(
        (*static_cast<void ***>(stats))[13])(stats, name, current, maximum);
}

void ReadSteamStatisticsFile(std::int32_t slot) {
    RequireSteamInitialized();
    if (slot < 0 || slot > 10) throw std::exception{};
#if SHENMUE_IS_S1
    reinterpret_cast<void (*)(int)>(executableBase + 0x246b40)(slot);
#else
    reinterpret_cast<void (*)(int)>(executableBase + 0x14980)(slot);
#endif
}

void WriteSteamStatisticsFile(std::int32_t slot) {
    RequireSteamInitialized();
    if (slot < 0 || slot > 10) throw std::exception{};
#if SHENMUE_IS_S1
    reinterpret_cast<void (*)(int)>(executableBase + 0x246e90)(slot);
#else
    reinterpret_cast<void (*)(int)>(executableBase + 0x14c60)(slot);
#endif
}

void PublishSteamStats() {
    RequireSteamInitialized();
#if SHENMUE_IS_S1
    reinterpret_cast<void (*)()>(executableBase + 0x247250)();
#else
    reinterpret_cast<void (*)()>(executableBase + 0x14e70)();
#endif
}

void UpdateSteamStat(std::int32_t stat) {
    RequireSteamInitialized();
    if (stat < 0 || stat >= OriginalGlobal<std::int32_t>(ExeInfo.statCount))
        throw std::exception{};
#if SHENMUE_IS_S1
    reinterpret_cast<void (*)(int)>(executableBase + 0x247bd0)(stat);
#else
    reinterpret_cast<void (*)(int)>(executableBase + 0x17a10)(stat);
#endif
}

void UpdateSteamAchievement(std::int32_t stat) {
    RequireSteamInitialized();
#if SHENMUE_IS_S1
    reinterpret_cast<void (*)(int)>(executableBase + 0x6d280)(stat);
#else
    reinterpret_cast<void (*)(int)>(executableBase + 0x425ce0)(stat);
#endif
}

#if SHENMUE_IS_S2
void IncrementSteamStat(std::int32_t stat) {
    RequireSteamInitialized();
    if (stat < 0 || stat >= OriginalGlobal<std::int32_t>(ExeInfo.statCount))
        throw std::exception{};
    reinterpret_cast<void (*)(int)>(executableBase + 0x425d90)(stat);
}

void ResetSteamStats() {
    RequireSteamInitialized();
    reinterpret_cast<void (*)()>(executableBase + 0x17930)();
}

void CheckSteamStats() {
    RequireSteamInitialized();
    reinterpret_cast<void (*)()>(executableBase + 0x425de0)();
}
#endif

} // namespace shenmue::d3t
