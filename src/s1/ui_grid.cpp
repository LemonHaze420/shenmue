#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
void UIGrid_GetCellRect(UIGrid *grid, UILayoutRect *holder, UILayoutRect *cell, std::int32_t row, std::int32_t column);

float UIGrid_RowOffset(UIGrid *grid, std::int32_t row);

std::int32_t UIGrid_MaxScrollRow(UIGrid &grid);

float UIGrid_ScrollSlotValue(const UIGrid &grid);

void UIGrid_ScrollBarRects(UIGrid *grid, UILayoutRect *window, UILayoutRect *track, UILayoutRect *thumb);

void UIGrid_DrawScrollBar(UIGrid *grid, std::int32_t element);

void UIGrid_HandlePadInput(UIGrid &grid);

void UIGrid_UpdateScroll(UIGrid &grid);

void UIFocusList_DescribeEntry(d3t::UIWindow *window, const d3t::UILabelWidget *label);



void UI_RestartCurrentGridSelection() {
    if (g_uiCurrentGrid) {
        g_uiCurrentGrid->previousSelectedRow = -1;
        g_uiCurrentGrid->previousSelectedColumn = -1;
        g_uiCurrentGrid->selectedRow = 0;
        g_uiCurrentGrid->selectedColumn = 0;
        g_uiCurrentGrid->changed = 1;
    }
}



const UIGridStyle &UIGrid_Style(const UIGrid *grid) {
    if (grid && grid->style)
        return *grid->style;
    return g_isInGame ? kUIGridInGameStyle : kUIGridFrontEndStyle;
}





void UIGrid_GetCellRect(UIGrid *grid, UILayoutRect *holder, UILayoutRect *cell, std::int32_t row, std::int32_t column) {
    const UIGridStyle &style = UIGrid_Style(grid);
    UILayoutRect rect{};
    if (grid->holder)
        UI_GetWindowRect(grid->holder, rect);
    else
        rect = grid->rect;
    UILayoutRect size{};
    if (cell) {
        const float safeHeight = UI_SafeAreaScale() * 1080.0f;
        const float gap = safeHeight * style.scrollBarGap;
        const float barWidth = safeHeight * style.scrollBarWidth;
        const float width = rect.width - barWidth - gap;
        const float spacing = WithinSafeArea(style.spacing);
        size.width = (width - static_cast<float>(grid->columns - 1) * spacing) / static_cast<float>(grid->columns);
        size.height = (rect.height - static_cast<float>(grid->visibleRows - 1) * spacing) / static_cast<float>(grid->visibleRows);
        if (row != -1 && column != -1 && grid->rowHeight)
            size.height = size.height * static_cast<float>(grid->rowHeight(grid, column + (row < 0 ? 0 : row) * grid->columns));
    }
    if (holder)
        *holder = rect;
    if (cell)
        *cell = size;
}



float UIGrid_RowOffset(UIGrid *grid, std::int32_t row) {
    const float spacing = WithinSafeArea(UIGrid_Style(grid).spacing);
    UILayoutRect holder{};
    UILayoutRect cell{};
    UIGrid_GetCellRect(grid, &holder, &cell, row, 0);
    if (!grid->rowHeight)
        return (cell.height + spacing) * static_cast<float>(row);
    const float rowHeight = holder.height / static_cast<float>(grid->visibleRows);
    float offset = 0.0f;
    for (std::int32_t index = 0; index < row; ++index) {
        UIGrid_GetCellRect(grid, nullptr, &cell, index, 0);
        offset = offset + (std::roundf(cell.height / rowHeight) * spacing + cell.height);
    }
    return offset - spacing;
}


UILayoutRect UIGrid_GetSelectedCellRect(UIGrid *grid) {
    const float spacing = WithinSafeArea(UIGrid_Style(grid).spacing);
    UILayoutRect holder{};
    UILayoutRect cell{};
    UIGrid_GetCellRect(grid, &holder, &cell, -1, -1);
    cell.x = (spacing + cell.width) * static_cast<float>(grid->selectedColumn);
    cell.y = UIGrid_RowOffset(grid, grid->selectedRow) + holder.y;
    cell.x = holder.x + cell.x;
    return cell;
}


std::int32_t UIGrid_SelectedIndex(const UIGrid &grid) {
    if (g_uiFocusedList && (g_uiFocusedList->hasFocus || g_uiFocusedList->previousHasFocus))
        return -1;
    const std::int32_t row = grid.scrollRow + grid.selectedRow < 0 ? 0 : grid.scrollRow + grid.selectedRow;
    return grid.selectedColumn + grid.columns * row;
}



std::int32_t UIGrid_MaxScrollRow(UIGrid &grid) {
    const std::int32_t visibleRows = grid.visibleRows;
    std::int32_t row = grid.rows <= visibleRows ? 0 : grid.rows - visibleRows;
    if (grid.columns * (row < 0 ? 0 : row) > grid.elementCount)
        row = grid.elementCount / grid.columns - visibleRows + 1;
    if (!grid.showEmptyCells) {
        const std::int32_t shown = visibleRows + row < 0 ? 0 : visibleRows + row;
        if (grid.columns * shown > grid.elementCount) {
            const float rows = static_cast<float>(grid.elementCount) / static_cast<float>(grid.columns) - static_cast<float>(visibleRows);
            row = static_cast<std::int32_t>(rows);
            if (rows - static_cast<float>(static_cast<std::int32_t>(rows)) > 0.0f)
                ++row;
        }
    }
    if (grid.rowHeight) {
        const UIGridStyle &style = UIGrid_Style(&grid);
        UILayoutRect holder{};
        UILayoutRect cell{};
        float height = 0.0f;
        row = grid.rows - 1;
        for (std::int32_t index = grid.rows - 1; index >= 0; row = index--) {
            UIGrid_GetCellRect(&grid, &holder, &cell, index, 0);
            height = height + (WithinSafeArea(style.spacing) + cell.height);
            if (height > holder.height)
                break;
        }
    }
    return row >= 0 ? row : 0;
}


