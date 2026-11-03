#include "s1/boot.hpp"

#if defined(SHENMUE_TARGET_WHATS_PC)
#pragma region WhatsShenmue
#include "s1/launch_options.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(_WIN32) && defined(_MSC_VER)
int wmain(int argc, wchar_t **argv) {
#else
int main(int argc, char **argv) {
#endif
    std::error_code error;
    std::filesystem::path gameDirectory;
#if defined(SHENMUE_WS_STANDALONE)
#pragma region StandaloneWS
    if (argc == 1) {
        wchar_t executable[32768]{};
        const DWORD length = GetModuleFileNameW(nullptr, executable, static_cast<DWORD>(sizeof(executable) / sizeof(*executable)));
        if (!length || length >= sizeof(executable) / sizeof(*executable)) {
            std::fputs("Cannot locate the executable directory.\n", stderr);
            return EXIT_FAILURE;
        }
        gameDirectory = std::filesystem::path(executable).parent_path();
    } else
#pragma endregion
#endif
    {
        const char *const wsRoot = std::getenv("SHEN_WS_ROOT");
        if (argc == 1 && wsRoot && *wsRoot)
            return shenmue::s1::Shenmue_Main(0, nullptr);
        if (argc != 2) {
            std::fputs("Pass the WS data directory as the only argument, or drag its folder onto this executable.\n", stderr);
            return EXIT_FAILURE;
        }
        gameDirectory = std::filesystem::absolute(argv[1], error);
    }
    if (error || !std::filesystem::is_regular_file(gameDirectory / "sprite" / "TNTITLE.SPR", error)) {
        std::fputs("The WS data directory must contain sprite/TNTITLE.SPR.\n", stderr);
        return EXIT_FAILURE;
    }

#if !defined(SHENMUE_WS_STANDALONE)
#if defined(_WIN32)
    wchar_t executable[32768]{};
    const DWORD length = GetModuleFileNameW(nullptr, executable, static_cast<DWORD>(sizeof(executable) / sizeof(*executable)));
    if (!length || length >= sizeof(executable) / sizeof(*executable)) {
        std::fputs("Cannot locate the executable directory.\n", stderr);
        return EXIT_FAILURE;
    }
    const std::filesystem::path hostDirectory = std::filesystem::path(executable).parent_path();
#else
    const std::filesystem::path hostDirectory = std::filesystem::canonical("/proc/self/exe", error).parent_path();
#endif
    if (error || !std::filesystem::is_directory(hostDirectory / "archives" / "dx11", error)) {
        std::fputs("Place this executable in the Shenmue I sm1 directory.\n", stderr);
        return EXIT_FAILURE;
    }
#endif
    std::filesystem::current_path(gameDirectory, error);
    if (error) {
        std::fputs("Cannot open the WS data directory.\n", stderr);
        return EXIT_FAILURE;
    }
#if defined(SHENMUE_WS_STANDALONE)
#pragma region StandaloneWS
    shenmue::s1::g_trialLaunchOptions = {gameDirectory, {}};
#pragma endregion
#else
    shenmue::s1::g_trialLaunchOptions = {gameDirectory, hostDirectory};
#endif
    return shenmue::s1::Shenmue_Main(0, nullptr);
}
#pragma endregion
#else
int main() {
    return shenmue::s1::Shenmue_Main(0, nullptr);
}
#endif
