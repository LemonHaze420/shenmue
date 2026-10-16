#include "s1/window.hpp"

#include "s1/d3t/settings.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/input.hpp"
#include "s1/launch_options.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/render_passes.hpp"
#include "s1/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/ui_nuklear.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_WIN32)
#include <dwmapi.h>
#include <windowsx.h>
#endif

namespace shenmue::s1 {

bool g_shouldResetWindow = false;
bool g_shouldQuit = false;
bool g_hasPresentedGameFrame = false;
std::int32_t g_isInputEnabled = 0;
std::int32_t g_pendingClientRectUpdate = 0;

#if defined(_WIN32)
HWND g_windowHandle = nullptr;
IDXGISwapChain *g_swapChain = nullptr;

namespace {

constexpr std::array<DWORD, 3> kWindowStyles{0x00ca0000, 0x80000000, 0x80000000};
HWND g_inputWindow = nullptr;

MonitorInfo *SelectRuntimeMonitor() {
    if (static_cast<std::uint32_t>(g_currentSettings.display.displayOutput) < g_monitorCount) {
        return &g_monitors[g_currentSettings.display.displayOutput];
    }
    std::uint32_t index = 0;
    while (index < g_monitorCount && !g_monitors[index].active) {
        ++index;
    }
    if (index >= g_monitorCount) {
        index = 0;
    }
    return index < g_monitorCount ? &g_monitors[index] : nullptr;
}

void HandleRawKeyboard(HRAWINPUT rawInputHandle) {
    UINT size = 0;
    GetRawInputData(rawInputHandle, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
    RAWINPUT input{};
    size = sizeof(input);
    if (GetRawInputData(rawInputHandle, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1) || input.header.dwType != RIM_TYPEKEYBOARD) {
        return;
    }

    const RAWKEYBOARD &keyboard = input.data.keyboard;
    std::uint32_t key = keyboard.VKey;
    if (key == 0xff) {
        return;
    }
    if ((keyboard.Flags & RI_KEY_E0) != 0) {
        if (key == VK_CONTROL) {
            key = VK_RCONTROL;
        } else if (key == VK_MENU) {
            key = VK_RMENU;
        }
    } else {
        switch (key) {
        case VK_CLEAR:
            key = VK_NUMPAD5;
            break;
        case VK_CONTROL:
            key = VK_LCONTROL;
            break;
        case VK_MENU:
            key = VK_LMENU;
            break;
        case VK_PRIOR:
            key = VK_NUMPAD9;
            break;
        case VK_NEXT:
            key = VK_NUMPAD3;
            break;
        case VK_END:
            key = VK_NUMPAD1;
            break;
        case VK_HOME:
            key = VK_NUMPAD7;
            break;
        case VK_LEFT:
            key = VK_NUMPAD4;
            break;
        case VK_UP:
            key = VK_NUMPAD8;
            break;
        case VK_RIGHT:
            key = VK_NUMPAD6;
            break;
        case VK_DOWN:
            key = VK_NUMPAD2;
            break;
        case VK_INSERT:
            key = VK_NUMPAD0;
            break;
        default:
            break;
        }
    }

    if (keyboard.Message == WM_KEYDOWN) {
        if (key == VK_F12) {
            d3t_DumpFramePng("s1_frame_dump.png");
        }
        SetKeyPressedState(key);
        g_windowInput.customInputHandler = 1;
    } else if (keyboard.Message == WM_KEYUP &&
        ((keyboard.VKey != VK_SHIFT && static_cast<unsigned int>(keyboard.VKey - VK_LSHIFT) > 1) || keyboard.MakeCode == 0x2a || keyboard.MakeCode == 0x36)) {
        SetKeyReleasedState(key);
        g_windowInput.customInputHandler = 1;
    }
}

} // namespace

std::int64_t NuklearHandleWindowMessages(HWND window, unsigned int message, std::uint64_t wParam, std::int64_t lParam);
void CreateDXTextureForNuklearBufferForScreen();
extern std::int32_t g_NuklearInitialized;


float g_nuklearMessageX = 0.0f;
float g_nuklearMessageY = 0.0f;







std::int64_t NuklearHandleWindowMessages(HWND window, unsigned int message, std::uint64_t wParam, std::int64_t lParam) {
    if (!g_NuklearInitialized)
        return 0;
    const float buffer = static_cast<float>(g_uiScreenWidth);
    const float aspect =
        static_cast<float>(static_cast<std::int32_t>(d3t::g_originalScreenHeight)) / static_cast<float>(static_cast<std::int32_t>(d3t::g_originalScreenWidth));
    const float clientX = static_cast<float>(static_cast<std::int16_t>(lParam)) / static_cast<float>(g_windowInput.width) * buffer;
    const float x = clientX / ((1.0f / (1080.0f / buffer)) * aspect) - g_currentUIViewportRect.x;
    const float y =
        static_cast<float>(static_cast<std::int16_t>(lParam >> 16)) / static_cast<float>(g_windowInput.height) * 1080.0f - g_currentUIViewportRect.y;
    g_nuklearMessageX = x;
    g_nuklearMessageY = y;
    nk_context &context = GNuklearWindow;
    const int pixelX = static_cast<int>(x);
    const int pixelY = static_cast<int>(y);
    const auto key = [&context, lParam](nk_keys id) {
        nk_input_key(&context, id, static_cast<int>((lParam >> 31) & 1));
        return std::int64_t{1};
    };
    switch (message) {
    case WM_MOUSEMOVE:
        nk_input_motion(&context, pixelX, pixelY);
        return 1;
    case WM_LBUTTONDOWN:
        nk_input_button(&context, NK_BUTTON_LEFT, pixelX, pixelY, 1);
        SetCapture(window);
        return 1;
    case WM_LBUTTONUP:
        nk_input_button(&context, NK_BUTTON_LEFT, pixelX, pixelY, 0);
        ReleaseCapture();
        return 1;
    case WM_RBUTTONDOWN:
        nk_input_button(&context, NK_BUTTON_RIGHT, pixelX, pixelY, 1);
        SetCapture(window);
        return 1;
    case WM_RBUTTONUP:
        nk_input_button(&context, NK_BUTTON_RIGHT, pixelX, pixelY, 0);
        ReleaseCapture();
        return 1;
    case WM_MBUTTONDOWN:
        nk_input_button(&context, NK_BUTTON_MIDDLE, pixelX, pixelY, 1);
        SetCapture(window);
        return 1;
    case WM_MBUTTONUP:
        nk_input_button(&context, NK_BUTTON_MIDDLE, pixelX, pixelY, 0);
        ReleaseCapture();
        return 1;
    case WM_MOUSEWHEEL:
        nk_input_scroll(&context, static_cast<float>(static_cast<std::int16_t>(wParam >> 16)) / 120.0f);
        return 1;
    case WM_CHAR:
        if (wParam < 32)
            return 0;
        nk_input_unicode(&context, static_cast<nk_rune>(wParam));
        return 1;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP: {
        const bool control = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        switch (wParam) {
        case VK_BACK:
            return key(NK_KEY_BACKSPACE);
        case VK_TAB:
            return key(NK_KEY_TAB);
        case VK_RETURN:
            return key(NK_KEY_ENTER);
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            return key(NK_KEY_SHIFT);
        case VK_END:
            return key(NK_KEY_TEXT_END);
        case VK_HOME:
            return key(NK_KEY_TEXT_START);
        case VK_LEFT:
            return key(control ? NK_KEY_TEXT_WORD_LEFT : NK_KEY_LEFT);
        case VK_RIGHT:
            return key(control ? NK_KEY_TEXT_WORD_RIGHT : NK_KEY_RIGHT);
        case VK_DELETE:
            return key(NK_KEY_DEL);
        case 'C':
            return control ? key(NK_KEY_COPY) : 0;
        case 'R':
            return control ? key(NK_KEY_TEXT_REDO) : 0;
        case 'V':
            return control ? key(NK_KEY_PASTE) : 0;
        case 'X':
            return control ? key(NK_KEY_CUT) : 0;
        case 'Z':
            return control ? key(NK_KEY_TEXT_UNDO) : 0;
        default:
            return 0;
        }
    }
    default:
        return 0;
    }
}

void CreateDXTextureForNuklearBufferForScreen() {
    RECT rect{};
    if (g_windowHandle && GetClientRect(g_windowHandle, &rect)) {
        g_windowInput.width = rect.right - rect.left;
        g_windowInput.height = rect.bottom - rect.top;
    }
    if (g_NuklearInitialized && g_windowInput.width > static_cast<std::int32_t>(g_uiScreenWidth)) {
        g_uiScreenWidth = static_cast<std::uint32_t>(g_windowInput.width);
        CreateDXTextureForNuklearBuffer();
    }
}

void UpdateClientRectScreenPos(HWND window) {
    RECT rect{};
    GetClientRect(window, &rect);
    POINT topLeft{rect.left, rect.top};
    POINT bottomRight{rect.right, rect.bottom};
    MapWindowPoints(window, nullptr, &topLeft, 1);
    MapWindowPoints(window, nullptr, &bottomRight, 1);
    g_applicationSettings.windowRect = {topLeft.x, topLeft.y, bottomRight.x, bottomRight.y};
}

void HandleWindowMessages(HWND window, unsigned int message, std::uint64_t wParam, std::int64_t lParam) {
    if (g_inputWindow == nullptr && window != nullptr) {
        g_inputWindow = window;
        SetTimer(window, 50, 100, nullptr);
    }

    switch (message) {
    case WM_INPUT:
        HandleRawKeyboard(reinterpret_cast<HRAWINPUT>(lParam));
        break;
    case WM_MOVE:
        g_pendingClientRectUpdate = 1;
        break;
    case WM_TIMER:
        if (wParam == 50 && g_isInputEnabled == 1 && g_windowInput.mouseDelta.x != 0.0f && g_windowInput.mouseDelta.y != 0.0f) {
            std::move_backward(g_applicationSettings.mouseDeltaHistory.begin(), g_applicationSettings.mouseDeltaHistory.end() - 1,
                g_applicationSettings.mouseDeltaHistory.end());
            g_applicationSettings.mouseDeltaHistory[0] = g_windowInput.mouseDelta;
            if (g_currentMouseHistoryIndex < 10) {
                ++g_currentMouseHistoryIndex;
            }
        }
        break;
    case WM_MOUSEMOVE: {
        if (g_isInputEnabled == 1) {
            if (g_pendingClientRectUpdate != 0) {
                UpdateClientRectScreenPos(window);
                g_pendingClientRectUpdate = 0;
            }
            ClipCursor(g_currentSettings.display.windowMode == 2 || g_windowInput.keyStates[257].pressed != 0 ?
                    reinterpret_cast<const RECT *>(&g_applicationSettings.windowRect) :
                    nullptr);
        }
        g_windowInput.currentMouseDelta[0] = g_windowInput.mouseDelta;
        const auto width = (std::max)(g_windowInput.width, 1);
        const auto height = (std::max)(g_windowInput.height, 1);
        const auto x = static_cast<std::int16_t>(lParam & 0xffff);
        const auto y = static_cast<std::int16_t>((lParam >> 16) & 0xffff);
        g_windowInput.mouseDelta.x = 2.0f * (static_cast<float>(x) / static_cast<float>(width) - 0.5f);
        g_windowInput.mouseDelta.y = 2.0f * (static_cast<float>(y) / static_cast<float>(height) - 0.5f);
        break;
    }
    case WM_LBUTTONDOWN:
        SetKeyPressedState(256);
        break;
    case WM_LBUTTONUP:
        SetKeyReleasedState(256);
        break;
    case WM_RBUTTONDOWN:
        SetKeyPressedState(257);
        break;
    case WM_RBUTTONUP:
        SetKeyReleasedState(257);
        break;
    case WM_MBUTTONDOWN:
        SetKeyPressedState(258);
        break;
    case WM_MBUTTONUP:
        SetKeyReleasedState(258);
        break;
    case WM_XBUTTONDOWN:
        if (HIWORD(wParam) == XBUTTON1)
            SetKeyPressedState(259);
        else if (HIWORD(wParam) == XBUTTON2)
            SetKeyPressedState(260);
        break;
    case WM_XBUTTONUP:
        if (HIWORD(wParam) == XBUTTON1)
            SetKeyReleasedState(259);
        else if (HIWORD(wParam) == XBUTTON2)
            SetKeyReleasedState(260);
        break;
    case WM_MOUSEWHEEL:
        g_windowInput.keyStates[258].toggled = std::bit_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(HIWORD(wParam))));
        break;
    default:
        break;
    }
}

