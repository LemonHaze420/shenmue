#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue { namespace s1 {

struct InputSlot {
    std::uint16_t enabled;
    std::uint16_t unknownMask;
    std::uint16_t held;
    std::uint16_t previousHeld;
    std::uint16_t pressedPriority;
    std::uint16_t repeatedPresses;
    std::uint8_t triggers[2];
    std::uint8_t leftStickX;
    std::uint8_t leftStickY;
    std::uint8_t rightStickX;
    std::uint8_t rightStickY;
    union {
        std::uint16_t rawPressed;
        std::uint16_t reserved12;
    };
};

#if SHENMUE_S2_GCC295
typedef char InputSlotLayoutCheck0[(sizeof(InputSlot) == 0x14) ? 1 : -1];
typedef char InputSlotLayoutCheck1[(offsetof(InputSlot, held) == 0x04) ? 1 : -1];
typedef char InputSlotLayoutCheck2[(offsetof(InputSlot, pressedPriority) == 0x08) ? 1 : -1];
typedef char InputSlotLayoutCheck3[(offsetof(InputSlot, triggers) == 0x0c) ? 1 : -1];
typedef char InputSlotLayoutCheck4[(offsetof(InputSlot, leftStickX) == 0x0e) ? 1 : -1];
typedef char InputSlotLayoutCheck5[(offsetof(InputSlot, rightStickX) == 0x10) ? 1 : -1];
typedef char InputSlotLayoutCheck6[(offsetof(InputSlot, rawPressed) == 0x12) ? 1 : -1];
#else
static_assert(sizeof(InputSlot) == 0x14);
static_assert(offsetof(InputSlot, held) == 0x04);
static_assert(offsetof(InputSlot, pressedPriority) == 0x08);
static_assert(offsetof(InputSlot, triggers) == 0x0c);
static_assert(offsetof(InputSlot, leftStickX) == 0x0e);
static_assert(offsetof(InputSlot, rightStickX) == 0x10);
static_assert(offsetof(InputSlot, rawPressed) == 0x12);

#endif

} } // namespace shenmue::s1

namespace shenmue { namespace dc {

#if SHENMUE_S2_GCC295
typedef s1::InputSlot InputSlot;
#else
using InputSlot = s1::InputSlot;
#endif
void PublishPeripheralInput(InputSlot *slots);

} } // namespace shenmue::dc

#endif
