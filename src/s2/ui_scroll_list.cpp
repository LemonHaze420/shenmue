#include "s2/ui_scroll_list.hpp"

#include "s2/ui_loadsave.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/pawn.hpp"
#include "s2/ui.hpp"
#include <cstdio>
#include <bit>
#include <cstring>

namespace shenmue::s2 {

std::array<ScrollListScrollData, 12> g_scrollListScrollData{};
std ::array<std::array<PawnFlyerSprite, 64>, 12> g_pawnFlyerSprites{}

;
UIScrollList *g_pawnFlyerScrollList{};
UITexturePage *g_pawnFlyerTexturePage{};

// @verified - p
PawnFlyerSprite *GetPawnFlyerSprite(std::int32_t row, std::int32_t index) {
    return &g_pawnFlyerSprites.data()[row].data()[index];
}

void RebuildPawnFlyerScrollRows(std::int32_t inspection) {

    if (!g_pawnFlyerScrollList)
        return;
    const std::int32_t total = GetPawnSaleSetCount();
    g_pawnFlyerScrollList->field6C = total;
    const std::int32_t count = total < 12 ? total : 12;
    for (std::int32_t ordinal = 0; ordinal < count; ++ordinal) {
        PawnFlyerSetRow &row = pawnFlyerRows[ordinal];
        const std::int32_t entry = ordinal + g_pawnFlyerScrollList->field74[0];
        const PawnSetRecord *const set = GetPawnOrderedSetDetails(entry, &row.count, &row.price);
        std::snprintf(row.label, sizeof(row.label), "%s", set->label);
        CopyPawnOrderedSetIcons(entry, row.icons.data());
        if (inspection == 1)
            std::memset(row.collected.data(), 1, sizeof(row.collected));
        g_pawnFlyerScrollList->texts[ordinal] = row.label;
        g_pawnFlyerScrollList->words[ordinal] = ordinal;
        for (std ::int32_t item = 0; item < row.count; ++item) {
            PawnFlyerSprite &sprite = g_pawnFlyerSprites[ordinal][item];
            const std::array<std::uint8_t, 4> rect = GetCommonIconRect(static_cast<std::uint16_t>(row.icons[item]));
            sprite.u0 = static_cast<float>(rect[0]) * 0.125f;
            sprite.u1 = static_cast<float>(rect[2]) * 0.125f;
            sprite.v0 = static_cast<float>(rect[1]) * 0.125f;
            sprite.v1 = static_cast<float>(rect[3]) * 0.125f;
            const UITexturePage *const page = g_pawnFlyerTexturePage;
            if (page) {
                for (std ::int32_t image = 0; image < page->count; ++image) {
                    if (row.icons[item] >= page->min.data()[image] && row.icons[item] < page->max.data()[image]) {
                        sprite.image = page->images.data()[image];
                        break;
                    }
                }
            }
            row.dictionary[item] = item;
        }

        // @verified - p
    }
}
d3t::UIViewSlot<kUIViewImageCapacity> *UI_GetViewSlot(std::uint32_t index) {
    return index < g_uiViewSlots.size() ? &g_uiViewSlots[index] : nullptr;
}

// @verified - p
UIScrollList *UI_GetScrollList(std::uint32_t index) {
    return index < g_uiScrollLists.size() ? &g_uiScrollLists[index] : nullptr;
}

struct nk_image UI_GetDefaultImage() {
    return g_uiDefaultImage;
}

// @verified - p
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
float UI_DrawScrollListPawnshopRow(nk_context &context, std::uint32_t index, std::int32_t row, float y, float &scroll, std::int32_t drawRow) {
    UIScrollList *const list = UI_GetScrollList(index);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
    if (!list || !view)
        return y;
    const UIScrollListDraw &draw = list->draws[0];
    d3t::UIWindow *const window = UI_GetWindow(list->window);
    if (!window)
        return y;
    const PawnFlyerSetRow data = GetPawnFlyerSetRow(list->words.data()[row]);
    UILayoutRect bounds{};
    UI_GetWindowRect(window, bounds);
    float extraRows = 0.0f;
    if (static_cast<float>(data.count) / 9.0f > 2.0f) {
        extraRows = static_cast<float>(data.count / 9 - 2);
        if (data.count % 9 > 0)
            extraRows = extraRows + 1.0f;
    }
    const struct nk_color savedTextColour = context.style.text.color;
    const float rowFraction = 1.0f / (extraRows + 2.0f);
    const std::uint32_t tint = UI_GetWindowColour(window);
    std::memcpy(&context.style.text.color, &tint, sizeof(tint));
    if (drawRow == 1 && g_waitingForNextTick == 1)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    const float baseY = y + scroll;
    const float baseHeight = bounds.height / static_cast<float>(list->rows);
    float cornerWidth = bounds.width * 0.5f, cornerHeight = baseHeight * 0.5f;
    if (draw.selectedStyle != -1) {
        const d3t::UIViewImage &corner = view->images.data()[draw.selectedStyle];
        UI_ScaleSize(
            cornerWidth, cornerHeight, 2, bounds.width * 0.5f, baseHeight * 0.5f, static_cast<float>(corner.width), static_cast<float>(corner.height), nullptr);
    }
    const float top = baseY + std::bit_cast<float>(list->field3A4);
    const float height = baseHeight + (baseHeight * 0.5f) * extraRows;
    if (draw.field108 != 0 && list->field74[1] == row) {
        const float bottom = height + top;
        if (bottom > bounds.height) {
            scroll = scroll - (bottom - bounds.height);
        } else {
            const float first = bounds.y + top;
            if (bounds.y > first)
                scroll = (bounds.y - first) + scroll;
            else if (list->field74[1] == list->rows - 1 && bounds.height > bottom)
                scroll = (bounds.height - bottom) + scroll;
            else if (list->field74[1] == 0 && first > bounds.y)
                scroll = scroll - (first - bounds.y);
        }
        if (draw.selectedStyle != -1 && drawRow == 1) {
            d3t::UIViewImage &image = view->images.data()[draw.selectedStyle];
            image.rotate = 1;
            image.angle = 0.0f;
            const float left = (bounds.width * 0.5f + 0.0f) - bounds.width * 0.5f;
            UILayoutRect corner{.x = left, .y = baseY, .width = cornerWidth, .height = cornerHeight};
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(corner.x, corner.y, corner.width, corner.height));
            UI_DrawViewImage(context, draw.selectedStyle, 0xffffffffU);
            image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
            corner.x = (bounds.width - cornerWidth) + left;
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(corner.x, corner.y, corner.width, corner.height));
            UI_DrawViewImage(context, draw.selectedStyle, 0xffffffffU);
            image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
            corner.y = (height - cornerHeight) + baseY;
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(corner.x, corner.y, corner.width, corner.height));
            UI_DrawViewImage(context, draw.selectedStyle, 0xffffffffU);
            image.angle = image.angle + UI_IMAGE_CORNER_ANGLE;
            corner.x = left;
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(corner.x, corner.y, corner.width, corner.height));
            UI_DrawViewImage(context, draw.selectedStyle, 0xffffffffU);
        }
        if (draw.image != -1 && drawRow == 1) {
            const UILayoutRect background{.x = (bounds.width * 0.5f + 0.0f) - bounds.width * 0.5f, .y = top, .width = bounds.width, .height = height};
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(background.x, background.y, background.width, background.height));
            UI_DrawViewImage(context, draw.image, draw.colours[0]);
        }
    }
    std::array<char, 32> price;
    std::sprintf(price.data(), "$%d", data.price);
    float priceWidth = 0.0f, priceHeight = 0.0f;
    UI_ScaleSize(priceWidth, priceHeight, 0, bounds.width, height, draw.size20[1][0] * 1920.0f, draw.size20[1][1] * 1080.0f, nullptr);
    const float itemHeight = height * rowFraction;
    const float itemWidth = bounds.width * 0.75f;
    const float itemTop = (itemHeight - itemHeight * 0.85f) * 0.5f + top;
    const std::int32_t lines = (data.count + 8) / 9;
    std::int32_t ordinal = 0;
    bool collected = true;
    for (std::int32_t line = 0; line < lines; ++line) {
        for (std::int32_t column = 0; column < 9; ++column) {
            if (ordinal >= data.count)
                break;
            float width = 0.0f, fittedHeight = 0.0f;
            UI_ScaleSize(width, fittedHeight, 2, itemWidth, itemHeight * 0.85f, draw.size20[2][0] * 1920.0f, draw.size20[2][1] * 1080.0f, nullptr);
            if (drawRow == 1) {
                const UILayoutRect cell{
                    .x = (bounds.width - itemWidth) * 0.5f + (bounds.width * 0.5f + 0.0f - bounds.width * 0.5f) + (width + 5.0f) * static_cast<float>(column),
                    .y = static_cast<float>(line) * itemHeight + itemTop,
                    .width = width,
                    .height = fittedHeight};
                if (g_waitingForNextTick == 1)
                    nk_layout_space_push(&context, nk_rect(cell.x, cell.y, cell.width, cell.height));
                const PawnFlyerSprite &sprite = *GetPawnFlyerSprite(row, data.dictionary[ordinal]);
                d3t::UIViewImage &image = view->images.data()[sprite.image];
                struct nk_image cropped {};
                std::memcpy(&cropped, &image.textureHandle, sizeof(cropped));
                const float left = static_cast<float>(cropped.w) * sprite.u0;
                const float cropWidth = static_cast<float>(cropped.w) * sprite.u1 - left;
                const float imageHeight = static_cast<float>(cropped.h);
                const float upper = imageHeight - imageHeight * sprite.v1;
                const float cropHeight = (imageHeight - imageHeight * sprite.v0) - upper;
                cropped.region[0] = static_cast<std::uint16_t>(static_cast<std::int64_t>(left));
                cropped.region[1] = static_cast<std::uint16_t>(static_cast<std::int64_t>(upper));
                cropped.region[2] = static_cast<std::uint16_t>(static_cast<std::int64_t>(cropWidth));
                cropped.region[3] = static_cast<std::uint16_t>(static_cast<std::int64_t>(cropHeight));
                std::memcpy(&image.textureHandle, &cropped, sizeof(cropped));
                std::uint32_t colour = tint;
                if (data.collected[ordinal] == 0) {
                    collected = false;
                    const std::uint32_t alpha = static_cast<std::uint32_t>(static_cast<float>(static_cast<std::uint8_t>(tint >> 24U)) * 0.1f);
                    colour = (colour & 0x00ffffffU) | (alpha << 24U);
                }
                d3t::UIViewSlot<kUIViewImageCapacity> *const drawnView = UI_GetViewSlot(static_cast<std::uint32_t>(g_uiDrawViewSlot));
                if (drawnView) {
                    const d3t::UIViewImage &drawnImage = drawnView->images.data()[sprite.image];
                    UI_DrawPreparedImage(context, &drawnImage.ready, &drawnImage.textureHandle, colour);
                }
            }
            ++ordinal;
        }
    }
    if (lines > 0 && !collected)
        context.style.text.color.a = static_cast<nk_byte>(static_cast<std::int64_t>(static_cast<float>(context.style.text.color.a) * 0.1f));
    const float labelHeight = (draw.size20[1][1] * 1080.0f) / static_cast<float>(list->rows);
    if (drawRow == 1) {
        const float labelY = labelHeight + top;
        const float left = (bounds.width * 0.5f + 0.0f) - bounds.width * 0.5f;
        const UILayoutRect label{.x = left, .y = labelY, .width = bounds.width, .height = height};
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, nk_rect(label.x, label.y, label.width, label.height));
        const char *const text = list->texts.data()[row];
        const UILabelFit fit = UI_FitLabelScale(context, text, -1, 4, label, window->layout.width.value.fraction * 1920.0f, labelHeight, 1);
        UI_DrawLabelText(context, text, -1, 4, label, fit);
        const UILayoutRect amount{.x = left, .y = labelY, .width = priceWidth, .height = height};
        if (g_waitingForNextTick == 1)
            nk_layout_space_push(&context, nk_rect(amount.x, amount.y, amount.width, amount.height));
        const UILabelFit priceFit = UI_FitLabelScale(context, price.data(), -1, 2, amount, draw.size20[1][0] * 1920.0f, labelHeight, 1);
        UI_DrawLabelText(context, price.data(), -1, 2, amount, priceFit);

        if (g_waitingForNextTick == 1)
            nk_layout_space_end(&context);
    }
    context.style.text.color = savedTextColour;
    return y + height;
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
        UI_FitAndDrawLabelText(context, GetStringForUILabel("UILOADEMPT" /* @text UILOADEMPT EN: "Empty space" | JA: "空きスペース" */), -1, 18, cell,
            fitWidth * draw.size20[2][0], fitHeight * draw.size20[2][1], 1);
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
