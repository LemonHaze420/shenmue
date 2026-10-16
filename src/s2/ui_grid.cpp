#include "s2/ui_grid.hpp"

#include "d3t/dx11_renderer.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/input.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/ui_transition.hpp"
#include <cmath>
#include <cstring>
#include <bit>
#include <cstdio>

namespace shenmue::s2 {
UIGridTabSelector *currentUIGridTabSelector;
UIGrid *currentUIGrid;
std::int32_t uiGridInputEnabled;
std::int64_t ResetUIGridTabSelector() {
    uiGridInputEnabled = 0;
    currentUIGridTabSelector = nullptr;
    return 0;
}
void SetUIGridTabSelector(UIGridTabSelector *selector, std::int32_t reset) {
    uiGridInputEnabled = 1;
    currentUIGridTabSelector = selector;
    if (currentUIGrid && reset == 1) {
        currentUIGrid->previousRow = currentUIGrid->previousColumn = -1;
        currentUIGrid->row = currentUIGrid->column = 0;
        currentUIGrid->notifySelection = 1;
    }
}

void UpdateUIGridTabColours(UIGridTabSelector &selector) {
    if (!selector.labels)
        return;
    for (std::int32_t tab = 0; tab <= selector.last; ++tab) {
        d3t::UILabelWidget *const label = static_cast<d3t::UILabelWidget *>(selector.labels[tab]);
        if (label)
            label->colour = UI_GRID_TAB_LABEL_UNSELECTED;
    }
    if (selector.selected >= selector.first && selector.selected <= selector.last) {
        d3t::UILabelWidget *const label = static_cast<d3t::UILabelWidget *>(selector.labels[selector.selected]);
        if (label)
            label->colour = UI_GRID_TAB_LABEL_SELECTED;
    }
    if (selector.hovered >= selector.first && selector.hovered <= selector.last) {
        d3t::UILabelWidget *const label = static_cast<d3t::UILabelWidget *>(selector.labels[selector.hovered]);
        if (label)
            label->colour = IsCameraIconHidden() == 1 ? UI_GRID_TAB_LABEL_SELECTED : UI_GRID_TAB_LABEL_FRONT_HOVER;
    }
}

void ClickUIGridTab(UIGridTabSelector &selector, void *window) {
    for (std::int32_t tab = 0; tab <= selector.last - selector.first; ++tab) {
        if (window == selector.buttons[tab]) {
            selector.selected = tab;
            AicaExecuteWrapper(2);
        }
    }
}

void EnterUIGridTab(UIGridTabSelector &selector, void *window) {
    for (std::int32_t tab = 0; tab <= selector.last - selector.first; ++tab) {
        if (window == selector.buttons[tab]) {
            selector.previousHover = -1;
            selector.hovered = tab;
            AicaExecuteWrapper(3);
        }
    }
}

void LeaveUIGridTab(UIGridTabSelector &selector, void *window) {
    for (std::int32_t tab = 0; tab <= selector.last - selector.first; ++tab) {
        if (window == selector.buttons[tab] && selector.hovered == tab) {
            selector.hovered = -1;
            UpdateUIGridTabColours(selector);
        }
    }
}

std::int32_t ProcessUIGridTabInput(UIGridTabSelector &selector) {
    if (selector.active) {
        if (g_inputBuffer[0].repeatedPresses & 0x1000U) {
            g_inputBuffer[0].repeatedPresses &= ~0x1000U;
            if (--selector.hovered < selector.first)
                selector.hovered = selector.last;
            AicaExecuteWrapper(3);
        }
        if (g_inputBuffer[0].repeatedPresses & 0x2000U) {
            g_inputBuffer[0].repeatedPresses &= ~0x2000U;
            if (++selector.hovered > selector.last)
                selector.hovered = selector.first;
            AicaExecuteWrapper(3);
        }
        if (g_inputBuffer[0].pressedPriority & 0x400U) {
            g_inputBuffer[0].pressedPriority &= ~0x400U;
            selector.selected = selector.hovered;
            selector.hovered = -1;
            selector.active = 0;
            SetUIGridTabSelector(&selector, 1);
            AicaExecuteWrapper(2);
        }
        if (g_inputBuffer[0].pressedPriority & 0x8000U) {
            g_inputBuffer[0].pressedPriority &= ~0x8000U;
            selector.hovered = -1;
            selector.active = 0;
            SetUIGridTabSelector(&selector, 1);
            AicaExecuteWrapper(3);
        }
    }
    if (!selector.coloursInitialized) {
        selector.hovered = -1;
        UpdateUIGridTabColours(selector);
        selector.coloursInitialized = 1;
    }
    if (selector.previousActive != selector.active || (selector.previousHover != selector.hovered && selector.hovered != -1) ||
        selector.previousSelected != selector.selected) {
        if (selector.hovered >= selector.first && selector.hovered <= selector.last) {
            d3t::UIWindow *const window = static_cast<d3t::UIWindow *>(selector.buttons[selector.hovered]);
            if (window) {
                UIMessageBox message{};
                message.imageIndex = message.widgetIndex = message.imageFrame = -1;
                UILayoutRect rect{};
                UI_GetWindowRect(window, rect);
                message.bounds = {.x = rect.x, .y = rect.y, .z = rect.width, .w = rect.height};
                message.hideTarget = IsCameraIconHidden() == 0;
                d3t::UILabelWidget *const label = selector.labels ? static_cast<d3t::UILabelWidget *>(selector.labels[selector.hovered]) : nullptr;
                if (label)
                    message.primaryText = label->token.token;
                CopyUIMessageBox(&message);
            } else
                CopyUIMessageBox(nullptr);
        }
        UpdateUIGridTabColours(selector);
    }
    selector.previousActive = selector.active;
    selector.previousSelected = selector.selected;
    if (selector.hovered >= 0)
        selector.previousHover = selector.hovered;
    return selector.hovered;
}
namespace {
std::array<std::uint64_t, 11> uiSoundFrames{~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL, ~0ULL};

void QueueGridAction(const UIGrid &grid, std::uint32_t element, std::uint32_t kind, const Vector4 &bounds, const std::int32_t *data) {
    if (element == ~0U || !grid.actions[kind])
        return;
    UIMouseCallbackQueue &queue = g_uiMouseCallbackQueues[kind];
    if (queue.count >= kUIMouseCallbackQueueLimit)
        return;
    queue.entries[queue.count++] = {.element = static_cast<std::int32_t>(element),
        .data = const_cast<std::int32_t *>(data),
        .rect = {bounds.x, bounds.y, bounds.z, bounds.w},
        .callback = grid.actions[kind]};
}

const UIGridStyle &GridStyle(const UIGrid &grid) {
    return grid.style ? *grid.style : IsCameraIconHidden() ? UI_GRID_GAME_STYLE : UI_GRID_FRONTEND_STYLE;
}

float GridSpacing(const UIGrid &grid) {
    const float width = GetUIViewportWidth() / 1920.0f;
    const float height = 1.0f;
    return GridStyle(grid).spacing * GetS2UserInterfaceSafeArea() * (width < height ? width : height);
}

std::int32_t GridDrawRowCount(const UIGrid &grid) {
    std::int32_t last = grid.rows - 1;
    const std::int32_t clamped = last < 0 ? 0 : last;
    if (grid.columns * clamped > grid.dictionaryCount)
        last = (grid.dictionaryCount - 1) / grid.columns;
    return last + 1;
}

} // namespace
void UpdateUIGridScroll(UIGrid &grid, const WindowUIInput &pointer, float frameTime) {
    const std::int32_t previousFirst = grid.firstRow;
    std::int32_t snapFirst = previousFirst;
    const std::int32_t maximum = GetUIGridMaximumFirstRow(grid);
    const float maximumOffset = GetUIGridRowOffset(grid, maximum);
    if (!grid.dragging) {
        grid.scrollOffset = GetUITransitionValue(grid.transition);
        if (grid.scrollOffset < 0.0f || grid.scrollOffset > maximumOffset) {
            RestartUITransition(grid.transition);
            if (grid.scrollOffset < 0.0f) {
                grid.firstRow = 0;
                grid.scrollOffset = 0.0f;
            } else if (grid.scrollOffset > maximumOffset) {
                grid.firstRow = maximum;
                grid.scrollOffset = maximumOffset;
            }
        }
    }
    if ((g_inputBuffer[0].repeatedPresses & 0x3000U) && !grid.dragging) {
        grid.pointerScrolling = 0;
        if (grid.rowSpan) {
            if (grid.firstRow < maximum || grid.firstRow + grid.row < maximum) {
                grid.firstRow += grid.row;
                grid.row = 0;
            }
            if (grid.row + grid.firstRow > grid.rows - 1)
                --grid.row;
            if (grid.firstRow < 0) {
                grid.firstRow = 0;
                grid.row = 0;
            }
        } else if (grid.row < 0) {
            if (grid.pageScroll == 1 && grid.firstRow > 0) {
                grid.firstRow -= grid.visibleRows;
                grid.row = grid.visibleRows - 1;
            } else {
                --grid.firstRow;
                grid.row = 0;
            }
        } else if (grid.row >= grid.visibleRows || (GetUIGridSelection(grid) >= grid.dictionaryCount && !grid.drawEmpty)) {
            if (grid.pageScroll == 1 && grid.firstRow < maximum) {
                grid.row -= grid.visibleRows;
                grid.firstRow += grid.visibleRows;
            } else {
                ++grid.firstRow;
                --grid.row;
            }
        }
        grid.transitionDuration = 0.25f;
    } else if (pointer.wheel != 0.0f && !grid.dragging) {
        grid.pointerScrolling = 0;
        const std::int32_t direction = pointer.wheel < 0.0f ? 1 : -1;
        grid.firstRow += grid.pageScroll == 1 ? direction * grid.visibleRows : direction;
        if (pointer.wheel < 0.0f && grid.columns * (grid.firstRow < 0 ? 0 : grid.firstRow) >= grid.dictionaryCount) {
            if (grid.pageScroll == 1)
                grid.firstRow = maximum;
            else
                --grid.firstRow;
        }
        grid.transitionDuration = 0.25f;
    } else {
        const std::int32_t drawRows = GridDrawRowCount(grid);
        Vector4 outer{}, track{}, thumb{};
        GetUIGridScrollbarBounds(grid, pointer, &outer, &track, &thumb);
        const float thumbTop = outer.y + thumb.y;
        const float thumbBottom = thumbTop + thumb.w;
        const float left = outer.x + thumb.x;
        if (!grid.dragging && pointer.leftClicks == 1 && pointer.position.x > left && pointer.position.x < left + thumb.z && pointer.position.y > thumbTop &&
            pointer.position.y < thumbBottom)
            grid.dragging = 1;
        else if (grid.dragging == 1 && !pointer.leftDown)
            grid.dragging = 0;
        if (grid.dragging == 1 || grid.dragFraction != 0.0f) {
            grid.pointerScrolling = 1;
            const float grabOffset = pointer.position.y - thumbTop;
            grid.pointerScrollOffset = static_cast<std::int32_t>(grabOffset < track.y ? track.y :
                    grabOffset > thumb.w + track.y                                    ? thumb.w + track.y :
                                                                                        grabOffset);
            const float location = pointer.position.y - grabOffset;
            float fraction = grid.dragFraction + grid.dragVelocity;
            if (grid.dragging)
                fraction = location < thumbTop ? 0.0f : location > thumbBottom ? 1.0f : (location - thumbTop) / (thumbBottom - thumbTop);
            const float totalOffset = GetUIGridRowOffset(grid, drawRows);
            float row = static_cast<float>(drawRows) * fraction;
            if (fraction - grid.dragFraction > 0.0f)
                row += 1.0f;
            grid.scrollOffset = totalOffset * fraction;
            if (grid.pageScroll == 1) {
                const float pageOffset = GetUIGridRowOffset(grid, grid.visibleRows);
                const float remainder =
                    static_cast<float>(static_cast<std::int32_t>(grid.scrollOffset * 100.0f) % static_cast<std::int32_t>(pageOffset * 100.0f)) / 100.0f;
                const std::int32_t rowRemainder = static_cast<std::int32_t>(row) % grid.visibleRows;
                row += remainder / pageOffset <= 0.5f ? -static_cast<float>(rowRemainder) : static_cast<float>(grid.visibleRows - rowRemainder);
            }
            grid.dragVelocity = fraction - grid.dragFraction;
            grid.firstRow = static_cast<std::int32_t>(row);
            const float speed = std::fabs(grid.dragVelocity);
            const float distance = std::fabs(GetUIGridRowOffset(grid, grid.firstRow) - grid.scrollOffset);
            const float velocity = totalOffset * grid.dragVelocity;
            if (speed >= 0.001f && velocity >= 1.0f && distance >= 1.0f)
                grid.transitionDuration = frameTime * velocity / distance;
            else {
                if (speed < 0.001f && grid.pageScroll != 1 && row - static_cast<float>(grid.firstRow) > 0.5f)
                    ++grid.firstRow;
                grid.transitionDuration = 0.25f;
            }
            grid.dragFraction = grid.dragging ? fraction : 0.0f;
        }
    }
    if (grid.transitionDuration < 0.25f)
        grid.transitionDuration = 0.25f;
    if (grid.wrap == 1) {
        if (grid.firstRow > maximum)
            grid.firstRow = 0;
        else if (grid.firstRow < 0)
            grid.firstRow = maximum;
    } else if (!IsUITransitionActive(grid.transition)) {
        if (grid.firstRow > maximum) {
            grid.firstRow = maximum;
            snapFirst = maximum;
        } else if (grid.firstRow < 0) {
            grid.firstRow = 0;
            snapFirst = 0;
        }
    }
    const float target = GetUIGridRowOffset(grid, grid.firstRow);
    if (grid.pointerScrolling == 1) {
        Vector4 row{};
        GetUIGridRowBounds(grid, nullptr, &row);
        const float ratio = grid.transitionDuration / row.w;
        const float distance = std::fabs(target - grid.scrollOffset);
        UIAnim_Start(grid.transition, grid.scrollOffset, target, distance * ratio, 0.0f);
    } else if (snapFirst != grid.firstRow)
        UIAnim_Start(grid.transition, grid.scrollOffset, target, grid.transitionDuration, 0.0f);
}

void AicaExecuteWrapper(std::uint32_t sound) {
    if (sound >= UI_SOUND_COMMANDS.size())
        return;
    const std::uint64_t frame = d3t::GetNumRenderedFrames();
    if (uiSoundFrames[sound] == frame)
        return;
    uiSoundFrames[sound] = frame;
    ExecuteAicaCommand(UI_SOUND_COMMANDS[sound], 0, 0);
}

void InitializeUIGrid(UIGrid &grid, const UIGrid &configuration) {
    std::memcpy(&grid, &configuration, 0x78);
    ResetUIGrid(grid);
    grid.transition = UIAnimAllocSlot();
    grid.transitionDuration = 0.25f;
}

void ResetUIGrid(UIGrid &grid) {
    UIAnim_Start(grid.transition, 0.0f, 0.0f, 0.01f, 0.0f);
    grid.firstRow = 0;
    grid.scrollOffset = 0.0f;
    grid.row = grid.column = 0;
    grid.dragFraction = grid.dragVelocity = 0.0f;
    grid.reserveda4 = grid.activate = 0;
    grid.previousFirstRow = grid.previousRow = grid.previousColumn = -1;
    grid.notifySelection = 1;
}

void SetUIGridDictionary(UIGrid &grid, std::int32_t count, const std::int32_t *dictionary) {
    grid.dictionary = dictionary;
    grid.dictionaryCount = count;
    const std::int32_t last = grid.rows - 1 < 0 ? 0 : grid.rows - 1;
    if (count > grid.columns * (last + 1) - 1)
        grid.rows = count / grid.columns + (count % grid.columns > 0);
}

std::int32_t GetUIGridSelection(const UIGrid &grid) {
    if (currentUIGridTabSelector && (currentUIGridTabSelector->active || currentUIGridTabSelector->previousActive))
        return -1;
    const std::int32_t row = grid.firstRow + grid.row;
    return grid.column + grid.columns * (row < 0 ? 0 : row);
}

void GetUIGridRowBounds(const UIGrid &grid, Vector4 *outer, Vector4 *row, std::int32_t rowIndex, std::int32_t column) {
    Vector4 bounds = grid.bounds;
    if (grid.panel) {
        UILayoutRect rect{};
        UI_GetWindowRect(static_cast<d3t::UIWindow *>(grid.panel), rect);
        bounds = {.x = rect.x, .y = rect.y, .z = rect.width, .w = rect.height};
    }
    if (outer)
        *outer = bounds;
    if (!row)
        return;
    const UIGridStyle &style = GridStyle(grid);
    const float safeHeight = GetS2UserInterfaceSafeArea() * 1080.0f;
    const float width = bounds.z - safeHeight * style.marginRight - safeHeight * style.marginLeft;
    const float spacing = GridSpacing(grid);
    *row = {.x = 0.0f,
        .y = 0.0f,
        .z = (width - static_cast<float>(grid.columns - 1) * spacing) / grid.columns,
        .w = (bounds.w - static_cast<float>(grid.visibleRows - 1) * spacing) / grid.visibleRows};
    if (rowIndex != -1 && column != -1 && grid.rowSpan)
        row->w *= grid.rowSpan(const_cast<UIGrid *>(&grid), column + (rowIndex < 0 ? 0 : rowIndex) * grid.columns);
}

float GetUIGridRowOffset(const UIGrid &grid, std::int32_t rowIndex) {
    Vector4 outer{}, row{};
    GetUIGridRowBounds(grid, &outer, &row, rowIndex, 0);
    const float spacing = GridSpacing(grid);
    if (!grid.rowSpan)
        return (row.w + spacing) * rowIndex;
    const float baseHeight = outer.w / grid.visibleRows;
    float offset = 0.0f;
    for (std::int32_t i = 0; i < rowIndex; ++i) {
        Vector4 item{};
        GetUIGridRowBounds(grid, nullptr, &item, i, 0);
        offset += std::round(outer.w / baseHeight) * spacing + item.w;
    }
    return offset - spacing;
}

std::int32_t GetUIGridMaximumFirstRow(const UIGrid &grid) {
    std::int32_t maximum = grid.rows <= grid.visibleRows ? 0 : grid.rows - grid.visibleRows;
    if (grid.columns * (maximum < 0 ? 0 : maximum) > grid.dictionaryCount)
        maximum = grid.dictionaryCount / grid.columns - grid.visibleRows + 1;
    if (!grid.drawEmpty && grid.columns * (grid.visibleRows + maximum < 0 ? 0 : grid.visibleRows + maximum) > grid.dictionaryCount) {
        const float rows = static_cast<float>(grid.dictionaryCount) / grid.columns - grid.visibleRows;
        maximum = static_cast<std::int32_t>(rows);
        if (rows - maximum > 0.0f)
            ++maximum;
    }
    if (grid.rowSpan) {
        float height = 0.0f;
        maximum = grid.rows - 1;
        for (std::int32_t i = maximum; i >= 0; maximum = i--) {
            Vector4 outer{}, row{};
            GetUIGridRowBounds(grid, &outer, &row, i, 0);
            height += GridSpacing(grid) + row.w;
            if (height > outer.w)
                break;
        }
    }
    return maximum < 0 ? 0 : maximum;
}

void GetUIGridScrollbarBounds(const UIGrid &grid, const WindowUIInput &pointer, Vector4 *outer, Vector4 *track, Vector4 *thumb) {
    const UIGridStyle &style = GridStyle(grid);
    const float safeHeight = GetS2UserInterfaceSafeArea() * 1080.0f;
    const float left = safeHeight * style.marginLeft;
    const float right = safeHeight * style.marginRight;
    const float trackHeight = grid.bounds.w - 2.0f;
    const std::int32_t drawRows = GridDrawRowCount(grid);
    const float totalOffset = GetUIGridRowOffset(grid, drawRows);
    float thumbHeight = trackHeight;
    if (drawRows > grid.visibleRows)
        thumbHeight = grid.rowSpan ? grid.bounds.w / totalOffset * trackHeight : static_cast<float>(grid.visibleRows) / drawRows * trackHeight;
    thumbHeight = std::fmax(right - 1.0f, thumbHeight);
    float top = grid.dragging == 1 ? pointer.position.y - grid.pointerScrollOffset - grid.bounds.y :
        totalOffset <= 0.0f        ? 1.0f :
                                     GetUITransitionValue(grid.transition) / totalOffset * trackHeight + 1.0f;
    if (top < 1.0f)
        top = 1.0f;
    if (top + thumbHeight > trackHeight)
        top = trackHeight - thumbHeight + 1.0f;
    if (outer)
        *outer = {.x = grid.bounds.x + grid.bounds.z - left - right, .y = grid.bounds.y, .z = left + right, .w = grid.bounds.w};
    if (track)
        *track = {.x = left - 1.0f, .y = 1.0f, .z = right - 1.0f, .w = trackHeight};
    if (thumb)
        *thumb = {.x = left - 1.0f, .y = top, .z = right - 1.0f, .w = thumbHeight};
}

void ProcessMovesGridInput(UIGrid &grid, const WindowUIInput &pointer, float frameTime) {
    grid.allowPointerSelection = 1;
    if (grid.previousFirstRow == grid.firstRow && grid.previousRow == grid.row) {
        if (grid.row < 0) {
            grid.row = 0;
            --grid.firstRow;
        } else if (grid.row >= grid.visibleRows || (GetUIGridSelection(grid) >= grid.dictionaryCount && !grid.drawEmpty)) {
            if (grid.pageScroll == 1 && grid.firstRow < GetUIGridMaximumFirstRow(grid)) {
                grid.row -= grid.visibleRows;
                grid.firstRow += grid.visibleRows;
            } else {
                --grid.row;
                ++grid.firstRow;
            }
        }
    }
    grid.previousFirstRow = grid.firstRow;
    grid.previousRow = grid.row;
    grid.previousColumn = grid.column;
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    const std::uint16_t repeated = g_inputBuffer[0].repeatedPresses;
    if (pressed & 0x400U)
        grid.activate = 1;
    if ((repeated & 0x4000U) && grid.column > 0) {
        --grid.column;
        AicaExecuteWrapper(grid.sound);
    }
    if ((repeated & 0x8000U) && grid.column < grid.columns - 1) {
        ++grid.column;
        if (GetUIGridSelection(grid) < grid.dictionaryCount || grid.drawEmpty)
            AicaExecuteWrapper(grid.sound);
        else
            --grid.column;
    }
    if ((repeated & 0x1000U) && grid.row + grid.firstRow > 0) {
        --grid.row;
        AicaExecuteWrapper(grid.sound);
    }
    if (repeated & 0x2000U) {
        ++grid.row;
        if (GetUIGridSelection(grid) < grid.dictionaryCount || grid.drawEmpty)
            AicaExecuteWrapper(grid.sound);
        else {
            const std::int32_t last = grid.dictionaryCount - 1;
            grid.row = last / grid.columns - grid.firstRow;
            grid.column = last - grid.columns * (grid.firstRow + grid.row);
        }
    }
    UpdateUIGridScroll(grid, pointer, frameTime);
}

void ProcessUIGridControllerInput(UIGrid &grid) {
    if (uiGridInputEnabled == 1) {
        const std::uint16_t pressed = g_inputBuffer[0].pressedPriority, repeated = g_inputBuffer[0].repeatedPresses;
        if (pressed & 0x400U)
            grid.activate = 1;
        if (repeated & 0x4000U) {
            if (grid.column > 0) {
                --grid.column;
                AicaExecuteWrapper(grid.sound);
            } else if (currentUIGridTabSelector && GetCurrentUIState() == -1) {
                uiGridInputEnabled = 0;
                currentUIGridTabSelector->active = 1;
                currentUIGridTabSelector->hovered = currentUIGridTabSelector->previousHover;
                AicaExecuteWrapper(grid.sound);
            }
        }
        if ((repeated & 0x8000U) && grid.column < grid.columns - 1) {
            ++grid.column;
            if (GetUIGridSelection(grid) < grid.dictionaryCount || grid.drawEmpty)
                AicaExecuteWrapper(grid.sound);
            else
                --grid.column;
        }
        if ((repeated & 0x1000U) && grid.row + grid.firstRow > 0) {
            --grid.row;
            AicaExecuteWrapper(grid.sound);
        }
        if (repeated & 0x2000U) {
            ++grid.row;
            if (GetUIGridSelection(grid) < grid.dictionaryCount || grid.drawEmpty)
                AicaExecuteWrapper(grid.sound);
            else {
                const std::int32_t last = grid.dictionaryCount - 1;
                if (last < grid.dictionaryCount) {
                    grid.row = last / grid.columns - grid.firstRow;
                    grid.column = last - grid.columns * (grid.firstRow + grid.row);
                }
            }
        }
        if ((pressed & 0x200U) && currentUIGridTabSelector && GetCurrentUIState() == -1) {
            uiGridInputEnabled = 0;
            currentUIGridTabSelector->active = 1;
            currentUIGridTabSelector->hovered = currentUIGridTabSelector->previousHover;
        }
    }
}

std::int64_t ProcessUIGridInput(UIGrid &grid, const WindowUIInput &pointer, float frameTime) {
    currentUIGrid = &grid;
    const std::int32_t previousFirst = grid.firstRow;
    grid.allowPointerSelection = 1;
    if (grid.previousFirstRow == previousFirst && grid.previousRow == grid.row) {
        if (grid.row < 0) {
            grid.row = 0;
            grid.firstRow = previousFirst - 1;
        } else if (grid.row >= grid.visibleRows || (GetUIGridSelection(grid) >= grid.dictionaryCount && !grid.drawEmpty)) {
            if (grid.pageScroll == 1 && previousFirst < GetUIGridMaximumFirstRow(grid)) {
                grid.row -= grid.visibleRows;
                grid.firstRow = previousFirst + grid.visibleRows;
            } else {
                --grid.row;
                grid.firstRow = previousFirst + 1;
            }
        }
    }
    grid.previousFirstRow = grid.firstRow;
    grid.previousRow = grid.row;
    grid.previousColumn = grid.column;
    ProcessUIGridControllerInput(grid);
    UpdateUIGridScroll(grid, pointer, frameTime);
    return 1;
}

void RenderUIGrid(UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer) {
    const std::int32_t selected = GetUIGridSelection(grid);
    if (grid.previousFirstRow != grid.firstRow || grid.previousRow != grid.row || grid.previousColumn != grid.column || grid.activate == 1 ||
        grid.notifySelection == 1) {
        if (selected >= 0) {
            const std::int32_t *const data =
                selected < grid.dictionaryCount && grid.dictionary && grid.selectionData ? grid.selectionData(&grid, selected) : nullptr;
            Vector4 bounds{};
            GetUIGridRowBounds(grid, nullptr, &bounds);
            bounds.x = grid.bounds.x + (bounds.z + GridSpacing(grid)) * grid.column;
            bounds.y = grid.bounds.y + GetUIGridRowOffset(grid, grid.row);
            if (grid.previousFirstRow != grid.firstRow || grid.previousRow != grid.row || grid.previousColumn != grid.column || grid.notifySelection == 1)
                QueueGridAction(grid, element, 0, bounds, data);
            if (grid.activate == 1) {
                QueueGridAction(grid, element, 3, bounds, data);
                grid.activate = 0;
            }
        }
        if (grid.notifySelection == 1)
            grid.notifySelection = 0;
    }
    if (grid.dictionaryCount > 0 || grid.drawEmpty) {
        for (std::int32_t row = 0; row < grid.rows; ++row) {
            Vector4 bounds{};
            GetUIGridRowBounds(grid, nullptr, &bounds);
            bounds.y = GetUIGridRowOffset(grid, row) - grid.scrollOffset;
            if (bounds.y + bounds.w < 0.0f)
                continue;
            if (bounds.y > grid.bounds.w)
                break;
            for (std::int32_t column = 0; column < grid.columns; ++column) {
                const std::int32_t ordinal = column + grid.columns * row;
                if (ordinal >= grid.dictionaryCount && !grid.drawEmpty)
                    break;
                GetUIGridRowBounds(grid, nullptr, &bounds, row, column);
                bounds.x = (bounds.z + GridSpacing(grid)) * column;
                bounds.y = GetUIGridRowOffset(grid, row) - grid.scrollOffset;
                const Vector4 absolute{.x = grid.bounds.x + bounds.x, .y = grid.bounds.y + bounds.y, .z = bounds.z, .w = bounds.w};
                const bool over = pointer.position.x > absolute.x && pointer.position.x < absolute.x + absolute.z && pointer.position.y > absolute.y &&
                    pointer.position.y < absolute.y + absolute.w;
                if (grid.allowPointerSelection && (pointer.position.x != pointer.previousPosition.x || pointer.position.y != pointer.previousPosition.y) &&
                    over && !IsUITransitionActive(grid.transition) && ordinal < grid.dictionaryCount) {
                    const std::int32_t oldRow = grid.row, oldColumn = grid.column;
                    grid.row = ordinal / grid.columns - grid.firstRow;
                    grid.column = ordinal - grid.columns * (grid.firstRow + grid.row);
                    if (oldRow != grid.row || oldColumn != grid.column)
                        AicaExecuteWrapper(grid.sound);
                }
                const std::int32_t *const data =
                    ordinal < grid.dictionaryCount ? (grid.selectionData ? grid.selectionData(&grid, ordinal) : grid.dictionary + ordinal) : nullptr;
                if (over && pointer.leftClicks)
                    QueueGridAction(grid, element, 3, absolute, data);
                if (over && pointer.leftDown) {
                    QueueGridAction(grid, element, 1, absolute, data);
                    grid.pointerWasDown = 1;
                } else if (grid.pointerWasDown) {
                    QueueGridAction(grid, element, 2, absolute, data);
                    grid.pointerWasDown = 0;
                }
                if (over)
                    QueueGridAction(grid, element, 0, absolute, data);
                if (grid.drawRow)
                    grid.drawRow(&grid, nullptr, &grid.bounds, &bounds, static_cast<std::int32_t>(GridSpacing(grid)), element,
                        ordinal < grid.dictionaryCount ? grid.dictionary : nullptr, ordinal);
            }
        }
    }
    grid.allowPointerSelection = 0;
}

void DrawUIGridScrollbar(const UIGrid &grid, std::uint32_t, const WindowUIInput &pointer) {
    const UIGridStyle &style = GridStyle(grid);
    Vector4 outer{}, track{}, thumb{};
    GetUIGridScrollbarBounds(grid, pointer, &outer, &track, &thumb);
    const float xScale = 1920.0f / GetUIViewportWidth();
    const d3t::UIRect clip{outer.x * xScale, outer.y, outer.z * xScale, outer.w};
    const float scale = GetS2UserInterfaceSafeArea() * (GetUIViewportWidth() < 1920.0f ? GetUIViewportWidth() / 1920.0f : 1.0f);
    const float padding = style.scrollPadding * scale;
    const auto rectangle = [&](float x, float y, float width, float height, float rounding, std::uint32_t colour) {
        d3t::DrawUIRoundedRectangle({(outer.x + x) * xScale, outer.y + y, width * xScale, height}, rounding * scale, colour, clip);
    };
    rectangle(track.x, track.y, track.z, track.w, style.windowRounding, style.colours[2]);
    rectangle(track.x + padding, track.y + padding, track.z - padding * 2.0f, track.w - padding * 2.0f, style.windowRounding, style.colours[0]);
    const float width = thumb.z - padding * 4.0f;
    const float halfWidth = width * 0.5f;
    const float middleHeight = track.w - halfWidth * 2.0f;
    const std::uint32_t fadedTrack =
        (style.colours[1] & 0xffffffU) | (static_cast<std::uint32_t>(static_cast<float>(style.colours[1] >> 24U) * 0.40000001f) << 24U);
    rectangle(track.x + track.z * 0.44999999f, track.y + halfWidth, (track.z - padding) * 0.5f, middleHeight, style.elementRounding, fadedTrack);
    rectangle(track.x + track.z * 0.44999999f + (track.z - padding) * 0.5f * 0.55000001f, track.y + halfWidth, (track.z - padding) * 0.5f * 0.44999999f,
        middleHeight, style.elementRounding, style.colours[1]);
    const float x = thumb.x + padding * 2.0f, y = thumb.y + padding * 2.0f;
    const float height = thumb.w - padding * 4.0f;
    rectangle(x, y, width, height, style.elementRounding, style.colours[3]);
    rectangle(x + width * 0.44999999f, y + halfWidth, width * 0.55000001f, height - halfWidth * 2.0f, style.elementRounding, style.colours[5]);
    const std::uint32_t thumbHighlight = (style.colours[3] & 0xffffffU) | 0x69000000U;
    rectangle(x, y + 1.0f, width, width * 0.69999999f, style.elementRounding, thumbHighlight);
    rectangle(x, y + height - width * 0.69999999f - 1.0f, width, width * 0.69999999f, style.elementRounding, thumbHighlight);
    rectangle(x, y, width * 0.55000001f, height, style.elementRounding * 0.5f, style.colours[4]);
    rectangle(x, y + 1.0f, width, width * 0.40000001f, style.elementRounding, thumbHighlight);
    rectangle(x, y + height - width * 0.40000001f - 1.0f, width, width * 0.40000001f, style.elementRounding, thumbHighlight);
}

std::int32_t PlaceUIGridCellWidgets(UIGridTemplate &cellTemplate, std::int32_t index, const UILayoutRect &holder, const UILayoutRect &cell) {
    if (!cellTemplate.windows || !cellTemplate.ids)
        return 0;
    cellTemplate.rootCount = cellTemplate.clipCount = 0;
    if (!cellTemplate.initialized) {
        for (std::int32_t window = cellTemplate.first; window < cellTemplate.end; ++window) {
            d3t::UIWindow *const record = cellTemplate.windows[window];
            if (!record)
                continue;
            cellTemplate.idTemplatePositions[window - cellTemplate.first] = static_cast<std::int32_t>(strnlen(record->id.data(), 64)) - 1;
            cellTemplate.parentTemplatePositions[window - cellTemplate.first] = static_cast<std::int32_t>(strnlen(record->layout.parentId.data(), 64)) - 1;
        }
        cellTemplate.initialized = 1;
    }
    for (std::int32_t window = cellTemplate.first; window < cellTemplate.end; ++window) {
        d3t::UIWindow *const record = cellTemplate.windows[window];
        if (!record)
            continue;
        const std::int32_t id = cellTemplate.idTemplatePositions[window - cellTemplate.first];
        const std::int32_t parent = cellTemplate.parentTemplatePositions[window - cellTemplate.first];
        if (id != -1)
            std::snprintf(record->id.data() + id, record->id.size() - id, "%i", index);
        if (parent == -1)
            cellTemplate.roots[cellTemplate.rootCount++] = &record->layout;
        else
            std::snprintf(record->layout.parentId.data() + parent, record->layout.parentId.size() - parent, "%i", index);
    }
    for (std::int32_t clip = 0; clip < cellTemplate.end - cellTemplate.first; ++clip) {
        const std::int32_t window = cellTemplate.clipWindows[clip];
        if (window == -1)
            break;
        if (cellTemplate.windows[window])
            cellTemplate.clips[cellTemplate.clipCount++] = &cellTemplate.windows[window]->layout;
    }
    float y = holder.y + cell.y;
    const float x = holder.x + cell.x;
    std::int32_t clipped = 1;
    if (cell.height + y <= holder.y + holder.height) {
        if (holder.y <= y) {
            for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
                d3t::UIWindowLayout *const layout = cellTemplate.clips[clip];
                if (layout) {
                    layout->height.scale = 1.0f;
                    layout->alignY = 1;
                }
            }
            for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item)
                if (cellTemplate.layoutItems[item])
                    cellTemplate.layoutItems[item]->offsetY = 0.0f;
            clipped = 0;
        } else {
            const float cut = holder.y - y;
            y += cut;
            for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
                d3t::UIWindowLayout *const layout = cellTemplate.clips[clip];
                if (layout) {
                    layout->height.scale = (cell.height - cut) / 1080.0f / layout->height.value.fraction;
                    layout->alignY = 6;
                }
            }
            for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item)
                if (cellTemplate.layoutItems[item])
                    cellTemplate.layoutItems[item]->offsetY = -(cut / cell.height);
        }
    } else {
        const float cut = cell.height + y - holder.y - holder.height;
        y -= cut;
        const float share = cut / cell.height;
        for (std::int32_t clip = 0; clip < cellTemplate.clipCount; ++clip) {
            d3t::UIWindowLayout *const layout = cellTemplate.clips[clip];
            if (layout) {
                layout->height.scale = 1.0f - share;
                layout->alignY = 7;
            }
        }
        for (std::int32_t item = cellTemplate.layoutFirst; item < cellTemplate.layoutEnd; ++item)
            if (cellTemplate.layoutItems[item])
                cellTemplate.layoutItems[item]->offsetY = share;
    }
    for (std::int32_t label = cellTemplate.labelFirst; label < cellTemplate.labelEnd; ++label)
        if (cellTemplate.labels[label])
            cellTemplate.labels[label]->scaleVarAssign = clipped ^ 1;
    for (std::int32_t root = 0; root < cellTemplate.rootCount; ++root) {
        d3t::UIWindowLayout *const layout = cellTemplate.roots[root];
        if (!layout)
            continue;
        layout->offsetX = x / static_cast<float>(UI_GetAspectWidth());
        layout->offsetY = y / 1080.0f;
        layout->width.value.fraction = cell.width / 1920.0f;
        layout->height.value.fraction = cell.height / 1080.0f;
    }
    return clipped;
}

