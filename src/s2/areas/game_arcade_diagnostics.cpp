#include "s2/areas/game_arcade_diagnostics.hpp"
#include "s1/modules/harrier_render.hpp"
#include <cstring>

namespace shenmue::s2 {
std::array<std::array<std::uint8_t, 1060>, 2> g_debugScreenText{};
std::array<std::int32_t, 2> g_debugScreenTextCursors{};

// @verified - p
void ScrollDebugScreenText(std::int32_t channel) {
    std::array<std::uint8_t, 1060> &text = g_debugScreenText[channel];
    for (std::size_t byte = 0; byte < 1007; ++byte)
        text[byte] = text[byte + 53];
    std::memset(text.data() + 1007, 0, 53);
}

// @verified - p
std::int64_t AppendDebugScreenText(std::uint8_t channel, const char *text) {
    const std::size_t selected = channel & 1;
    std::array<std::uint8_t, 1060> &buffer = g_debugScreenText[selected];
    std::int32_t &column = g_debugScreenTextCursors[selected];
    buffer[1007 + column] = ' ';
    for (const unsigned char *cursor = reinterpret_cast<const unsigned char *>(text); *cursor; ++cursor) {
        const std::uint32_t glyph = *cursor;
        if (glyph - 32 <= 95) {
            buffer[1007 + column++] = static_cast<std::uint8_t>(glyph);
            if (column >= 53) {
                column = 0;
                ScrollDebugScreenText(static_cast<std::int32_t>(selected));
            }
        }
        switch (glyph) {
        case 8:
            if (--column < 0)
                column = 0;
            break;
        case 10:
            column = 0;
            ScrollDebugScreenText(static_cast<std::int32_t>(selected));
            break;
        case 13:
            column = 0;
            break;
        default:
            break;
        }
    }
    buffer[1007 + column] = '_';
    return static_cast<std::int64_t>(1060 * selected);
}
} // namespace shenmue::s2

namespace shenmue::s1 {
void Harrier_DebugPrint(const char *text) {
    s2::AppendDebugScreenText(1, text);
}
} // namespace shenmue::s1