LRESULT CALLBACK MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    HandleWindowMessages(window, message, wParam, lParam);
    NuklearHandleWindowMessages(window, message, wParam, lParam);

    if (g_isInputEnabled == 0 && GetForegroundWindow() == window) {
        g_isInputEnabled = 1;
    }
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    if (message == WM_SIZE) {
        return 1;
    }
    if (message == WM_ERASEBKGND) {
        return 1;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        if (!g_hasPresentedGameFrame) {
            FillRect(deviceContext, &paint.rcPaint, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        }
        EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_SETFOCUS) {
        g_isInputEnabled = 1;
    } else if ((message == WM_ACTIVATE && wParam == WA_INACTIVE) || message == WM_KILLFOCUS) {
        g_isInputEnabled = 0;
        ClearWindowInput();
    } else if (message == WM_CLOSE) {
        g_shouldQuit = true;
    } else if ((message == WM_KEYDOWN || message == WM_KEYUP || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP) && wParam == VK_MENU) {
        return 1;
    }
    return DefWindowProcA(window, message, wParam, lParam);
}

void ResetWindow() {
    MonitorInfo *monitor = SelectRuntimeMonitor();
    DisplayModeInfo *resolution = GetScreenResolution();
    MonitorInfo fallbackMonitor{};
    DisplayModeInfo fallbackResolution{
        static_cast<std::uint32_t>((std::max)(1, g_windowInput.width)), static_cast<std::uint32_t>((std::max)(1, g_windowInput.height)), 0, 1, {}};
    if (!monitor) {
        fallbackMonitor.monitorCoordinates.right = GetSystemMetrics(SM_CXSCREEN);
        fallbackMonitor.monitorCoordinates.bottom = GetSystemMetrics(SM_CYSCREEN);
        fallbackMonitor.active = 1;
        monitor = &fallbackMonitor;
    }
    if (!resolution)
        resolution = &fallbackResolution;
    LONG currentStyle = GetWindowLongA(g_windowHandle, GWL_STYLE);
    if (g_swapChain != nullptr) {
        g_swapChain->SetFullscreenState(FALSE, nullptr);
    }

    if (g_currentSettings.display.windowMode == 1 || g_currentSettings.display.windowMode == 2) {
        RECT rect{
            0, 0, monitor->monitorCoordinates.right - monitor->monitorCoordinates.left, monitor->monitorCoordinates.bottom - monitor->monitorCoordinates.top};
        const auto style = (currentStyle & 0xff30ffff) | kWindowStyles[g_currentSettings.display.windowMode];
        AdjustWindowRect(&rect, style, FALSE);
        SetWindowLongA(g_windowHandle, GWL_STYLE, style);
        SetWindowPos(
            g_windowHandle, HWND_TOP, monitor->monitorCoordinates.left, monitor->monitorCoordinates.top, rect.right - rect.left, rect.bottom - rect.top, 0x230);
    } else if (g_currentSettings.display.windowMode == 0) {
        RECT rect{0, 0, static_cast<LONG>(resolution->width), static_cast<LONG>(resolution->height)};
        const auto style = (currentStyle & 0x7f35ffff) | 0x00ca0000;
        AdjustWindowRect(&rect, style, FALSE);
        SetWindowLongA(g_windowHandle, GWL_STYLE, style);
        auto x = monitor->monitorCoordinates.left;
        auto y = monitor->monitorCoordinates.top;
        const auto width = rect.right - rect.left;
        const auto height = rect.bottom - rect.top;
        SetWindowPos(g_windowHandle, HWND_TOP, x, y, width, height, 0x230);
        RECT frame{};
        if (DwmGetWindowAttribute(g_windowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(frame)) == S_OK) {
            x = 2 * x - frame.left;
            y = 2 * y - frame.top;
        }
        SetWindowPos(g_windowHandle, HWND_TOP, x, y, width, height, 0x230);
    }
    ShowWindow(g_windowHandle, g_hideValidationWindow ? SW_HIDE : SW_SHOW);
    if (g_swapChain != nullptr) {
        g_swapChain->SetFullscreenState(g_currentSettings.display.windowMode == 2, nullptr);
    }
    CreateDXTextureForNuklearBufferForScreen();
    g_shouldResetWindow = false;
}

