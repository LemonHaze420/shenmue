#pragma once
#include "shenmue.h"

#include "s2/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

enum ControllerInputSelector : std::int32_t {
    INPUT_HELD = 0,
    INPUT_PRESSED = 1,
    INPUT_REPEAT = 2,
    INPUT_RIGHT_TRIGGER = 3,
    INPUT_LEFT_TRIGGER = 4,
    INPUT_RIGHT_X = 5,
    INPUT_RIGHT_Y = 6,
    INPUT_NEW_PRESSES = 7,
    INPUT_LEFT_X = 8,
    INPUT_LEFT_Y = 9,
    INPUT_CONNECTION = 10,
    INPUT_DISCONNECTED = 11,
    INPUT_DEVICE_STATE = 12,
    INPUT_DEVICE_UNAVAILABLE = 13,
};
inline constexpr std::uint16_t PAD_BUTTON_X = 0x0400U;
inline constexpr std::uint16_t PAD_UP = 0x1000U;
inline constexpr std::uint16_t PAD_DOWN = 0x2000U;
inline constexpr std::uint16_t PAD_LEFT = 0x4000U;
inline constexpr std::uint16_t PAD_RIGHT = 0x8000U;
inline constexpr std::uint16_t PAD_TRIGGER_LEFT = 0x0040U;
inline constexpr std::uint16_t PAD_TRIGGER_RIGHT = 0x0080U;



struct LegacyInputSlot {
    std::uint8_t enabled;
    std::int8_t unknown01;
    std::int16_t unknown02;
    std::uint16_t held;
    std::uint16_t previousHeld;
    std::uint16_t pressedPriority;
    std::uint16_t repeatedPresses;
    std::uint8_t analog0c;
    std::uint8_t analog0d;
    std::uint8_t analog0e;
    std::uint8_t analog0f;
    std::uint8_t analog10;
    std::uint8_t analog11;
    std::uint16_t newPresses;
    std::uint16_t connectionState;
    std::uint16_t deviceState;
};

static_assert(sizeof(LegacyInputSlot) == 0x18);
static_assert(offsetof(LegacyInputSlot, held) == 0x04);
static_assert(offsetof(LegacyInputSlot, pressedPriority) == 0x08);
static_assert(offsetof(LegacyInputSlot, repeatedPresses) == 0x0a);
static_assert(offsetof(LegacyInputSlot, analog0c) == 0x0c);
static_assert(offsetof(LegacyInputSlot, analog11) == 0x11);
static_assert(offsetof(LegacyInputSlot, newPresses) == 0x12);
static_assert(offsetof(LegacyInputSlot, connectionState) == 0x14);
static_assert(offsetof(LegacyInputSlot, deviceState) == 0x16);

extern std::array<LegacyInputSlot, 5> g_inputBuffer;


std::uint32_t EVT_GetControllerInput(std::uint8_t userIndex, std::int32_t selector);
std::uint64_t GetCurrentControlInput(std::int32_t selector);
std::uint32_t GetControllerUserIndex();
bool HasPressedCancel();
void RefreshTriggerInputs();
void InitializeSoftResetInput();
void SetSoftResetInputEnabled(std::int32_t enabled);
bool CheckSoftResetInput();
std::byte *GetControllerEventWork(std::uint8_t userIndex);

struct KeyState {
    std::int32_t pressed;
    std::int32_t released;
    std::int32_t toggled;
};
static_assert(sizeof(KeyState) == 0x0c);

// Window-event adapter for the replaced Nuklear input owner; coordinates are client pixels.
struct WindowUIInput {
    Vector2 position{};
    Vector2 previousPosition{};
    std::int32_t leftDown{};
    std::int32_t leftClicks{};
    float wheel{};
};
WindowUIInput ConsumeWindowUIInput();



