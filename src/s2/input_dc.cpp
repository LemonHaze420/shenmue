#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/input_dc.hpp"
#include <sg_pad.h>

namespace shenmue {
namespace s2 {

LegacyInputSlot g_inputBuffer[4] = {};
PeripheralInputSnapshot g_peripheralInputSnapshots[4] = {};
std::uint16_t g_previousPeripheralButtons[4] = {};
std::uint32_t g_peripheralSnapshotCounter = 0;
struct PeripheralSnapshotTiming {
    std::uint32_t reserved[3];
    std::uint32_t startTicks;
    std::uint32_t endTicks;
};
PeripheralSnapshotTiming g_peripheralSnapshotTiming = {};

namespace {

std::int32_t peripheralPortMap[6] = {SHENMUE_PERIPHERAL_PORTS};
#define SHENMUE_INPUT_PORT_DATA peripheralPortMap
#define SHENMUE_INPUT_DATA_BUFFER g_inputBuffer

void *peripheralReceiveBuffers[6] = {reinterpret_cast<void *>(kPeripheralReceiveBegin),
    reinterpret_cast<void *>(kPeripheralReceiveBegin + kPeripheralReceiveStride),
    reinterpret_cast<void *>(kPeripheralReceiveBegin + kPeripheralReceiveStride * 2),
    reinterpret_cast<void *>(kPeripheralReceiveBegin + kPeripheralReceiveStride * 3),
    reinterpret_cast<void *>(kPeripheralReceiveBegin + kPeripheralReceiveStride * 4),
    reinterpret_cast<void *>(kPeripheralReceiveBegin + kPeripheralReceiveStride * 5)};

} // namespace

// @verified - d
void InitializePeripheralInput() {
    pdInitPeripheralEx(0, SHENMUE_INPUT_PORT_DATA, peripheralReceiveBuffers, reinterpret_cast<void *>(kPeripheralSendBegin), 6);
}

// @verified - d
void InitializeInputSlots() {
    for (int index = 0; index < 4; ++index) {
        g_inputBuffer[index].enabled = 0;
        g_inputBuffer[index].unknownMask = 0;
        g_inputBuffer[index].held = 0;
        g_inputBuffer[index].previousHeld = 0;
        g_inputBuffer[index].pressedPriority = 0;
        g_inputBuffer[index].repeatedPresses = 0;
        g_inputBuffer[index].rawPressed = 0;
    }
}

// @verified - d
void SnapshotPeripheralInputs() {
    ++g_peripheralSnapshotCounter;
    pdExecPeripheralServer();
    g_peripheralSnapshotTiming.startTicks = *reinterpret_cast<volatile std::uint32_t *>(0xff100008);
    PeripheralInputSnapshot *snapshot = g_peripheralInputSnapshots;
    for (int index = 0; index < 4; ++index, ++snapshot) {
        const PDS_PERIPHERAL *const peripheral = pdGetPeripheral(kPeripheralPortMap[index]);
        snapshot->previousHeld = g_previousPeripheralButtons[index];
        snapshot->held =
            static_cast<std::uint16_t>(((static_cast<std::uint16_t>(peripheral->on) << 8) & 0xff00) | (static_cast<std::uint16_t>(peripheral->on) >> 8));
        snapshot->pressed =
            static_cast<std::uint16_t>(((static_cast<std::uint16_t>(peripheral->press) << 8) & 0xff00) | (static_cast<std::uint16_t>(peripheral->press) >> 8));
        snapshot->released = static_cast<std::uint16_t>(
            ((static_cast<std::uint16_t>(peripheral->release) << 8) & 0xff00) | (static_cast<std::uint16_t>(peripheral->release) >> 8));
        snapshot->rightTrigger = static_cast<std::uint8_t>(peripheral->r);
        snapshot->leftTrigger = static_cast<std::uint8_t>(peripheral->l);
        snapshot->leftStickX = static_cast<std::uint8_t>(peripheral->x1);
        snapshot->leftStickY = static_cast<std::uint8_t>(peripheral->y1);
        snapshot->rightStickX = static_cast<std::uint8_t>(peripheral->x2);
        snapshot->rightStickY = static_cast<std::uint8_t>(peripheral->y2);
        g_previousPeripheralButtons[index] = snapshot->held;
    }
    g_peripheralSnapshotTiming.endTicks = *reinterpret_cast<volatile std::uint32_t *>(0xff100008);
}

void UpdateInputsFromInputDevices() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t index = 0;
    do {
        const PDS_PERIPHERAL *const peripheral = pdGetPeripheral(index * 6);
        g_inputBuffer[index].enabled = 1;
        g_inputBuffer[index].previousHeld = g_inputBuffer[index].held;
        g_inputBuffer[index].unknownMask = static_cast<std::uint16_t>(peripheral->support);
        const std::uint16_t held = static_cast<std::uint16_t>(peripheral->on);
        g_inputBuffer[index].held = static_cast<std::uint16_t>((held << 8) | (held >> 8));
        if (g_inputBuffer[index].enabled == 1) {
            if ((g_inputBuffer[index].triggers[0] = static_cast<std::uint8_t>(peripheral->r)) != 0)
                g_inputBuffer[index].held |= 0x80U;
            if ((g_inputBuffer[index].triggers[1] = static_cast<std::uint8_t>(peripheral->l)) != 0)
                g_inputBuffer[index].held |= 0x40U;
            g_inputBuffer[index].leftStickX = static_cast<std::uint8_t>(peripheral->x1 - 0x80);
            g_inputBuffer[index].leftStickY = static_cast<std::uint8_t>(peripheral->y1 - 0x80);
            g_inputBuffer[index].rightStickX = static_cast<std::uint8_t>(peripheral->x2 - 0x80);
            g_inputBuffer[index].rightStickY = static_cast<std::uint8_t>(peripheral->y2 - 0x80);
        }
        LegacyInputSlot *slots = g_inputBuffer;
        slots[index].rawPressed = (g_inputBuffer[index].previousHeld ^ g_inputBuffer[index].held) & g_inputBuffer[index].held;
        const std::int32_t pressed = static_cast<std::int16_t>(slots[index].rawPressed);
        const std::int32_t priority = pressed & 0x606;
        if (priority) {
            g_inputBuffer[index].pressedPriority = pressed & 0xf9f9U;
            if (priority & 4U)
                g_inputBuffer[index].pressedPriority |= 4U;
            else if (priority & 2U)
                g_inputBuffer[index].pressedPriority |= 2U;
            else if (priority & 0x400U)
                g_inputBuffer[index].pressedPriority |= 0x400U;
            else
                g_inputBuffer[index].pressedPriority |= 0x200U;
        } else {
            g_inputBuffer[index].pressedPriority = g_inputBuffer[index].rawPressed;
        }
    } while (++index <= 3);
#else
    dc::PublishPeripheralInput(SHENMUE_INPUT_DATA_BUFFER);
#endif
}

// @verified - d
void KeyRepeat_Callback(void *taskData) {
    KeyRepeatTaskData *const repeat = static_cast<KeyRepeatTaskData *>(taskData);
    const std::uint16_t pressed = g_inputBuffer[repeat->inputIndex].pressedPriority;
    const std::uint16_t held = g_inputBuffer[repeat->inputIndex].held;
    std::uint16_t repeated = pressed;
#define SHENMUE_KEY_REPEAT(counter, mask)                                                                                                                      \
    if (held & (mask)) {                                                                                                                                       \
        if (pressed & (mask))                                                                                                                                  \
            repeat->counters[counter] = 15;                                                                                                                    \
        else if (--repeat->counters[counter] < 0) {                                                                                                            \
            repeat->counters[counter] = 0;                                                                                                                     \
            repeated |= (mask);                                                                                                                                \
        }                                                                                                                                                      \
    }
    SHENMUE_KEY_REPEAT(0, 0x1000)
    SHENMUE_KEY_REPEAT(1, 0x2000)
    SHENMUE_KEY_REPEAT(2, 0x4000)
    SHENMUE_KEY_REPEAT(3, 0x8000)
    SHENMUE_KEY_REPEAT(4, 0x40)
    SHENMUE_KEY_REPEAT(5, 0x80)
    SHENMUE_KEY_REPEAT(6, 0x400)
    SHENMUE_KEY_REPEAT(7, 0x200)
#undef SHENMUE_KEY_REPEAT
    g_inputBuffer[repeat->inputIndex].repeatedPresses = repeated;
}

// @verified - d
std::int32_t Queue_KeyRepeat(std::uint32_t inputIndex) {
    if (inputIndex < 4) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(KeyRepeat_Callback, 15, 2, sizeof(KeyRepeatTaskData), TASK_KREP);
        if (task) {
            KeyRepeatTaskData *const repeat = static_cast<KeyRepeatTaskData *>(HLib::GetTaskParameter(task));
            repeat->inputIndex = inputIndex;
            repeat->counters[0] = 15;
            repeat->counters[1] = 15;
            repeat->counters[2] = 15;
            repeat->counters[3] = 15;
            repeat->counters[4] = 15;
            repeat->counters[5] = 15;
            repeat->counters[6] = 15;
            repeat->counters[7] = 15;
            repeat->counters[8] = 15;
            return 1;
        }
    }
    return 0;
}