float UIGrid_ScrollSlotValue(const UIGrid &grid) {
    return static_cast<std::uint32_t>(grid.animSlot) < g_uiAnimSlots.size() ? g_uiAnimSlots[static_cast<std::size_t>(grid.animSlot)].current[0] : 0.0f;
}




void UIGrid_ScrollBarRects(UIGrid *grid, UILayoutRect *window, UILayoutRect *track, UILayoutRect *thumb) {
    const UIGridStyle &style = UIGrid_Style(grid);
    const float safeHeight = UI_SafeAreaScale() * 1080.0f;
    const float barWidth = safeHeight * style.scrollBarWidth;
    const float gap = safeHeight * style.scrollBarGap;
    UILayoutRect holder{};
    if (grid->holder)
        UI_GetWindowRect(grid->holder, holder);
    else
        holder = grid->rect;
    static_cast<void>(WithinSafeArea(style.spacing));
    const float span = barWidth + gap;
    const float trackHeight = holder.height - 2.0f;
    const UILayoutRect windowRect{.x = holder.width + holder.x - span, .y = holder.y, .width = span, .height = holder.height};
    const UILayoutRect trackRect{.x = gap - 1.0f, .y = 1.0f, .width = barWidth - 1.0f, .height = trackHeight};
    std::int32_t lastRow = grid->rows - 1;
    if (grid->columns * (lastRow < 0 ? 0 : lastRow) > grid->elementCount)
        lastRow = (grid->elementCount - 1) / grid->columns;
    const std::int32_t rows = lastRow + 1;
    const float rowsHeight = UIGrid_RowOffset(grid, rows);
    static_cast<void>(UIGrid_RowOffset(grid, grid->visibleRows));
    float thumbHeight = trackHeight;
    if (grid->rowHeight) {
        if (rows > grid->visibleRows)
            thumbHeight = holder.height / rowsHeight * trackHeight;
    } else if (rows > grid->visibleRows) {
        thumbHeight = static_cast<float>(grid->visibleRows) / static_cast<float>(rows) * trackHeight;
    }
    thumbHeight = std::fmaxf(barWidth - 1.0f, thumbHeight);
    float thumbY = 1.0f;
    if (grid->dragging == 1 || rowsHeight > 0.0f) {
        if (grid->dragging == 1) {
            static_cast<void>(UIGrid_RowOffset(grid, rows));
            thumbY = GNuklearWindow.input.mouse.pos.y - grid->dragOffset - holder.y;
        } else {
            thumbY = UIGrid_ScrollSlotValue(*grid) / rowsHeight * trackHeight + 1.0f;
        }
        if (thumbY < 1.0f)
            thumbY = 1.0f;
        else if (thumbY + thumbHeight > trackHeight)
            thumbY = trackHeight - thumbHeight + 1.0f;
    }
    if (window)
        *window = windowRect;
    if (track)
        *track = trackRect;
    if (thumb)
        *thumb = {.x = gap - 1.0f, .y = thumbY, .width = barWidth - 1.0f, .height = thumbHeight};
}




void UIGrid_DrawScrollBar(UIGrid *grid, std::int32_t element) {
    nk_context &context = GNuklearWindow;
    const UIGridStyle &style = UIGrid_Style(grid);
    UILayoutRect window{};
    UILayoutRect track{};
    UILayoutRect thumb{};
    UIGrid_ScrollBarRects(grid, &window, &track, &thumb);
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "Scroll %d", element);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(
            &context, &g_uiGridScrollBarPanel, name.data(), name.data(), nk_rect(window.x, window.y, window.width, window.height), NK_WINDOW_NO_SCROLLBAR);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    }
    const auto place = [&context](const UILayoutRect &rect, float rounding) {
        context.style.button.rounding = ScaleTo1080WithSafeArea(nk_rect(rect.x, rect.y, rect.width, rect.height), rounding);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
    };
    const auto button = [&context](std::uint32_t colour) {
        if (g_waitingForNextTick != 1)
            return;
        nk_color tint{};
        std::memcpy(&tint, &colour, sizeof(colour));
        nk_button_color(&context, tint);
    };
    place(track, style.trackRounding);
    const float border = WithinSafeArea(style.border);
    const float thumbWidth = thumb.width - border * 4.0f;
    const float half = thumbWidth * 0.5f;
    context.style.button.border = border;
    std::memcpy(&context.style.button.border_color, &style.trackBorderColour, sizeof(style.trackBorderColour));
    button(style.trackColour);
    context.style.button.border = 0.0f;
    context.style.button.border_color = nk_rgba(0, 0, 0, 0);
    const UILayoutRect line{
        .x = track.width * 0.45f + track.x, .y = track.y + half, .width = (track.width - border) * 0.5f, .height = track.height - (half + half)};
    place(line, style.rounding);
    const std::uint32_t lineAlpha = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(style.trackLineColour >> 24) * 0.4f));
    button((style.trackLineColour & 0x00ffffffU) | (lineAlpha & 0xffU) << 24);
    place({.x = line.x + line.width * 0.55f, .y = line.y, .width = line.width * 0.45f, .height = line.height}, style.rounding);
    button(style.trackLineColour);
    const float x = border * 2.0f + thumb.x;
    const float y = border * 2.0f + thumb.y;
    const float height = thumb.height - border * 4.0f;
    place({.x = x, .y = y, .width = thumbWidth, .height = height}, style.rounding);
    button(style.thumbColour);
    place({.x = thumbWidth * 0.45f + x, .y = y + half, .width = thumbWidth * 0.55f, .height = height - (half + half)}, style.rounding);
    button(style.thumbCentreColour);
    const std::uint32_t grip = (style.thumbColour & 0x00ffffffU) | 105U << 24;
    place({.x = x, .y = y + 1.0f, .width = thumbWidth, .height = thumbWidth * 0.7f}, style.rounding);
    button(grip);
    place({.x = x, .y = height + y - thumbWidth * 0.7f - 1.0f, .width = thumbWidth, .height = thumbWidth * 0.7f}, style.rounding);
    button(grip);
    place({.x = x, .y = y, .width = thumbWidth * 0.55f, .height = height}, style.rounding * 0.5f);
    button(style.thumbLineColour);
    place({.x = x, .y = y + 1.0f, .width = thumbWidth, .height = thumbWidth * 0.4f}, style.rounding);
    button(grip);
    place({.x = x, .y = height + y - thumbWidth * 0.4f - 1.0f, .width = thumbWidth, .height = thumbWidth * 0.4f}, style.rounding);
    button(grip);
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = 0.0f;
            row.item_height = 0.0f;
            row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    context.style.button.rounding = 0.0f;
}
}

