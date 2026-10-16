#pragma once
#include <cstdint>

namespace shenmue::s2::EventTransformFlags {
enum Value : std::uint32_t {
    Local = 0,
    World = 0x40000000U,
    Relative = 0x80000000U,
    X = 0x20000000U,
    Y = 0x10000000U,
    Z = 0x08000000U,
    AllAxes = 0x38000000U,
    CharacterNodeSpace = 0x01000000U,
    MotionYaw = 0x02000000U,
    TurnMotion = 0x04000000U,
    CustomIdleMotion = 0x00200000U,
    MotionTurnMask = TurnMotion | CustomIdleMotion,
};
}
