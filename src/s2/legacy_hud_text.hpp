#pragma once

#include "s2/hlib.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct LegacyHudText {
    std::uint32_t size;
    shen_name allocationToken;
    std::array<char, 12> text;
    std::array<std::uint32_t, 12> glyphCache;
    std::array<std::int32_t, 12> glyphTextures;
    float depth;
    std::uint32_t colour;
    float scaleX;
    float scaleY;
    float x;
    float y;
    float spacing;
    std::int64_t (*release)(LegacyHudText *);
    void (*draw)(LegacyHudText *);
    std::int64_t (*loadGlyph)(const void *);
};
static_assert(sizeof(LegacyHudText) == 0xa8);
static_assert(offsetof(LegacyHudText, glyphTextures) == 0x44);
static_assert(offsetof(LegacyHudText, depth) == 0x74);
static_assert(offsetof(LegacyHudText, release) == 0x90);
void InitTIMR();
LegacyHudText *CreateLegacyHudText();
void DrawLegacyHudText(LegacyHudText *text);
std::int64_t LoadLegacyHudTextGlyph(const void *texn);
std::int64_t ReleaseLegacyHudText(LegacyHudText *text);
}