namespace shenmue::s1::d3t::ui_runtime_detail {


void *cbGetMenuElementPointer(void *elements, std::int32_t index) {
    return elements ? static_cast<UIGridEntry *>(elements) + static_cast<std::uint32_t>(index) : nullptr;
}








void DrawGridItem(UIGrid *grid, std::int32_t element) {
    nk_context &context = GNuklearWindow;
    g_uiCurrentGrid = grid;
    const UIGridStyle &style = UIGrid_Style(grid);
    const float spacing = WithinSafeArea(style.spacing);
    UILayoutRect holder{};
    UILayoutRect cell{};
    UIGrid_GetCellRect(grid, &holder, &cell, -1, -1);
    context.style.button.rounding = ScaleTo1080WithSafeArea(nk_rect(cell.x, cell.y, cell.width, cell.height), style.rounding);
    std::memcpy(&context.style.window.fixed_background, style.fixedBackground.data(), sizeof(nk_style_item));
    if (g_uiFontSize != style.fontSize && static_cast<std::uint32_t>(style.fontSize) < 6U) {
        g_uiFontSize = style.fontSize;
        UI_SelectFont();
    }
    std::memcpy(&context.style.text.color, &style.textColour, sizeof(style.textColour));
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "Grid %d", element);
    const auto moved = [grid] {
        return grid->previousScrollRow != grid->scrollRow || grid->previousSelectedRow != grid->selectedRow ||
            grid->previousSelectedColumn != grid->selectedColumn;
    };
    if (moved() || grid->confirmRequested == 1 || grid->changed == 1) {
        if (!g_uiFocusedList || (!g_uiFocusedList->hasFocus && !g_uiFocusedList->previousHasFocus)) {
            const std::int32_t row = grid->selectedRow + grid->scrollRow;
            const std::int32_t index = grid->selectedColumn + grid->columns * (row < 0 ? 0 : row);
            if (index >= 0) {
                void *data = nullptr;
                if (index < grid->elementCount && grid->elements && grid->getElement)
                    data = grid->getElement(grid->elements, index);
                const auto queue = [element, data](UIMouseCallbackQueue &target, d3t::UIMouseCallback callback, const UILayoutRect &rect) {
                    if (element == -1 || !callback || static_cast<std::uint32_t>(target.count) >= static_cast<std::uint32_t>(kUIMouseCallbackQueueLimit))
                        return;
                    target.entries[static_cast<std::size_t>(target.count)] = {
                        .element = element, .data = data, .rect = {rect.x, rect.y, rect.width, rect.height}, .callback = callback};
                    ++target.count;
                };
                if (moved() || grid->changed == 1)
                    queue(g_uiMouseCallbackQueues[0], grid->mouseCallbacks.entered, UIGrid_GetSelectedCellRect(grid));
                if (grid->confirmRequested == 1) {
                    queue(g_uiMouseCallbackQueues[3], grid->mouseCallbacks.clicked, UIGrid_GetSelectedCellRect(grid));
                    grid->confirmRequested = 0;
                }
            }
        }
        if (grid->changed == 1)
            grid->changed = 0;
    }
    if (g_waitingForNextTick == 1)
        nk_begin_titled(&context, &g_uiGridPanel, name.data(), name.data(), nk_rect(holder.x, holder.y, holder.width, holder.height), NK_WINDOW_NO_SCROLLBAR);
    if (grid->elementCount > 0 || grid->showEmptyCells) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, grid->elementCount);
        const struct nk_mouse &mouse = context.input.mouse;
        for (std::int32_t row = 0; row < grid->rows; ++row) {
            const float top = UIGrid_RowOffset(grid, row) - grid->scrollOffset;
            cell.y = top;
            if (cell.height + top < 0.0f)
                continue;
            if (top > holder.height)
                break;
            for (std::int32_t column = 0; column < grid->columns; ++column) {
                UIGrid_GetCellRect(grid, &holder, &cell, row, column);
                cell.y = UIGrid_RowOffset(grid, row) - grid->scrollOffset;
                const std::int32_t index = column + grid->columns * (row < 0 ? 0 : row);
                void *elements = grid->elements;
                if (index >= grid->elementCount) {
                    if (!grid->showEmptyCells)
                        break;
                    elements = nullptr;
                }
                cell.x = (cell.width + spacing) * static_cast<float>(column);
                if (grid->hovered && (mouse.pos.x != mouse.prev.x || mouse.pos.y != mouse.prev.y)) {
                    const float left = cell.x + holder.x;
                    const float cellTop = cell.y + holder.y;
                    const bool idle =
                        static_cast<std::uint32_t>(grid->animSlot) < g_uiAnimSlots.size() && !g_uiAnimSlots[static_cast<std::size_t>(grid->animSlot)].status;
                    if (mouse.pos.x > left && left + cell.width > mouse.pos.x && mouse.pos.y > cellTop && cellTop + cell.height > mouse.pos.y && idle) {
                        const std::int32_t selectedRow = grid->selectedRow;
                        const std::int32_t selectedColumn = grid->selectedColumn;
                        if (index < grid->elementCount) {
                            grid->selectedRow = index / grid->columns - grid->scrollRow;
                            grid->selectedColumn = index % grid->columns;
                        }
                        if (selectedRow != grid->selectedRow || selectedColumn != grid->selectedColumn)
                            UI_PlaySound(grid->navigateSound);
                    }
                }
                if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                    context.current->layout->row.item = nk_rect(cell.x, cell.y, cell.width, cell.height);
                if (grid->drawCell)
                    grid->drawCell(grid, &g_uiGridPanel, &holder, &cell, spacing, element, elements, index);
            }
        }
        if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = 0.0f;
            row.item_height = 0.0f;
            row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    UIGrid_DrawScrollBar(grid, element);
    context.style.button.rounding = 0.0f;
    grid->hovered = 0;
}
}

