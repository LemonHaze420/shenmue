#pragma once

#include "shenmue.h"
#if SHENMUE_IS_DC
#include "s2/legacy_sprite_dc.hpp"
#define STAFF_ROLL_CONSTANT static const
#else
#include "s2/legacy_sprite.hpp"
#define STAFF_ROLL_CONSTANT inline constexpr
#endif

namespace shenmue { namespace s2 {
STAFF_ROLL_CONSTANT std::int32_t OPENING_STAFF_ROLL_INTERVALS[8][2] = {
    {455, 605}, {700, 845}, {935, 1085}, {1175, 1325}, {1420, 1560}, {1660, 1810}, {2125, 2290}, {3320, 3700}};
STAFF_ROLL_CONSTANT std::int8_t OPENING_STAFF_ROLL_ITEMS[9][2] = {{0, 0}, {0, -1}, {1, 2}, {3, -1}, {4, 5}, {6, 7}, {8, -1}, {10, 11}, {9, -1}};
STAFF_ROLL_CONSTANT std::int8_t OPENING_STAFF_ROLL_SIDES[10] = {1, 0, 1, 1, 0, 1, 0, 1, 1, 1};
STAFF_ROLL_CONSTANT float OPENING_STAFF_ROLL_POSITIONS[3][2] = {{32.0f, 96.0f}, {608.0f, 384.0f}, {320.0f, 240.0f}};
STAFF_ROLL_CONSTANT std::int8_t OPENING_STAFF_ROLL_ALIGNMENTS[4] = {0, 10, 5, 0};
} } // namespace shenmue::s2
#undef STAFF_ROLL_CONSTANT
