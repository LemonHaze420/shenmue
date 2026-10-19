#include "s1/input.hpp"
#include "common/keyboard.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/vfma.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/window.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xinput.h>
#endif

namespace shenmue::s1 {

InputStateStorage g_inputState{{
    InputSlot{1, 0x06fe, 0, 0, 0, 0, 0x8080, 0x00008080, 1},
    InputSlot{},
    InputSlot{},
    InputSlot{},
    InputSlot{},
}};
std::array<ControllerState, 4> g_controllers{};
std::array<ProcessedControllerState, 4> g_processedControllers{};
std::int32_t g_controllerUserIndex = -1;
std::int32_t g_lastXInputError = 0;
std::int32_t g_xInputActive = 0;
std::int32_t g_xInputReserved1 = 0;
std::int32_t g_xInputReservedOverride = 0;
std::int32_t g_forceXInputProcessing = 0;
std::int32_t g_forceXInputAnalogProcessing = 0;
std::int32_t g_forceXInputAnalogRefresh = 0;
std::int32_t g_inputActive = 0;
std::int32_t g_keyboardActive = 1;
std::int32_t g_hasMovedMouse = 0;
std::int32_t g_forceAlternateButtonLayout = 0;
std::int32_t g_steamInputGameLayoutState = 1;
std::int32_t g_debugAllowInputGameLayout = 0;
std::uint16_t g_pendingInputFlags = 0;
std::uint16_t g_forcedHeldInputFlags = 0;
std::uint8_t g_forcedRightTrigger = 0;
std::uint8_t g_forcedLeftTrigger = 0;
std::int16_t g_forcedRightStickY = 0;
std::uint64_t g_inputContextFlags = 0;
AnalogStickData g_analogStickData{};
std::int32_t g_currentMouseHistoryIndex = 0;
extern std::uint8_t g_forceDisplayCursor;

namespace {



struct WindowInputStorage {
    WindowInputState window;
    std::int32_t adjacentMappingState[165];
};

static_assert(offsetof(WindowInputStorage, adjacentMappingState) == 0xdf0);
static_assert(sizeof(WindowInputStorage) == 0x1084);

WindowInputStorage g_windowInputStorage{};
std::int32_t g_startButtonSuppressionFrames = 0;

constexpr std::array<std::int32_t, 55> kMappableKeys{
    KeyBackspace, KeyTab, KeyEnter, KeyShift, KeyControl, KeyAlt, KeyCapsLock, KeySpace, KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, KeyF8, KeyF9,
    KeyF11, KeyF12, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', KeyLeftShift, KeyRightShift, KeyLeftControl, KeyRightControl, KeyLeftAlt,
    KeyRightAlt, KeyOem1, KeyOemPlus, KeyOemComma, KeyOemMinus, KeyOemPeriod, KeyOem2, KeyOem3, KeyOem3, KeyOem5, KeyOem6, KeyOem7, KeyOem8, KeyUp,
    KeyDown, KeyLeft, KeyRight, KeyMouse1, KeyMouse2, KeyMouse3, KeyMouse4};
constexpr std::array<std::int32_t, 3> kNonMappableKeys{KeyOem1, KeyOem3, KeyOem7};

} // namespace


constexpr std::array<std::int32_t, 18> kKeyboardKeys{
    KeyEnter, KeyEscape, '1', '2', 'Z', 'C', KeyF3, KeyUnbound, 'Q', 'E', KeyUp, KeyDown, KeyLeft, KeyRight, 'W', 'S', 'A', 'D'};

namespace {

constexpr std::array<std::uint32_t, 8> kActionButtonFlags{0x1000U, 0x2000U, 0x4000U, 0x8000U, 0x0100U, 0x0200U, 0x0020U, 0x0010U};
constexpr std::array<std::uint32_t, 4> kDirectionButtonFlags{0x0001U, 0x0002U, 0x0004U, 0x0008U};
constexpr std::array<float, 4> kKeyboardAnalogValues{1.0F, -1.0F, -1.0F, 1.0F};
constexpr std::array<std::uint16_t, 9> kPressedPriority{0x0004U, 0x0002U, 0x0400U, 0x0100U, 0x0020U, 0x0010U, 0x0008U, 0x0001U, 0x0200U};

bool KeyPressed(std::int32_t key) {
    return key >= 0 && static_cast<std::size_t>(key) < std::size(g_windowInput.keyStates) && g_windowInput.keyStates[key].pressed != 0;
}

} // namespace

std::int32_t UserMapping(std::uint32_t index) {
    const SavedInputSettings &saved = g_applicationSettings.savedInput;
    if (index < 4)
        return saved.mappings[index];
    index -= 4;
    if (index < 4)
        return saved.functionKeys[index];
    index -= 4;
    if (index < 4)
        return saved.modifierAndVerticalKeys[index];
    index -= 4;
    if (index < 4)
        return saved.arrowAndMovementKeys[index];
    index -= 4;
    if (index < 2)
        return saved.userMappedQteKeys[index];
    index -= 2;
    if (index < 4)
        return saved.qteMappings[index];
    index -= 4;
    if (index < 4)
        return saved.fightMappings[index];
    index -= 4;
    if (index < 4)
        return saved.reservedMappings[index];
    index -= 4;
    if (index < 2)
        return saved.modifierMappings[index];
    index -= 2;
    if (index < 4)
        return saved.vehicleMappings[index];
    index -= 4;
    if (index < 4)
        return saved.reservedVehicleMappings[index];
    index -= 4;
    if (index < 4)
        return saved.vehicleMovementMappings[index];
    index -= 4;
    if (index < 4)
        return saved.reservedVehicleMovementMappings[index];
    index -= 4;
    if (index < 2)
        return saved.vehicleTurnMappings[index];
    return -1;
}

namespace {

std::int32_t ActionMapping(InputMode mode, std::uint32_t action) {
    if (action == 7 && mode != InputMode::Menu && mode != InputMode::GameUi)
        return UserMapping(7);

    switch (mode) {
    case InputMode::Menu:
    case InputMode::GameUi:
        return kKeyboardKeys[action];
    case InputMode::Game:
        return UserMapping(action);
    case InputMode::Fight:
        return action < 4 ? UserMapping(22 + action) : UserMapping(action);
    case InputMode::Qte2:
        return UserMapping(18 + action);
    case InputMode::Qte:
        return action < 4 ? UserMapping(18 + action) : UserMapping(action);
    case InputMode::Vehicle:
        return action == 7 ? UserMapping(7) : UserMapping(32 + action);
    }
    return -1;
}

std::int32_t DirectionMapping(InputMode mode, std::uint32_t direction) {
    switch (mode) {
    case InputMode::Game:
    case InputMode::Fight:
    case InputMode::Qte:
        return UserMapping(14 + direction);
    case InputMode::Vehicle:
        return UserMapping(46 + direction);
    default:
        return -1;
    }
}

std::int32_t AnalogMapping(InputMode mode, std::uint32_t direction) {
    if (mode == InputMode::Menu || mode == InputMode::GameUi)
        return kKeyboardKeys[10 + direction];
    if (mode == InputMode::Game || mode == InputMode::Fight)
        return UserMapping(10 + direction);
    return -1;
}

std::int32_t TriggerMapping(InputMode mode, std::uint32_t trigger) {
    switch (mode) {
    case InputMode::Menu:
    case InputMode::GameUi:
        return kKeyboardKeys[8 + trigger];
    case InputMode::Game:
        return UserMapping(8 + trigger);
    case InputMode::Fight:
        return UserMapping(30 + trigger);
    case InputMode::Vehicle:
        return UserMapping(40 + trigger);
    default:
        return -1;
    }
}

std::int16_t AnalogSample(float value, bool applyDeadzone) {
    const auto sample = static_cast<std::int32_t>(value * 127.0F);
    if (applyDeadzone && std::abs(sample) <= 0x19)
        return 0;
    return static_cast<std::int16_t>(sample);
}

std::uint16_t PrioritizePressedInput(std::uint16_t state) {
    const std::uint16_t prioritized = state & 0x073fU;
    if (!prioritized)
        return state;
    const std::uint16_t retained = state & 0xf8c0U;
    for (const std::uint16_t button : kPressedPriority) {
        if ((prioritized & button) != 0)
            return retained | button;
    }
    return retained;
}

#if defined(_WIN32)
float NormalizeAxis(std::int16_t value) {
    const float normalized = (static_cast<float>(value) + 0.5F) / 32767.5F;
    return std::fabs(normalized) < 0.25F ? 0.0F : normalized;
}
#endif

void ClearPhysicalControllerState(ControllerState &controller) {
    controller.buttons = 0;
    controller.leftTrigger = 0;
    controller.rightTrigger = 0;
    controller.thumbLX = 0;
    controller.thumbLY = 0;
    controller.thumbRX = 0;
    controller.thumbRY = 0;
    controller.normalizedLX = 0.0F;
    controller.normalizedLY = 0.0F;
    controller.normalizedRX = 0.0F;
    controller.normalizedRY = 0.0F;
}

std::uint32_t TranslateControllerButtons(std::uint32_t buttons) {
    std::uint32_t translated = 0;
    if ((buttons & 0x0001U) != 0)
        translated |= 0x000010U;
    if ((buttons & 0x0002U) != 0)
        translated |= 0x000020U;
    if ((buttons & 0x0004U) != 0)
        translated |= 0x000040U;
    if ((buttons & 0x0008U) != 0)
        translated |= 0x000080U;
    if ((buttons & 0x1000U) != 0)
        translated |= 0x000004U;
    if ((buttons & 0x2000U) != 0)
        translated |= 0x000002U;
    if ((buttons & 0x4000U) != 0)
        translated |= 0x000400U;
    if ((buttons & 0x8000U) != 0)
        translated |= 0x000200U;
    if ((buttons & 0x0010U) != 0)
        translated |= 0x000008U;
    if ((buttons & 0x0020U) != 0)
        translated |= 0x100000U;
    if ((buttons & 0x0200U) != 0)
        translated |= 0x040000U;
    if ((buttons & 0x0100U) != 0)
        translated |= 0x080000U;
    if ((buttons & 0x0040U) != 0)
        translated |= 0x200000U;
    if ((buttons & 0x0080U) != 0)
        translated |= 0x400000U;
    return translated;
}

std::uint16_t TranslateLegacyHeld(std::uint32_t buttons, bool alternateLayout) {
    std::uint16_t held = static_cast<std::uint16_t>((buttons & 0x00f8U) << 8U);
    if ((buttons & 0x0004U) != 0)
        held |= alternateLayout ? 0x0200U : 0x0400U;
    if ((buttons & 0x0002U) != 0)
        held |= alternateLayout ? 0x0400U : 0x0200U;
    held |= static_cast<std::uint16_t>((buttons >> 14U) & 0x0100U);
    held |= static_cast<std::uint16_t>((buttons >> 16U) & 0x0020U);
    held |= static_cast<std::uint16_t>((buttons >> 15U) & 0x0010U);
    held |= static_cast<std::uint16_t>((buttons >> 15U) & 0x0008U);
    held |= static_cast<std::uint16_t>((buttons >> 8U) & 0x0004U);
    held |= static_cast<std::uint16_t>((buttons >> 8U) & 0x0002U);
    held |= static_cast<std::uint16_t>((buttons >> 20U) & 0x0001U);
    return held;
}

} // namespace

WindowInputState &g_windowInput = g_windowInputStorage.window;

void ClearWindowInput() {
    g_windowInput.customInputHandler = 0;
    for (KeyState &key : g_windowInput.keyStates) {
        key.pressed = 0;
        key.released = 0;
        key.toggled = 0;
    }
    g_windowInput.currentMouseDelta[0] = {};
    g_applicationSettings.mouseDeltaHistory.fill({});
    g_windowInput.mouseDelta = {};
    g_currentMouseHistoryIndex = 0;
}


void InitializeGlobalInput() {
    g_windowInput.previousKeyStateChangedFlag = g_windowInput.currentKeyStateChangedFlag;
    g_windowInput.customInputHandler = 0;
    g_windowInput.currentKeyStateChangedFlag = 0;
    for (KeyState &key : g_windowInput.keyStates) {
        key.released = 0;
        key.toggled = 0;
    }
}

std::int64_t InitInputMapping() {
    auto *const mappingBytes = reinterpret_cast<std::byte *>(&g_windowInputStorage) + 0xc70;
    std::memset(mappingBytes, 0, 261 * sizeof(std::int32_t));
    const auto setMapping = [mappingBytes](std::size_t index, std::int32_t value) { std::memcpy(mappingBytes + index * sizeof(value), &value, sizeof(value)); };
    for (const std::int32_t key : kMappableKeys)
        setMapping(static_cast<std::size_t>(key), 1);
    for (std::size_t index = 65; index <= 90; ++index)
        setMapping(index, 1);
    for (std::size_t index = 96; index <= 151; ++index)
        setMapping(index, 1);
    for (const std::int32_t key : kNonMappableKeys)
        setMapping(static_cast<std::size_t>(key), 0);
    return kNonMappableKeys.back();
}

bool UpdateMouseHistoryAndGetMouseIsMoving() {
    const std::size_t sampleCount = std::min<std::size_t>(
        g_currentMouseHistoryIndex > 0 ? static_cast<std::size_t>(g_currentMouseHistoryIndex) : 0U, g_applicationSettings.mouseDeltaHistory.size());
    float distance = 0.0F;
    for (std::size_t index = 1; index < sampleCount; ++index) {
        const MouseDelta &previous = g_applicationSettings.mouseDeltaHistory[index - 1];
        const MouseDelta &current = g_applicationSettings.mouseDeltaHistory[index];
        const float dx = current.x - previous.x;
        const float dy = current.y - previous.y;
        distance += std::sqrt(dx * dx + dy * dy);
    }
    if (distance > 0.1F)
        return true;
    for (std::size_t index = 256; index < std::size(g_windowInput.keyStates); ++index) {
        const KeyState &key = g_windowInput.keyStates[index];
        if (key.pressed || key.released || key.toggled)
            return true;
    }
    return false;
}

void RefreshTriggerInputs() {
#if defined(_WIN32)
    for (DWORD user = 0; user < 4; ++user) {
        XINPUT_STATE state{};
        if (XInputGetState(user, &state) == ERROR_SUCCESS && (state.Gamepad.wButtons || state.Gamepad.bLeftTrigger || state.Gamepad.bRightTrigger)) {
            g_controllerUserIndex = static_cast<std::int32_t>(user);
            g_controllers[user].active = 1;
            return;
        }
    }
#endif
}

void ControlCursorVisibilityAndPosition() {
#if defined(_WIN32)
    if (g_keyboardActive == 1 || g_forceDisplayCursor != 0) {
        while (::ShowCursor(TRUE) <= 0) {
        }
        ::SetCursor(::LoadCursorA(nullptr, MAKEINTRESOURCEA(32512)));
    } else {
        while (::ShowCursor(FALSE) >= 0) {
        }
    }

    if (g_keyboardActive != 1)
        return;
    const InputMode mode = GetCurrentInputType();
    const std::uint32_t relative = static_cast<std::uint32_t>(static_cast<std::int32_t>(mode) - static_cast<std::int32_t>(InputMode::Fight));
    if (relative <= static_cast<std::uint32_t>(InputMode::GameUi)) {
        POINT point{g_windowInput.width / 2, g_windowInput.height / 2};
        ::ClientToScreen(g_windowHandle, &point);
        ::SetCursorPos(point.x, point.y);
    }

    if (mode == InputMode::Game) {
        if (!KeyPressed(257))
            return;
    } else if (relative > static_cast<std::uint32_t>(InputMode::GameUi)) {
        return;
    }
    while (::ShowCursor(FALSE) >= 0) {
    }
#endif
}

void DetectActiveInputDevice(std::uint32_t controllerIndex) {
    const ControllerState &controller = g_controllers[controllerIndex];
    if (g_keyboardActive == 1) {
        if (controller.active == 1 &&
            (controller.buttons || controller.normalizedLX != 0.0F || controller.normalizedLY != 0.0F || controller.normalizedRX != 0.0F ||
                controller.normalizedRY != 0.0F || controller.leftTrigger || controller.rightTrigger)) {
            g_keyboardActive = 0;
            ClearWindowInput();
        }
    } else if (g_windowInput.customInputHandler != 0 || UpdateMouseHistoryAndGetMouseIsMoving()) {
        g_keyboardActive = 1;
    }
}

void HandleControllerInput(std::uint32_t controllerIndex) {
    if (g_keyboardActive != 1)
        return;

    ControllerState &controller = g_controllers[controllerIndex];
    ClearPhysicalControllerState(controller);
    const InputMode mode = GetCurrentInputType();

    for (std::uint32_t action = 0; action < kActionButtonFlags.size(); ++action) {
        bool pressed = false;
        if ((mode == InputMode::Menu || mode == InputMode::GameUi) && action == 0) {
            pressed = KeyPressed(0x0d) || KeyPressed(0x20);
            if ((g_inputContextFlags & 0x8000U) == 0 && (g_inputContextFlags & 0x10000000U) != 0) {
                pressed = pressed || KeyPressed(256);
            }
        } else {
            pressed = KeyPressed(ActionMapping(mode, action));
        }
        if (pressed)
            controller.buttons |= kActionButtonFlags[action];
    }

    for (std::uint32_t direction = 0; direction < kDirectionButtonFlags.size(); ++direction) {
        bool pressed = false;
        if ((g_inputContextFlags & 0x1000U) != 0) {
            pressed = KeyPressed(kKeyboardKeys[14 + direction]);
        } else if (mode == InputMode::Menu || mode == InputMode::GameUi) {
            pressed = KeyPressed(kKeyboardKeys[14 + direction]) || KeyPressed(kKeyboardKeys[10 + direction]);
        } else {
            pressed = KeyPressed(DirectionMapping(mode, direction));
        }
        if (pressed)
            controller.buttons |= kDirectionButtonFlags[direction];
    }
    if (mode == InputMode::Qte2) {
        if (KeyPressed(0x27) || KeyPressed(UserMapping(17)))
            controller.buttons |= 0x08U;
        if (KeyPressed(0x25) || KeyPressed(UserMapping(16)))
            controller.buttons |= 0x04U;
        if (KeyPressed(0x26) || KeyPressed(UserMapping(16)))
            controller.buttons |= 0x01U;
        if (KeyPressed(0x28) || KeyPressed(UserMapping(17)))
            controller.buttons |= 0x02U;
    }

    bool usedKeyboardAnalog = false;
    if (mode == InputMode::Menu || mode == InputMode::GameUi || mode == InputMode::Game || mode == InputMode::Fight) {
        for (std::uint32_t direction = 0; direction < 4; ++direction) {
            if (!KeyPressed(AnalogMapping(mode, direction)))
                continue;
            usedKeyboardAnalog = true;
            if (direction < 2)
                controller.normalizedRY = kKeyboardAnalogValues[direction];
            else
                controller.normalizedRX = kKeyboardAnalogValues[direction];
        }
    }

    g_hasMovedMouse = 0;
    if (!usedKeyboardAnalog && mode == InputMode::Game && KeyPressed(257)) {
        controller.normalizedRX = g_windowInput.mouseDelta.x;
        controller.normalizedRY = -g_windowInput.mouseDelta.y;
        g_hasMovedMouse = 1;
    }

    for (std::uint32_t trigger = 0; trigger < 2; ++trigger) {
        if (!KeyPressed(TriggerMapping(mode, trigger)))
            continue;
        if (trigger == 0)
            controller.leftTrigger = 0xffU;
        else
            controller.rightTrigger = 0xffU;
    }
}

void PostProcessControllerState(std::uint32_t controllerIndex) {
    ControllerState &controller = g_controllers[controllerIndex];
    if (!g_forceXInputProcessing) {
        if (controller.normalizedLX < -0.5F)
            controller.buttons |= 0x04U;
        else if (controller.normalizedLX > 0.5F)
            controller.buttons |= 0x08U;
        if (controller.normalizedLY < -0.5F)
            controller.buttons |= 0x02U;
        else if (controller.normalizedLY > 0.5F)
            controller.buttons |= 0x01U;
        controller.normalizedLX = 0.0F;
        controller.normalizedLY = 0.0F;
        return;
    }

    if (controller.normalizedRX == 0.0F && controller.normalizedRY == 0.0F) {
        controller.normalizedRX = controller.normalizedLX;
        controller.normalizedRY = controller.normalizedLY;
    }
    if (controller.normalizedLX != 0.0F || controller.normalizedLY != 0.0F || controller.normalizedRY != 0.0F || g_windowInput.customInputHandler == 1) {
        g_inputActive = 0;
    } else if ((controller.buttons & 0x0fU) != 0) {
        g_inputActive = 1;
    }

    if (g_forceXInputAnalogProcessing == 1 && g_inputActive == 1) {
        if ((controller.buttons & 0x04U) != 0) {
            g_analogStickData.y = (std::max)(-1.0F, g_analogStickData.y - 0.061999999F);
            controller.buttons &= ~0x04U;
        }
        if ((controller.buttons & 0x08U) != 0) {
            g_analogStickData.y = (std::min)(1.0F, g_analogStickData.y + 0.061999999F);
            controller.buttons &= ~0x08U;
        }
        if ((controller.buttons & 0x01U) != 0) {
            g_analogStickData.x = (std::min)(1.0F, g_analogStickData.x + 0.061999999F);
            controller.buttons &= ~0x01U;
        }
        if ((controller.buttons & 0x02U) != 0) {
            g_analogStickData.x = (std::max)(-1.0F, g_analogStickData.x - 0.061999999F);
            controller.buttons &= ~0x02U;
        }
        controller.normalizedRX = g_analogStickData.y;
        controller.normalizedRY = g_analogStickData.x;
    } else if (!g_inputActive) {
        g_analogStickData = {};
    }
}

void UpdateButtonAndTriggerStates(std::uint32_t controllerIndex) {
    ControllerState &controller = g_controllers[controllerIndex];
    const std::uint16_t buttons = static_cast<std::uint16_t>(controller.buttons);
    controller.buttonsPressed = buttons & (buttons ^ controller.previousButtons[0]);
    controller.previousButtons[0] = buttons;

    const bool left = controller.leftTrigger != 0;
    const bool right = controller.rightTrigger != 0;
    controller.leftTriggerPressed = left && controller.leftTriggerNormalized == 0;
    controller.rightTriggerPressed = right && controller.rightTriggerNormalized == 0;
    controller.leftTriggerNormalized = left ? 1U : 0U;
    controller.rightTriggerNormalized = right ? 1U : 0U;
}

ProcessedControllerState *UpdatePreviousInputStatesForUser(std::uint32_t controllerIndex) {
    const ControllerState &controller = g_controllers[controllerIndex];
    ProcessedControllerState &processed = g_processedControllers[controllerIndex];
    const std::uint32_t previous = processed.buttons;

    processed.legacyMask = 0x06feU;
    processed.reserved06 = 0x000fU;
    processed.previousButtons = previous;
    processed.buttons = TranslateControllerButtons(controller.buttons);
    processed.buttonsInverted = ~processed.buttons;
    processed.buttonsReleased = previous & ~processed.buttons;
    processed.buttonsPressed = processed.buttons & ~previous;
    processed.rightTrigger = controller.rightTrigger;
    processed.leftTrigger = controller.leftTrigger;
    processed.leftStickX = AnalogSample(controller.normalizedLX, true);
    processed.leftStickY = AnalogSample(-controller.normalizedLY, true);
    processed.rightStickX = AnalogSample(controller.normalizedRX, g_keyboardActive != 1);
    processed.rightStickY = AnalogSample(-controller.normalizedRY, g_keyboardActive != 1);
    return &processed;
}

void UpdateControllerState(std::uint32_t userIndex) {
    ControllerState &controller = g_controllers[userIndex];
    SteamControllerInput steam{};
    const bool steamActive = PollSteamInputEvents(&steam) == 1;

    controller.buttons = (steam.button4000 == 1 ? 0x4000U : 0U) | (steam.button2000 == 1 ? 0x2000U : 0U) | (steam.button01 == 1 ? 0x1000U : 0U) |
        (steam.button0200 == 1 ? 0x0200U : 0U) | (steam.button0100 == 1 ? 0x0100U : 0U) | (steam.button0010 == 1 ? 0x0010U : 0U) |
        (steam.dpadUp == 1 ? 0x0001U : 0U) | (steam.dpadDown == 1 ? 0x0002U : 0U) | (steam.dpadLeft == 1 ? 0x0004U : 0U) |
        (steam.dpadRight == 1 ? 0x0008U : 0U) | (steam.button8000 == 1 ? 0x8000U : 0U);
    controller.normalizedLX = std::fabs(steam.normalizedLX) < 0.25F ? 0.0F : std::clamp(steam.normalizedLX, -1.0F, 1.0F);
    controller.normalizedLY = std::fabs(steam.normalizedLY) < 0.25F ? 0.0F : std::clamp(steam.normalizedLY, -1.0F, 1.0F);
    controller.normalizedRX = std::fabs(steam.normalizedRX) < 0.25F ? 0.0F : std::clamp(steam.normalizedRX, -1.0F, 1.0F);
    controller.normalizedRY = std::fabs(steam.normalizedRY) < 0.25F ? 0.0F : std::clamp(steam.normalizedRY, -1.0F, 1.0F);
    controller.leftTrigger = static_cast<std::uint8_t>(steam.leftTrigger * 255.0F);
    controller.rightTrigger = static_cast<std::uint8_t>(steam.rightTrigger * 255.0F);
    PostProcessControllerState(userIndex);
    UpdateButtonAndTriggerStates(userIndex);

    if (steamActive) {
        controller.active = 1;
        g_xInputActive = 1;
        return;
    }

#if defined(_WIN32)
    XINPUT_STATE state{};
    if (XInputGetState(userIndex, &state) == ERROR_SUCCESS) {
        controller.buttons = state.Gamepad.wButtons;
        controller.leftTrigger = state.Gamepad.bLeftTrigger;
        controller.rightTrigger = state.Gamepad.bRightTrigger;
        controller.thumbLX = state.Gamepad.sThumbLX;
        controller.thumbLY = state.Gamepad.sThumbLY;
        controller.thumbRX = state.Gamepad.sThumbRX;
        controller.thumbRY = state.Gamepad.sThumbRY;
        controller.normalizedLX = NormalizeAxis(controller.thumbLX);
        controller.normalizedLY = NormalizeAxis(controller.thumbLY);
        controller.normalizedRX = NormalizeAxis(controller.thumbRX);
        controller.normalizedRY = NormalizeAxis(controller.thumbRY);
        controller.active = 1;
        PostProcessControllerState(userIndex);
        UpdateButtonAndTriggerStates(userIndex);
        if (controller.buttons || controller.normalizedLX != 0.0F || controller.normalizedLY != 0.0F || controller.normalizedRX != 0.0F ||
            controller.normalizedRY != 0.0F || controller.leftTrigger || controller.rightTrigger) {
            g_xInputActive = 0;
        }
    } else if (!g_xInputActive) {
        if (controller.active == 1)
            g_lastXInputError = 1;
        controller.active = 0;
        g_controllerUserIndex = -1;
    }
#endif

    g_xInputReserved1 = 0;
    g_forceXInputProcessing = 0;
    g_forceXInputAnalogProcessing = 0;
}

void UpdateInputsFromInputDevices() {
    const std::uint32_t userIndex = g_controllerUserIndex < 0 ? 0U : static_cast<std::uint32_t>(g_controllerUserIndex);

    if (g_isInputEnabled != 0) {
        UpdateControllerState(userIndex);
        DetectActiveInputDevice(userIndex);
        HandleControllerInput(userIndex);
        ControlCursorVisibilityAndPosition();
    } else {
        ClearPhysicalControllerState(g_controllers[userIndex]);
    }

    ProcessedControllerState *const processed = UpdatePreviousInputStatesForUser(userIndex);

    InputSlot &input = g_inputState.slots[0];
    const std::uint16_t previousHeld = input.held;
    input.previousHeld = previousHeld;
    input.enabled = 1;
    input.unknownMask = processed->legacyMask;
    const bool alternateLayout = ResolveCurrentInputForGame(-1);
    std::uint16_t held = TranslateLegacyHeld(processed->buttons, alternateLayout);
    held |= g_pendingInputFlags | g_forcedHeldInputFlags;
    g_pendingInputFlags = 0;

    if ((held & 0x0202U) != 0) {
        g_startButtonSuppressionFrames = 15;
        held &= static_cast<std::uint16_t>(~0x0800U);
    } else if (g_startButtonSuppressionFrames > 0) {
        --g_startButtonSuppressionFrames;
        held &= static_cast<std::uint16_t>(~0x0800U);
    }
    input.held = held;

    if (g_forcedRightTrigger)
        processed->rightTrigger = g_forcedRightTrigger;
    if (g_forcedLeftTrigger)
        processed->leftTrigger = g_forcedLeftTrigger;
    input.triggers = static_cast<std::uint16_t>((processed->rightTrigger & 0xffU) | ((processed->leftTrigger & 0xffU) << 8U));
    const bool fightTriggerLayout = GetCurrentInputType() == InputMode::Fight && (g_applicationSettings.savedInput.swapTriggers == 0 || g_keyboardActive == 1);
    if (processed->rightTrigger != 0)
        input.held |= fightTriggerLayout ? 0x0040U : 0x0080U;
    if (processed->leftTrigger != 0)
        input.held |= fightTriggerLayout ? 0x0080U : 0x0040U;

    if (g_forcedRightStickY)
        processed->rightStickY = g_forcedRightStickY;
    const auto stickByte = [](std::int16_t value) { return static_cast<std::uint8_t>(value + 0x80); };
    const std::uint8_t rightY =
        stickByte(g_applicationSettings.savedInput.invertY ? static_cast<std::int16_t>(-processed->rightStickY) : processed->rightStickY);
    input.rightStick = static_cast<std::uint16_t>(stickByte(processed->rightStickX) | (static_cast<std::uint16_t>(rightY) << 8U));

    const std::uint16_t newState = static_cast<std::uint16_t>(input.held & (previousHeld ^ input.held));
    input.leftStick = static_cast<std::uint32_t>(stickByte(processed->leftStickX)) | (static_cast<std::uint32_t>(stickByte(processed->leftStickY)) << 8U) |
        (static_cast<std::uint32_t>(newState) << 16U);
    input.pressedPriority = PrioritizePressedInput(newState);

    g_xinputReserved = 1;
    if (g_NumFramesProcessed != 0)
        g_xinputReserved = 1;
    if (g_forceXInputAnalogRefresh != 0) {
        g_xinputReserved = 1;
        RefreshTriggerInputs();
    }
    if (g_lastXInputError != 0) {
        g_lastXInputError = 0;
        if (GetCurrentInputType() != InputMode::Menu)
            g_xinputReserved = 0;
    }
    RefreshTriggerInputs();
    if (g_xInputReservedOverride != 0)
        g_xinputReserved = 0;
    input.xinputReserved = g_xinputReserved;
}

namespace {


bool UIElementRequested(std::uint32_t element) {
    return ((g_uiPreviousInputElements[element >> 5U] >> (element & 31U)) & 1U) != 0;
}


bool UIElementTicked(std::uint32_t element) {
    return ((g_uiRefreshedElements[element >> 5U] >> (element & 31U)) & 1U) != 0;
}

} // namespace




InputMode GetCurrentInputType() {
    const bool inGame = g_isInGame != 0;
    const bool inGameExact = g_isInGame == 1;
    if (UIElementRequested(60) || UIElementRequested(4) || UIElementRequested(28) || (UIElementRequested(55) && !inGame))
        return InputMode::Menu;
    if (UIElementRequested(30) || UIElementRequested(59) || UIElementRequested(27) || (UIElementRequested(56) && !inGame))
        return InputMode::Menu;
    const bool element17 = UIElementRequested(17);
    if (element17 && !inGame)
        return InputMode::Menu;
    for (std::uint32_t element = 18; element <= 26; ++element) {
        if (UIElementRequested(element) && !inGame)
            return InputMode::Menu;
    }
    if ((UIElementRequested(31) || element17) && !inGame)
        return InputMode::Menu;

    bool gameUi = UIElementRequested(6) || UIElementRequested(9) || UIElementRequested(10) || UIElementRequested(11) || UIElementRequested(13) ||
        UIElementRequested(14) || UIElementRequested(15) || UIElementRequested(16) || UIElementRequested(3);
    for (std::uint32_t element = 44; element <= 49; ++element)
        gameUi = gameUi || UIElementRequested(element);
    gameUi = gameUi || ((UIElementRequested(55) || UIElementRequested(56) || element17 || UIElementRequested(39) || UIElementRequested(31)) && inGameExact);
    for (std::uint32_t element = 18; element <= 26; ++element)
        gameUi = gameUi || (UIElementRequested(element) && inGameExact);
    gameUi = gameUi || (UIElementRequested(76) && (g_uiElements[6].isVisible || g_uiElements[7].isVisible));
    if (gameUi)
        return InputMode::GameUi;

    if (UIElementRequested(52) || UIElementTicked(52) || g_sfxOverlayState != UISfxOverlayState::Idle)
        return UIElementRequested(76) ? InputMode::Game : InputMode::Qte2;
    if (g_uiElements[68].isVisible || UIElementRequested(68) || UIElementTicked(68))
        return InputMode::Qte;
    constexpr std::uint8_t kSparringBattle = 2;
    if (UIElementRequested(35) || g_uiElements[35].isVisible || UIElementTicked(35) || UIElementRequested(36) || UIElementRequested(5) ||
        g_shenFightState == 4 || (g_bIsFighting && g_shenFightState2 == kSparringBattle))
        return InputMode::Fight;
    if (UIElementRequested(79) || UIElementRequested(66))
        return InputMode::Vehicle;
    return InputMode::Game;
}

bool ResolveCurrentInputForGame(std::int32_t inputContext) {
    if (g_keyboardActive == 1 || (g_steamInputGameLayoutState != 1 && g_debugAllowInputGameLayout == 0)) {
        return false;
    }
    if (g_cinemaBars[1].isVisible != 0 || (g_uiRefreshedElements[2] & 0x400U) != 0 || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0 ||
        g_forceAlternateButtonLayout != 0) {
        return true;
    }
    if (inputContext >= 0)
        return inputContext <= 2;
    const std::uint32_t relative = static_cast<std::uint32_t>(static_cast<std::int32_t>(GetCurrentInputType()) - static_cast<std::int32_t>(InputMode::Fight));
    return relative > static_cast<std::uint32_t>(InputMode::Game);
}

std::uint16_t ApplyCurrentInputForGameSwap(std::uint16_t value) {
    if (!ResolveCurrentInputForGame(-1))
        return value;
    return static_cast<std::uint16_t>((value & 0xf9ffU) | (2U * (value & 0x0200U)) | ((value >> 1U) & 0x0200U));
}

std::uint16_t ReadInputSlotMode(std::uint32_t slot, std::uint32_t mode) {
    if (slot >= std::size(g_inputState.slots))
        return 0;
    const InputSlot &input = g_inputState.slots[slot];
    switch (mode) {
    case 0:
        return input.held;
    case 1:
        return input.pressedPriority;
    case 2:
        return input.repeatedPresses;
    case 3:
        return static_cast<std::uint8_t>(input.triggers);
    case 4:
        return static_cast<std::uint8_t>(input.triggers >> 8U);
    case 5:
        return static_cast<std::uint8_t>(input.rightStick);
    case 6:
        return static_cast<std::uint8_t>(input.rightStick >> 8U);
    case 7:
        return static_cast<std::uint16_t>(input.leftStick >> 16U);
    case 8:
        return ApplyCurrentInputForGameSwap(input.pressedPriority);
    case 9:
        return ApplyCurrentInputForGameSwap(static_cast<std::uint16_t>(input.leftStick >> 16U));
    default:
        return 0;
    }
}

#if defined(_WIN32)
std::uint32_t ExtendedMapVirtualKey(std::uint32_t virtualKeyCode, void *inputLocale, void *outputLocale) {
    if (virtualKeyCode - VK_NUMPAD0 <= 9)
        return virtualKeyCode;
    UINT code = MapVirtualKeyExA(virtualKeyCode, MAPVK_VK_TO_VSC, static_cast<HKL>(inputLocale));
    switch (virtualKeyCode) {
    case VK_PRIOR:
    case VK_NEXT:
    case VK_END:
    case VK_HOME:
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:
    case VK_INSERT:
    case VK_DELETE:
    case VK_DIVIDE:
    case VK_NUMLOCK:
        code |= 0x100;
        break;
    default:
        break;
    }
    if (code) {
        const UINT mapped = MapVirtualKeyExA(code, MAPVK_VSC_TO_VK_EX, static_cast<HKL>(outputLocale));
        if (mapped)
            return mapped;
    }
    return virtualKeyCode;
}

void SetKeyPressedState(std::uint32_t keyCode) {
    const auto key = ExtendedMapVirtualKey(keyCode, GetKeyboardLayout(0), reinterpret_cast<void *>(0x08090809));
    if (key < std::size(g_windowInput.keyStates))
        g_windowInput.keyStates[key].pressed = 1;
}

void SetKeyReleasedState(std::uint32_t keyCode) {
    const auto key = ExtendedMapVirtualKey(keyCode, GetKeyboardLayout(0), reinterpret_cast<void *>(0x08090809));
    if (key < std::size(g_windowInput.keyStates) && g_windowInput.keyStates[key].pressed == 1) {
        g_windowInput.keyStates[key].pressed = 0;
        g_windowInput.keyStates[key].released = 1;
        g_windowInput.currentKeyStateChangedFlag = 1;
        g_windowInput.lastKeyIndex = static_cast<std::int32_t>(key);
    }
}
#endif

} // namespace shenmue::s1
