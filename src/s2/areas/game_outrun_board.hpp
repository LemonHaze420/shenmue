#pragma once

#include "s2/areas/game_arcade.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s2 {

struct OutRunSoundCommand {
    std::uint8_t type;
    std::uint8_t value;
};
static_assert(sizeof(OutRunSoundCommand) == 2);
extern std::array<OutRunSoundCommand, 32> g_outRunSoundCommands;
extern std::uint32_t g_outRunSoundCommandCount;
extern std::uint8_t g_outRunRoadBank;
extern std::uint8_t g_outRunFlipBank;
extern std::array<std::uint8_t, 0x800> g_outRunRoadSnapshot;
extern std::uint32_t g_outRunRoadOffset0, g_outRunRoadOffset1;
extern std::int32_t g_outRunGear;

void InitializeOutRunBoard(GameArcadeRuntime &runtime, const std::uint8_t *rankings);
void UpdateOutRunBoard(GameArcadeRuntime &runtime);
void CopyOutRunRankings(std::uint8_t *rankings);

} // namespace shenmue::s2
