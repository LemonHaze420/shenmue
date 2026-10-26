#include "hangon_abi.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>

namespace {

inline constexpr wchar_t kWindowClass[] = L"HangOnStandalone";
inline constexpr wchar_t kWindowTitle[] = L"Hang-On | Enter: start/pause | Arrows/WASD: drive | Space: pause | Esc: quit";

struct AudioSlot {
    WAVEHDR header{};
    char samples[hangon::kAudioBytes]{};
};

struct Host {
    HMODULE library{};
    hangon::Update update{};
    hangon::Destroy destroy{};
    void* arena{};
    void* session{};
    hangon::SharedState shared{};
    hangon::Config config{};
    hangon::UpdateBlock block{};
    HWAVEOUT audio{};
    HANDLE audioEvent{};
    AudioSlot audioSlots[hangon::kAudioBuffers]{};
    unsigned audioIndex{};
    HWND window{};
    const void* frame{};
    bool keys[256]{};
    bool startRequested{};
    bool paused{};
    bool running{true};
    int exitCode{};
} g_host;

void ReportError(const char *format, ...) {
    char message[1024]{};
    va_list arguments;
    va_start(arguments, format);
    std::vsnprintf(message, sizeof(message), format, arguments);
    va_end(arguments);
    MessageBoxA(g_host.window, message, "Shenmue Hang-On", MB_OK | MB_ICONERROR);
}

void AudioError(const char* operation, MMRESULT result) {
    char text[MAXERRORLENGTH]{};
    waveOutGetErrorTextA(result, text, MAXERRORLENGTH);
    ReportError("%s: %s (%u)\n", operation, text, result);
    g_host.exitCode = 1;
    g_host.running = false;
}

void SetPaused(bool paused) {
    if (g_host.paused == paused) return;
    g_host.paused = paused;
    g_host.startRequested = false;
    if (paused && g_host.audio) {
        const MMRESULT result = waveOutReset(g_host.audio);
        if (result != MMSYSERR_NOERROR) AudioError("waveOutReset", result);
    }
    SetWindowTextW(g_host.window, paused ? L"Hang-On [paused] | Enter/Space: resume | Esc: quit" : kWindowTitle);
}

void PaintFrame(HWND window) {
    PAINTSTRUCT paint{};
    const HDC dc = BeginPaint(window, &paint);
    RECT client{};
    GetClientRect(window, &client);
    const HBRUSH black = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    const int widthScale = client.right / static_cast<int>(hangon::kPictureWidth);
    const int heightScale = client.bottom / static_cast<int>(hangon::kPictureHeight);
    const int scale = widthScale < heightScale ? widthScale : heightScale;
    if (g_host.frame && scale > 0) {
        const int width = scale * hangon::kPictureWidth;
        const int height = scale * hangon::kPictureHeight;
        const int x = (client.right - width) / 2;
        const int y = (client.bottom - height) / 2;
        const RECT borders[4] = {
            {.left = 0, .top = 0, .right = client.right, .bottom = y},
            {.left = 0, .top = y + height, .right = client.right, .bottom = client.bottom},
            {.left = 0, .top = y, .right = x, .bottom = y + height},
            {.left = x + width, .top = y, .right = client.right, .bottom = y + height},
        };
        for (const RECT& border : borders) {
            if (border.right > border.left && border.bottom > border.top) FillRect(dc, &border, black);
        }
        BITMAPINFO info{};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = hangon::kFrameWidth;
        info.bmiHeader.biHeight = -static_cast<LONG>(hangon::kFrameHeight);
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        info.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(dc, COLORONCOLOR);
        const int result = StretchDIBits(dc, x, y, width, height,
            hangon::kPictureX, hangon::kPictureY, hangon::kPictureWidth, hangon::kPictureHeight,
            g_host.frame, &info, DIB_RGB_COLORS, SRCCOPY);
        if (result == GDI_ERROR || result == 0) {
            ReportError("StretchDIBits failed (%lu)\n", GetLastError());
            g_host.exitCode = 1;
            g_host.running = false;
        }
    } else {
        FillRect(dc, &client, black);
    }
    EndPaint(window, &paint);
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_PAINT:
        PaintFrame(window);
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_GETMINMAXINFO: {
        RECT minimum{0, 0, static_cast<LONG>(hangon::kPictureWidth), static_cast<LONG>(hangon::kPictureHeight)};
        AdjustWindowRect(&minimum, WS_OVERLAPPEDWINDOW, FALSE);
        MINMAXINFO* const size = reinterpret_cast<MINMAXINFO*>(lparam);
        size->ptMinTrackSize = {minimum.right - minimum.left, minimum.bottom - minimum.top};
        return 0;
    }
    case WM_SIZE:
        InvalidateRect(window, nullptr, FALSE);
        return 0;
    case WM_ACTIVATEAPP:
        if (!wparam) {
            std::memset(g_host.keys, 0, sizeof(g_host.keys));
            SetPaused(true);
        }
        return 0;
    case WM_KEYDOWN:
        if (wparam < 256) g_host.keys[wparam] = true;
        if ((lparam & (1LL << 30)) != 0) return 0;
        if (wparam == VK_ESCAPE) {
            DestroyWindow(window);
        } else if (wparam == VK_SPACE || (wparam == VK_RETURN && (g_host.paused || g_host.shared.gameInProgress))) {
            SetPaused(!g_host.paused);
        } else if (wparam == VK_RETURN) {
            g_host.startRequested = true;
        }
        return 0;
    case WM_KEYUP:
        if (wparam < 256) g_host.keys[wparam] = false;
        return 0;
    case WM_CLOSE:
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        g_host.running = false;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(window, message, wparam, lparam);
    }
}

