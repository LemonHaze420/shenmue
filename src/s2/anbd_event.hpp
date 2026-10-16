#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"

#include <bit>
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name ANBD_BOARD_CHARACTER{SHEN_NAME(brdX) | 0x07000000U};
inline constexpr char ANBD_BOARD_PACKAGE[] = "ANANBORD";
inline constexpr std::uint64_t ANBD_PACKAGE_LOAD_MODE = 0, ANBD_PACKAGE_WAIT_MODE = 7;
inline constexpr std::uint32_t ANBD_RECURSIVE_XYZ = 0x78000000U;
inline constexpr std::int16_t ANBD_BOARD_PLAYER_FLAG = 29;
inline constexpr std::uint32_t ANBD_BOARD_ROLL_ADJUSTMENT = 1092;
inline constexpr Vector3 ANBD_BOARD_LOCAL_POSITION{.x = std::bit_cast<float>(0x3f9d70a4U), .y = std::bit_cast<float>(0x404eb852U), .z = 0.0f};
inline constexpr Vector3 ANBD_BOARD_ORIGIN{
    .x = std::bit_cast<float>(0x42be6f0eU), .y = std::bit_cast<float>(0x42c46666U), .z = std::bit_cast<float>(0x43028bcdU)};
inline constexpr Vector3i ANBD_BOARD_ROTATION{.x = 0, .y = 24576, .z = 0};

void ANBDInit();
void ANBDMain(std::uintptr_t argument);
extern const ScriptAreaDefinition anbdScriptDefinition;

} // namespace shenmue::s2
