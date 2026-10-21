#include "s2/ui_scroll_list.hpp"

#include "s2/ui_loadsave.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {

std::array<ScrollListScrollData, 12> g_scrollListScrollData{};
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
d3t::UISpriteSheet *UI_GetSpriteSheet(std::uint32_t index) {
    return index < g_uiSpriteSheets.size() ? &g_uiSpriteSheets[index] : nullptr;
}

// @verified - p
ScrollListScrollData GetScrollListScrollData(std::int32_t index) {
    return g_scrollListScrollData.data()[index];
}

// @verified - p
void UI_DrawImageCorners(nk_context *context, const UILayoutRect &bounds, std::int32_t index, std::uint32_t colour) {
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!view || !context)
        return;
    d3t::UIViewImage &image = view->images.data()[index];
    image.rotate = 1;
    UILayoutRect corner{.x = bounds.x, .y = bounds.y, .width = 0.0f, .height = 0.0f};
    UI_ScaleSize(
        corner.width, corner.height, 2, bounds.width * 0.5f, bounds.height * 0.5f, static_cast<float>(image.width), static_cast<float>(image.height), nullptr);
    image.angle = 0.0f;
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(context, nk_rect(corner.x, corner.y, corner.width, corner.height));
    {
        d3t::UIViewSlot<kUIViewImageCapacity> *const drawnView = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
        if (drawnView) {
            const d3t::UIViewImage &drawnImage = drawnView->images.data()[index];
            UI_DrawPreparedImage(*context, &drawnImage.ready, &drawnImage.textureHandle, colour);
        }
    }
    image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
    corner.y = bounds.y;
    corner.x = (bounds.width * 0.5f + bounds.x) + (bounds.width * 0.5f - corner.width);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(context, nk_rect(corner.x, corner.y, corner.width, corner.height));
    {
        d3t::UIViewSlot<kUIViewImageCapacity> *const drawnView = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
        if (drawnView) {
            const d3t::UIViewImage &drawnImage = drawnView->images.data()[index];
            UI_DrawPreparedImage(*context, &drawnImage.ready, &drawnImage.textureHandle, colour);
        }
    }
    image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
    corner.x = (bounds.width * 0.5f + bounds.x) + (bounds.width * 0.5f - corner.width);
    corner.y = (bounds.height * 0.5f + bounds.y) + (bounds.height * 0.5f - corner.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(context, nk_rect(corner.x, corner.y, corner.width, corner.height));
    {
        d3t::UIViewSlot<kUIViewImageCapacity> *const drawnView = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
        if (drawnView) {
            const d3t::UIViewImage &drawnImage = drawnView->images.data()[index];
            UI_DrawPreparedImage(*context, &drawnImage.ready, &drawnImage.textureHandle, colour);
        }
    }
    image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
    corner.x = bounds.x;
    corner.y = (bounds.height * 0.5f + bounds.y) + (bounds.height * 0.5f - corner.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(context, nk_rect(corner.x, corner.y, corner.width, corner.height));
    {
        d3t::UIViewSlot<kUIViewImageCapacity> *const drawnView = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
        if (drawnView) {
            const d3t::UIViewImage &drawnImage = drawnView->images.data()[index];
            UI_DrawPreparedImage(*context, &drawnImage.ready, &drawnImage.textureHandle, colour);
        }
    }
}

// @verified - p
void UI_DrawScrollListScrollRow(nk_context &context, std::uint32_t index, std::int32_t row, UILayoutRect &bounds) {
    const float height = bounds.height;
    const float fittedHeight = height * 0.95f;
    const float margin = height - fittedHeight;
    bounds.height = fittedHeight;
    bounds.y = margin * 0.5f + bounds.y;
    bounds.x = margin * 0.5f + bounds.x;
    bounds.width = bounds.width - margin;
    UIScrollList *const list = UI_GetScrollList(index);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!list || !view)
        return;
    const UIScrollListDraw &draw = list->draws.data()[list->field7C];
    d3t::UIWindow *const window = UI_GetWindow(list->window);
    if (!window)
        return;
    const d3t::UISpriteSheet *const sheet = UI_GetSpriteSheet(static_cast<std::uint32_t>(draw.image));
    if (!sheet)
        return;
    const std::uint32_t tint = UI_GetWindowColour(window);
    const struct nk_color savedTextColour = context.style.text.color;
    std::memcpy(&context.style.text.color, &tint, sizeof(tint));
    const struct nk_style_button savedButton = context.style.button;
    std::memcpy(&context.style.button.text_hover, &tint, sizeof(tint));
    std::memcpy(&context.style.button.text_normal, &tint, sizeof(tint));
    std::memcpy(&context.style.button.text_background, &tint, sizeof(tint));
    const ScrollListScrollData data = GetScrollListScrollData(list->words.data()[row]);
    if (g_waitingForNextTick == 1)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    const UILayoutRect textBounds = bounds;
    d3t::UIViewImage &image = view->images.data()[sheet->image];
    const std::array<float, 2> &minimum = sheet->frameMin.data()[data.frame];
    const std::array<float, 2> &maximum = sheet->frameMax.data()[data.frame];
    image.region[0] = static_cast<std::uint16_t>(static_cast<std::int64_t>(minimum[0]));
    image.region[1] = static_cast<std::uint16_t>(static_cast<std::int64_t>(minimum[1]));
    image.region[2] = static_cast<std::uint16_t>(static_cast<std::int64_t>(maximum[0] - minimum[0]));
    image.region[3] = static_cast<std::uint16_t>(static_cast<std::int64_t>(maximum[1] - minimum[1]));
    const float gap = bounds.width * 0.01f;
    UILayoutRect icon{.x = bounds.x, .y = bounds.y, .width = 0.0f, .height = 0.0f};
    UI_ScaleSize(
        icon.width, icon.height, 2, bounds.width, bounds.height * 0.8f, static_cast<float>(image.region[2]), static_cast<float>(image.region[3]), nullptr);
    UILayoutRect label = textBounds;
    label.x = textBounds.x + icon.width * 0.5f;
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, nk_rect(label.x, label.y, label.width, label.height));
    label = UI_FitAndDrawLabelText(context, data.text, -1, 18, label, textBounds.width, textBounds.height, 1);
    icon.y = (label.height * 0.5f + label.y) - icon.height * 0.5f;
    icon.x = (label.x - gap * 0.5f) - icon.width;
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, nk_rect(icon.x, icon.y, icon.width, icon.height));
    UI_DrawViewImage(context, sheet->image, tint);
    if (list->field74[1] == row) {
        const UILayoutRect selected{.x = (icon.x - gap * 0.5f) - (bounds.width * 0.05f) * 0.5f,
            .y = bounds.y,
            .width = ((gap + gap) + (label.width + icon.width)) + bounds.width * 0.05f,
            .height = bounds.height};
        UI_DrawImageCorners(&context, selected, draw.images[0], tint);
    }
    if (g_waitingForNextTick == 1)
        nk_layout_space_end(&context);
    context.style.text.color = savedTextColour;
    context.style.button = savedButton;
}