namespace shenmue::s1::ui_runtime_detail {





void UIGrid_HandlePadInput(UIGrid &grid) {
    if (g_uiGridInputEnabled != 1)
        return;
    const InputSlot &input = g_inputState.slots[0];
    if ((input.pressedPriority & 0x400U) != 0)
        grid.confirmRequested = 1;
    if ((input.repeatedPresses & 0x4000U) != 0) {
        bool moved = true;
        if (grid.selectedColumn > 0) {
            --grid.selectedColumn;
        } else if (g_uiFocusedList && g_currUIState == -1) {
            g_uiFocusedList->focused = g_uiFocusedList->lastFocused;
            g_uiGridInputEnabled = 0;
            g_uiFocusedList->hasFocus = 1;
        } else {
            moved = false;
        }
        if (moved)
            UI_PlaySound(grid.navigateSound);
    }
    if ((input.repeatedPresses & 0x8000U) != 0 && grid.selectedColumn < grid.columns - 1) {
        const std::int32_t column = grid.selectedColumn++;
        if (UIGrid_SelectedIndex(grid) < grid.elementCount || grid.showEmptyCells)
            UI_PlaySound(grid.navigateSound);
        else
            grid.selectedColumn = column;
    }
    if ((input.repeatedPresses & 0x1000U) != 0 && grid.selectedRow + grid.scrollRow > 0) {
        --grid.selectedRow;
        UI_PlaySound(grid.navigateSound);
    }
    if ((input.repeatedPresses & 0x2000U) != 0) {
        ++grid.selectedRow;
        if (UIGrid_SelectedIndex(grid) < grid.elementCount || grid.showEmptyCells) {
            UI_PlaySound(grid.navigateSound);
        } else if (grid.elementCount - 1 < grid.elementCount) {
            const std::int32_t last = grid.elementCount - 1;
            grid.selectedRow = last / grid.columns - grid.scrollRow;
            grid.selectedColumn = last - grid.columns * (grid.scrollRow + grid.selectedRow);
        }
    }
    if ((input.pressedPriority & 0x200U) != 0 && g_uiFocusedList && g_currUIState == -1) {
        g_uiFocusedList->focused = g_uiFocusedList->lastFocused;
        g_uiGridInputEnabled = 0;
        g_uiFocusedList->hasFocus = 1;
    }
}







void UIGrid_UpdateScroll(UIGrid &grid) {
    nk_context &context = GNuklearWindow;
    std::int32_t previousScrollRow = grid.scrollRow;
    const std::int32_t maxScrollRow = UIGrid_MaxScrollRow(grid);
    const float wheel = context.input.mouse.scroll_delta;
    const float maxOffset = UIGrid_RowOffset(&grid, maxScrollRow);
    const std::size_t slotIndex = static_cast<std::uint32_t>(grid.animSlot);
    UIAnimSlot *const slot = slotIndex < g_uiAnimSlots.size() ? &g_uiAnimSlots[slotIndex] : nullptr;
    if (!grid.dragging && slot) {
        grid.scrollOffset = slot->current[0];
        if (0.0f > grid.scrollOffset || grid.scrollOffset > maxOffset) {
            slot->status = 0;
            slot->elapsed = 0.0f;
            slot->allocated = 1;
            if (0.0f > grid.scrollOffset) {
                grid.scrollRow = 0;
                grid.scrollOffset = 0.0f;
            } else if (grid.scrollOffset > maxOffset) {
                grid.scrollOffset = maxOffset;
                grid.scrollRow = maxScrollRow;
            }
        }
    }
    const InputSlot &input = g_inputState.slots[0];
    if ((input.repeatedPresses & 0x3000U) != 0 && !grid.dragging) {
        grid.smoothScroll = 0;
        if (grid.rowHeight) {
            if (grid.scrollRow < maxScrollRow || grid.selectedRow + grid.scrollRow < maxScrollRow) {
                const std::int32_t row = grid.selectedRow + grid.scrollRow;
                grid.selectedRow = 0;
                grid.scrollRow = row;
            }
            if (grid.selectedRow + grid.scrollRow > grid.rows - 1)
                grid.selectedRow = grid.selectedRow - 1;
            if (grid.scrollRow < 0) {
                grid.scrollRow = 0;
                grid.selectedRow = 0;
            }
        } else if (grid.selectedRow < 0) {
            if (grid.pagedScroll == 1 && grid.scrollRow > 0) {
                grid.scrollRow = grid.scrollRow - grid.visibleRows;
                grid.selectedRow = grid.visibleRows - 1;
            } else {
                --grid.scrollRow;
                grid.selectedRow = 0;
            }
        } else {
            const std::int32_t row = grid.selectedRow;
            const std::int32_t visibleRows = grid.visibleRows;
            if (row >= visibleRows || (UIGrid_SelectedIndex(grid) >= grid.elementCount && !grid.showEmptyCells)) {
                if (grid.pagedScroll == 1 && grid.scrollRow < maxScrollRow) {
                    grid.selectedRow = row - visibleRows;
                    grid.scrollRow = grid.scrollRow + visibleRows;
                } else {
                    ++grid.scrollRow;
                    grid.selectedRow = row - 1;
                }
            }
        }
        grid.scrollSpeed = 0.25f;
    } else if (wheel != 0.0f && !grid.dragging) {
        const std::uint8_t paged = grid.pagedScroll;
        grid.smoothScroll = 0;
        std::int32_t step = 0.0f > wheel ? 1 : -1;
        if (paged == 1)
            step = step * grid.visibleRows;
        grid.scrollRow += step;
        if (0.0f > wheel) {
            const std::int32_t row = grid.scrollRow;
            if ((row < 0 ? 0 : row) * grid.columns >= grid.elementCount) {
                if (paged == 1)
                    grid.scrollRow = maxScrollRow;
                else
                    grid.scrollRow = row - 1;
            }
        }
        grid.scrollSpeed = 0.25f;
    } else {
        std::int32_t lastRow = grid.rows - 1;
        if (grid.columns * (lastRow < 0 ? 0 : lastRow) > grid.elementCount)
            lastRow = (grid.elementCount - 1) / grid.columns;
        const std::int32_t rows = lastRow + 1;
        UILayoutRect window{};
        UILayoutRect track{};
        UILayoutRect thumb{};
        UIGrid_ScrollBarRects(&grid, &window, &track, &thumb);
        const float trackTop = window.y + track.y;
        const float thumbY = thumb.y + window.y;
        const float thumbX = thumb.x + window.x;
        const struct nk_mouse &mouse = context.input.mouse;
        if (!grid.dragging && mouse.buttons[NK_BUTTON_LEFT].clicked == 1 && mouse.pos.x > thumbX && thumb.width + thumbX > mouse.pos.x &&
            mouse.pos.y > thumbY && thumb.height + thumbY > mouse.pos.y) {
            grid.dragging = 1;
        } else if (grid.dragging == 1 && !mouse.buttons[NK_BUTTON_LEFT].down) {
            grid.dragging = 0;
        }
        if (grid.dragging == 1 || grid.dragStart != 0.0f) {
            float trackY = track.y;
            grid.smoothScroll = 1;
            const float grab = mouse.pos.y - thumbY;
            if (!(trackY > grab))
                trackY = std::fminf(track.height + trackY, grab);
            const float trackBottom = track.height + trackTop;
            grid.dragOffset = trackY;
            const float pointer = mouse.pos.y - (mouse.pos.y - thumbY);
            float share = 0.0f;
            if (!grid.dragging) {
                share = grid.dragDelta + grid.dragStart;
            } else if (trackTop > pointer) {
                share = 0.0f;
            } else if (pointer > trackBottom) {
                share = 1.0f;
            } else {
                const float ratio = (pointer - trackTop) / (trackBottom - trackTop);
                share = 0.0f > ratio ? 0.0f : std::fminf(1.0f, ratio);
            }
            grid.scrollOffset = UIGrid_RowOffset(&grid, rows) * share;
            float row = static_cast<float>(rows) * share;
            if (share - grid.dragStart > 0.0f)
                row = row + 1.0f;
            if (grid.pagedScroll == 1) {
                const std::int32_t whole = static_cast<std::int32_t>(row);
                static_cast<void>(UIGrid_RowOffset(&grid, whole));
                const std::int32_t page = static_cast<std::int32_t>(UIGrid_RowOffset(&grid, grid.visibleRows) * 100.0f);
                float into = static_cast<float>(static_cast<std::int32_t>(grid.scrollOffset * 100.0f) % page) / 100.0f;
                into = into / UIGrid_RowOffset(&grid, grid.visibleRows);
                const std::int32_t remainder = whole % grid.visibleRows;
                if (into > 0.5f)
                    row = row + static_cast<float>(grid.visibleRows - remainder);
                else
                    row = row - static_cast<float>(remainder);
            }
            grid.dragDelta = share - grid.dragStart;
            grid.scrollRow = static_cast<std::int32_t>(row);
            if (0.001f > std::fabs(grid.dragDelta)) {
                if (grid.pagedScroll != 1) {
                    std::int32_t whole = static_cast<std::int32_t>(row);
                    if (row - static_cast<float>(whole) > 0.5f)
                        ++whole;
                    grid.scrollRow = whole;
                }
                grid.scrollSpeed = 0.25f;
            } else {
                const float distance = UIGrid_RowOffset(&grid, rows) * grid.dragDelta;
                const float remaining = std::fabs(UIGrid_RowOffset(&grid, grid.scrollRow) - grid.scrollOffset);
                if (distance < 1.0f || remaining < 1.0f)
                    grid.scrollSpeed = 0.25f;
                else
                    grid.scrollSpeed = distance / remaining * (g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f);
            }
            grid.dragStart = share;
            if (!grid.dragging)
                grid.dragStart = 0.0f;
        }
    }
    if (0.25f > grid.scrollSpeed)
        grid.scrollSpeed = 0.25f;
    if (grid.wrap == 1) {
        if (grid.scrollRow > maxScrollRow)
            grid.scrollRow = 0;
        else if (grid.scrollRow < 0)
            grid.scrollRow = maxScrollRow;
    } else if (slot && !slot->status) {
        if (grid.scrollRow > maxScrollRow) {
            grid.scrollRow = maxScrollRow;
            previousScrollRow = maxScrollRow;
        } else if (grid.scrollRow < 0) {
            previousScrollRow = 0;
            grid.scrollRow = 0;
        }
    }
    const float target = UIGrid_RowOffset(&grid, grid.scrollRow);
    if (grid.smoothScroll == 1) {
        UILayoutRect cell{};
        UIGrid_GetCellRect(&grid, nullptr, &cell, -1, -1);
        const float speed = grid.scrollSpeed / cell.height;
        const float from = grid.scrollOffset;
        const float duration = std::fabs(UIGrid_RowOffset(&grid, grid.scrollRow) - from) * speed;
        if (slot && duration > 0.0f) {
            slot->start[0] = from;
            slot->current[0] = from;
            slot->end[0] = target;
            slot->duration = duration;
            slot->allocated = 1;
            slot->status = 1;
            slot->interpolation = 2;
            slot->delay = 0.0f;
            slot->elapsed = 0.0f;
            slot->reserved40 = -1;
        }
    } else if (previousScrollRow != grid.scrollRow) {
        UIAnim_Start(grid.animSlot, grid.scrollOffset, target, grid.scrollSpeed, 0.0f);
    }
}



std::int64_t UIGrid_IsPressed(UIGrid &grid) {
    const std::int32_t scrollRow = grid.scrollRow;
    g_uiCurrentGrid = &grid;
    grid.hovered = 1;
    if (grid.previousScrollRow == scrollRow && grid.previousSelectedRow == grid.selectedRow) {
        if (grid.selectedRow >= 0) {
            if (grid.selectedRow >= grid.visibleRows || (UIGrid_SelectedIndex(grid) >= grid.elementCount && !grid.showEmptyCells)) {
                if (grid.pagedScroll == 1 && scrollRow < UIGrid_MaxScrollRow(grid)) {
                    grid.selectedRow -= grid.visibleRows;
                    grid.scrollRow = grid.visibleRows + scrollRow;
                } else {
                    --grid.selectedRow;
                    grid.scrollRow = scrollRow + 1;
                }
            }
        } else {
            grid.selectedRow = 0;
            grid.scrollRow = scrollRow - 1;
        }
    }
    grid.previousScrollRow = grid.scrollRow;
    grid.previousSelectedRow = grid.selectedRow;
    grid.previousSelectedColumn = grid.selectedColumn;
    UIGrid_HandlePadInput(grid);
    UIGrid_UpdateScroll(grid);
    return 1;
}



void UIGrid_ResetRuntime(UIGrid &grid) {
    if (grid.animSlot != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(grid.animSlot)];
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 0.0f;
        slot.current[0] = 0.0f;
        slot.end[0] = 0.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.duration = 0.01f;
        slot.reserved40 = -1;
    }
    grid.scrollRow = 0;
    grid.scrollOffset = 0.0f;
    grid.selectedRow = 0;
    grid.selectedColumn = 0;
    grid.previousSelectedRow = -1;
    grid.previousSelectedColumn = -1;
    grid.previousScrollRow = -1;
    grid.dragStart = 0.0f;
    grid.dragDelta = 0.0f;
    grid.fieldA4 = 0;
    grid.confirmRequested = 0;
    grid.changed = 1;
}






