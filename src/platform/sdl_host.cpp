#include "platform/host.hpp"

#include <SDL.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>

namespace shenmue::host {
namespace {
SDL_Window *window;
std::array<SDL_GameController *, ControllerCount> controllers{};
InputSnapshot state{.width = 1280, .height = 720, .focused = true};
std::int32_t enterPresses;
bool quit, initialized;

std::int32_t VirtualKey(const SDL_Keysym &key) {
    const SDL_Scancode code = key.scancode;
    if (code >= SDL_SCANCODE_A && code <= SDL_SCANCODE_Z)
        return 'A' + code - SDL_SCANCODE_A;
    if (code >= SDL_SCANCODE_1 && code <= SDL_SCANCODE_9)
        return '1' + code - SDL_SCANCODE_1;
    if (code >= SDL_SCANCODE_F1 && code <= SDL_SCANCODE_F12)
        return 112 + code - SDL_SCANCODE_F1;
    if (code >= SDL_SCANCODE_KP_1 && code <= SDL_SCANCODE_KP_9)
        return 97 + code - SDL_SCANCODE_KP_1;
    switch (code) {
    case SDL_SCANCODE_0: return '0';
    case SDL_SCANCODE_BACKSPACE: return 8;
    case SDL_SCANCODE_TAB: return 9;
    case SDL_SCANCODE_RETURN: case SDL_SCANCODE_KP_ENTER: return 13;
    case SDL_SCANCODE_ESCAPE: return 27;
    case SDL_SCANCODE_SPACE: return 32;
    case SDL_SCANCODE_PAGEUP: return 33;
    case SDL_SCANCODE_PAGEDOWN: return 34;
    case SDL_SCANCODE_END: return 35;
    case SDL_SCANCODE_HOME: return 36;
    case SDL_SCANCODE_LEFT: return 37;
    case SDL_SCANCODE_UP: return 38;
    case SDL_SCANCODE_RIGHT: return 39;
    case SDL_SCANCODE_DOWN: return 40;
    case SDL_SCANCODE_INSERT: return 45;
    case SDL_SCANCODE_DELETE: return 46;
    case SDL_SCANCODE_KP_0: return 96;
    case SDL_SCANCODE_KP_MULTIPLY: return 106;
    case SDL_SCANCODE_KP_PLUS: return 107;
    case SDL_SCANCODE_KP_MINUS: return 109;
    case SDL_SCANCODE_KP_PERIOD: return 110;
    case SDL_SCANCODE_KP_DIVIDE: return 111;
    case SDL_SCANCODE_CAPSLOCK: return 20;
    case SDL_SCANCODE_NUMLOCKCLEAR: return 144;
    case SDL_SCANCODE_SCROLLLOCK: return 145;
    case SDL_SCANCODE_LSHIFT: return 160;
    case SDL_SCANCODE_RSHIFT: return 161;
    case SDL_SCANCODE_LCTRL: return 162;
    case SDL_SCANCODE_RCTRL: return 163;
    case SDL_SCANCODE_LALT: return 164;
    case SDL_SCANCODE_RALT: return 165;
    case SDL_SCANCODE_SEMICOLON: return 186;
    case SDL_SCANCODE_EQUALS: return 187;
    case SDL_SCANCODE_COMMA: return 188;
    case SDL_SCANCODE_MINUS: return 189;
    case SDL_SCANCODE_PERIOD: return 190;
    case SDL_SCANCODE_SLASH: return 191;
    case SDL_SCANCODE_GRAVE: return 192;
    case SDL_SCANCODE_LEFTBRACKET: return 219;
    case SDL_SCANCODE_BACKSLASH: return 220;
    case SDL_SCANCODE_RIGHTBRACKET: return 221;
    case SDL_SCANCODE_APOSTROPHE: return 222;
    default: return -1;
    }
}

void OpenControllers() {
    for (int index = 0; index < SDL_NumJoysticks(); ++index) {
        if (!SDL_IsGameController(index))
            continue;
        const SDL_JoystickID id = SDL_JoystickGetDeviceInstanceID(index);
        bool opened = false;
        for (SDL_GameController *controller : controllers)
            opened = opened || (controller && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller)) == id);
        if (opened)
            continue;
        for (SDL_GameController *&controller : controllers) {
            if (!controller) {
                controller = SDL_GameControllerOpen(index);
                break;
            }
        }
    }
}

