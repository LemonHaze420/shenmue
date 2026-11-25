#include "platform/dc/input.hpp"

#if SHENMUE_IS_DC

#include <sg_pad.h>

namespace shenmue { namespace dc {

// @verified - d
void PublishPeripheralInput(InputSlot *slots) {
    for (std::uint32_t index = 0; index < 4; ++index) {
        const PDS_PERIPHERAL &peripheral = *pdGetPeripheral(index * 6U);
        InputSlot &slot = slots[index];
        slot.enabled = 1;
        slot.previousHeld = slot.held;
        slot.unknownMask = static_cast<std::uint16_t>(peripheral.support);
        slot.held = static_cast<std::uint16_t>(((peripheral.on & 0xffffU) << 8) | ((peripheral.on & 0xffffU) >> 8));
        if (slot.enabled == 1) {
            slot.triggers[0] = static_cast<std::uint8_t>(peripheral.r);
            if (slot.triggers[0])
                slot.held |= 0x80;
            slot.triggers[1] = static_cast<std::uint8_t>(peripheral.l);
            if (slot.triggers[1])
                slot.held |= 0x40;
            slot.leftStickX = static_cast<std::uint8_t>(peripheral.x1 - 0x80);
            slot.leftStickY = static_cast<std::uint8_t>(peripheral.y1 - 0x80);
            slot.rightStickX = static_cast<std::uint8_t>(peripheral.x2 - 0x80);
            slot.rightStickY = static_cast<std::uint8_t>(peripheral.y2 - 0x80);
        }
        slot.rawPressed = (slot.previousHeld ^ slot.held) & slot.held;
        if ((slot.rawPressed & 0x606) == 0) {
            slot.pressedPriority = slot.rawPressed;
        } else {
            slot.pressedPriority = slot.rawPressed & 0xf9f9;
            if (slot.rawPressed & 4)
                slot.pressedPriority |= 4;
            else if (slot.rawPressed & 2)
                slot.pressedPriority |= 2;
            else if (slot.rawPressed & 0x400)
                slot.pressedPriority |= 0x400;
            else
                slot.pressedPriority |= 0x200;
        }
    }
}

} } // namespace shenmue::dc

#endif