struct PhysicalControllerState {
    std::uint32_t buttons;
    std::uint8_t leftTrigger;
    std::uint8_t rightTrigger;
    std::int16_t thumbLX;
    std::int16_t thumbLY;
    std::int16_t thumbRX;
    std::int16_t thumbRY;
    std::array<std::byte, 2> reserved0e;
    Vector4 normalizedSticks;
    std::array<std::byte, 0x30> retained20;
    std::uint32_t active;
};
static_assert(sizeof(PhysicalControllerState) == 0x54);
static_assert(offsetof(PhysicalControllerState, normalizedSticks) == 0x10);
static_assert(offsetof(PhysicalControllerState, active) == 0x50);

extern std::int32_t g_mouseLookInputActive;
std::uint64_t GetIsKeyboardActive();
void DetectActiveInputDevice(std::uint32_t controllerIndex);
void SetXInputFlags();
void SetXInputDigitalProcessing(std::int32_t enabled);
void SetXInputAnalogProcessing(std::int32_t enabled);
std::uint64_t PostProcessControllerState(std::uint32_t controllerIndex);
void ClearWindowInput();
void InitializeWindowInput(std::uint32_t width, std::uint32_t height);
void ResetInputHandler();
bool UpdateAndGetMouseIsMoving();
std::uint32_t GetLastReleasedWindowKey() noexcept;
bool HasWindowInputStateChanged() noexcept;
bool IsWindowKeyBindable(std::int32_t action, std::uint32_t key) noexcept;
std::uintptr_t SetKeyPressed(std::uint32_t key);
std::uint64_t SetKeyReleased(std::uint32_t key);
void HandleWindowInputMessage(void *window, std::uint32_t message, std::uint64_t wParam, std::int64_t lParam);

struct PadControlEntry {
    std::uint32_t flags;
    LegacyInputSlot input;
};

struct PadControlTaskData {
    std::array<PadControlEntry, 16> entries;
    PadControlEntry *currentBoundEntry;
};

static_assert(sizeof(PadControlEntry) == 0x1c);
static_assert(offsetof(PadControlEntry, input) == 0x04);
static_assert(offsetof(PadControlTaskData, currentBoundEntry) == 0x1c0);
static_assert(sizeof(PadControlTaskData) == 0x1c8);
struct KeyRepeatTaskData {
    std::uint32_t inputIndex;
#if SHENMUE_IS_PC
    std::array<float, 9> counters;
#else
    std::array<std::int32_t, 9> counters;
#endif
};

static_assert(sizeof(KeyRepeatTaskData) == 0x28);
static_assert(offsetof(KeyRepeatTaskData, inputIndex) == 0x00);
static_assert(offsetof(KeyRepeatTaskData, counters) == 0x04);

constexpr std::array<std::uint16_t, 8> repeatMasks{
    0x1000,
    0x2000,
    0x4000,
    0x8000,
    0x0040,
    0x0080,
    0x0400,
    0x0200,
};

HLib::Task *Queue_PrePadControlTask(std::uint8_t nextFunctionIndex);
void padEnableTask();
HLib::Task *Queue_PadControlTask(std::uint8_t nextFunctionIndex);
PadControlEntry *AllocatePadControlEntry();
PadControlEntry *BindPadControlEntry(PadControlEntry *entry);
void ReleasePadControlEntry(PadControlEntry *entry);
LegacyInputSlot *ctrlGetPadInputBuffer0(PadControlEntry *control);

std::int64_t Queue_KeyRepeat(std::uint32_t inputIndex);
void HandleInputs();

void InitializeInputSelectionState();
void SetInputDeviceSelectionSuspended(std::int32_t suspended);
void InjectInputButtons(std::uint16_t buttons);

void InjectInputAnalog(std::int16_t stickX, std::int16_t stickY);
std::int32_t CountConnectedInputDevices();
bool SelectInputDeviceForButtons(std::uint16_t buttons);
void padcCheckControllerConnection();
void ResetSelectedInputDevice();
void ForceInputDeviceDisconnected(std::int32_t disconnected);