std::int32_t UIGrid_PlaceCellWidgets(UIGridTemplate &cellTemplate, std::int32_t index, const UILayoutRect &holder, const UILayoutRect &cell) {
    if (!cellTemplate.windows || !cellTemplate.ids)
        return 0;
    std::int32_t clipped = 1;
    cellTemplate.rootCount = 0;
    cellTemplate.clipCount = 0;
    const std::uint32_t first = static_cast<std::uint32_t>(cellTemplate.first);
    const std::uint32_t end = static_cast<std::uint32_t>(cellTemplate.end);
    if (!cellTemplate.initialized) {
        for (std::uint32_t window = first; window < end; ++window) {
            if (d3t::UIWindow *const record = cellTemplate.windows[window]) {
                cellTemplate.idTemplatePositions[window - first] = static_cast<std::int32_t>(strnlen(record->id.data(), 0x40)) - 1;
                cellTemplate.parentTemplatePositions[window - first] = static_cast<std::int32_t>(strnlen(record->layout.parentId.data(), 0x40)) - 1;
            }
        }
        cellTemplate.initialized = 1;
    }
    for (std::uint32_t window = first; window < end; ++window) {
        d3t::UIWindow *const record = cellTemplate.windows[window];
        if (!record)
            continue;
        if (const std::int32_t at = cellTemplate.idTemplatePositions[window - first]; at != -1)
            std::snprintf(record->id.data() + at, record->id.size() - static_cast<std::size_t>(at), "%i", index);
        if (const std::int32_t at = cellTemplate.parentTemplatePositions[window - first]; at == -1)
            cellTemplate.roots[static_cast<std::size_t>(cellTemplate.rootCount++)] = &record->layout;
        else
            std::snprintf(record->layout.parentId.data() + at, record->layout.parentId.size() - static_cast<std::size_t>(at), "%i", index);
    }
    for (std::uint32_t clip = 0; end != first && clip < end - first; ++clip) {
        const std::int32_t window = cellTemplate.clipWindows[clip];
        if (window == -1)
            break;
        if (d3t::UIWindow *const record = cellTemplate.windows[window])
            cellTemplate.clips[static_cast<std::size_t>(cellTemplate.clipCount++)] = &record->layout;
    }
    const float holderTop = holder.y;
    const float height = cell.height;
    float y = holderTop + cell.y;
    const float x = holder.x + cell.x;
    const float width = cell.width;
    const float holderBottom = holderTop + holder.height;
    if (height + y <= holderBottom) {
        if (holderTop <= y) {
            for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
                if (d3t::UIWindowLayout *const layout = cellTemplate.clips[static_cast<std::size_t>(clip)]) {
                    layout->height.scale = 1.0f;
                    layout->alignY = 1;
                }
            }
            for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item) {
                if (d3t::UILayoutItem *const record = cellTemplate.layoutItems[item])
                    record->offsetY = 0.0f;
            }
            clipped = 0;
        } else {
            const float cut = holderTop - y;
            y = y + cut;
            for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
                if (d3t::UIWindowLayout *const layout = cellTemplate.clips[static_cast<std::size_t>(clip)]) {
                    layout->height.scale = (height - cut) / 1080.0f / layout->height.value.fraction;
                    layout->alignY = 6;
                }
            }
            for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item) {
                if (d3t::UILayoutItem *const record = cellTemplate.layoutItems[item])
                    record->offsetY = -(cut / height);
            }
        }
    } else {
        const float cut = height + y - holderBottom;
        y = y - cut;
        const float share = cut / height;
        for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
            if (d3t::UIWindowLayout *const layout = cellTemplate.clips[static_cast<std::size_t>(clip)]) {
                layout->height.scale = 1.0f - share;
                layout->alignY = 7;
            }
        }
        for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item) {
            if (d3t::UILayoutItem *const record = cellTemplate.layoutItems[item])
                record->offsetY = share;
        }
    }
    for (std::int32_t label = cellTemplate.labelFirst; label < cellTemplate.labelEnd; ++label) {
        if (d3t::UILabelWidget *const record = cellTemplate.labels[label])
            record->scaleVarAssign = clipped ^ 1;
    }
    for (std::int32_t root = 0; root < cellTemplate.rootCount; ++root) {
        if (d3t::UIWindowLayout *const layout = cellTemplate.roots[static_cast<std::size_t>(root)]) {
            layout->offsetX = x / static_cast<float>(UI_GetAspectWidth());
            layout->offsetY = y / 1080.0f;
            layout->width.value.fraction = width / 1920.0f;
            layout->height.value.fraction = height / 1080.0f;
        }
    }
    return clipped;
}