void SampleController(std::size_t index) {
    Controller &output = state.controllers[index];
    SDL_GameController *&controller = controllers[index];
    if (controller && !SDL_GameControllerGetAttached(controller)) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
    output = {};
    if (!controller)
        return;
    output.connected = true;
    if (!state.focused)
        return;
    constexpr std::uint16_t masks[]{0x1000, 0x2000, 0x4000, 0x8000, 0x20, 0, 0x10, 0x40, 0x80, 0x100, 0x200, 1, 2, 4, 8};
    for (int button = 0; button < SDL_CONTROLLER_BUTTON_MAX && button < static_cast<int>(std::size(masks)); ++button)
        if (SDL_GameControllerGetButton(controller, static_cast<SDL_GameControllerButton>(button)))
            output.buttons |= masks[button];
    const auto axis = [controller](SDL_GameControllerAxis value) { return SDL_GameControllerGetAxis(controller, value); };
    const auto inverted = [](std::int16_t value) { return static_cast<std::int16_t>(std::clamp(-static_cast<int>(value), -32768, 32767)); };
    output.leftX = axis(SDL_CONTROLLER_AXIS_LEFTX);
    output.leftY = inverted(axis(SDL_CONTROLLER_AXIS_LEFTY));
    output.rightX = axis(SDL_CONTROLLER_AXIS_RIGHTX);
    output.rightY = inverted(axis(SDL_CONTROLLER_AXIS_RIGHTY));
    output.leftTrigger = static_cast<std::uint8_t>(std::max(0, static_cast<int>(axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT))) * 255 / 32767);
    output.rightTrigger = static_cast<std::uint8_t>(std::max(0, static_cast<int>(axis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT))) * 255 / 32767);
}
} // namespace

bool Initialize() {
    if (initialized)
        return true;
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
        std::fprintf(stderr, "Cannot initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    initialized = true;
    std::atexit([] {
        for (SDL_GameController *controller : controllers)
            if (controller)
                SDL_GameControllerClose(controller);
        if (window)
            SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);
    });
    SDL_setenv("DXVK_WSI_DRIVER", "SDL2", 0);
    OpenControllers();
    return true;
}

void *CreateWindow(std::int32_t width, std::int32_t height) {
    if (!Initialize())
        return nullptr;
    if (!window)
        window = SDL_CreateWindow("What's Shenmue", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
            SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window)
        std::fprintf(stderr, "Cannot create the game window: %s\n", SDL_GetError());
    else
        SDL_GetWindowSize(window, &state.width, &state.height);
    return window;
}

void ResizeWindow(std::int32_t width, std::int32_t height) {
    if (window)
        SDL_SetWindowSize(window, width, height);
}

void PollInput(InputSnapshot &input, bool consumeEvents) {
    if (consumeEvents) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                const int key = VirtualKey(event.key.keysym);
                if (key >= 0)
                    state.keys[static_cast<std::size_t>(key)] = event.type == SDL_KEYDOWN;
                state.keys[16] = state.keys[160] || state.keys[161];
                state.keys[17] = state.keys[162] || state.keys[163];
                state.keys[18] = state.keys[164] || state.keys[165];
                if (key == 13 && event.type == SDL_KEYDOWN && !event.key.repeat)
                    ++enterPresses;
                state.changed = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                state.mouseX = event.motion.x;
                state.mouseY = event.motion.y;
                state.changed = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
                constexpr std::uint16_t keys[]{0, 256, 258, 257, 259, 260};
                if (event.button.button < std::size(keys))
                    state.keys[keys[event.button.button]] = event.type == SDL_MOUSEBUTTONDOWN;
                if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN)
                    ++state.leftClicks;
                state.changed = true;
            } else if (event.type == SDL_MOUSEWHEEL) {
                state.wheel += static_cast<float>(event.wheel.y);
                state.changed = true;
            } else if (event.type == SDL_CONTROLLERDEVICEADDED) {
                OpenControllers();
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    state.keys.fill(false);
                    state.focused = false;
                    state.changed = true;
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    state.focused = true;
                } else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    quit = true;
                }
            }
        }
        if (window)
            SDL_GetWindowSize(window, &state.width, &state.height);
        for (std::size_t index = 0; index < controllers.size(); ++index)
            SampleController(index);
    }
    input = state;
    if (consumeEvents) {
        state.leftClicks = 0;
        state.wheel = 0;
        state.changed = false;
    }
}

std::int32_t ConsumeEnterPresses() {
    const std::int32_t count = enterPresses;
    enterPresses = 0;
    return count;
}

void ReadController(std::size_t index, Controller &controller) {
    controller = index < state.controllers.size() ? state.controllers[index] : Controller{};
}

void SetStatus(const char *text) {
    if (window)
        SDL_SetWindowTitle(window, text);
}

void RequestQuit() { quit = true; }
bool QuitRequested() { return quit; }

} // namespace shenmue::host