enum ButtonMediationFlag : std::uint32_t {
    ButtonMediationShowButtonHint = 0x4U,
    ButtonMediationShowActionHint = 0x8U,
    ButtonMediationSuppressTalk = 0x10U,
    ButtonMediationPlayerRelative = 0x20U,
    ButtonMediationRejected = 0x100U,
    ButtonMediationPressedX = 0x800U,
    ButtonMediationPressedY = 0x1000U,
    ButtonMediationPressedA = 0x2000U,
    ButtonMediationPressedB = 0x4000U,
    ButtonMediationPressedStart = 0x8000U,
    ButtonMediationBindX = 0x10000U,
    ButtonMediationBindY = 0x20000U,
    ButtonMediationBindA = 0x40000U,
    ButtonMediationBindB = 0x80000U,
    ButtonMediationBindStart = 0x100000U,
};
inline constexpr std::uint32_t buttonMediationFaceBindings = ButtonMediationBindX | ButtonMediationBindY | ButtonMediationBindA | ButtonMediationBindB;
inline constexpr std::uint32_t buttonMediationPressedShift = 11;
inline constexpr std::uint32_t buttonMediationBindingShift = 16;

struct ButtonMediationRequest {
    std::uint32_t flags;
    shen_name name;
    Vector3 position;
};

struct ButtonMediationResult {
    shen_name name;
    std::uint32_t flags;
};



struct ButtonMediationControl {
    std::uint32_t flags;
    std::int32_t count;
    std::uint32_t field08;
    std::array<ButtonMediationRequest, 12> requests;
    std::byte reserved0fc[0x14];
    std::uint32_t field110;
    std::uint32_t field114;
    std::byte reserved118[0x10];
    std::uint64_t field128;
    LegacyInputSlot input;
    std::uint64_t field148;
    std::int32_t currentButton;
    std::array<ButtonMediationResult, 5> results;
    std::array<shen_name, 5> priorities;
};

static_assert(sizeof(ButtonMediationRequest) == 0x14);
static_assert(offsetof(ButtonMediationRequest, name) == 4);
static_assert(offsetof(ButtonMediationRequest, position) == 8);
static_assert(offsetof(ButtonMediationControl, flags) == 0x00);
static_assert(offsetof(ButtonMediationControl, count) == 0x04);
static_assert(offsetof(ButtonMediationControl, field08) == 0x08);
static_assert(offsetof(ButtonMediationControl, requests) == 0x0c);
static_assert(offsetof(ButtonMediationControl, field110) == 0x110);
static_assert(offsetof(ButtonMediationControl, field114) == 0x114);
static_assert(offsetof(ButtonMediationControl, field128) == 0x128);
static_assert(offsetof(ButtonMediationControl, input) == 0x130);
static_assert(offsetof(ButtonMediationControl, currentButton) == 0x150);
static_assert(offsetof(ButtonMediationControl, results) == 0x154);
static_assert(offsetof(ButtonMediationControl, priorities) == 0x17c);
static_assert(sizeof(ButtonMediationControl) == 0x190);

inline constexpr shen_name BUTTON_DOOR{SHEN_NAME(BTDR)};
inline constexpr shen_name BUTTON_BTME{SHEN_NAME(BTME)};

HLib::Task *Queue_btnButtonMediationCtrl(std::uint8_t nextFunctionIndex);
std::int64_t RegisterButtonMediation(shen_name name, const Vector3 *position, std::uint32_t flags);
std::uint32_t btnCheck(shen_name name);
std::uint32_t GetMediatedButtonBindings(shen_name name);
// Mode 0 registers the named battle button, mode 1 queries its bindings and mode 2 its press state.
std::int64_t GetBattleDataConfig(std::int32_t mode, shen_name name);
shen_name GetMediatedButtonOwner(std::uint32_t button);
std::uint64_t CheckMediatedDoublePress();

} // namespace shenmue::s2