void UIGrid_AfterDraw(
    UIGridTemplate &cellTemplate, UIGrid *grid, std::int32_t index, std::int32_t firstId, std::int32_t endId, const UILayoutRect &holder, nk_panel *panel) {
    nk_context &context = GNuklearWindow;
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = 0.0f;
            row.item_height = 0.0f;
            row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    if (static_cast<std::uint32_t>(cellTemplate.viewSlot) <= 0x4bU) {
        const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(cellTemplate.viewSlot)];
        const auto matches = [&cellTemplate](const d3t::UIWindow &window, std::int32_t id) {
            const std::int32_t length = cellTemplate.idTemplatePositions[static_cast<std::uint32_t>(id - cellTemplate.first)];
            return !std::strncmp(window.id.data(), cellTemplate.ids[id], static_cast<std::size_t>(length));
        };
        std::int32_t from = -1;
        std::int32_t to = view.widgetCount;
        for (std::int32_t widget = 0; widget < view.widgetCount; ++widget) {
            if (view.widgetKinds[static_cast<std::size_t>(widget)] != 1)
                continue;
            const d3t::UIWindow *const window = UI_GetWindow(view.widgetPoolIndices[static_cast<std::size_t>(widget)]);
            if (!window)
                continue;
            if (from == -1 && matches(*window, firstId)) {
                from = widget - 1;
                continue;
            }
            if (endId != -1 && matches(*window, endId)) {
                to = widget ? widget - 1 : view.widgetCount;
                break;
            }
        }
        if (from < to)
            UI_InputHandler(context, cellTemplate.viewSlot, from, to);
        for (std::int32_t root = 0; root < cellTemplate.rootCount; ++root) {
            if (d3t::UIWindowLayout *const layout = cellTemplate.roots[static_cast<std::size_t>(root)]) {
                layout->offsetX = 1920.0f;
                layout->offsetY = 1080.0f;
            }
        }
        const std::uint32_t first = static_cast<std::uint32_t>(cellTemplate.first);
        for (std::uint32_t window = first; window < static_cast<std::uint32_t>(cellTemplate.end); ++window) {
            d3t::UIWindow *const record = cellTemplate.windows[window];
            if (!record)
                continue;
            if (const std::int32_t at = cellTemplate.idTemplatePositions[window - first]; at != -1)
                std::snprintf(record->id.data() + at, record->id.size() - static_cast<std::size_t>(at), "#");
            if (const std::int32_t at = cellTemplate.parentTemplatePositions[window - first]; at != -1)
                std::snprintf(record->layout.parentId.data() + at, record->layout.parentId.size() - static_cast<std::size_t>(at), "#");
        }
    }
    context.style.window.fixed_background.data.color.a = 0;
    std::array<char, 256> name{};
    std::snprintf(name.data(), name.size(), "Grid_AfterDraw_%i", index);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(holder.x, holder.y, holder.width, holder.height), NK_WINDOW_NO_SCROLLBAR);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, grid->elementCount);
    }
}




