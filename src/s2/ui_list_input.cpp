#include "d3t/ui_runtime.hpp"
#include "s2/input.hpp"
#include "s2/ui_scroll_list.hpp"
#include <bit>
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

void UI_SetCollectionCount(std::uint32_t index, std::int32_t count) {
    UICollection *const collection = UI_GetCollection(index);
    if (!collection)
        return;
    const UICollectionPage &layout = collection->pages[collection->field94];
    collection->field64[0] = count;
    std::int32_t perPage = layout.columns * layout.rows;
    std::int32_t pages = (perPage + count - 1) / perPage;
    if (pages >= 1) {
        if (pages > count)
            pages = count;
    } else {
        pages = 1;
    }
    collection->field64[1] = pages;
    if (pages == 1)
        perPage = count;
    collection->field64[2] = perPage;
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
    const std::int32_t previousRow = list->field74[1];
    if (list->field3AC) {
        if (UIWidget_IsAnimating(static_cast<std::uint32_t>(list->field3A8)))
            return;
        list->field74[0] += list->field3AC;
        list->field3AC = 0;
        list->field3A4 = 0;
    }
    const std::int32_t count = list->field6C;
    const std::int32_t visible = list->rows <= count ? list->rows : count;
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    if (input & PAD_DOWN) {
        const std::int32_t row = ++list->field74[1];
        if (row >= visible) {
            if (row + list->field74[0] < count) {
                list->field3AC = 1;
                list->field74[1] = row - 1;
                if (list->field64 == 1)
                    UIWidget_Retarget(list->field3A8, {0.0f, 0.0f}, {-std::bit_cast<float>(list->field358[row - 1]), 0.0f});
                UIWidget_Animate(list->field3A8, 1, 0.15f, 0.0f);
            }
            list->field74[1] = visible - 1;
        }
        UIWidget_Retarget(list->field3D4, {static_cast<float>(previousRow), 0.0f}, {static_cast<float>(list->field74[1]), 0.0f});
        UIWidget_Animate(list->field3D4, 1, 0.15f, 0.0f);
    }
    if (input & PAD_UP) {
        if (--list->field74[1] < 0) {
            if (list->field74[0] > 0) {
                list->field3AC = -1;
                if (list->field64 == 1)
                    UIWidget_Retarget(list->field3A8, {0.0f, 0.0f}, {-std::bit_cast<float>(list->field3B0[1]), 0.0f});
                UIWidget_Animate(list->field3A8, 2, 0.15f, 0.0f);
            }
            list->field74[1] = 0;
        }
        UIWidget_Retarget(list->field3D4, {static_cast<float>(previousRow), 0.0f}, {static_cast<float>(list->field74[1]), 0.0f});
        UIWidget_Animate(list->field3D4, 1, 0.15f, 0.0f);
    }
}

std::int32_t UI_UpdateMenuBarSelection(std::uint32_t index, std::uint16_t previous, std::uint16_t next) {
    const std::uint16_t input = g_inputBuffer[0].pressedPriority;
    UIMenuBar *const menuBar = UI_GetMenuBar(index);
    if (!menuBar)
        return 0;
    UIMenuBar &bar = *menuBar;
    if (input & previous) {
        if (--bar.field184 < 0)
            bar.field184 = bar.optionCount - 1;
        return 1;
    }
    if (input & next) {
        std::int32_t selected = ++bar.field184;
        if (selected >= bar.optionCount)
            selected = 0;
        bar.field184 = selected;
        return 1;
    }
    return 0;
}

UISelector *UI_UpdateSelectorInput(std::uint32_t index) {
    if (index >= g_uiSelectors.size())
        return nullptr;
    UISelector &selector = g_uiSelectors[index];
    std::uint16_t input = g_inputBuffer[0].pressedPriority;
    if ((input & PAD_LEFT) && selector.hasOption[3]) {
        selector.pos = 3;
        input = 0;
    }
    if ((input & PAD_RIGHT) && selector.hasOption[1]) {
        selector.pos = 1;
        input = 0;
    }
    if ((input & PAD_UP) && selector.hasOption[0]) {
        selector.pos = 0;
        input = 0;
    }
    if ((input & PAD_DOWN) && selector.hasOption[2])
        selector.pos = 2;
    return &selector;
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