void AfterDrawUIGridCell(
    UIGridTemplate &cellTemplate, UIGrid &grid, std::int32_t index, std::int32_t firstId, std::int32_t endId, const UILayoutRect &holder, nk_panel *panel) {
    nk_context &context = *GetNativeUiContext();
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = row.item_height = row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(cellTemplate.viewSlot);
    if (view) {
        std::int32_t from = -1, to = view->widgetCount;
        for (std::int32_t widget = 0; widget < view->widgetCount; ++widget) {
            if (view->widgetKinds[widget] != 1)
                continue;
            d3t::UIWindow *const window = UI_GetWindow(view->widgetPoolIndices[widget]);
            if (!window)
                continue;
            const std::int32_t firstLength = cellTemplate.idTemplatePositions[firstId - cellTemplate.first];
            if (from == -1 && !std::strncmp(window->id.data(), cellTemplate.ids[firstId], firstLength)) {
                from = widget - 1;
                continue;
            }
            if (endId != -1 && !std::strncmp(window->id.data(), cellTemplate.ids[endId], cellTemplate.idTemplatePositions[endId - cellTemplate.first])) {
                to = widget ? widget - 1 : view->widgetCount;
                break;
            }
        }
        if (from < to)
            UI_InputHandler(context, cellTemplate.viewSlot, from, to);
        for (std::int32_t root = 0; root < cellTemplate.rootCount; ++root)
            if (cellTemplate.roots[root]) {
                cellTemplate.roots[root]->offsetX = 1920.0f;
                cellTemplate.roots[root]->offsetY = 1080.0f;
            }
        for (std::int32_t window = cellTemplate.first; window < cellTemplate.end; ++window) {
            d3t::UIWindow *const record = cellTemplate.windows[window];
            if (!record)
                continue;
            const std::int32_t id = cellTemplate.idTemplatePositions[window - cellTemplate.first];
            const std::int32_t parent = cellTemplate.parentTemplatePositions[window - cellTemplate.first];
            if (id != -1)
                std::snprintf(record->id.data() + id, record->id.size() - id, "#");
            if (parent != -1)
                std::snprintf(record->layout.parentId.data() + parent, record->layout.parentId.size() - parent, "#");
        }
    }
    context.style.window.fixed_background.data.color.a = 0;
    std::array<char, 256> name{};
    std::snprintf(name.data(), name.size(), "Grid_AfterDraw_%i", index);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(holder.x, holder.y, holder.width, holder.height), NK_WINDOW_NO_SCROLLBAR);
        if (context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, grid.dictionaryCount);
    }
}

