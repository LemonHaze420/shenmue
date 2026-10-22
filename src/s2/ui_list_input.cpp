#include "d3t/ui_runtime.hpp"
#include "s2/input.hpp"
#include "s2/ui_scroll_list.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
template <class T, std::size_t N> std::int32_t FindListWidget(std::int32_t kind, std::uint32_t slotIndex, const char *id, const std::array<T, N> &pool) {
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slotIndex);
    if (!view || !id)
        return -1;
    for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
        if (view->widgetKinds[widget] != kind)
            continue;
        const std::uint32_t index = static_cast<std::uint32_t>(view->widgetPoolIndices[widget]);
        if (index < pool.size() && pool[index].id[0] && !std::strcmp(pool[index].id.data(), id))
            return static_cast<std::int32_t>(index);
    }
    return -1;
}
} // namespace

std::int32_t UI_FindViewCollection(std::uint32_t slot, const char *id) {
    return FindListWidget(25, slot, id, g_uiCollections);
}
std::int32_t UI_FindViewScrollList(std::uint32_t slot, const char *id) {
    return FindListWidget(27, slot, id, g_uiScrollLists);
}
std::int32_t UI_FindViewTexturePage(std::uint32_t slot, const char *id) {
    return FindListWidget(37, slot, id, g_uiTexturePages);
}
UICollection *UI_GetCollection(std::uint32_t index) {
    return index < g_uiCollections.size() ? &g_uiCollections[index] : nullptr;
}
UITexturePage *UI_GetTexturePage(std::uint32_t index) {
    return index < g_uiTexturePages.size() ? &g_uiTexturePages[index] : nullptr;
}

void UI_UpdateCollectionPageSize(std::uint32_t index) {
    UICollection *const collection = UI_GetCollection(index);
    if (!collection)
        return;
    const std::int32_t page = collection->field64[3];
    const UICollectionPage &layout = collection->pages[collection->field94];
    if (page == collection->field64[1] - 1) {
        const std::int32_t count = collection->field64[0];
        const std::int32_t selected = collection->field64[4];
        collection->field64[2] = count - layout.columns * page * layout.rows;
        if (selected >= count && selected / layout.columns > count / layout.columns)
            collection->field64[4] = selected - layout.columns * (selected / layout.columns - count / layout.columns);
    } else {
        collection->field64[2] = layout.columns * layout.rows;
    }
}

void UI_UpdateCollectionInput(std::uint32_t index) {
    UICollection *const collection = UI_GetCollection(index);
    if (!collection)
        return;
    const UICollectionPage &layout = collection->pages[collection->field94];
    const std::int32_t columns = layout.columns, rows = layout.rows;
    const std::int32_t previousPage = collection->field64[3];
    std::uint16_t input = g_inputBuffer[0].pressedPriority;
    if (input & PAD_RIGHT) {
        ++collection->field64[4];
        if (collection->field64[4] >= collection->field64[0])
            collection->field64[4] = collection->field64[0] - 1;
        if (collection->field64[4] == columns * rows * (previousPage + 1)) {
            if (previousPage + 1 >= collection->field64[1])
                collection->field64[4] = collection->field64[0] - 1;
            else {
                collection->field64[3] = previousPage + 1;
                UI_UpdateCollectionPageSize(index);
            }
        }
        input = 0;
    }
    if (input & PAD_LEFT) {
        const std::int32_t page = collection->field64[3], selected = collection->field64[4];
        if (selected == columns * rows * page) {
            if (page <= 0)
                collection->field64[4] = 0;
            else {
                collection->field64[3] = page - 1;
                collection->field64[4] = columns * rows * page - 1;
            }
            if (previousPage != collection->field64[3])
                collection->field64[2] = columns * rows;
        } else {
            collection->field64[4] = selected - 1 > 0 ? selected - 1 : 0;
        }
        input = 0;
    }
    if (input & PAD_UP) {
        const std::int32_t selected = collection->field64[4], page = collection->field64[3];
        if (selected - columns >= 0) {
            if ((selected / columns - 1) / rows == page)
                collection->field64[4] = selected - columns;
            else if (page > 0) {
                collection->field64[4] = selected - columns;
                collection->field64[3] = page - 1;
                UI_UpdateCollectionPageSize(index);
            }
        }
        input = 0;
    }
    if (input & PAD_DOWN) {
        const std::int32_t selected = collection->field64[4], page = collection->field64[3];
        if ((selected / columns + 1) / rows == page) {
            if (collection->field64[0] / columns > selected / columns) {
                collection->field64[4] = selected + columns;
                if (collection->field64[4] >= collection->field64[0])
                    collection->field64[4] = collection->field64[0] - 1;
            }
        } else if (page + 1 < collection->field64[1]) {
            collection->field64[3] = page + 1;
            collection->field64[4] = selected + columns;
            UI_UpdateCollectionPageSize(index);
        }
    }
}

void UI_UpdateScrollListSelection(std::uint32_t index) {
    UIScrollList *const list = UI_GetScrollList(index);
    if (!list)
        return;
    if (g_inputBuffer[0].pressedPriority & PAD_UP) {
        if (--list->field74[1] < 0) {
            --list->field74[0];
            if (list->field74[0] < 0)
                list->field74[0] = 0;
            list->field74[1] = 0;
        }
    }
    if (g_inputBuffer[0].pressedPriority & PAD_DOWN) {
        if (++list->field74[1] >= list->rows) {
            ++list->field74[0];
            --list->field74[1];
            if (list->field74[0] + list->field74[1] >= list->field6C)
                --list->field74[0];
        }
    }
}

void UI_UpdateScrollListPage(std::uint32_t index) {
    UIScrollList *const list = UI_GetScrollList(index);
    if (!list)
        return;
    if (g_inputBuffer[0].pressedPriority & PAD_UP) {
        if (--list->field74[0] < 0)
            list->field74[0] = 0;
    }
    if (g_inputBuffer[0].pressedPriority & PAD_DOWN) {
        ++list->field74[0];
        if (list->field74[0] + list->field74[1] > list->field6C)
            --list->field74[0];
    }
}
} // namespace shenmue::s2
