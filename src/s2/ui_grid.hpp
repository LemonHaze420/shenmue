#pragma once

#include "common/math.hpp"
#include "d3t/ui_runtime.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct WindowUIInput;
struct UIGridStyle {
    float spacing;
    float labelHeight;
    std::array<std::byte, 40> background;
    std::uint32_t borderColour;
    float borderWidth;
    float marginLeft;
    float marginRight;
    float windowRounding;
    float elementRounding;
    float scrollPadding;
    std::array<std::uint32_t, 7> colours;
    std::int32_t font;
    std::uint32_t reserved6c;
};
static_assert(sizeof(UIGridStyle) == 0x70);
static_assert(offsetof(UIGridStyle, marginLeft) == 0x38);
inline constexpr UIGridStyle UI_GRID_FRONTEND_STYLE{.spacing = 8.0f,
    .labelHeight = 25.0f,
    .background = {},
    .borderColour = 0,
    .borderWidth = 10.0f,
    .marginLeft = 0.023148147389292717f,
    .marginRight = 0.025925925001502037f,
    .windowRounding = 8.0f,
    .elementRounding = 6.0f,
    .scrollPadding = 3.0f,
    .colours = {0xff2f2f2fU, 0x0ac8ebf0U, 0xaa5f8ca0U, 0xff3e7195U, 0x23c8ebf0U, 0x50502328U, 0xffffffffU},
    .font = 4};
inline constexpr UIGridStyle UI_GRID_GAME_STYLE{.spacing = 8.0f,
    .labelHeight = 25.0f,
    .background = {},
    .borderColour = 0,
    .borderWidth = 10.0f,
    .marginLeft = 0.023148147389292717f,
    .marginRight = 0.025925925001502037f,
    .windowRounding = 8.0f,
    .elementRounding = 6.0f,
    .scrollPadding = 3.0f,
    .colours = {0xff140f0cU, 0x03c8ebf0U, 0x645b5548U, 0xff5b5548U, 0x0ac8ebf0U, 0x5a502328U, 0xffffffffU},
    .font = 4};
inline constexpr std::array<std::uint32_t, 11> UI_SOUND_COMMANDS{0x400ab, 0x100ab, 0x900ab, 0x200ab, 0x800ab, 0x8ab, 0x108ab, 0x800ab, 0x800ab, 0x900ab, 0xab};

struct UIGrid;
using UIGridAction = d3t::UIMouseCallback;
using UIGridRowDraw = void (*)(UIGrid *, void *, const Vector4 *, const Vector4 *, std::int32_t, std::uint32_t, const std::int32_t *, std::int32_t);

struct UIGrid {
    std::int32_t columns;
    std::int32_t visibleRows;
    std::int32_t rows;
    std::uint8_t drawEmpty;
    std::uint8_t wrap;
    std::uint8_t pageScroll;
    std::uint8_t reserved0f;
    const UIGridStyle *style;
    void *panel;
    Vector4 bounds;
    UIGridRowDraw drawRow;
    std::array<UIGridAction, 4> actions;
    std::int32_t pointerWasDown;
    std::int32_t reserved5c;
    const std::int32_t *(*selectionData)(UIGrid *, std::int32_t);
    std::int32_t (*rowSpan)(UIGrid *, std::int32_t);
    std::uint32_t sound;
    std::uint32_t reserved74;
    std::int32_t dictionaryCount;
    std::uint32_t reserved7c;
    const std::int32_t *dictionary;
    std::int32_t transition;
    float transitionDuration;
    std::int32_t dragging;
    std::int32_t pointerScrolling;
    std::int32_t pointerScrollOffset;
    std::int32_t firstRow;
    float scrollOffset;
    std::int32_t reserveda4;
    std::int32_t row;
    std::int32_t column;
    std::int32_t previousFirstRow;
    std::int32_t previousRow;
    std::int32_t previousColumn;
    float dragFraction;
    float dragVelocity;
    std::int32_t activate;
    std::int32_t notifySelection;
    std::int32_t allowPointerSelection;
};
static_assert(sizeof(UIGrid) == 0xd0);
static_assert(offsetof(UIGrid, drawRow) == 0x30);
static_assert(offsetof(UIGrid, actions) == 0x38);
static_assert(offsetof(UIGrid, rowSpan) == 0x68);
static_assert(offsetof(UIGrid, dictionaryCount) == 0x78);
static_assert(offsetof(UIGrid, dictionary) == 0x80);
static_assert(offsetof(UIGrid, firstRow) == 0x9c);
static_assert(offsetof(UIGrid, row) == 0xa8);
static_assert(offsetof(UIGrid, allowPointerSelection) == 0xcc);


