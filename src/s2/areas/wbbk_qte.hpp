#pragma once
#include <cstdint>
namespace shenmue::s2::areas {
extern char bookQteCommands[48];
extern std::int32_t bookQteCommandIndex, bookQteTimingIndex;
extern float bookQteTimings[9];
std::uint32_t SelectBookQteCommands(std::int32_t type, std::int32_t pattern);
std::uint32_t RandomizeBookQteCommands(std::int32_t type);
void InitializeBookCarryQte();
std::uint32_t AdvanceBookQteCommand();
void StartBookCarryQte(std::int32_t type);
std::int32_t CheckBookCarryQte();
} // namespace shenmue::s2::areas