void UIGrid_DrawItemCell(
    UIGrid *grid, nk_panel *, const UILayoutRect *, const UILayoutRect *cell, float, std::int32_t element, void *elements, std::int32_t index) {
    nk_context &context = GNuklearWindow;
    const std::int32_t waiting = g_waitingForNextTick;
    const UILayoutRect rect = *cell;
    const auto place = [&context](const UILayoutRect &at) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(at.x, at.y, at.width, at.height);
    };
    place(rect);
    const UIGridStyle &style = UIGrid_Style(grid);
    if (waiting == 1) {
        nk_color colour{};
        std::memcpy(&colour, &style.cellColour, sizeof(colour));
        nk_button_color(&context, colour);
    }
    if (!elements)
        return;
    UIGridEntry &entry = static_cast<UIGridEntry *>(elements)[static_cast<std::uint32_t>(index)];
    UI_WindowMouseCallbacks(grid->mouseCallbacks, element, rect, &entry);
    const auto icon = [&entry](struct nk_image &image, std::int32_t mask) {
        if (entry.type == 0 && static_cast<std::uint32_t>(entry.id) < kOwnedItemCatalogCount)
            RenderingRelatedInventory(image, g_ownedItemCatalog[static_cast<std::size_t>(entry.id)].iconIndex, mask);
        else if (entry.type == 1 && static_cast<std::uint32_t>(entry.id) < kCassetteCatalogCount)
            RenderingRelatedInventory(image, g_cassetteCatalog[static_cast<std::size_t>(entry.id)].entry.iconIndex, mask);
    };
    struct nk_image image {};
    icon(image, 0);
    float drawWidth = 0.0f;
    float drawHeight = 0.0f;
    const float imageHeight = static_cast<float>(image.region[3]);
    const float imageWidth = static_cast<float>(image.region[2]);
    if (rect.height != 0.0f && rect.width != 0.0f && imageWidth != 0.0f && imageHeight != 0.0f) {
        if (rect.height / rect.width > imageHeight / imageWidth) {
            drawHeight = rect.width / imageWidth * imageHeight;
            drawWidth = rect.width;
        } else {
            drawHeight = rect.height;
            drawWidth = imageHeight / imageWidth > rect.height / rect.width ? rect.height / imageHeight * imageWidth : rect.width;
        }
    }
    const float top = rect.height * 0.5f + rect.y - drawHeight * 0.5f;
    const float insetX = WithinSafeArea(12.0f);
    const float insetY = WithinSafeArea(12.0f);
    const UILayoutRect fitted{.x = rect.width * 0.5f + rect.x - drawWidth * 0.5f + insetX,
        .y = top + insetY,
        .width = drawWidth - insetX * 2.0f,
        .height = drawHeight - insetY * 2.0f};
    const bool owned = entry.count != 0;
    icon(image, owned ? 0 : 1);
    place(fitted);
    if (g_waitingForNextTick == 1) {
        const std::uint32_t tint = owned ? 0xffffffffU : kItemsGridMissingIconColour;
        nk_color colour{};
        std::memcpy(&colour, &tint, sizeof(colour));
        nk_image_color(&context, image, colour);
    }
    entry.drawnCell = index;
}