// @verified - p
void UI_DrawScrollListSaveRow(nk_context &context, std::uint32_t index, std::int32_t row, float height) {
    UIScrollList *const list = UI_GetScrollList(index);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!list || !view)
        return;
    d3t::UIWindow *const window = UI_GetWindow(list->window);
    const UIScrollListDraw &draw = list->draws.data()[list->field7C];
    if (!window)
        return;
    const SaveDataInfoText *const text = GetSaveDataInfoText(static_cast<std::uint32_t>(row));
    UILayoutRect bounds{};
    UI_GetWindowRect(window, bounds);
    const float fitWidth = window->layout.width.value.fraction * 1920.0f;
    const float fitHeight = window->layout.height.value.fraction * 1080.0f;
    const float y = static_cast<float>(row) * height + std::bit_cast<float>(list->field3A4);
    const struct nk_color savedTextColour = context.style.text.color;
    const std::uint32_t tint = UI_GetWindowColour(window);
    std::uint32_t textColour = tint;
    if (list->field74[1] != row) {
        const std::int32_t difference = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(row) - static_cast<std::uint32_t>(list->field74[1]));
        const std::uint32_t distance = (difference < 0 ? 0U - static_cast<std::uint32_t>(difference) : static_cast<std::uint32_t>(difference)) - 1U;
        std::array<std::uint8_t, 4> font{}, offset{};
        std::memcpy(font.data(), &draw.fontUnselectedColour, sizeof(font));
        std::memcpy(offset.data(), &draw.offsetColour, sizeof(offset));
        const std::uint32_t faded = ConstructColour(static_cast<std::int32_t>(font[0] - distance * offset[0]),
            static_cast<std::int32_t>(font[1] - distance * offset[1]), static_cast<std::int32_t>(font[2] - distance * offset[2]),
            static_cast<std::int32_t>(offset[3] - distance * static_cast<std::uint32_t>(static_cast<float>(offset[3]) * 0.25f)));
        textColour = UI_ModulateColour(faded, tint);
    }
    std::memcpy(&context.style.text.color, &textColour, sizeof(textColour));
    if (text && text->valid != 0) {
        if (g_waitingForNextTick == 1)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 7);
        const std::array<const char *, 5> fields{
            text->gameTime.data(), text->gameDate.data(), text->location.data(), text->saveTime.data(), text->saveDate.data()};
        for (std::size_t field = 0; field < fields.size(); ++field) {
            if (field == 3 && list->field74[1] == row) {
                const std::uint32_t selectedColour = UI_ModulateColour(draw.fontSelectedColour, tint);
                std::memcpy(&context.style.text.color, &selectedColour, sizeof(selectedColour));
            }
            const std::uint32_t font = static_cast<std::uint32_t>(draw.sizes[field]);
            if (g_uiFontSize != font && font < 6) {
                g_uiFontSize = static_cast<std::int32_t>(font);
                UI_SelectFont();
            }
            const UILayoutRect cell{.x = bounds.width * draw.place60[field][0],
                .y = bounds.height * draw.place60[field][1] + y,
                .width = bounds.width * draw.size20[field][0],
                .height = bounds.height * draw.size20[field][1]};
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(cell.x, cell.y, cell.width, cell.height));
            UI_FitAndDrawLabelText(context, fields[field], -1, 18, cell, fitWidth * draw.size20[field][0], fitHeight * draw.size20[field][1], 1);
        }
    } else {
        if (g_uiFontSize != 2) {
            g_uiFontSize = 2;
            UI_SelectFont();
        }
        if (g_waitingForNextTick == 1)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
        const UILayoutRect cell{.x = bounds.width * draw.place60[2][0],
            .y = bounds.height * draw.place60[2][1] + y,
            .width = bounds.width * draw.size20[2][0],
            .height = bounds.height * draw.size20[2][1]};
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, nk_rect(cell.x, cell.y, cell.width, cell.height));
        UI_FitAndDrawLabelText(context, GetStringForUILabel("UILOADEMPT"), -1, 18, cell, fitWidth * draw.size20[2][0], fitHeight * draw.size20[2][1], 1);
    }
    if (draw.image != -1) {
        const float imageHeight = bounds.height * view->images.data()[draw.image].heightScale;
        const UILayoutRect cell{
            .x = 50.0f, .y = (height - imageHeight * 0.5f) + bounds.height * y, .width = bounds.width * draw.field11C, .height = imageHeight};
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, nk_rect(cell.x, cell.y, cell.width, cell.height));
        UI_DrawViewImage(context, draw.image, tint);
    }
    if (g_waitingForNextTick == 1)
        nk_layout_space_end(&context);
    context.style.text.color = savedTextColour;
}
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
