#pragma once

#include "shenmue.h"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hlib.hpp"
#include "s2/legacy_sprite_types.hpp"

namespace shenmue { namespace s2 {

const shen_name TASK_XTLP = SHEN_NAME(XTLP);
const shen_name RESOURCE_TLPF = SHEN_NAME(TLPF);

struct ExtraTelopDefinition {
    const char *text, *spriteName;
    std::int16_t glyphSize;
};
static const ExtraTelopDefinition extraTelopDefinitions[7] = {
    {"\263\260\314\347\304\272\311\252", "GAIMON.SPR", 128},
    {"\304\260\322\246", "CHOUKEI.SPR", 128},
    {"\317\242\274\352\313\244", "RENSYUHO.SPR", 128},
    {"\265\301", "BTK_GI.SPR", 256}, {"\262\374", "BTK_KAI.SPR", 256},
    {"\270\371", "BTK_KOU.SPR", 256}, {"\303\300", "BTK_TAN.SPR", 256}
};
struct ExtraTelopGlyph {
    std::uint32_t retained00;
    float x, y, scaleX, scaleY;
    std::byte retained14[0x78];
};
typedef char ExtraTelopGlyphSizeCheck[(sizeof(ExtraTelopGlyph) == 0x8c) ? 1 : -1];
typedef char ExtraTelopGlyphPositionCheck[(offsetof(ExtraTelopGlyph, x) == 4) ? 1 : -1];
static const float extraTelopRowPositions[9] = {0, 240, 340, 340, 340, 340, 340, 340, 340};
extern HLib::Task *g_extraTelopTask;
extern std::int32_t g_extraTelopMode, g_extraTelopPhase, g_extraTelopRemainingFrames, g_extraTelopGlyphCount;
extern std::int32_t g_extraTelopTextures[8];
extern ExtraTelopGlyph g_extraTelopGlyphs[8];
extern char g_extraTelopText[24];
extern const char *g_extraTelopDisplayedText;
std::int32_t ShowBigKanjiText(const char *text);
void QueueExtraTelop(const char *text, std::int32_t mode);

} } // namespace shenmue::s2

#else
#include "s2/ui.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_XTLP{SHEN_NAME(XTLP)};
inline constexpr shen_name RESOURCE_TLPF{SHEN_NAME(TLPF)};

struct ExtraTelopDefinition {
    const char *text;
    const char *spriteName;
    std::int16_t glyphSize;
};
static_assert(sizeof(ExtraTelopDefinition) == 0x18);
static_assert(offsetof(ExtraTelopDefinition, spriteName) == 0x08);
static_assert(offsetof(ExtraTelopDefinition, glyphSize) == 0x10);

inline constexpr std::array<ExtraTelopDefinition, 14> extraTelopDefinitions{{
    {.text = "AAAAAAAA", .spriteName = "GAIMON", .glyphSize = 128},
    {.text = "BBBB", .spriteName = "CHOUKEI", .glyphSize = 128},
    {.text = "CCCCCC", .spriteName = "RENSYUHO", .glyphSize = 128},
    {.text = "DDDDDD", .spriteName = "SYOUKIN", .glyphSize = 128},
    {.text = "EEEE", .spriteName = "SYAKOU", .glyphSize = 128},
    {.text = "FFFF", .spriteName = "SITUHOU", .glyphSize = 128},
    {.text = "GGGG", .spriteName = "CYUSUI", .glyphSize = 128},
    {.text = "HHHHHHHHHHHH", .spriteName = "KENKA", .glyphSize = 128},
    {.text = "IIIIIIIIII", .spriteName = "SINKUU", .glyphSize = 128},
    {.text = "JJ", .spriteName = "BTK_GI", .glyphSize = 256},
    {.text = "KK", .spriteName = "BTK_KAI", .glyphSize = 256},
    {.text = "LL", .spriteName = "BTK_KOU", .glyphSize = 256},
    {.text = "MM", .spriteName = "BTK_TAN", .glyphSize = 256},
    {.text = nullptr, .spriteName = nullptr, .glyphSize = 0},
}};
inline constexpr std::array<std::int32_t, 10> extraTelopSixGlyphFrames{0, 7, 5, 7, 5, 5, 8, 8, 20, 50};
inline constexpr std::array<float, 9> extraTelopRowPositions{0, 240, 340, 340, 340, 340, 340, 340, 340};
inline constexpr std::array<Vector2, 5> extraTelopFiveGlyphPositions{{
    {320, 240},
    {532, 140},
    {532, 340},
    {108, 140},
    {108, 340},
}};

extern HLib::Task *g_extraTelopTask;
extern std::int32_t g_extraTelopMode;
extern std::int32_t g_extraTelopPhase;
extern std::int32_t g_extraTelopRemainingFrames;
extern std::int32_t g_extraTelopGlyphCount;
extern std::int32_t g_extraTelopActiveGlyph;
extern std::array<LegacyIconSprite, 8> g_extraTelopSprites;
extern std::array<std::uint32_t, 8> g_extraTelopColours;
extern std::array<float, 8> g_extraTelopOffsets;
extern std::array<char, 24> g_extraTelopText;
extern const char *g_extraTelopDisplayedText;

std::int64_t ShowBigKanjiText(const char *text);

void QueueExtraTelop(const char *text, std::int32_t mode);

} // namespace shenmue::s2
#endif