void UIFocusList_ColourLabels(const UIFocusList &list) {
    if (!list.labels)
        return;
    const UIFocusListColours &colours = g_isInGame == 1 ? kUIFocusListInGameColours : kUIFocusListFrontEndColours;
    for (std::int32_t index = 0; index <= list.last; ++index) {
        if (d3t::UILabelWidget *const label = list.labels[index])
            label->colour = colours.normal;
    }
    if (list.selected >= list.first && list.selected <= list.last) {
        if (d3t::UILabelWidget *const label = list.labels[list.selected])
            label->colour = colours.selected;
    }
    if (list.focused >= list.first && list.focused <= list.last) {
        if (d3t::UILabelWidget *const label = list.labels[list.focused])
            label->colour = colours.focused;
    }
}



void UIFocusList_DescribeEntry(d3t::UIWindow *window, const d3t::UILabelWidget *label) {
    if (!window) {
        d3t::CopyStringToUILabel(nullptr);
        return;
    }
    InformationBarMessage message{};
    message.iconAction = -1;
    message.outlinePanel = -1;
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    message.outline = {rect.x, rect.y, rect.width, rect.height};
    message.iconIndex = -1;
    message.noOutline = !g_isInGame && label ? 1 : 0;
    if (label)
        message.text = label->token.token;
    d3t::CopyStringToUILabel(&message);
}




std::int32_t UIFocusList_HandleInput(UIFocusList &list) {
    InputSlot &input = g_inputState.slots[0];
    if (list.hasFocus) {
        if ((input.repeatedPresses & 0x1000U) != 0) {
            input.repeatedPresses &= static_cast<std::uint16_t>(~0x1000U);
            if (--list.focused < list.first)
                list.focused = list.last;
            PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        }
        if ((input.repeatedPresses & 0x2000U) != 0) {
            input.repeatedPresses &= static_cast<std::uint16_t>(~0x2000U);
            if (++list.focused > list.last)
                list.focused = list.first;
            PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        }
        if ((input.pressedPriority & 0x400U) != 0) {
            g_uiGridInputEnabled = 1;
            g_uiFocusedList = &list;
            input.pressedPriority &= static_cast<std::uint16_t>(~0x400U);
            list.selected = list.focused;
            list.focused = -1;
            list.hasFocus = 0;
            UI_RestartCurrentGridSelection();
            PlayGatedUiSound(g_uiAcceptSoundFrame, kUiAcceptSound);
        }
        if ((input.pressedPriority & 0x8000U) != 0) {
            g_uiGridInputEnabled = 1;
            g_uiFocusedList = &list;
            input.pressedPriority &= static_cast<std::uint16_t>(~0x8000U);
            list.hasFocus = 0;
            list.focused = -1;
            UI_RestartCurrentGridSelection();
            PlayGatedUiSound(g_uiNavigateSoundFrame, kUiNavigateSound);
        }
    }
    if (!list.initialized) {
        list.focused = -1;
        UIFocusList_ColourLabels(list);
        list.initialized = 1;
    }
    if (list.previousHasFocus != list.hasFocus || (list.lastFocused != list.focused && list.focused != -1) || list.previousSelected != list.selected) {
        if (list.focused >= list.first && list.focused <= list.last)
            UIFocusList_DescribeEntry(list.windows[list.focused], list.labels ? list.labels[list.focused] : nullptr);
        UIFocusList_ColourLabels(list);
    }
    list.previousHasFocus = list.hasFocus;
    list.previousSelected = list.selected;
    if (list.focused >= 0)
        list.lastFocused = list.focused;
    return list.focused;
}



void UIGrid_SelectElement(UIGrid &grid, std::int32_t index) {
    if (index >= grid.elementCount)
        return;
    grid.selectedRow = index / grid.columns - grid.scrollRow;
    grid.selectedColumn = index - grid.columns * (grid.selectedRow + grid.scrollRow);
    while (grid.selectedRow < 0) {
        --grid.scrollRow;
        ++grid.selectedRow;
    }
    while (grid.selectedRow >= grid.visibleRows) {
        --grid.selectedRow;
        ++grid.scrollRow;
    }
    const float target = UIGrid_RowOffset(&grid, grid.scrollRow);
    if (grid.animSlot != -1 && grid.scrollSpeed > 0.0f) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(grid.animSlot)];
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.start[0] = grid.scrollOffset;
        slot.current[0] = grid.scrollOffset;
        slot.end[0] = target;
        slot.duration = grid.scrollSpeed;
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.reserved40 = -1;
    }
}
}
