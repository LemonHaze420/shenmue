#pragma once

#include "shenmue.h"

#include "common/types.hpp"
#include <stddef.h>

#if !SHENMUE_IS_DC
#include <array>
#include <string_view>
#endif

namespace shenmue {
namespace s2 {

struct FontRuntimeData {
    void *kanaData;
    void *kanjiData;
    void *ryouData;
    char kanjiName[16];
    char kanaName[16];
    char ryouName[16];
};

#if SHENMUE_IS_DC
typedef char FontRuntimeSizeCheck[(sizeof(FontRuntimeData) == (SHENMUE_IS_DC ? 0x3c : 0x48)) ? 1 : -1];
typedef char FontRuntimeKanjiNameCheck[(offsetof(FontRuntimeData, kanjiName) == (SHENMUE_IS_DC ? 0x0c : 0x18)) ? 1 : -1];
typedef char FontRuntimeKanaNameCheck[(offsetof(FontRuntimeData, kanaName) == (SHENMUE_IS_DC ? 0x1c : 0x28)) ? 1 : -1];
typedef char FontRuntimeRyouNameCheck[(offsetof(FontRuntimeData, ryouName) == (SHENMUE_IS_DC ? 0x2c : 0x38)) ? 1 : -1];

#else
static_assert(sizeof(FontRuntimeData) == 0x48);

static_assert(offsetof(FontRuntimeData, kanjiName) == 0x18);

static_assert(offsetof(FontRuntimeData, kanaName) == 0x28);

static_assert(offsetof(FontRuntimeData, ryouName) == 0x38);

#endif

#if SHENMUE_IS_DC
void AllocateFONT();
#else
bool AllocateFONT();
#endif
FontRuntimeData *GetFontRuntimeData();
void KanjiFontLoad(const char *name);
void KanaFontLoad(const char *name);




#if !SHENMUE_IS_DC
std::int64_t SelectAppropriateFontDefinition(std::int32_t language, std::uint32_t renderHandle);
void FreeUIFontDefinition(std::int32_t language);

struct UIFontSelection {
    std::int32_t language{};
    std::uint32_t family{};
    std::uint32_t image{4};
};

float GetUIFontHeight(const UIFontSelection &font);
float GetUIFontTextWidth(const UIFontSelection &font, float height, std::string_view text);
bool DrawUIFontText(
    const UIFontSelection &font, float height, std::string_view text, float x, float y, std::uint32_t colour, const std::array<std::int32_t, 4> &scissor);


#endif

} // namespace s2
} // namespace shenmue
