#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

#include "s1/d3t/settings.hpp"
#include "s1/steam.hpp"

namespace shenmue::s1 {

#pragma pack(push, 1)
struct InputSlot {
    std::uint16_t enabled;
    std::uint16_t unknownMask;
    std::uint16_t held;
    std::uint16_t previousHeld;
    std::uint16_t pressedPriority;
    std::uint16_t repeatedPresses;
    std::uint16_t triggers;
    std::uint16_t rightStick;
    std::uint32_t leftStick;
    std::uint16_t xinputReserved;
};
#pragma pack(pop)

struct InputStateStorage {
    InputSlot slots[5];
};

static_assert(sizeof(InputSlot) == 0x16);
static_assert(offsetof(InputSlot, held) == 4);
static_assert(offsetof(InputSlot, pressedPriority) == 8);
static_assert(offsetof(InputSlot, repeatedPresses) == 0xa);
static_assert(offsetof(InputSlot, leftStick) == 0x10);
static_assert(offsetof(InputSlot, xinputReserved) == 0x14);
static_assert(sizeof(InputStateStorage) == 0x6e);

extern InputStateStorage g_inputState;

struct ControllerState {
    std::uint32_t buttons;
    std::uint8_t leftTrigger;
    std::uint8_t rightTrigger;
    std::int16_t thumbLX;
    std::int16_t thumbLY;
    std::int16_t thumbRX;
    std::int16_t thumbRY;
    std::byte reserved_0e[2];
    float normalizedLX;
    float normalizedLY;
    float normalizedRX;
    float normalizedRY;
    std::uint16_t previousButtons[2];
    std::uint32_t leftTriggerNormalized;
    std::uint32_t rightTriggerNormalized;
    std::uint16_t buttonsPressed;
    std::uint16_t reserved_2e;
    std::uint32_t leftTriggerPressed;
    std::uint32_t rightTriggerPressed;
    float internalAnalogData[6];
    std::uint32_t active;
};

static_assert(sizeof(ControllerState) == 0x54);
static_assert(offsetof(ControllerState, normalizedLX) == 0x10);
static_assert(offsetof(ControllerState, previousButtons) == 0x20);
static_assert(offsetof(ControllerState, leftTriggerNormalized) == 0x24);
static_assert(offsetof(ControllerState, buttonsPressed) == 0x2c);
static_assert(offsetof(ControllerState, leftTriggerPressed) == 0x30);
static_assert(offsetof(ControllerState, active) == 0x50);
struct ProcessedControllerState {
    std::uint32_t reserved00;
    std::uint16_t legacyMask;
    std::uint16_t reserved06;
    std::uint32_t buttons;
    std::uint32_t buttonsInverted;
    std::uint32_t buttonsReleased;
    std::uint32_t buttonsPressed;
    std::uint16_t rightTrigger;
    std::uint16_t leftTrigger;
    std::int16_t leftStickX;
    std::int16_t leftStickY;
    std::int16_t rightStickX;
    std::int16_t rightStickY;
    std::byte reserved24[0x24];
    std::uint32_t previousButtons;
    std::uint32_t reserved4c;
};

static_assert(sizeof(ProcessedControllerState) == 0x50);
static_assert(offsetof(ProcessedControllerState, legacyMask) == 0x04);
static_assert(offsetof(ProcessedControllerState, buttons) == 0x08);
static_assert(offsetof(ProcessedControllerState, buttonsPressed) == 0x14);
static_assert(offsetof(ProcessedControllerState, rightTrigger) == 0x18);
static_assert(offsetof(ProcessedControllerState, leftStickX) == 0x1c);
static_assert(offsetof(ProcessedControllerState, rightStickX) == 0x20);
static_assert(offsetof(ProcessedControllerState, previousButtons) == 0x48);

enum class InputMode : std::int32_t {
    Menu = 0,
    GameUi = 1,
    Game = 2,
    Fight = 3,
    Qte2 = 4,
    Qte = 5,
    Vehicle = 6,
};

extern std::array<ControllerState, 4> g_controllers;
extern std::array<ProcessedControllerState, 4> g_processedControllers;
extern std::int32_t g_controllerUserIndex;
extern std::int32_t g_lastXInputError;
extern std::int32_t g_xInputActive;
extern std::int32_t g_xInputReserved1;
extern std::int32_t g_xInputReservedOverride;
extern std::int32_t g_forceXInputProcessing;
extern std::int32_t g_forceXInputAnalogProcessing;
extern std::int32_t g_forceXInputAnalogRefresh;
extern std::int32_t g_inputActive;
extern std::int32_t g_keyboardActive;


extern const std::array<std::int32_t, 18> kKeyboardKeys;
std::int32_t UserMapping(std::uint32_t index);
extern std::int32_t g_hasMovedMouse;
extern std::int32_t g_forceAlternateButtonLayout;
extern std::int32_t g_steamInputGameLayoutState;
extern std::int32_t g_debugAllowInputGameLayout;
extern std::uint16_t g_pendingInputFlags;
extern std::uint16_t g_forcedHeldInputFlags;

extern std::uint8_t g_forcedRightTrigger;
extern std::uint8_t g_forcedLeftTrigger;

extern std::int16_t g_forcedRightStickY;
extern std::uint64_t g_inputContextFlags;

using AnalogStickData = Vector2;

extern AnalogStickData g_analogStickData;

struct KeyState {
    std::int32_t pressed;
    std::int32_t released;
    std::int32_t toggled;
};

struct WindowInputState {
    MouseDelta mouseDelta;
    std::int32_t customInputHandler;
    std::int32_t unknown;
    KeyState keyStates[261];
    std::int32_t lastKeyIndex;
    std::int32_t previousKeyStateChangedFlag;
    std::int32_t currentKeyStateChangedFlag;
    std::int32_t height;
    std::int32_t width;
    MouseDelta currentMouseDelta[2];
    std::int32_t mappableKeys[65];
    std::byte keyData[124];
};

static_assert(sizeof(KeyState) == 0xc);
static_assert(offsetof(WindowInputState, keyStates) == 0x10);
static_assert(offsetof(WindowInputState, lastKeyIndex) == 0xc4c);
static_assert(offsetof(WindowInputState, currentMouseDelta) == 0xc60);
static_assert(offsetof(WindowInputState, mappableKeys) == 0xc70);
static_assert(offsetof(WindowInputState, keyData) == 0xd74);
static_assert(sizeof(WindowInputState) == 0xdf0);

extern WindowInputState &g_windowInput;
extern std::int32_t g_currentMouseHistoryIndex;

void ClearWindowInput();
void InitializeGlobalInput();
std::int64_t InitInputMapping();
bool UpdateMouseHistoryAndGetMouseIsMoving();
void RefreshTriggerInputs();
void ControlCursorVisibilityAndPosition();
void DetectActiveInputDevice(std::uint32_t controllerIndex);
void HandleControllerInput(std::uint32_t controllerIndex);
void PostProcessControllerState(std::uint32_t controllerIndex);
void UpdateButtonAndTriggerStates(std::uint32_t controllerIndex);
ProcessedControllerState *UpdatePreviousInputStatesForUser(std::uint32_t controllerIndex);
void UpdateControllerState(std::uint32_t userIndex);
void UpdateInputsFromInputDevices();

InputMode GetCurrentInputType();
bool ResolveCurrentInputForGame(std::int32_t inputContext);
std::uint16_t ApplyCurrentInputForGameSwap(std::uint16_t value);
std::uint16_t ReadInputSlotMode(std::uint32_t slot, std::uint32_t mode);

#if defined(_WIN32)
std::uint32_t ExtendedMapVirtualKey(std::uint32_t virtualKeyCode, void *inputLocale, void *outputLocale);
void SetKeyPressedState(std::uint32_t keyCode);
void SetKeyReleasedState(std::uint32_t keyCode);
#endif

} // namespace shenmue::s1
