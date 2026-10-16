#pragma once

#include <cstddef>
#include <array>
#include <cstdint>
#include <string_view>

namespace shenmue::s2 {

struct FontRuntimeData {
    void *kanaData;
    void *kanjiData;
    void *ryouData;
    char kanjiName[16];
    char kanaName[16];
    char ryouName[16];
};

static_assert(sizeof(FontRuntimeData) == 0x48);
static_assert(offsetof(FontRuntimeData, kanjiName) == 0x18);
static_assert(offsetof(FontRuntimeData, kanaName) == 0x28);
static_assert(offsetof(FontRuntimeData, ryouName) == 0x38);

bool AllocateFONT();
FontRuntimeData *GetFontRuntimeData();
void KanjiFontLoad(const char *name);
void KanaFontLoad(const char *name);




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

} // namespace shenmue::s2
