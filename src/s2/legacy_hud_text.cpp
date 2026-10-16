#include "s2/legacy_hud_text.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
std::int32_t references{};
std::array<std::int32_t, 24> digits = [] {
    std::array<std::int32_t, 24> result;
    result.fill(-1);
    return result;
}();
struct GlyphSlot {
    std::int32_t reserved{-1};
    std::int32_t texture{-1};
};
std::array<GlyphSlot, 16> glyphs{};
float Flush(float value) {
    return std::fabs(value) < 1.1920929e-7f ? 0.0f : value;
}
void ReleaseDigits() {
    for (auto &texture : digits)
        if (texture != -1) {
            ReleaseNamedTexture(texture);
            texture = -1;
        }
}
void LoadDigits() {
    void *allocation{};
    std::size_t size{};
    LoadLegacySprite("sprite", "LOADNUM.GZ", SHEN_NAME(TIMR), allocation, size);
    const auto *cursor = static_cast<const std::byte *>(allocation);
    const auto *end = cursor ? cursor + size : cursor;
    for (std::size_t i = 0; cursor && end - cursor >= 8 && i < 10; ++i) {
        std::uint32_t signature{}, chunkSize{};
        std::memcpy(&signature, cursor, 4);
        std::memcpy(&chunkSize, cursor + 4, 4);
        if (signature != SHEN_NAME(TEXN) || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        digits[i] = static_cast<std::int32_t>(ReadTexnTexture(cursor));
        cursor += chunkSize;
    }
    HLib::FreeTaskData(allocation);
}
}



void InitTIMR() {
    references = 0;
    digits.fill(-1);
    for (GlyphSlot &glyph : glyphs)
        glyph = {.reserved = 0, .texture = -1};
}

LegacyHudText *CreateLegacyHudText() {
    auto *task = HLib::GetCurrentTask();
    auto *text =
        task ? static_cast<LegacyHudText *>(HLib::AllocateTaskData(sizeof(LegacyHudText), shen_name{static_cast<std::uint32_t>(task->taskToken)})) : nullptr;
    if (!text) {
        if (references <= 0)
            ReleaseDigits();
        return nullptr;
    }
    std::memset(text, 0, sizeof(*text));
    if (references <= 0)
        LoadDigits();
    text->size = sizeof(*text);
    text->allocationToken = shen_name{static_cast<std::uint32_t>(task->taskToken)};
    text->glyphTextures.fill(-1);
    text->depth = text->scaleX = text->scaleY = 1.0f;
    text->colour = 0xffffffffU;
    text->x = 350.0f;
    text->y = 50.0f;
    text->release = ReleaseLegacyHudText;
    text->draw = DrawLegacyHudText;
    text->loadGlyph = LoadLegacyHudTextGlyph;
    ++references;
    return text;
}


void DrawLegacyHudText(LegacyHudText *text) {
    if (!text)
        return;
    const auto colour =
        static_cast<std::uint16_t>((text->colour >> 16 & 0xf000) | (text->colour >> 4 & 0xf) | (text->colour >> 8 & 0xf0) | (text->colour >> 12 & 0xf00));
    for (std::size_t i = 0; i < 12; ++i) {
        if (text->glyphTextures[i] != -1 && text->glyphCache[i] != ((static_cast<std::uint32_t>(colour) << 8) | static_cast<std::uint8_t>(text->text[i]))) {
            ReleaseNamedTexture(text->glyphTextures[i]);
            text->glyphTextures[i] = -1;
        }
    }
    std::size_t count{};
    while (count < 12 && text->text[count])
        ++count;
    const auto width = Flush(text->scaleX * 32.0f);
    const auto height = Flush(text->scaleY * 32.0f);
    const auto advance = Flush(text->scaleX * 32.0f + text->spacing);
    LegacySpritePacket packet{};
    packet.quad.flags = 1024;
    packet.colour = text->colour;
    packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    for (std::size_t i = 0; i < count; ++i) {
        const auto x = Flush(Flush(Flush(static_cast<float>(i) - Flush(static_cast<float>(count) * 0.5f)) * advance) + text->x);
        auto glyph = static_cast<std::uint8_t>(text->text[i]);
        if (glyph >= '0' && glyph <= '9') {
            TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(text->y),
                TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(digits[glyph - '0']), text->depth);
        } else if (glyph == ':') {
            auto &texture = text->glyphTextures[i];
            if (texture == -1) {
                constexpr char colonGlyph[]{static_cast<char>(0xa1), static_cast<char>(0xa7), 0};
                const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                texture = static_cast<std::int32_t>(SetupUIElement(control->windowTask, colonGlyph, 32, static_cast<std::int16_t>(colour), 0, 1));
            }
            if (texture != -1) {
                LegacyIconSprite sprite{};
                sprite.x = x;
                sprite.y = text->y;
                sprite.scaleX = sprite.scaleY = 1.0f;
                sprite.depth = text->depth;
                sprite.icon = static_cast<std::uint16_t>(texture);
                sprite.width = sprite.height = 32;
                sprite.alignment = 5;
                PrepareDrawTextureWithMode(sprite, ScreenMode::AdjustedWithOffset);
            }
        } else {
            --glyph;
            if (glyph < 16 && glyphs[glyph].texture != -1) {
                TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(text->y),
                    TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
                LegacySpriteNullsub(packet, static_cast<std::uint32_t>(glyphs[glyph].texture), text->depth);
            }
        }
        text->glyphCache[i] = (static_cast<std::uint32_t>(colour) << 8) | glyph;
    }
}


std::int64_t LoadLegacyHudTextGlyph(const void *texn) {
    for (std::size_t i = 0; i < glyphs.size(); ++i)
        if (glyphs[i].texture == -1) {
            auto texture = static_cast<std::int32_t>(ReadTexnTexture(texn));
            if (texture == -1)
                return -1;
            glyphs[i].texture = texture;
            return static_cast<std::int64_t>(i + 1);
        }
    return -1;
}


std::int64_t ReleaseLegacyHudText(LegacyHudText *text) {
    if (!text)
        return 0;
    auto *cursor = text->glyphTextures.data();
    for (std::size_t i = 0; i < 12; ++i)
        if (*cursor != -1) {
            ReleaseNamedTexture(*cursor);
            *cursor++ = -1;
        }
    if (--references <= 0) {
        ReleaseDigits();
        for (auto &glyph : glyphs)
            if (glyph.texture != -1) {
                ReleaseNamedTexture(glyph.texture);
                glyph.reserved = 0;
                glyph.texture = -1;
            }
    }
    return HLib::FreeTaskData(text);
}
}