inline constexpr std::size_t UI_GRID_TEMPLATE_CAPACITY = 52;
struct UIGridTemplate {
    std::int32_t initialized, viewSlot, first, end;
    std::array<std::int32_t, UI_GRID_TEMPLATE_CAPACITY> clipWindows;
    d3t::UIWindow **windows;
    const char *const *ids;
    std::int32_t layoutFirst, layoutEnd;
    d3t::UILayoutItem **layoutItems;
    std::int32_t labelFirst, labelEnd;
    d3t::UILabelWidget **labels;
    std::array<std::int32_t, UI_GRID_TEMPLATE_CAPACITY> idTemplatePositions, parentTemplatePositions;
    std::int32_t rootCount;
    std::array<d3t::UIWindowLayout *, UI_GRID_TEMPLATE_CAPACITY> roots;
    std::int32_t clipCount;
    std::array<d3t::UIWindowLayout *, UI_GRID_TEMPLATE_CAPACITY> clips;
};
static_assert(sizeof(UIGridTemplate) == 0x600);
static_assert(offsetof(UIGridTemplate, windows) == 0xe0 && offsetof(UIGridTemplate, idTemplatePositions) == 0x110);
static_assert(offsetof(UIGridTemplate, rootCount) == 0x2b0 && offsetof(UIGridTemplate, clipCount) == 0x458);
inline constexpr std::array<const char *, 5> UI_ITEMS_GRID_WINDOWS{"SubMenu_Items::Panel_GridHolder", "SubMenu_Items::GridElementParent_#",
    "SubMenu_Items::GridElement_#", "SubMenu_Items::GridElement_Texture_#", "SubMenu_Items::GridElement_Shadow_#"};
inline constexpr std::array<const char *, 2> UI_ITEMS_GRID_LAYOUT_ITEMS{
    "SubMenu_Items::GridElementParent_Background_Spacepush", "SubMenu_Items::GridElementBackground_Game_Shading"};
std::int32_t PlaceUIGridCellWidgets(UIGridTemplate &cellTemplate, std::int32_t index, const UILayoutRect &holder, const UILayoutRect &cell);
void AfterDrawUIGridCell(
    UIGridTemplate &cellTemplate, UIGrid &grid, std::int32_t index, std::int32_t firstId, std::int32_t endId, const UILayoutRect &holder, nk_panel *panel);
void RenderNativeUIGrid(UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer);

inline constexpr std::uint32_t UI_GRID_TAB_LABEL_SELECTED = 0xffffffffU, UI_GRID_TAB_LABEL_UNSELECTED = 0x40ffffffU;
inline constexpr std::uint32_t UI_GRID_TAB_LABEL_FRONT_HOVER = 0xff258fe6U;

struct UIGridTabSelector {
    std::int32_t hovered, previousHover, selected, previousSelected;
    std::uint8_t active, previousActive, reserved12[2];
    std::int32_t first, last, reserved1c;
    void **buttons;
    void **labels;
    std::uint8_t coloursInitialized, reserved31[7];
};
static_assert(sizeof(UIGridTabSelector) == 0x38 && offsetof(UIGridTabSelector, active) == 0x10 && offsetof(UIGridTabSelector, labels) == 0x28);
extern UIGridTabSelector *currentUIGridTabSelector;
extern UIGrid *currentUIGrid;
std::int64_t ResetUIGridTabSelector();
void SetUIGridTabSelector(UIGridTabSelector *selector, std::int32_t reset);
void UpdateUIGridTabColours(UIGridTabSelector &selector);
std::int32_t ProcessUIGridTabInput(UIGridTabSelector &selector);
void ClickUIGridTab(UIGridTabSelector &selector, void *window);
void EnterUIGridTab(UIGridTabSelector &selector, void *window);
void LeaveUIGridTab(UIGridTabSelector &selector, void *window);

void InitializeUIGrid(UIGrid &grid, const UIGrid &configuration);
void ResetUIGrid(UIGrid &grid);
void SetUIGridDictionary(UIGrid &grid, std::int32_t count, const std::int32_t *dictionary);
std::int32_t GetUIGridSelection(const UIGrid &grid);
std::int32_t GetUIGridMaximumFirstRow(const UIGrid &grid);
float GetUIGridRowOffset(const UIGrid &grid, std::int32_t row);
void GetUIGridRowBounds(const UIGrid &grid, Vector4 *outer, Vector4 *row, std::int32_t rowIndex = -1, std::int32_t column = -1);
void GetUIGridScrollbarBounds(const UIGrid &grid, const WindowUIInput &pointer, Vector4 *outer, Vector4 *track, Vector4 *thumb);
void ProcessMovesGridInput(UIGrid &grid, const WindowUIInput &pointer, float frameTime);
std::int64_t ProcessUIGridInput(UIGrid &grid, const WindowUIInput &pointer, float frameTime);
void UpdateUIGridScroll(UIGrid &grid, const WindowUIInput &pointer, float frameTime);
void RenderUIGrid(UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer);
void DrawUIGridScrollbar(const UIGrid &grid, std::uint32_t element, const WindowUIInput &pointer);
void AicaExecuteWrapper(std::uint32_t sound);
} // namespace shenmue::s2