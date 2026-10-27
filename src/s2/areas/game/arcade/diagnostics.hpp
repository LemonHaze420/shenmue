#pragma once
#include <array>
#include <cstdint>

namespace shenmue::s2 {
extern std::array<std::array<std::uint8_t, 1060>, 2> g_debugScreenText;
extern std::array<std::int32_t, 2> g_debugScreenTextCursors;
void ScrollDebugScreenText(std::int32_t channel);
std::int64_t AppendDebugScreenText(std::uint8_t channel, const char *text);
} // namespace shenmue::s2
