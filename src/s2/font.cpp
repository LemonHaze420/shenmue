#include "s2/font.hpp"

#include "s2/archive_data.hpp"
#include "s2/cold_archive.hpp"
#include "s2/hlib.hpp"
#include "common/types.hpp"
#include "d3t/content.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/shader_load.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <span>

namespace shenmue::s2 {
namespace {

FontRuntimeData *fontRuntimeData = nullptr;

template <std::size_t DestinationSize, std::size_t SourceSize> void CopyFontName(char (&destination)[DestinationSize], const char (&source)[SourceSize]) {
    static_assert(SourceSize <= DestinationSize);
    std::copy_n(source, SourceSize, destination);
}

struct nk_font *GetUIFont(const UIFontSelection &selection) {
    if (static_cast<std::uint32_t>(selection.language) >= g_uiFonts.size() || selection.family >= 4 || selection.image >= 6)
        return nullptr;
    return g_uiFonts[selection.language][selection.family].atlases[selection.image].fonts;
}

std::size_t DecodeUIFontCodepoint(std::string_view text, std::uint32_t &codepoint) {


    codepoint = 0xfffd;
    if (text.empty())
        return 0;

    const auto first = static_cast<std::uint8_t>(text[0]);
    if (first < 0x80) {
        codepoint = first;
        return 1;
    }
    const std::size_t count = first < 0xc0 ? 0 : first < 0xe0 ? 2 : first < 0xf0 ? 3 : first < 0xf8 ? 4 : 0;
    if (!count)
        return 1;
    std::uint32_t value = first & (0x7fU >> count);
    for (std::size_t index = 1; index < count; ++index) {
        if (index >= text.size())
            return 0;

        const auto next = static_cast<std::uint8_t>(text[index]);
        if ((next & 0xc0U) != 0x80U)
            return index;
        value = (value << 6U) | (next & 0x3fU);
    }
    constexpr std::array<std::uint32_t, 5> minimum{0, 0, 0x80, 0x800, 0x10000};
    if (value >= minimum[count] && value <= 0x10ffff && (value < 0xd800 || value > 0xdfff))
        codepoint = value;
    return count;
}

// @verified - p
std::int32_t FontContentPostLoadNullsub(std::uint32_t) {
    return 0;
}

// @verified - p
void UIFontMetadataLoaded(void *data, std::uint32_t contentHandle, void *metadata, std::uint64_t size, const char *sourcePath) {
    LoadUIFont(data, contentHandle, metadata, size, sourcePath);
    FontContentPostLoadNullsub(contentHandle);
    d3t::ProcessThreadStates(contentHandle, 1);
}

} // namespace

bool AllocateFONT() {
    fontRuntimeData = static_cast<FontRuntimeData *>(HLib::AllocateTaskData(sizeof(FontRuntimeData), SHEN_NAME(FONT)));
    if (!fontRuntimeData) {
        return false;
    }
    HLib::SetTaskDataMotionFlag(fontRuntimeData);
    std::fill_n(fontRuntimeData->kanjiName, 16, '\0');
    std::fill_n(fontRuntimeData->kanaName, 16, '\0');
    std::fill_n(fontRuntimeData->ryouName, 16, '\0');

    fontRuntimeData->kanaData = CopyColdArchiveSection(4, true, SHEN_NAME(FONT));
    CopyFontName(fontRuntimeData->kanaName, "DC_KANA.FON");

    fontRuntimeData->kanjiData = CopyColdArchiveSection(3, true, SHEN_NAME(FONT));
    CopyFontName(fontRuntimeData->kanjiName, "DC_KANJI.FON");

    fontRuntimeData->ryouData = CopyColdArchiveSection(5, true, SHEN_NAME(FONT));
    CopyFontName(fontRuntimeData->ryouName, "RYOU.FON");

    return fontRuntimeData->kanaData && fontRuntimeData->kanjiData && fontRuntimeData->ryouData;
}

FontRuntimeData *GetFontRuntimeData() {
    return fontRuntimeData;
}


void KanjiFontLoad(const char *name) {
    if (fontRuntimeData->kanjiName[0] && std::strcmp(name, fontRuntimeData->kanjiName) == 0)
        return;
    char path[64];
    std::snprintf(path, sizeof(path), "/misc/%s", name);
    constexpr std::uint64_t kanjiBufferSize = 0x55c46;
    const std::uint64_t size = std::min<std::uint64_t>(ArchiveData_GetFileSize(path), kanjiBufferSize);
    if (size)
        ArchiveData_Read(path, 0, fontRuntimeData->kanjiData, size);
    std::snprintf(fontRuntimeData->kanjiName, sizeof(fontRuntimeData->kanjiName), "%s", name);
}


void KanaFontLoad(const char *name) {
    if (fontRuntimeData->kanaName[0] && std::strcmp(name, fontRuntimeData->kanaName) == 0)
        return;
    char path[64];
    std::snprintf(path, sizeof(path), "/misc/%s", name);
    constexpr std::uint64_t kanaBufferSize = 0x1e20;
    const std::uint64_t size = std::min<std::uint64_t>(ArchiveData_GetFileSize(path), kanaBufferSize);
    if (size)
        ArchiveData_Read(path, 0, fontRuntimeData->kanaData, size);
    std::snprintf(fontRuntimeData->kanaName, sizeof(fontRuntimeData->kanaName), "%s", name);
}

// @verified - p
std::int64_t SelectAppropriateFontDefinition(std::int32_t language, std::uint32_t renderHandle) {
    void *payload = nullptr;
    d3t::d3t_AllocateAsyncLoadRecord(&payload);
    std::uint32_t *const words = static_cast<std::uint32_t *>(payload);
    words[0] = renderHandle;
    words[1] = static_cast<std::uint32_t>(language);
    const char *const path = kUIFontDefinitionPaths[static_cast<std::uint32_t>(language) <= 6U ? language : 0];
    const d3t::D3TShaderLoadTask task{.path = path, .renderHandle = renderHandle, .thread = {UIFontMetadataLoaded, payload}};
    return d3t::PushShaderLoadToThreadQueue(&task);
}

float GetUIFontHeight(const UIFontSelection &selection) {
    const struct nk_font *const font = GetUIFont(selection);
    return font ? font->handle.height : 0.0f;
}

float GetUIFontTextWidth(const UIFontSelection &selection, float height, std::string_view text) {
    struct nk_font *const font = GetUIFont(selection);
    if (!font || !(height > 0.0f))
        return 0.0f;

    const float scale = height / font->info.height;
    float width = 0.0f;
    while (!text.empty()) {
        std::uint32_t codepoint;
        const std::size_t consumed = DecodeUIFontCodepoint(text, codepoint);
        if (!consumed || codepoint == 0xfffd)
            break;
        if (const struct nk_font_glyph *const glyph = nk_font_find_glyph(font, codepoint))
            width += glyph->xadvance * scale;
        text.remove_prefix(consumed);
    }
    return width;
}

bool DrawUIFontText(
    const UIFontSelection &selection, float height, std::string_view text, float x, float y, std::uint32_t colour, const std::array<std::int32_t, 4> &scissor) {
    struct nk_font *const font = GetUIFont(selection);
    if (!font || !font->texture.id || !(height > 0.0f) || text.empty())
        return false;




    const float scale = height / font->info.height;
    constexpr std::size_t batchGlyphs = 64;
    static constexpr auto indices = [] {
        std::array<std::uint16_t, batchGlyphs * 6> result{};
        for (std::size_t index = 0; index < batchGlyphs; ++index) {
            const auto vertex = static_cast<std::uint16_t>(index * 4);
            result[index * 6] = vertex;
            result[index * 6 + 1] = vertex + 1;
            result[index * 6 + 2] = vertex + 2;
            result[index * 6 + 3] = vertex;
            result[index * 6 + 4] = vertex + 2;
            result[index * 6 + 5] = vertex + 3;
        }
        return result;
    }();
    std::array<d3t::UITexturedVertex, batchGlyphs * 4> vertices;
    std::size_t count = 0;
    bool submitted = false;
    auto flush = [&] {
        if (!count)
            return true;

        const bool drawn = d3t::DrawUITexturedGeometry(
            static_cast<std::uint32_t>(font->texture.id), std::span{vertices}.first(count * 4), std::span{indices}.first(count * 6), scissor);
        submitted |= drawn;
        count = 0;
        return drawn;
    };
    while (!text.empty()) {
        std::uint32_t codepoint;
        const std::size_t consumed = DecodeUIFontCodepoint(text, codepoint);
        if (!consumed || codepoint == 0xfffd)
            break;

        text.remove_prefix(consumed);
        const struct nk_font_glyph *const glyph = nk_font_find_glyph(font, codepoint);
        if (!glyph)
            continue;

        const float left = x + glyph->x0 * scale;
        const float top = y + glyph->y0 * scale;
        const float right = x + glyph->x1 * scale;
        const float bottom = y + glyph->y1 * scale;
        x += glyph->xadvance * scale;
        if (right <= left || bottom <= top || right <= scissor[0] || bottom <= scissor[1] || left >= scissor[2] || top >= scissor[3])
            continue;
        vertices[count * 4] = {left, top, glyph->u0, glyph->v0, colour};
        vertices[count * 4 + 1] = {right, top, glyph->u1, glyph->v0, colour};
        vertices[count * 4 + 2] = {right, bottom, glyph->u1, glyph->v1, colour};
        vertices[count * 4 + 3] = {left, bottom, glyph->u0, glyph->v1, colour};
        if (++count == batchGlyphs && !flush())
            return false;
    }
    return flush() && submitted;
}

} // namespace shenmue::s2
