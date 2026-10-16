#define NK_IMPLEMENTATION
#include "d3t/ui_nuklear.hpp"

#include <cstring>





struct nk_font *Nuklear_LoadFontRecord(struct nk_font_atlas &atlas, struct nk_font_config &config, const std::byte *record, const std::byte *ranges,
    nk_handle texture, struct nk_draw_null_texture &null) {
    std::int32_t glyphCount = 0;
    std::memcpy(&glyphCount, record + 16, sizeof(glyphCount));
    struct nk_font_glyph *const glyphs = reinterpret_cast<struct nk_font_glyph *>(const_cast<std::byte *>(record + kFontRecordHeaderSize));
    const std::byte *const tail = record + kFontRecordHeaderSize + sizeof(struct nk_font_glyph) * static_cast<std::size_t>(glyphCount);
    std::uint64_t rangeOffset = 0;
    std::memcpy(&rangeOffset, tail, sizeof(rangeOffset));
    std::memcpy(&config, tail + kFontRecordConfigOffset, sizeof(config));
    config.range = reinterpret_cast<const nk_rune *>(ranges) + rangeOffset;
    config.font = nullptr;
    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    nk_font_atlas_add(&atlas, &config);
    config.ttf_blob = nullptr;
    config.ttf_size = 0;
    atlas.config->ttf_blob = nullptr;
    atlas.config->ttf_size = 0;
    struct nk_font *const font = atlas.fonts;
    font->glyphs = glyphs;
    std::memcpy(&font->info, tail + kFontRecordBakedOffset, sizeof(font->info));
    font->info.ranges = config.range;
    atlas.glyph_count = glyphCount;
    atlas.glyphs = glyphs;
    std::memcpy(atlas.cursors, tail + kFontRecordCursorsOffset, sizeof(atlas.cursors));
    nk_rune fallback = 0;
    std::memcpy(&fallback, tail + kFontRecordFallbackOffset, sizeof(fallback));
    nk_font_init(font, config.size, fallback, glyphs, &font->info, texture);
    nk_font_atlas_end(&atlas, texture, &null);
    return atlas.fonts;
}
