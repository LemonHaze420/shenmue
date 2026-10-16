#pragma once

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
