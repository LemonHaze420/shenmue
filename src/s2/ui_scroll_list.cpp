#include "s2/ui_scroll_list.hpp"

#include <bit>
#include <cstring>

namespace shenmue::s2 {

d3t::UIViewSlot<kUIViewImageCapacity> *UI_GetViewSlot(std::uint32_t index) {
    return index < g_uiViewSlots.size() ? &g_uiViewSlots[index] : nullptr;
}

UIScrollList *UI_GetScrollList(std::uint32_t index) {
    return index < g_uiScrollLists.size() ? &g_uiScrollLists[index] : nullptr;
}

struct nk_image UI_GetDefaultImage() {
    return g_uiDefaultImage;
}

void UI_DrawPreparedImage(nk_context &context, const std::int32_t *ready, const void *image, std::uint32_t colour) {
    struct nk_image fallback {};
    if (!ready || *ready != 1) {
        fallback = UI_GetDefaultImage();
        image = &fallback;
    }
    if (g_waitingForNextTick != 1)
        return;
    struct nk_image drawn {};
    struct nk_color tint {};
    std::memcpy(&drawn, image, sizeof(drawn));
    std::memcpy(&tint, &colour, sizeof(tint));
    nk_image_color(&context, drawn, tint);
}

void UI_DrawViewImage(nk_context &context, std::int32_t index, std::uint32_t colour) {
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!view)
        return;
    const d3t::UIViewImage &image = view->images.data()[index];
    UI_DrawPreparedImage(context, &image.ready, &image.textureHandle, colour);
}

// @verified - p
void UI_DrawScrollListStandardRow(nk_context &context, std::uint32_t index, std::int32_t row) {
    UIScrollList *const list = UI_GetScrollList(index);
    if (!list)
        return;
    const float width = UI_GetReferenceWidth() * list->columnWidth.value.fraction;
    const float height = UI_GetReferenceHeight() * list->rowHeight.value.fraction;
    const float y = static_cast<float>(row) * height + std::bit_cast<float>(list->field3A4);
    if (g_waitingForNextTick == 1)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    const struct nk_rect rect = nk_rect(0.0f, y, width, height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, rect);
    const char *const text = list->texts.data()[row];
    const UILayoutRect labelRect{.x = rect.x, .y = rect.y, .width = rect.w, .height = rect.h};
    const UILabelFit fit =
        UI_FitLabelScale(context, text, -1, 18, labelRect, list->columnWidth.value.fraction * 1920.0f, list->rowHeight.value.fraction * 1080.0f, 1);
    UI_DrawLabelText(context, text, -1, 18, labelRect, fit);
    if (list->field74[1] == row && UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot))) {
        if (list->draws[0].selectedStyle != -1) {
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, rect);
            UI_DrawViewImage(context, list->draws[0].selectedStyle, 0xffffffffU);
        }
        if (list->draws[0].image != -1) {
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, rect);
            UI_DrawViewImage(context, list->draws[0].image, list->draws[0].colours[0]);
        }
    }
    if (g_waitingForNextTick == 1)
        nk_layout_space_end(&context);
}

// @verified - p
void UI_DrawScrollListMapRow(nk_context &context, std::uint32_t index, std::int32_t row) {
    UIScrollList *const list = UI_GetScrollList(index);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!list || !view)
        return;
    d3t::UIWindow *const window = UI_GetWindow(list->window);
    const UIScrollListDraw &draw = list->draws.data()[list->field7C];
    if (!window)
        return;
    UILayoutRect bounds{};
    UI_GetWindowRect(window, bounds);
    const std::uint32_t tint = UI_GetWindowColour(window);
    const struct nk_color previousTextColour = context.style.text.color;
    const std::uint32_t textColour = UI_ModulateColour(list->field74[1] == row ? draw.fontSelectedColour : draw.fontUnselectedColour, tint);
    std::memcpy(&context.style.text.color, &textColour, sizeof(textColour));
    if (g_waitingForNextTick == 1)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    const float height = bounds.height / static_cast<float>(list->rows);
    const UILayoutRect cell{.x = (bounds.width * 0.5f + 0.0f) - (bounds.width * 0.75f * 0.5f),
        .y = (static_cast<float>(row) * height + 0.0f) + std::bit_cast<float>(list->field3A4),
        .width = bounds.width * 0.75f,
        .height = height};
    const struct nk_rect rect = nk_rect(cell.x, cell.y, cell.width, cell.height);
    if (std::strlen(list->texts.data()[row])) {
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, rect);
        const float fitWidth = window->layout.width.value.fraction * 1920.0f * 0.75f;
        const float fitHeight = window->layout.height.value.fraction * 1080.0f / static_cast<float>(list->rows);
        UI_FitAndDrawLabelText(context, GetStringForUILabel(list->texts.data()[row]), -1, 18, cell, fitWidth, fitHeight, 1);
    }
    if (list->field74[1] == row) {
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, rect);
        UI_DrawViewImage(context, draw.image, UI_ModulateColour(draw.colours[0], tint));
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, rect);
        UI_DrawViewImage(context, draw.selectedStyle, tint);
    }
    if (g_waitingForNextTick == 1)
        nk_layout_space_end(&context);
    context.style.text.color = previousTextColour;
}

} // namespace shenmue::s2
