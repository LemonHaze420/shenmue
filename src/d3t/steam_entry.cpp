#include "d3t/steam.hpp"
#include <windows.h>

extern "C" __declspec(dllexport) void InitializeASI() {
    shenmue::d3t::InstallSteam();
}

BOOL WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        shenmue::d3t::InstallSteam();
    return TRUE;
}