void DrawNativeUIGridScrollbar(UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer) {
    nk_context &context = *GetNativeUiContext();
    const UIGridStyle &style = GridStyle(grid);
    Vector4 outer{}, track{}, thumb{};
    GetUIGridScrollbarBounds(grid, pointer, &outer, &track, &thumb);
    static nk_panel panel{};
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "Scroll %u", element);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &panel, name.data(), name.data(), nk_rect(outer.x, outer.y, outer.z, outer.w), NK_WINDOW_NO_SCROLLBAR);
        if (context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    }
    const float border = WithinSafeArea(style.scrollPadding);
    const float width = thumb.z - border * 4.0f, half = width * 0.5f;
    const auto rectangle = [&context](const Vector4 &rect, float rounding, std::uint32_t colour) {
        context.style.button.rounding = ScaleTo1080WithSafeArea(nk_rect(rect.x, rect.y, rect.z, rect.w), rounding);
        if (g_waitingForNextTick != 1)
            return;
        if (context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.z, rect.w);
        nk_color tint{};
        std::memcpy(&tint, &colour, sizeof(tint));
        nk_button_color(&context, tint);
    };
    context.style.button.border = border;
    std::memcpy(&context.style.button.border_color, &style.colours[2], sizeof(nk_color));
    rectangle(track, style.windowRounding, style.colours[0]);
    context.style.button.border = 0.0f;
    context.style.button.border_color = nk_rgba(0, 0, 0, 0);
    const Vector4 line{.x = track.x + track.z * 0.45f, .y = track.y + half, .z = (track.z - border) * 0.5f, .w = track.w - half * 2.0f};
    const std::uint32_t alpha = static_cast<std::uint32_t>(static_cast<float>(style.colours[1] >> 24U) * 0.4f);
    rectangle(line, style.elementRounding, (style.colours[1] & 0xffffffU) | (alpha << 24U));
    rectangle({.x = line.x + line.z * 0.55f, .y = line.y, .z = line.z * 0.45f, .w = line.w}, style.elementRounding, style.colours[1]);
    const float x = thumb.x + border * 2.0f, y = thumb.y + border * 2.0f, height = thumb.w - border * 4.0f;
    rectangle({.x = x, .y = y, .z = width, .w = height}, style.elementRounding, style.colours[3]);
    rectangle({.x = x + width * 0.45f, .y = y + half, .z = width * 0.55f, .w = height - half * 2.0f}, style.elementRounding, style.colours[5]);
    const std::uint32_t grip = (style.colours[3] & 0xffffffU) | 0x69000000U;
    rectangle({.x = x, .y = y + 1.0f, .z = width, .w = width * 0.7f}, style.elementRounding, grip);
    rectangle({.x = x, .y = y + height - width * 0.7f - 1.0f, .z = width, .w = width * 0.7f}, style.elementRounding, grip);
    rectangle({.x = x, .y = y, .z = width * 0.55f, .w = height}, style.elementRounding * 0.5f, style.colours[4]);
    rectangle({.x = x, .y = y + 1.0f, .z = width, .w = width * 0.4f}, style.elementRounding, grip);
    rectangle({.x = x, .y = y + height - width * 0.4f - 1.0f, .z = width, .w = width * 0.4f}, style.elementRounding, grip);
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = row.item_height = row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    context.style.button.rounding = 0.0f;
}

