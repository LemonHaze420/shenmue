#include "d3t/debug_font.hpp"

#ifdef _DEBUG
#include <array>
#include <cstddef>
#include <cstring>
#if SHENMUE_IS_S1
#include "s1/font.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/render_passes.hpp"
namespace shenmue::s1 {
#else
#include "s2/font.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
namespace shenmue::s2 {
#endif

bool DrawLegacyDebugText(std::string_view text, float x, float y, float width, float height, float advance, float lineHeight, std::uint32_t colour) {
    const std::byte *kana = nullptr;
#if SHENMUE_IS_S1
    if (g_pFont)
        std::memcpy(&kana, g_pFont, sizeof(kana));
    std::array<NuklearQuad, 120> quads;
#else
    if (const FontRuntimeData *const font = GetFontRuntimeData())
        kana = static_cast<const std::byte *>(font->kanaData);
    std::array<d3t::UITexturedVertex, 480> vertices;
    std::array<std::uint16_t, 720> indices;
#endif
    if (!kana)
        return false;
    const float startX = x;
    bool drew = false;
    for (const unsigned char character : text) {
        if (character == '\n') {
            x = startX;
            y += lineHeight;
            continue;
        }
        const std::byte *const bitmap = kana + 30U * character;
        std::size_t count = 0;
        for (std::uint32_t row = 0; row < 15; ++row) {
            for (std::uint32_t column = 0; column < 16;) {
                if ((std::to_integer<std::uint8_t>(bitmap[2U * row + column / 8U]) & (1U << (column & 7U))) == 0) {
                    ++column;
                    continue;
                }
                const std::uint32_t first = column++;
                while (column < 16 && (std::to_integer<std::uint8_t>(bitmap[2U * row + column / 8U]) & (1U << (column & 7U))) != 0)
                    ++column;
                const float left = x + static_cast<float>(first) * width / 13.0f;
                const float top = y + static_cast<float>(row) * height / 15.0f;
                const float runWidth = static_cast<float>(column - first) * width / 13.0f;
                const float pixelHeight = height / 15.0f;
#if SHENMUE_IS_S1
                quads[count] = {.x = left, .y = top, .width = runWidth, .height = pixelHeight, .u0 = 0, .v0 = 0, .u1 = 1, .v1 = 1, .colour = colour};
#else
                const std::uint16_t base = static_cast<std::uint16_t>(4U * count);
                vertices[base] = {.x = left, .y = top, .u = 0, .v = 0, .colour = colour};
                vertices[base + 1] = {.x = left + runWidth, .y = top, .u = 1, .v = 0, .colour = colour};
                vertices[base + 2] = {.x = left + runWidth, .y = top + pixelHeight, .u = 1, .v = 1, .colour = colour};
                vertices[base + 3] = {.x = left, .y = top + pixelHeight, .u = 0, .v = 1, .colour = colour};
                const std::array<std::uint16_t, 6> corners{base, static_cast<std::uint16_t>(base + 1), static_cast<std::uint16_t>(base + 2), base,
                    static_cast<std::uint16_t>(base + 2), static_cast<std::uint16_t>(base + 3)};
                for (std::size_t corner = 0; corner < corners.size(); ++corner)
                    indices[6U * count + corner] = corners[corner];
#endif
                ++count;
            }
        }
        if (count) {
#if SHENMUE_IS_S1
            drew |= DrawNuklearQuads(g_RenderDeviceContext, g_nuklearTexture, g_rendererWhiteTexture, {quads.data(), count});
#else
            const std::array<std::int32_t, 4> clip{
                0, 0, static_cast<std::int32_t>(d3t::g_originalScreenWidth), static_cast<std::int32_t>(d3t::g_originalScreenHeight)};
            drew |= d3t::DrawUITexturedGeometry(d3t::g_defaultWhiteTexture, {vertices.data(), 4U * count}, {indices.data(), 6U * count}, clip);
#endif
        }
        x += advance;
    }
    return drew;
}

} // namespace shenmue
#endif