bool OpenAudio() {
    g_host.audioEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (!g_host.audioEvent) {
        ReportError("CreateEvent failed (%lu)\n", GetLastError());
        return false;
    }
    WAVEFORMATEX format{};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = hangon::kSampleRate;
    format.wBitsPerSample = 16;
    format.nBlockAlign = 4;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    MMRESULT result = waveOutOpen(&g_host.audio, WAVE_MAPPER, &format, reinterpret_cast<DWORD_PTR>(g_host.audioEvent), 0, CALLBACK_EVENT);
    if (result != MMSYSERR_NOERROR) {
        AudioError("waveOutOpen", result);
        return false;
    }
    for (AudioSlot& slot : g_host.audioSlots) {
        slot.header.lpData = slot.samples;
        slot.header.dwBufferLength = hangon::kAudioBytes;
        result = waveOutPrepareHeader(g_host.audio, &slot.header, sizeof(WAVEHDR));
        if (result != MMSYSERR_NOERROR) {
            AudioError("waveOutPrepareHeader", result);
            return false;
        }
    }
    return true;
}

bool Run() {
    LARGE_INTEGER frequency{}, now{};
    if (!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&now)) {
        ReportError("Performance counter unavailable\n");
        return false;
    }
    LONGLONG deadline = now.QuadPart;
    LONGLONG remainder = 0;
    const LONGLONG period = frequency.QuadPart / hangon::kTickRate;
    while (g_host.running) {
        MSG message{};
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) g_host.running = false;
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
        if (!g_host.running) break;
        QueryPerformanceCounter(&now);
        if (g_host.paused) {
            deadline = now.QuadPart;
            remainder = 0;
            MsgWaitForMultipleObjects(0, nullptr, FALSE, INFINITE, QS_ALLINPUT);
            continue;
        }
        AudioSlot& slot = g_host.audioSlots[g_host.audioIndex];
        if (now.QuadPart < deadline || (slot.header.dwFlags & WHDR_INQUEUE) != 0) {
            const LONGLONG remaining = deadline > now.QuadPart ? deadline - now.QuadPart : period;
            const DWORD milliseconds = static_cast<DWORD>((remaining * 1000 + frequency.QuadPart - 1) / frequency.QuadPart);
            const DWORD result = MsgWaitForMultipleObjects(1, &g_host.audioEvent, FALSE, milliseconds, QS_ALLINPUT);
            if (result == WAIT_FAILED) {
                ReportError("Frame wait failed (%lu)\n", GetLastError());
                return false;
            }
            continue;
        }
        g_host.block.startSignal = g_host.startRequested ? 2 : 0;
        g_host.startRequested = false;
        const bool right = g_host.keys[VK_RIGHT] || g_host.keys['D'];
        const bool left = g_host.keys[VK_LEFT] || g_host.keys['A'];
        g_host.block.steering = right ? 0 : left ? 255 : 127;
        g_host.block.accelerator = g_host.keys[VK_UP] || g_host.keys['W'] ? 255 : 0;
        g_host.block.brake = g_host.keys[VK_DOWN] || g_host.keys['S'] ? 255 : 0;
        g_host.block.audioData = slot.samples;
        g_host.block.audioBytes = hangon::kAudioBytes;
        g_host.block.audioPlayLength = hangon::kSampleFrames;
        g_host.update(g_host.session, &g_host.block);
        if (g_host.block.frameBytes != hangon::kFrameWidth * hangon::kFrameHeight * 4 || !g_host.block.frame ||
            g_host.block.audioData != slot.samples || g_host.block.audioBytes != hangon::kAudioBytes) {
            ReportError("DLL returned an incompatible frame/audio block\n");
            return false;
        }
        g_host.frame = g_host.block.frame;
        const MMRESULT result = waveOutWrite(g_host.audio, &slot.header, sizeof(WAVEHDR));
        if (result != MMSYSERR_NOERROR) {
            AudioError("waveOutWrite", result);
            return false;
        }
        g_host.audioIndex = (g_host.audioIndex + 1) % hangon::kAudioBuffers;
        InvalidateRect(g_host.window, nullptr, FALSE);
        UpdateWindow(g_host.window);
        if (g_host.shared.exitRequested) g_host.running = false;
        deadline += period;
        remainder += frequency.QuadPart % hangon::kTickRate;
        if (remainder >= hangon::kTickRate) {
            ++deadline;
            remainder -= hangon::kTickRate;
        }
        QueryPerformanceCounter(&now);
        if (now.QuadPart - deadline > period) deadline = now.QuadPart;
    }
    return g_host.exitCode == 0;
}

} // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    wchar_t directory[MAX_PATH]{}, dllPath[MAX_PATH]{};
    const DWORD length = GetCurrentDirectoryW(MAX_PATH, directory);
    if (!length || length >= MAX_PATH || swprintf_s(dllPath, L"%s\\%s", directory, hangon::kDllName) < 0) {
        ReportError("Game directory path is invalid or too long");
        return 1;
    }
    g_host.library = LoadLibraryExW(dllPath, nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!g_host.library) {
        ReportError("Cannot load GameHangon_Winx64_Release.dll (Win32 error %lu). Run from your installed game directory.", GetLastError());
        return 1;
    }
    const hangon::ComputeMemoryRequired compute = reinterpret_cast<hangon::ComputeMemoryRequired>(
        GetProcAddress(g_host.library, "HangOn_ComputeMemoryRequired"));
    const hangon::Create create = reinterpret_cast<hangon::Create>(GetProcAddress(g_host.library, "HangOn_Create"));
    g_host.update = reinterpret_cast<hangon::Update>(GetProcAddress(g_host.library, "HangOn_Update"));
    g_host.destroy = reinterpret_cast<hangon::Destroy>(GetProcAddress(g_host.library, "HangOn_Destroy"));
    int exitCode = 1;
    do {
        if (!compute || !create || !g_host.update || !g_host.destroy) {
            ReportError("DLL does not export the four required HangOn functions\n");
            break;
        }
        g_host.shared.credits = -1;
        std::memcpy(g_host.shared.scores, hangon::kDefaultRanking, sizeof(g_host.shared.scores));
        g_host.config = {.mode = 2, .field04 = 0, .shared = &g_host.shared};
        const std::size_t size = compute(&g_host.config);
        if (size) g_host.arena = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!g_host.arena) {
            ReportError("Cannot allocate %zu-byte DLL arena (%lu)\n", size, GetLastError());
            break;
        }
        g_host.session = create(&g_host.config, g_host.arena);
        if (!g_host.session) {
            ReportError("HangOn_Create failed\n");
            break;
        }
        if (!OpenAudio()) break;
        const HINSTANCE instance = GetModuleHandleW(nullptr);
        WNDCLASSW windowClass{};
        windowClass.lpfnWndProc = WindowProc;
        windowClass.hInstance = instance;
        windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        windowClass.lpszClassName = kWindowClass;
        if (!RegisterClassW(&windowClass)) {
            ReportError("RegisterClass failed (%lu)\n", GetLastError());
            break;
        }
        RECT rectangle{0, 0, 3 * hangon::kPictureWidth, 3 * hangon::kPictureHeight};
        AdjustWindowRect(&rectangle, WS_OVERLAPPEDWINDOW, FALSE);
        g_host.window = CreateWindowExW(0, kWindowClass, kWindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, nullptr, nullptr, instance, nullptr);
        if (!g_host.window) {
            ReportError("CreateWindow failed (%lu)\n", GetLastError());
            break;
        }
        ShowWindow(g_host.window, SW_SHOW);
        exitCode = Run() ? 0 : 1;
    } while (false);

    if (g_host.audio) {
        const MMRESULT reset = waveOutReset(g_host.audio);
        if (reset != MMSYSERR_NOERROR) {
            AudioError("waveOutReset during shutdown", reset);
            exitCode = 1;
        }
        for (AudioSlot& slot : g_host.audioSlots) {
            if ((slot.header.dwFlags & WHDR_PREPARED) == 0) continue;
            const MMRESULT result = waveOutUnprepareHeader(g_host.audio, &slot.header, sizeof(WAVEHDR));
            if (result != MMSYSERR_NOERROR) {
                AudioError("waveOutUnprepareHeader", result);
                exitCode = 1;
            }
        }
        const MMRESULT result = waveOutClose(g_host.audio);
        if (result != MMSYSERR_NOERROR) {
            AudioError("waveOutClose", result);
            exitCode = 1;
        }
    }
    if (g_host.window && IsWindow(g_host.window)) DestroyWindow(g_host.window);
    if (g_host.session) g_host.destroy(g_host.session);
    if (g_host.arena) VirtualFree(g_host.arena, 0, MEM_RELEASE);
    if (g_host.audioEvent) CloseHandle(g_host.audioEvent);
    FreeLibrary(g_host.library);
    return exitCode;
}