void RenderNativeUIGrid(UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer) {
    nk_context &context = *GetNativeUiContext();
    currentUIGrid = &grid;
    const UIGridStyle &style = GridStyle(grid);
    Vector4 holder{}, cell{};
    GetUIGridRowBounds(grid, &holder, &cell);
    grid.bounds = holder;
    const float spacing = WithinSafeArea(style.spacing);
    context.style.button.rounding = ScaleTo1080WithSafeArea(nk_rect(cell.x, cell.y, cell.z, cell.w), style.elementRounding);
    std::memcpy(&context.style.window.fixed_background, style.background.data(), sizeof(nk_style_item));
    if (g_uiFontSize != style.font && static_cast<std::uint32_t>(style.font) < 6U) {
        g_uiFontSize = style.font;
        UI_SelectFont();
    }
    std::memcpy(&context.style.text.color, &style.colours[6], sizeof(nk_color));
    const bool moved = grid.previousFirstRow != grid.firstRow || grid.previousRow != grid.row || grid.previousColumn != grid.column;
    if (moved || grid.activate == 1 || grid.notifySelection == 1) {
        const std::int32_t selected = GetUIGridSelection(grid);
        if (selected >= 0) {
            const std::int32_t *const data =
                selected < grid.dictionaryCount && grid.dictionary && grid.selectionData ? grid.selectionData(&grid, selected) : nullptr;
            Vector4 selectedBounds{};
            GetUIGridRowBounds(grid, nullptr, &selectedBounds);
            selectedBounds.x = holder.x + (selectedBounds.z + spacing) * grid.column;
            selectedBounds.y = holder.y + GetUIGridRowOffset(grid, grid.row);
            if (moved || grid.notifySelection == 1)
                QueueGridAction(grid, element, 0, selectedBounds, data);
            if (grid.activate == 1) {
                QueueGridAction(grid, element, 3, selectedBounds, data);
                grid.activate = 0;
            }
        }
        grid.notifySelection = 0;
    }
    static nk_panel panel{};
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "Grid %u", element);
    if (g_waitingForNextTick == 1)
        nk_begin_titled(&context, &panel, name.data(), name.data(), nk_rect(holder.x, holder.y, holder.z, holder.w), NK_WINDOW_NO_SCROLLBAR);
    if (grid.dictionaryCount > 0 || grid.drawEmpty) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, grid.dictionaryCount);
        for (std::int32_t row = 0; row < grid.rows; ++row) {
            const float top = GetUIGridRowOffset(grid, row) - grid.scrollOffset;
            if (cell.w + top < 0.0f)
                continue;
            if (top > holder.w)
                break;
            for (std::int32_t column = 0; column < grid.columns; ++column) {
                GetUIGridRowBounds(grid, &holder, &cell, row, column);
                cell.x = (cell.z + spacing) * column;
                cell.y = GetUIGridRowOffset(grid, row) - grid.scrollOffset;
                const std::int32_t ordinal = column + grid.columns * row;
                if (ordinal >= grid.dictionaryCount && !grid.drawEmpty)
                    break;
                const Vector4 absolute{.x = holder.x + cell.x, .y = holder.y + cell.y, .z = cell.z, .w = cell.w};
                const bool over = pointer.position.x > absolute.x && pointer.position.x < absolute.x + absolute.z && pointer.position.y > absolute.y &&
                    pointer.position.y < absolute.y + absolute.w;
                if (grid.allowPointerSelection && over && !IsUITransitionActive(grid.transition) && ordinal < grid.dictionaryCount &&
                    (pointer.position.x != pointer.previousPosition.x || pointer.position.y != pointer.previousPosition.y)) {
                    const std::int32_t oldRow = grid.row, oldColumn = grid.column;
                    grid.row = ordinal / grid.columns - grid.firstRow;
                    grid.column = ordinal % grid.columns;
                    if (oldRow != grid.row || oldColumn != grid.column)
                        AicaExecuteWrapper(grid.sound);
                }
                const std::int32_t *const data = ordinal < grid.dictionaryCount && grid.selectionData ? grid.selectionData(&grid, ordinal) : nullptr;
                if (over && pointer.leftClicks)
                    QueueGridAction(grid, element, 3, absolute, data);
                if (over && pointer.leftDown) {
                    QueueGridAction(grid, element, 1, absolute, data);
                    grid.pointerWasDown = 1;
                } else if (grid.pointerWasDown) {
                    QueueGridAction(grid, element, 2, absolute, data);
                    grid.pointerWasDown = 0;
                }
                if (over)
                    QueueGridAction(grid, element, 0, absolute, data);
                if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                    context.current->layout->row.item = nk_rect(cell.x, cell.y, cell.z, cell.w);
                if (grid.drawRow)
                    grid.drawRow(&grid, &panel, &holder, &cell, std::bit_cast<std::int32_t>(spacing), element,
                        ordinal < grid.dictionaryCount ? grid.dictionary : nullptr, ordinal);
            }
        }
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = row.item_height = row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    DrawNativeUIGridScrollbar(grid, element, pointer);
    context.style.button.rounding = 0.0f;
    grid.allowPointerSelection = 0;
}

} // namespace shenmue::s2
