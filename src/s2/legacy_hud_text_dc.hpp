#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {

struct LegacyHudText {
    std::uint32_t size;
    shen_name allocationToken;
    char text[12];
    std::uint32_t glyphCache[12];
    std::int32_t glyphTextures[12];
    float depth;
    std::uint32_t colour;
    float scaleX, scaleY, x, y, spacing;
    void (*release)(LegacyHudText *);
    void (*draw)(LegacyHudText *);
    std::int8_t (*loadGlyph)(const void *);
};
struct LegacyHudGlyphSlot {
    std::uint32_t colourCode;
    std::int32_t texture;
};
typedef char Layout_legacy_hud_text_dc_0[(sizeof(LegacyHudText) == 0x9c) ? 1 : -1];
typedef char Layout_legacy_hud_text_dc_1[(offsetof(LegacyHudText, glyphTextures) == 0x44) ? 1 : -1];
typedef char Layout_legacy_hud_text_dc_2[(offsetof(LegacyHudText, depth) == 0x74) ? 1 : -1];
typedef char Layout_legacy_hud_text_dc_3[(offsetof(LegacyHudText, release) == 0x90) ? 1 : -1];
typedef char Layout_legacy_hud_text_dc_4[(sizeof(LegacyHudGlyphSlot) == 8) ? 1 : -1];

#define HUD_DIGIT_STORAGE SHEN_NAME4('T', 'I', 'M', 'R')
extern std::int32_t g_legacyHudTextReferences;
extern std::int32_t g_legacyHudDigitTextures[24];
extern LegacyHudGlyphSlot g_legacyHudGlyphs[16];
void InitTIMR();
LegacyHudText *CreateLegacyHudText();
void DrawLegacyHudText(LegacyHudText *text);
void ReleaseLegacyHudText(LegacyHudText *text);
std::int8_t LoadLegacyHudTextGlyph(const void *texture);

} } // namespace shenmue::s2

#endif
