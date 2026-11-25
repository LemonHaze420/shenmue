#pragma once

#include "platform/dc/input.hpp"
#include "s2/hlib.hpp"

#define SHENMUE_INPUT_DATA static const
#define SHENMUE_INPUT_INIT =

namespace shenmue { namespace s2 {

void ProcessDebugInputNullsub();

typedef dc::InputSlot LegacyInputSlot;
extern LegacyInputSlot g_inputBuffer[4];

struct PeripheralInputSnapshot {
    std::uint16_t reserved00;
    std::uint16_t held;
    std::uint16_t previousHeld;
    std::uint16_t pressed;
    std::uint16_t released;
    std::uint8_t rightTrigger;
    std::uint8_t leftTrigger;
    std::uint8_t leftStickX;
    std::uint8_t leftStickY;
    std::uint8_t rightStickX;
    std::uint8_t rightStickY;
};
typedef char InputLayoutCheck0[(sizeof(PeripheralInputSnapshot) == 0x10) ? 1 : -1];
typedef char InputLayoutCheck1[(offsetof(PeripheralInputSnapshot, held) == 2) ? 1 : -1];
typedef char InputLayoutCheck2[(offsetof(PeripheralInputSnapshot, previousHeld) == 4) ? 1 : -1];
typedef char InputLayoutCheck3[(offsetof(PeripheralInputSnapshot, rightTrigger) == 10) ? 1 : -1];

extern PeripheralInputSnapshot g_peripheralInputSnapshots[4];
extern std::uint16_t g_previousPeripheralButtons[4];
extern std::uint32_t g_peripheralSnapshotCounter;
extern std::uint32_t g_peripheralSnapshotStartTicks;
extern std::uint32_t g_peripheralSnapshotEndTicks;

enum ControllerInputSelector {
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
};

SHENMUE_INPUT_DATA std::uintptr_t kPeripheralMemoryAlias =
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    0;
#else
    0x80000000;
#endif
SHENMUE_INPUT_DATA std::uintptr_t kPeripheralReceiveBegin = 0x0c019f20 | kPeripheralMemoryAlias;
SHENMUE_INPUT_DATA std::uintptr_t kPeripheralSendBegin = 0x0c01cfe0 | kPeripheralMemoryAlias;
SHENMUE_INPUT_DATA std::uint32_t kPeripheralReceiveStride = 0x820;
#define SHENMUE_PERIPHERAL_PORTS 0, 6, 12, 18, 1, 2
static const std::int32_t kPeripheralPortMap[6] = {SHENMUE_PERIPHERAL_PORTS};

struct KeyRepeatTaskData {
    std::uint32_t inputIndex;
    std::int32_t counters[9];
};
typedef char InputLayoutCheck4[(sizeof(KeyRepeatTaskData) == 0x28) ? 1 : -1];
using ::shenmue::shen_name;
#define TASK_KREP shen_name(SHEN_NAME4('K', 'R', 'E', 'P'))
SHENMUE_INPUT_DATA std::uint16_t kRepeatMasks[8] SHENMUE_INPUT_INIT {0x1000, 0x2000, 0x4000, 0x8000, 0x40, 0x80, 0x400, 0x200};
void KeyRepeat_Callback(void *taskData);
std::int32_t Queue_KeyRepeat(std::uint32_t inputIndex);
void InitializePeripheralInput();
void InitializeInputSlots();
void SnapshotPeripheralInputs();
void UpdateInputsFromInputDevices();
std::uint32_t EVT_GetControllerInput(std::uint8_t userIndex, std::int32_t selector);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
PeripheralInputSnapshot *GetControllerEventWork(std::uint8_t userIndex);
#endif

} } // namespace shenmue::s2
#undef SHENMUE_INPUT_DATA
#undef SHENMUE_INPUT_INIT
