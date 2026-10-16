#include "s2/d3t_renderer.hpp"
#include "s2/camera.hpp"

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/texture.hpp"
#include "d3t/content.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/object_store.hpp"
#include "d3t/threads.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/input_bindings.hpp"
#include "s2/ui.hpp"
#include "s2/screen_film.hpp"
#include "s2/display_configuration.hpp"
#include "s2/save.hpp"
#include "s2/audio_data.hpp"
#include "s2/conversation.hpp"
#include "s2/scene_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <string>

#include <cJSON.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <dwmapi.h>
#endif

namespace shenmue::s2 {

std::int32_t g_aspectRatioRenderSuppressed = 0;
std::int32_t renQteDoorMotionPending = 0;
float g_s2CurrentGamma;
float g_s2CurrentContrast;
bool g_s2FxaaEnabled;
bool g_s2PostEffectsRequested;
bool g_s2NeedsLoadTextureResources;
std::int32_t g_PPFX_Enabled = 1;
std::uint32_t g_backBufferCaptureTexture2D = 0;

namespace {


// @verified - p
void CopyOrResolveTexture(d3t::D3TDeviceContext *context, std::uint32_t sourceTexture, std::uint32_t destinationTexture) {
#if defined(_WIN32)
    d3t::D3TTextureBackendView source{};
    d3t::D3TTextureBackendView destination{};
    d3t::Texture2DDescription sourceDescription{};
    d3t::Texture2DDescription destinationDescription{};
    if (!context || !d3t::GetTexture2DBackendView(sourceTexture, &source) || !d3t::GetTexture2DBackendView(destinationTexture, &destination) ||
        !d3t::GetTexture2DDescription(sourceTexture, &sourceDescription) || !d3t::GetTexture2DDescription(destinationTexture, &destinationDescription))
        return;
    ID3D11DeviceContext *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    ID3D11Texture2D *const sourceResource = static_cast<ID3D11Texture2D *>(source.texture);
    ID3D11Texture2D *const destinationResource = static_cast<ID3D11Texture2D *>(destination.texture);
    if (sourceDescription.sampleCount == destinationDescription.sampleCount)
        native->CopyResource(destinationResource, sourceResource);
    else if (destinationDescription.sampleCount == 1)
        native->ResolveSubresource(
            destinationResource, 0, sourceResource, 0, static_cast<DXGI_FORMAT>(d3t::GetD3TNativeTextureFormat(destinationDescription.format)));
#else
    static_cast<void>(context);
    static_cast<void>(sourceTexture);
    static_cast<void>(destinationTexture);
#endif
}

} // namespace

// @verified - p
void DX_CreateTexture2D(
    std::uint32_t width, std::uint32_t height, std::uint32_t mipLevels, std::uint32_t format, std::uint32_t *texture, std::uint32_t bindFlags) {
    const d3t::Texture2DDescription description{
        .width = width, .height = height, .arraySize = 1, .mipLevels = mipLevels, .format = format, .bindFlags = bindFlags, .sampleCount = 1};
    *texture = d3t::CreateTexture2D(&description);
}

// @verified - p
bool ReleaseTexture2DHandle(const std::uint32_t *texture) {
    if (!texture || !*texture)
        return false;
    d3t::InitTexture2DResource(*texture);
    return true;
}


// @verified - p
void CaptureBackbuffer_SetupImmutableDXTex2D() {
#if defined(_WIN32)
    d3t::Texture2DDescription backBuffer{};
    if (!d3t::GetTexture2DDescription(d3t::CreateNewTexture2D(), &backBuffer))
        return;
    const d3t::Texture2DDescription description{
        .width = backBuffer.width, .height = backBuffer.height, .arraySize = 1, .mipLevels = 1, .format = backBuffer.format, .sampleCount = 1};
    g_backBufferCaptureTexture2D = d3t::CreateTexture2D(&description);
#endif
}



// @verified - p
void AllocateAndCopyTexture() {
#if defined(_WIN32)
    CopyOrResolveTexture(d3t::DX11::g_ppD3DDeviceContext, d3t::CreateNewTexture2D(), g_backBufferCaptureTexture2D);
#endif
}

// @verified - p
void CopyBackBufferCaptureTexture(std::uint32_t destinationTexture) {
    CopyOrResolveTexture(d3t::DX11::g_ppD3DDeviceContext, g_backBufferCaptureTexture2D, destinationTexture);
}

namespace {

bool resetWindowPending;
bool presentAt60Hz;
std::int32_t scaleForScreen = -1;
std::uint32_t gameConfigurationFlags{0x63U};
constexpr std::array<std::uint8_t, 8> dialogueModeFlagTable{1, 0, 1, 0, 1, 1, 0, 1};

void ApplyDialogueModeFlags(std::uint8_t mode) noexcept {
    const std::size_t tableIndex = static_cast<std::size_t>(mode) * 2U;
    gameConfigurationFlags = (gameConfigurationFlags & ~0x30U) | (static_cast<std::uint32_t>(mode) << 4U);
    if (dialogueModeFlagTable[tableIndex])
        gameConfigurationFlags |= 2U;
    else
        gameConfigurationFlags &= ~2U;
    if (dialogueModeFlagTable[tableIndex + 1U])
        gameConfigurationFlags |= 1U;
    else
        gameConfigurationFlags &= ~1U;
}

void ReadRendererUserSettings() {
    std::ifstream stream{S2Save_GetDirectory() / "Settings", std::ios::binary | std::ios::ate};
    if (!stream)
        return;
    const std::streamoff size = stream.tellg();
    if (size < 0)
        return;
    std::string text(static_cast<std::size_t>(size), '\0');
    stream.seekg(0);
    if (!stream.read(text.data(), size))
        return;
    cJSON *const settings = cJSON_Parse(text.c_str());
    if (!settings)
        return;
    S2UserConfiguration280 &configuration = GetDraftUserConfiguration();
    cJSON *value = cJSON_GetObjectItem(settings, "Version");
    if (value)
        configuration.version = value->valueint;
    cJSON *const controls = cJSON_GetObjectItem(settings, "Controls");
    if (controls) {
        value = cJSON_GetObjectItem(controls, "SwapTriggers");
        if (value)
            configuration.swapTriggers = value->valueint == 1;
        value = cJSON_GetObjectItem(controls, "InvertLookY");
        if (value)
            configuration.invertLookY = value->valueint == 1;
        ReadInputBindingSettings(controls);
    }
    cJSON *const audio = cJSON_GetObjectItem(settings, "Audio");
    if (audio) {
        value = cJSON_GetObjectItem(audio, "AudioDevice");
        if (value)
            configuration.audio.device = value->valueint;
        if (!configuration.version)
            ++configuration.audio.device;
        value = cJSON_GetObjectItem(audio, "AudioLanguage");
        if (value)
            configuration.audio.audioLanguage = value->valueint;
        value = cJSON_GetObjectItem(audio, "SubtitleLanguage");
        if (value)
            configuration.audio.subtitleLanguage = value->valueint;
        value = cJSON_GetObjectItem(audio, "DialogueMode");
        if (value)
            configuration.audio.dialogueMode = value->valueint;
        value = cJSON_GetObjectItem(audio, "DialogueVolume");
        if (value)
            configuration.audio.dialogueVolume = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(audio, "SFXVolume");
        if (value)
            configuration.audio.sfxVolume = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(audio, "MusicVolume");
        if (value)
            configuration.audio.musicVolume = static_cast<float>(value->valuedouble);
    }
    cJSON *const graphics = cJSON_GetObjectItem(settings, "Graphics");
    if (graphics) {
        value = cJSON_GetObjectItem(graphics, "WindowMode");
        if (value)
            configuration.windowMode = value->valueint;
        value = cJSON_GetObjectItem(graphics, "DisplayOutput");
        if (value)
            configuration.displayOutput = value->valueint;
        if (!GetS2DisplayOutput(configuration.displayOutput))
            configuration.displayOutput = GetS2PrimaryDisplayIndex();
        cJSON *const width = cJSON_GetObjectItem(graphics, "ResolutionWidth");
        cJSON *const height = cJSON_GetObjectItem(graphics, "ResolutionHeight");
        if (width && height) {
            configuration.resolutionIndex = GetS2DesktopResolutionIndex(configuration.displayOutput);
            const S2DisplayOutput *const output = GetS2DisplayOutput(configuration.displayOutput);
            if (output)
                for (std::uint32_t index = 0; index < output->modeCount; ++index) {
                    if (output->modes[index].width == static_cast<std::uint32_t>(width->valueint) &&
                        output->modes[index].height == static_cast<std::uint32_t>(height->valueint)) {
                        configuration.resolutionIndex = static_cast<std::int32_t>(index);
                        break;
                    }
                }
        }
        value = cJSON_GetObjectItem(graphics, "ResolutionScale");
        if (value)
            configuration.resolutionScale = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(graphics, "SuperSample");
        if (value)
            configuration.superSampling = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(graphics, "FXAA");
        if (value)
            configuration.fxaa = value->valueint == 1;
        value = cJSON_GetObjectItem(graphics, "BloomMode");
        if (value)
            configuration.bloomMode = value->valueint;
        value = cJSON_GetObjectItem(graphics, "AspectRatioMode");
        if (value)
            configuration.aspectRatio = value->valueint;
        value = cJSON_GetObjectItem(graphics, "Gamma");
        if (value)
            configuration.gamma = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(graphics, "Contrast");
        if (value)
            configuration.contrast = static_cast<float>(value->valuedouble);
        value = cJSON_GetObjectItem(graphics, "UISafeArea");
        if (value)
            configuration.uiSafeArea = static_cast<float>(value->valuedouble);
    }
    ApplyDraftUserConfiguration();
    cJSON_Delete(settings);
}

} // namespace

void ApplyS2UserConfigurationChanges(const S2UserConfiguration280 &previous) {
    const S2UserConfiguration280 &current = GetRuntimeUserConfiguration();
    if (current.windowMode != previous.windowMode || current.displayOutput != previous.displayOutput || current.resolutionIndex != previous.resolutionIndex)
        resetWindowPending = true;
    if (std::fabs(current.resolutionScale - previous.resolutionScale) >= 0.0099999998f ||
        std::fabs(current.superSampling - previous.superSampling) >= 0.0099999998f)
        g_s2NeedsLoadTextureResources = true;
    g_s2CurrentContrast = current.contrast;
    g_s2CurrentGamma = (current.gamma - 0.5f) + 1.0f;
    if (current.contrast < 0.5f)
        g_s2CurrentContrast = 0.5f - ((0.5f - current.contrast) / 1.5f);
    g_s2FxaaEnabled = current.fxaa != 0;
    g_s2PostEffectsRequested = current.bloomMode != 0;
    d3t::SelectResolvedSceneFrame(current.enableSuperSampling != 0);
    if (current.audio.device != previous.audio.device)
        QueueS2AudioOutputChange(current.audio.device);
    ApplyInputBindings();
    ApplyS2AudioVolumeSettings();
    SetConversationAudioLanguage(current.audio.audioLanguage);
    ApplyS2SubtitleLanguage(GetLoadingScreenTextType(current.audio.subtitleLanguage));
    ApplyDialogueModeFlags(static_cast<std::uint8_t>(current.audio.dialogueMode));
}

bool AreS2TriggersSwapped() noexcept {
    return GetRuntimeUserConfiguration().swapTriggers != 0;
}

const S2AudioSettings &GetS2AudioSettings() noexcept {
    return GetRuntimeUserConfiguration().audio;
}
std::int32_t GetS2DialogueMode() noexcept {
    return GetRuntimeUserConfiguration().audio.dialogueMode;
}

std::uint32_t GetS2GameConfigurationFlags() noexcept {
    return gameConfigurationFlags;
}

void SetS2GameConfigurationFlags(std::uint32_t flags) noexcept {
    gameConfigurationFlags = flags;
}

std::int32_t GetS2ControlMode() noexcept {
    return static_cast<std::int32_t>((gameConfigurationFlags >> 8U) & 1U);
}

void ApplyS2DialogueMode(std::int32_t mode) noexcept {
    ApplyDialogueModeFlags(static_cast<std::uint8_t>(mode));
}

void ApplyS2ControlMode(std::int32_t mode) noexcept {
    gameConfigurationFlags = (gameConfigurationFlags & ~0x100U) | (mode == 1 ? 0x100U : 0U);
}

void ApplyS2TriggerSwapMode(std::int32_t swapped) noexcept {
    gameConfigurationFlags = (gameConfigurationFlags & ~0x40U) | (swapped == 0 ? 0x40U : 0U);
}

bool GetVoiceSettingEnabled() noexcept {
    const std::int32_t mode = GetRuntimeUserConfiguration().audio.dialogueMode;
    return mode != 0 && mode != 1;
}

// @verified - p
float GetS2UserInterfaceSafeArea() noexcept {
    const float safeArea = GetRuntimeUserConfiguration().uiSafeArea;
    return (1.0f - safeArea) * 0.80000001f + safeArea;
}

// @verified - p
void shenSetForcedFilmbars(std::int32_t enabled) noexcept {
    g_aspectRatioRenderSuppressed = enabled;
}

bool ShouldScaleS2AspectRatio() noexcept {
    return scaleForScreen != 0;
}

// @verified - p
bool GetAspectRatioModeShouldScale() noexcept {
    return renQteDoorMotionPending != 0 ||
        (g_aspectRatioRenderSuppressed == 0 && (GetRuntimeUserConfiguration().aspectRatio == 0 || g_cameraFilmBarsEnabled != 0));
}

// @verified - p
void ApplyS2AspectRatio() noexcept {
    scaleForScreen = GetAspectRatioModeShouldScale();
    SetNuklearAspectRatio(scaleForScreen);
}

// @verified - p
void ApplyUnscaledS2AspectRatio() noexcept {
    scaleForScreen = 0;
    SetNuklearAspectRatio(0);
}

float GetS2AspectRatioAdjustment() noexcept {
    return 1.3333334f / (static_cast<float>(d3t::g_originalScreenWidth) / static_cast<float>(d3t::g_originalScreenHeight));
}

#if defined(_WIN32)
namespace {

HWND windowHandle = nullptr;
bool shouldQuit = false;
bool inputEnabled = true;
bool validationHiddenWindow;

void ResetS2Window() {
    const S2UserConfiguration280 &configuration = GetRuntimeUserConfiguration();
    const S2DisplayOutput *output = GetS2DisplayOutput(configuration.displayOutput);
    if (!output)
        output = GetS2DisplayOutput(GetS2PrimaryDisplayIndex());
    const d3t::DisplayModeInfo *resolution = GetS2DisplayResolution(configuration.displayOutput, configuration.resolutionIndex);
    if (!resolution)
        resolution = GetS2DisplayResolution(configuration.displayOutput, GetS2DesktopResolutionIndex(configuration.displayOutput));
    if (!resolution)
        resolution = GetS2DisplayResolution(GetS2PrimaryDisplayIndex(), GetS2DesktopResolutionIndex(configuration.displayOutput));
    if (!windowHandle || !output || !resolution)
        return;
    IDXGISwapChain *const swapChain = static_cast<IDXGISwapChain *>(d3t::GetNativeSwapChain());
    if (swapChain)
        swapChain->SetFullscreenState(FALSE, nullptr);
    const LONG oldStyle = GetWindowLongA(windowHandle, GWL_STYLE);
    LONG left = output->left;
    LONG top = output->top;
    RECT rectangle{};
    LONG style = oldStyle;
    if (configuration.windowMode == 0) {
        rectangle.right = resolution->width;
        rectangle.bottom = resolution->height;
        style = (oldStyle & 0x7f35ffff) | 0x00ca0000;
    } else if (configuration.windowMode == 1 || configuration.windowMode == 2) {
        rectangle.right = output->right - output->left;
        rectangle.bottom = output->bottom - output->top;
        style = (oldStyle & 0xff30ffff) | 0x80000000;
    }
    if (configuration.windowMode >= 0 && configuration.windowMode <= 2) {
        AdjustWindowRect(&rectangle, static_cast<DWORD>(style), FALSE);
        SetWindowLongA(windowHandle, GWL_STYLE, style);
        SetWindowPos(windowHandle, nullptr, left, top, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, 0x230);
        if (!configuration.windowMode) {
            RECT bounds{};
            if (SUCCEEDED(DwmGetWindowAttribute(windowHandle, DWMWA_EXTENDED_FRAME_BOUNDS, &bounds, sizeof(bounds)))) {
                left = 2 * left - bounds.left;
                top = 2 * top - bounds.top;
            }
            SetWindowPos(windowHandle, nullptr, left, top, rectangle.right - rectangle.left, rectangle.bottom - rectangle.top, 0x230);
        }
    }
    ShowWindow(windowHandle, validationHiddenWindow ? SW_HIDE : SW_SHOW);
    if (swapChain)
        swapChain->SetFullscreenState(configuration.windowMode == 2 && !validationHiddenWindow, nullptr);
    resetWindowPending = false;
    d3t::DirectXUpdateWindowDimensions(resolution->width, resolution->height);
    if (g_backBufferCaptureTexture2D) {
        d3t::InitTexture2DResource(g_backBufferCaptureTexture2D);
        CaptureBackbuffer_SetupImmutableDXTex2D();
    }
    InitializeWindowInput(resolution->width, resolution->height);
    InitGeneralTextureResources();
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    HandleWindowInputMessage(window, message, wParam, lParam);
    NuklearHandleWindowMessages(window, message, wParam, lParam);
    switch (message) {
    case WM_ACTIVATE:
        inputEnabled = LOWORD(wParam) != WA_INACTIVE;
        break;
    case WM_ACTIVATEAPP:
        inputEnabled = wParam != 0;
        break;
    case WM_SETFOCUS:
        inputEnabled = true;
        break;
    case WM_KILLFOCUS:
        inputEnabled = false;
        break;
    default:
        break;
    }
    if (!inputEnabled && (message == WM_ACTIVATE || message == WM_ACTIVATEAPP || message == WM_KILLFOCUS))
        ClearWindowInput();
    if (message == WM_KEYDOWN && wParam == VK_RETURN) {
        NotifyUIEnterPressed();
        return 0;
    }
    if (message == WM_CLOSE || message == WM_DESTROY) {
        shouldQuit = true;
        if (message == WM_CLOSE)
            DestroyWindow(window);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

} // namespace
#endif

float g_uiClientWidth = 0.0f, g_uiClientHeight = 0.0f, g_uiWindowWidth = 0.0f, g_uiWindowHeight = 0.0f;
float g_nuklearMessageX = 0.0f, g_nuklearMessageY = 0.0f;

// @verified - p
void UpdateMouseForNuklear() {
    if (!g_NuklearInitialized)
        return;
    nk_input &input = GNuklearWindow.input;
    input.mouse.prev = input.mouse.pos;
    for (nk_mouse_button &button : input.mouse.buttons)
        button.clicked = 0;
    input.keyboard.text_len = 0;
    input.mouse.delta = {};
    input.mouse.scroll_delta = 0.0f;
    for (nk_key &key : input.keyboard.keys)
        key.clicked = 0;
}

#if defined(_WIN32)
// @verified - p
std::int64_t NuklearHandleWindowMessages(void *window, std::uint32_t message, std::uint64_t wParam, std::int64_t lParam) {
    if (!g_NuklearInitialized)
        return 0;
    const float buffer = static_cast<float>(g_uiScreenWidth);
    const float aspect =
        static_cast<float>(static_cast<std::int32_t>(d3t::g_originalScreenHeight)) / static_cast<float>(static_cast<std::int32_t>(d3t::g_originalScreenWidth));
    const float x = ((static_cast<float>(static_cast<std::int16_t>(lParam)) / g_uiClientWidth) * buffer) / ((1.0f / (1080.0f / buffer)) * aspect) -
        g_currentUIViewportRect.x;
    const float y = (static_cast<float>(static_cast<std::int16_t>(lParam >> 16)) / g_uiClientHeight) * 1080.0f - g_currentUIViewportRect.y;
    g_nuklearMessageX = x;
    g_nuklearMessageY = y;
    nk_input &input = GNuklearWindow.input;
    const auto button = [&](nk_buttons id, int down) {
        nk_mouse_button &target = input.mouse.buttons[id];
        if (target.down != down) {
            ++target.clicked;
            target.down = down;
            target.clicked_pos = nk_vec2(x, y);
        }
        if (down)
            SetCapture(static_cast<HWND>(window));
        else
            ReleaseCapture();
        return std::int64_t{1};
    };
    const auto key = [&](nk_keys id) {
        nk_key &target = input.keyboard.keys[id];
        ++target.clicked;
        target.down = static_cast<int>((lParam >> 31) & 1);
        return std::int64_t{1};
    };
    switch (message) {
    case WM_MOUSEMOVE:
        input.mouse.pos = nk_vec2(x, y);
        input.mouse.delta = nk_vec2(x - input.mouse.prev.x, y - input.mouse.prev.y);
        return 1;
    case WM_LBUTTONDOWN:
        return button(NK_BUTTON_LEFT, 1);
    case WM_LBUTTONUP:
        return button(NK_BUTTON_LEFT, 0);
    case WM_RBUTTONDOWN:
        return button(NK_BUTTON_RIGHT, 1);
    case WM_RBUTTONUP:
        return button(NK_BUTTON_RIGHT, 0);
    case WM_MBUTTONDOWN:
        return button(NK_BUTTON_MIDDLE, 1);
    case WM_MBUTTONUP:
        return button(NK_BUTTON_MIDDLE, 0);
    case WM_MOUSEWHEEL:
        input.mouse.scroll_delta += static_cast<float>(static_cast<std::int16_t>(wParam >> 16)) / 120.0f;
        return 1;
    case WM_CHAR:
        if (wParam < 32)
            return 0;
        nk_input_unicode(&GNuklearWindow, static_cast<nk_rune>(wParam));
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
#endif

// @verified - p
std::int32_t CreateDXTextureForNuklearBufferForScreen() {
    std::int32_t result = 0;
#if defined(_WIN32)
    RECT rectangle{};
    if (GetClientRect(windowHandle, &rectangle)) {
        g_uiClientWidth = static_cast<float>(rectangle.right - rectangle.left);
        g_uiClientHeight = static_cast<float>(rectangle.bottom - rectangle.top);
    }
    result = GetWindowRect(windowHandle, &rectangle);
    if (result) {
        result = rectangle.bottom - rectangle.top;
        g_uiWindowWidth = static_cast<float>(rectangle.right - rectangle.left);
        g_uiWindowHeight = static_cast<float>(result);
    }
#endif
    if (g_NuklearInitialized && g_uiClientWidth > static_cast<float>(g_uiScreenWidth)) {
        g_uiScreenWidth = static_cast<std::uint32_t>(g_uiClientWidth);
        return static_cast<std::int32_t>(InitNuklearScreenTexture());
    }
    return result;
}

bool IsD3TWindowInputEnabled() noexcept {
#if defined(_WIN32)
    return inputEnabled;
#else

    return true;
#endif
}

bool InitializeD3TRenderer(std::uint32_t width, std::uint32_t height, bool hidden) {
    EnumerateS2Displays();
    InitializeInputBindings();
    ReadRendererUserSettings();
    SnapshotUserConfiguration();
#if !defined(_WIN32)
    static_cast<void>(width);
    static_cast<void>(height);
    static_cast<void>(hidden);
    d3t::CreateTemp2x2Textures();
    return true;
#else
    InitializeWindowInput(width, height);
    RAWINPUTDEVICE rawKeyboard{.usUsagePage = 1, .usUsage = 6, .dwFlags = 0, .hwndTarget = nullptr};
    RegisterRawInputDevices(&rawKeyboard, 1, sizeof(rawKeyboard));
    const HINSTANCE instance = GetModuleHandleW(nullptr);
    constexpr wchar_t className[] = L"Shenmue2RecoveredWindow";
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.lpszClassName = className;
    if (!RegisterClassExW(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        return false;
    }

    RECT rectangle{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
    constexpr DWORD style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&rectangle, style, FALSE);
    windowHandle = CreateWindowExW(0, className, L"Shenmue II", style, CW_USEDEFAULT, CW_USEDEFAULT, rectangle.right - rectangle.left,
        rectangle.bottom - rectangle.top, nullptr, nullptr, instance, nullptr);
    if (!windowHandle)
        return false;
    inputEnabled = true;

    const d3t::DisplayModeInfo displayMode{width, height, 60, 1, {}};
    d3t::DX11_D3D11CreateDeviceAndSwapChain(&displayMode, windowHandle);
    if (!d3t::GetNativeSwapChain() || !d3t::g_D3DDevice) {
        ShutdownD3TRenderer();
        return false;
    }
    d3t::CreateTemp2x2Textures();
    CaptureBackbuffer_SetupImmutableDXTex2D();

    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    ShowWindow(windowHandle, hidden ? SW_HIDE : SW_SHOW);
    validationHiddenWindow = hidden;
    resetWindowPending = false;
    UpdateWindow(windowHandle);
    return true;
#endif
}

bool BeginD3TRenderPass(bool clearTargets) {
#if defined(_WIN32)


    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    if (!d3t::DX11::g_ppD3DDeviceContext)
        return false;

    if (clearTargets ? d3t::BeginUIFrame() : d3t::BindSceneFrameRenderTarget()) {
        return true;
    }
    ExecuteCurrentCommandList();
#else
    static_cast<void>(clearTargets);
#endif
    return false;
}

void ExecuteCurrentCommandList() {
#if defined(_WIN32)
    auto *&context = d3t::DX11::g_ppD3DDeviceContext;
    if (context) {
        d3t::DX11::WaitForCommandListFinish(context);
        context = nullptr;
    }
#endif
}

bool ProcessD3TMessages() {
    PublishS2AudioOutputAcknowledgement();
#if !defined(_WIN32)
    g_frameGlareIntensity = g_configuredGlareIntensity;
    return false;
#else
    MSG message{};
    UpdateMouseForNuklear();
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT)
            shouldQuit = true;
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    if (g_NuklearInitialized) {
        nk_mouse &mouse = GNuklearWindow.input.mouse;
        mouse.grab = 0;
        if (mouse.ungrab) {
            mouse.grabbed = 0;
            mouse.ungrab = 0;
        }
    }
    if (!inputEnabled && windowHandle && GetForegroundWindow() == windowHandle) {
        inputEnabled = true;
    }
    d3t::ProcessRemappings();
    d3t::ExecuteADynamicFunctionOnce();
    d3t::TexSortWrapper(GetFrameTime());
    if (g_s2NeedsLoadTextureResources) {
        InitGeneralTextureResources();
        g_s2NeedsLoadTextureResources = false;
    }
    g_frameGlareIntensity = g_configuredGlareIntensity;
    if (shouldQuit)
        return false;
    d3t::GenerateMipsAndUpdateResourcesForDirectX();
    return BeginD3TRenderPass(true);
#endif
}

// @verified - p
void SetPPFXEnabled(std::int32_t enabled) noexcept {
    g_PPFX_Enabled = enabled;
}

// @verified - p
void Set60FPSForCredits() {
    presentAt60Hz = true;
}

void PresentD3TFrame(std::int32_t renderMode) {
    d3t::ResetHashedModelCounts();
    ExecuteCurrentCommandList();
    UpdateMipMaps();
    if (presentAt60Hz) {
        renderMode = 2;
        presentAt60Hz = false;
    }
#if defined(_WIN32)
    d3t::PresentFrame(renderMode);
    if (resetWindowPending)
        ResetS2Window();
#else
    static_cast<void>(renderMode);
#endif
    HLib::ProcessTaskPostUpdateCallbacks();
    (void)ProfileFramePerformance();
}

void ShutdownD3TRenderer() {
#if defined(_WIN32)
    ExecuteCurrentCommandList();
    d3t::ShutdownDX11();
    if (windowHandle) {
        DestroyWindow(windowHandle);
        windowHandle = nullptr;
        inputEnabled = false;
    }
#endif
    ReleaseS2Displays();
}

} // namespace shenmue::s2