void CreateSMWindow() {
    MonitorInfo *monitor = SelectRuntimeMonitor();
    DisplayModeInfo *resolution = GetScreenResolution();
    MonitorInfo fallbackMonitor{};
    DisplayModeInfo fallbackResolution{
        static_cast<std::uint32_t>((std::max)(1, g_windowInput.width)), static_cast<std::uint32_t>((std::max)(1, g_windowInput.height)), 0, 1, {}};
    if (!monitor) {
        fallbackMonitor.monitorCoordinates.right = GetSystemMetrics(SM_CXSCREEN);
        fallbackMonitor.monitorCoordinates.bottom = GetSystemMetrics(SM_CYSCREEN);
        fallbackMonitor.active = 1;
        monitor = &fallbackMonitor;
    }
    if (!resolution)
        resolution = &fallbackResolution;
    HINSTANCE instance = GetModuleHandleA(nullptr);
    WNDCLASSA windowClass{};
    windowClass.lpfnWndProc = MessageHandler;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "SM";
    RegisterClassA(&windowClass);

    RECT rect{0, 0, 0, 0};
    const DWORD style = kWindowStyles[g_currentSettings.display.windowMode];
    if (g_currentSettings.display.windowMode != 0) {
        rect.right = monitor->monitorCoordinates.right - monitor->monitorCoordinates.left;
        rect.bottom = monitor->monitorCoordinates.bottom - monitor->monitorCoordinates.top;
    } else {
        rect.right = static_cast<LONG>(resolution->width);
        rect.bottom = static_cast<LONG>(resolution->height);
    }
    AdjustWindowRect(&rect, style, FALSE);
    g_windowHandle = CreateWindowExA(0, "SM", "Shenmue I", style, monitor->monitorCoordinates.left, monitor->monitorCoordinates.top, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);
    ShowWindow(g_windowHandle, g_hideValidationWindow ? SW_HIDE : SW_SHOW);
    ResetWindow();
    g_shouldResetWindow = false;
}
#endif

} // namespace shenmue::s1
