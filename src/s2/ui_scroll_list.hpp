#pragma once

#include "d3t/ui_runtime.hpp"

namespace shenmue::s2 {

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
void UI_DrawScrollListStandardRow(nk_context &context, std::uint32_t index, std::int32_t row);
void UI_DrawScrollListMapRow(nk_context &context, std::uint32_t index, std::int32_t row);

} // namespace shenmue::s2
