#pragma once

#include "d3t/ui_runtime.hpp"

namespace shenmue::s2 {

inline constexpr float UI_IMAGE_CORNER_ANGLE = 1.5707951f;

struct ScrollListScrollData {
    std::int32_t frame;
    std::int32_t unknown04;
    const char *text;
};
static_assert(sizeof(ScrollListScrollData) == 16 && offsetof(ScrollListScrollData, text) == 8);
extern std::array<ScrollListScrollData, 12> g_scrollListScrollData;
ScrollListScrollData GetScrollListScrollData(std::int32_t index);
d3t::UISpriteSheet *UI_GetSpriteSheet(std::uint32_t index);
void UI_DrawImageCorners(nk_context *context, const UILayoutRect &bounds, std::int32_t index, std::uint32_t colour);
void UI_DrawScrollListScrollRow(nk_context &context, std::uint32_t index, std::int32_t row, UILayoutRect &bounds);
enum class UIScrollListMethod : std::int32_t {
    Move = 1,
    Scroll = 2,
    Save = 3,
    Map = 4,
    Pawnshop = 5,
    Standard = 6,
};

d3t::UIViewSlot<kUIViewImageCapacity> *UI_GetViewSlot(std::uint32_t index);
UIScrollList *UI_GetScrollList(std::uint32_t index);
struct nk_image UI_GetDefaultImage();
void UI_DrawPreparedImage(nk_context &context, const std::int32_t *ready, const void *image, std::uint32_t colour);
void UI_DrawViewImage(nk_context &context, std::int32_t index, std::uint32_t colour);
void UI_DrawScrollListSaveRow(nk_context &context, std::uint32_t index, std::int32_t row, float height);
void UI_DrawScrollListStandardRow(nk_context &context, std::uint32_t index, std::int32_t row);
void UI_DrawScrollListMapRow(nk_context &context, std::uint32_t index, std::int32_t row);

} // namespace shenmue::s2
