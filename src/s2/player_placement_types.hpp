#pragma once

#include "shenmue.h"
#include "common/math.hpp"

namespace shenmue { namespace s2 {

struct Character;
struct PlayerPlacementNode {
    std::uint32_t flags;
#if SHENMUE_IS_PC
    std::uint32_t reserved004;
#endif
    Character *character;
    Vector3 position, previousPosition, delta;
    std::int16_t targetYaw, characterBearingYaw;
    float distance, minimumDistance;
    std::byte reserved040[0xa];
    std::int16_t movementBearingYaw;
    Vector3 targetPosition, targetCharacterPosition;
    std::int16_t stationaryFrames;
    std::uint16_t reserved066;
    PlayerPlacementNode *next;
};

} } // namespace shenmue::s2