// @verified - d
std::uint32_t EVT_GetControllerInput(std::uint8_t userIndex, std::int32_t selector) {
    std::uint32_t value = 0;
    const LegacyInputSlot &slot = g_inputBuffer[userIndex & 3U];
    switch (selector) {
    case INPUT_HELD:
        value = slot.held;
        break;
    case INPUT_PRESSED:
        value = slot.pressedPriority;
        break;
    case INPUT_REPEAT:
        value = slot.repeatedPresses;
        break;
    case INPUT_RIGHT_TRIGGER:
        value = slot.triggers[0];
        break;
    case INPUT_LEFT_TRIGGER:
        value = slot.triggers[1];
        break;
    case INPUT_RIGHT_X:
        value = slot.leftStickX;
        break;
    case INPUT_RIGHT_Y:
        value = slot.leftStickY;
        break;
    case INPUT_NEW_PRESSES:
        value = slot.rawPressed;
        break;
    case INPUT_LEFT_X:
        value = slot.rightStickX;
        break;
    case INPUT_LEFT_Y:
        value = slot.rightStickY;
        break;
    }
    return value;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
PeripheralInputSnapshot *GetControllerEventWork(std::uint8_t userIndex) {
    return &g_peripheralInputSnapshots[userIndex & 3U];
}
#endif

} // namespace s2
} // namespace shenmue
#undef SHENMUE_INPUT_PORT_DATA
#undef SHENMUE_INPUT_DATA_BUFFER

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" shenmue::s2::PeripheralInputSnapshot *__attribute__((renesas)) shenmue_s2_event_GetControllerEventWork(std::uint32_t userIndex) {
    return shenmue::s2::GetControllerEventWork(static_cast<std::uint8_t>(userIndex));
}
#endif

#endif
