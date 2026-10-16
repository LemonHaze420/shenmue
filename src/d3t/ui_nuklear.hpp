#pragma once

#include <array>
#include <cstddef>
#include <cstdint>










#define NK_ASSERT(expr) ((void)0)
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_DEFAULT_COMMAND_BUFFER_SIZE (32 * 1024)
#define NK_BUFFER_DEFAULT_INITIAL_SIZE (16 * 1024)
#include "nuklear.h"


static_assert(sizeof(nk_size) == 8);
static_assert(sizeof(nk_draw_index) == 2);
static_assert(sizeof(struct nk_image) == 0x20);
static_assert(sizeof(struct nk_style_item) == 0x28);
static_assert(sizeof(struct nk_command_image) == 0x60 && offsetof(struct nk_command_image, img) == 0x18);
static_assert(offsetof(struct nk_command_image, img.rotate) == 0x2c && offsetof(struct nk_command_image, img.angle) == 0x30);
static_assert(offsetof(struct nk_command_image, col) == 0x38 && offsetof(struct nk_command_image, block) == 0x40);
static_assert(sizeof(struct nk_draw_command) == 0x28 && offsetof(struct nk_draw_command, texture) == 0x18);
static_assert(offsetof(struct nk_draw_command, block_handle) == 0x20);
static_assert(offsetof(struct nk_draw_list, config.null.texture) == 0x18 && offsetof(struct nk_draw_list, config.vertex_layout) == 0x28);
static_assert(offsetof(struct nk_draw_list, buffer) == 0x50 && offsetof(struct nk_draw_list, vertex_count) == 0x6c);
static_assert(offsetof(struct nk_draw_list, cmd_offset) == 0x70 && offsetof(struct nk_draw_list, cmd_count) == 0x78);
static_assert(sizeof(struct nk_context) == 0x4590);
static_assert(sizeof(struct nk_panel) == 0x190);
static_assert(sizeof(struct nk_font_atlas) == 0x1c8 && offsetof(struct nk_font_atlas, glyph_count) == 0x198);
static_assert(offsetof(struct nk_font_atlas, glyphs) == 0x1a0 && offsetof(struct nk_font_atlas, fonts) == 0x1b0);
static_assert(sizeof(struct nk_font_glyph) == 0x30 && sizeof(struct nk_font_config) == 0x48 && offsetof(struct nk_font, handle) == 8);
static_assert(sizeof(struct nk_baked_font) == 0x28 && offsetof(struct nk_font, info.range_count) == 0x50 && offsetof(struct nk_font, scale) == 0x58);
static_assert(offsetof(struct nk_font, glyphs) == 0x60 && offsetof(struct nk_font, fallback) == 0x68 && offsetof(struct nk_font, texture) == 0x78);
static_assert(offsetof(struct nk_font, fallback_codepoint) == 0x70 && sizeof(struct nk_font) == 0x88);

// A .fontdef font record (the games' pre-baked Nuklear fonts): after its tag, face, image, pixel size and glyph count, the glyphs, then the
// offset of its ranges in the file's range chunk (in runes), its baked font, its font configuration, its fallback codepoint, the atlas
// cursors, and its BC3 texture's size.
constexpr std::uint32_t kFontDefinitionChunkTag = 0x12345678;
constexpr std::size_t kFontRecordHeaderSize = 20;
constexpr std::size_t kFontRecordTailSize = 476;
constexpr std::size_t kFontRecordBakedOffset = 8;
constexpr std::size_t kFontRecordConfigOffset = 48;
constexpr std::size_t kFontRecordFallbackOffset = 120;
constexpr std::size_t kFontRecordCursorsOffset = 124;
constexpr std::size_t kFontRecordTextureSizeOffset = 460;



struct nk_font *Nuklear_LoadFontRecord(struct nk_font_atlas &atlas, struct nk_font_config &config, const std::byte *record, const std::byte *ranges,
    nk_handle texture, struct nk_draw_null_texture &null);
static_assert(offsetof(struct nk_context, style.font) == 0x150);
static_assert(offsetof(struct nk_context, style.button.normal) == 0x1b0 && offsetof(struct nk_context, style.button.border_color) == 0x228);
static_assert(offsetof(struct nk_context, style.window.fixed_background) == 0x2228 && offsetof(struct nk_context, style.window.background) == 0x2250);
static_assert(offsetof(struct nk_context, style.window.tooltip_padding) == 0x2300);
static_assert(offsetof(struct nk_context, memory) == 0x2308 && offsetof(struct nk_context, draw_list) == 0x2ee0);
static_assert(offsetof(struct nk_context, use_pool) == 0x4514 && offsetof(struct nk_context, pool) == 0x4518);
static_assert(offsetof(struct nk_context, current) == 0x4578 && offsetof(struct nk_context, count) == 0x4588 && offsetof(struct nk_context, seq) == 0x458c);



inline constexpr std::array<nk_draw_vertex_layout_element, 4> kNuklearVertexLayout{{
    {.attribute = NK_VERTEX_POSITION, .format = NK_FORMAT_FLOAT, .offset = 0},
    {.attribute = NK_VERTEX_TEXCOORD, .format = NK_FORMAT_FLOAT, .offset = 8},
    {.attribute = NK_VERTEX_COLOR, .format = NK_FORMAT_R8G8B8A8, .offset = 16},
    {.attribute = NK_VERTEX_ATTRIBUTE_COUNT, .format = NK_FORMAT_COUNT, .offset = 0},
}};
static_assert(NK_VERTEX_TEXCOORD == 2 && NK_VERTEX_COLOR == 1 && NK_FORMAT_FLOAT == 6 && NK_FORMAT_R8G8B8A8 == 11 && NK_FORMAT_COUNT == 18);
static_assert(sizeof(nk_draw_vertex_layout_element) == 0x10 && sizeof(nk_draw_null_texture) == 0x10 && sizeof(nk_buffer) == 0x78);
