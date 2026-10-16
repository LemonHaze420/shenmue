#pragma once

#include <cstdint>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi.h>
#endif

namespace shenmue::s1 {

extern bool g_shouldResetWindow;
extern bool g_shouldQuit;
extern bool g_hasPresentedGameFrame;
extern std::int32_t g_isInputEnabled;
extern std::int32_t g_pendingClientRectUpdate;

#if defined(_WIN32)
extern HWND g_windowHandle;
extern IDXGISwapChain *g_swapChain;

void UpdateClientRectScreenPos(HWND window);
void HandleWindowMessages(HWND window, unsigned int message, std::uint64_t wParam, std::int64_t lParam);
LRESULT CALLBACK MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
void ResetWindow();
void CreateSMWindow();
#endif

} // namespace shenmue::s1
