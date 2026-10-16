#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif






void UI_WindowMouseCallbacks(d3t::UIMouseCallbacks &callbacks, std::int32_t element, const UILayoutRect &rect, void *data) {
    if (!callbacks.left && !callbacks.clicked && !callbacks.hovered && !callbacks.entered)
        return;
    nk_context &context = GNuklearWindow;
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
        context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
    }
    std::array<float, 4> area{rect.x, rect.y, rect.width, rect.height};
    if (context.current && context.current->layout) {
        area[0] = area[0] + context.current->layout->bounds.x;
        area[1] = area[1] + context.current->layout->bounds.y;
    }
    const std::uint32_t debugColour = g_uiDebugDrawHitAreas ? 0x64646464U : 0U;
    const auto queue = [element, data, &area](UIMouseCallbackQueue &target, d3t::UIMouseCallback callback) {
        if (element == -1 || !callback || static_cast<std::uint32_t>(target.count) >= static_cast<std::uint32_t>(kUIMouseCallbackQueueLimit))
            return;
        target.entries[static_cast<std::size_t>(target.count)] = {.element = element, .data = data, .rect = area, .callback = callback};
        ++target.count;
    };
    if (g_waitingForNextTick == 1) {
        nk_color colour{};
        std::memcpy(&colour, &debugColour, sizeof(colour));
        if (nk_button_color(&context, colour))
            queue(g_uiMouseCallbackQueues[3], callbacks.clicked);
    }
    if (context.last_widget_state & NK_WIDGET_STATE_HOVER) {
        queue(g_uiMouseCallbackQueues[1], callbacks.hovered);
        callbacks.isHovered = 1;
    } else if (callbacks.isHovered) {
        queue(g_uiMouseCallbackQueues[2], callbacks.left);
        callbacks.isHovered = 0;
    }
    if (context.last_widget_state & NK_WIDGET_STATE_ENTERED)
        queue(g_uiMouseCallbackQueues[0], callbacks.entered);
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item_offset = 0.0f;
        std::memset(&row.item, static_cast<int>(debugColour & 0xffU), sizeof(row.item));
    }
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
