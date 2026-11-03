#pragma once
#include "shenmue.h"
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

extern void *g_pFont;

void AllocFONTs();
void LoadFont(const char *filename);

#if SHENMUE_IS_DEMO
std::uint16_t ConvertJisGlyphToKana(std::uint16_t token);
struct TrialFontStorage {
    void *kana;
    void *kanji;
    void *ryo;
    char kanjiName[16];
    char kanaName[16];
    char ryoName[16];
};
static_assert(sizeof(TrialFontStorage) == 3 * sizeof(void *) + 48);
static_assert(offsetof(TrialFontStorage, kanjiName) == 3 * sizeof(void *));
void LoadKanaFont(const char *filename);
void LoadRyoFont(const char *filename);
#endif
} // namespace shenmue::s1
