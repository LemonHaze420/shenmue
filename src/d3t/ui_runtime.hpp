#pragma once




#include "d3t/ui_definition.hpp"
#include "d3t/buffer.hpp"
#include "d3t/ui_nuklear.hpp"
#include "d3t/ui_view.hpp"
#include "shenmue.h"
#if SHENMUE_IS_S1
#include "s1/d3t/namespace.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/hlib.hpp"
#include "s1/screen_space.hpp"
#endif
#include "common/math.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#if SHENMUE_IS_S1
#include <vector>
#endif

struct cJSON;

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif


struct UIViewport {
    Vector4 rect;
    Vector2 depthRange;
};
static_assert(sizeof(UIViewport) == 0x18);
void Nuklear_RenderPass(const UIViewport *viewport);
void DrawAspectBars(float alpha, const UIViewport &viewport, std::int32_t reveal);
#if SHENMUE_IS_S2
using NuklearFrameCallback = std::intptr_t (*)();
extern NuklearFrameCallback g_nuklearFrameCallback;
extern bool g_nuklearTargetRedirectPending;
extern const std::uint32_t *g_nuklearTargetRedirect;
std::intptr_t InvokeNuklearFrame(d3t::D3TDeviceContext *context);
void FinishNuklearFrame();
UIViewport AdjustScreenForAspectRatio(std::int32_t mode);
void SetNuklearAspectRatio(std::int32_t mode);
void Nuklear_BindRenderTarget();
void NuklearUpdateForAspectRatio(std::int32_t mode);
void NuklearDrawAspectBars(float alpha, std::int32_t mode, std::int32_t reveal);
inline constexpr std::uint64_t kNuklearCompositeBlend = 0xf0000291ULL;
void NuklearCompositeNullsub(d3t::D3TDeviceContext *context, ...);
void CopyNuklearTexToBackBuffer(d3t::D3TDeviceContext *context, const std::uint32_t *texture);
void CopyNuklearTexToBackBuffer();
#endif
// A view slot's image capacity: 128 in S1, 80 in S2.
#if SHENMUE_IS_S1
inline constexpr std::size_t kUIViewImageCapacity = d3t::kS1UIViewImageCapacity;
#else
inline constexpr std::size_t kUIViewImageCapacity = d3t::kS2UIViewImageCapacity;
// The S2 element indices the runtime reads (S1 keeps them in its own header).
inline constexpr std::int32_t kUiGameMenu = 6;
inline constexpr std::int32_t kUiGameMenuBackground = 7;
inline constexpr std::int32_t kUiSharedItems = 8;
#endif


extern nk_context GNuklearWindow;
extern nk_draw_null_texture g_nuklearNullTexture;
extern nk_buffer g_nuklearCommands;
extern std::int32_t g_waitingForNextTick;

extern std::int32_t g_uiDrawViewSlot;

extern std::array<d3t::UIViewSlot<kUIViewImageCapacity>, d3t::kUIViewSlotCount> g_uiViewSlots;
extern std::array<std::uint32_t, d3t::kUIViewSlotCount * kUIViewImageCapacity> g_uiViewTextures;
extern std::int32_t g_currentUIViewSlot;
extern cJSON *g_parsedUIDocument;
extern std::int32_t g_uiReadStateInitialized;
std::int32_t TryFinalizeUIViewTextures(std::int32_t slotIndex);
#if SHENMUE_IS_S2

struct UITextureLoadData {
    std::uint32_t texture;
    std::uint32_t linearColour;
};
union UILoadPayload {
    UITextureLoadData image;
    void *data;
};
static_assert(sizeof(UILoadPayload) == 8);
struct alignas(16) UIAsyncLoadRequest {
    std::uint16_t generation;
    std::byte reserved02[14];
    UILoadPayload payload;
    std::byte reserved18[56];
};
static_assert(sizeof(UIAsyncLoadRequest) == 0x50 && offsetof(UIAsyncLoadRequest, payload) == 0x10);
static_assert(offsetof(UIAsyncLoadRequest, payload) + offsetof(UITextureLoadData, linearColour) == 0x14);
extern std::array<UIAsyncLoadRequest, 8192> g_uiLoadRequests;
extern std::atomic<std::uint16_t> g_uiLoadRequestIndex;
std::uint32_t AllocateUILoadRequest();
UILoadPayload *GetUILoadRequest(std::uint32_t handle);
std::uint32_t InitTexture2d(const char *path, std::uint32_t linearColour, std::uint32_t renderHandle);
std::uint32_t GetShaderDataByIndexInternal(std::uint32_t requestHandle);
std::uint32_t GetShaderDataByIndex(std::uint32_t requestHandle);
std::uint32_t SetupUsageForTexture(const char *path, std::uint32_t renderHandle);
void InitTexture2dLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);
void TextureLoadReleaseNullsub(void *);

struct UIViewImageLoadDiagnostic {
    std::int32_t viewSlot;
    std::int32_t count;
    std::array<std::int32_t, 80> imageIndices;
    std::array<std::uint32_t, 80> requestHandles;
    std::array<std::int32_t, 80> ready;
    std::array<std::int32_t, 80> missing;
    std::array<std::uint32_t, 80> failureHandles;
};
static_assert(sizeof(UIViewImageLoadDiagnostic) == 0x648 && offsetof(UIViewImageLoadDiagnostic, requestHandles) == 0x148);
static_assert(offsetof(UIViewImageLoadDiagnostic, ready) == 0x288 && offsetof(UIViewImageLoadDiagnostic, missing) == 0x3c8);
static_assert(offsetof(UIViewImageLoadDiagnostic, failureHandles) == 0x508);

extern std::array<UIViewImageLoadDiagnostic, 83> g_uiImageLoadDiagnostics;
extern std::int32_t g_uiImageLoadDiagnosticIndex;
void UI_RecordImageRequest(std::int32_t image, std::uint32_t requestHandle);
void UI_RecordImageFailure(std::int32_t image, std::uint32_t requestHandle);
void UI_RecordImageReady(std::int32_t image);
void UI_RecordImageViewSlot(std::int32_t slotIndex);
#endif

extern std::array<d3t::UISpriteSheet, d3t::kUISpriteSheetCount> g_uiSpriteSheets;

extern std::array<d3t::UIWindow, d3t::kUIWindowCount> g_uiWindows;
extern std::array<d3t::UILayoutItem, d3t::kUILayoutItemCount> g_uiLayoutItems;

// A window or button style item (1024 records): a colour (R, G, B, A, each read as an int) or a view image.
struct UIStyleItem {
    std::int32_t inUse;
    std::int32_t isColour;
    std::int32_t image;
    std::array<std::uint32_t, 4> colour;
};
static_assert(sizeof(UIStyleItem) == 28);
extern std::array<UIStyleItem, 1024> g_uiStyleItems;

// A label's Data binding (50 records): the view variable it names (-1 for none) and its Method (Read 0, Set 1).
struct UIDataBinding {
    std::int32_t inUse;
    std::int32_t variable;
    std::int32_t method;
};
static_assert(sizeof(UIDataBinding) == 12);
extern std::array<UIDataBinding, 50> g_uiDataBindings;

// The Button widget (kind 17, 128 records): its Id, Colour, Rounding, BorderSize, BorderColour, Alpha and owning window; +0x58..+0x7F and
// +0x84 are not traced yet.
struct UIButtonWidget {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::uint32_t colour;
    float rounding;
    float borderSize;
    std::uint32_t borderColour;
    float alpha;
    std::array<std::byte, 0x28> field58;
    std::int32_t window;
    std::int32_t field84;
};
static_assert(sizeof(UIButtonWidget) == 0x88);
extern std::array<UIButtonWidget, 128> g_uiButtonWidgets;

// The ColourImage widget (kind 19, 256 records): its Id, view image, Colour, Alpha, Rotate (radians) and window.
struct UIColourImageWidget {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t image;
    std::uint32_t colour;
    float alpha;
    float angle;
    std::int32_t window;
};
static_assert(sizeof(UIColourImageWidget) == 0x58);
#if SHENMUE_IS_S2
static_assert(offsetof(UIColourImageWidget, alpha) == 0x4c);
#endif
extern std::array<UIColourImageWidget, 256> g_uiColourImageWidgets;

// The Scissor widget (kind 22, 10 records): its rectangle as fractions of 1920x1080 and its Align.
struct UIScissorWidget {
    std::int32_t inUse;
    float x;
    float y;
    float width;
    float height;
    std::int32_t align;
};
static_assert(sizeof(UIScissorWidget) == 0x18);
extern std::array<UIScissorWidget, 10> g_uiScissorWidgets;

// The IconLabel widget (kind 16, 64 records): its Id, then the label record it lays out and draws (that record's own Id is the byte
// InitializeUIReadState clears), and its ControlScheme.
struct UIIconLabelWidget {
    std::array<char, 64> id;
    d3t::UILabelWidget label;
    std::int32_t controlScheme;
    std::int32_t fieldE4;
};
static_assert(sizeof(UIIconLabelWidget) == 0xe8);
static_assert(offsetof(UIIconLabelWidget, label) == 0x40 && offsetof(UIIconLabelWidget, controlScheme) == 0xe0);
extern std::array<UIIconLabelWidget, 64> g_uiIconLabelWidgets;

// The Sprite (kind 20) and ButtonIcon (kind 21) widget (256 records): its Id and Image name, a ButtonIcon's Action, AllowDoubleWidth,
// ControlMethod, ControlState, FontColour and Alignment, a Sprite's sheet and Index, both kinds' IconFontSize, Colour, Rotate (radians) and
// window. +0x88 is set to 1 on allocation. With inventoryIcon set (the cash overlay's icon), a Sprite's Index is an item whose SharedItems
// inventory icon it draws.
struct UIButtonIconWidget {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::array<char, 64> image;
    std::int32_t action;
    std::int32_t field88;
    std::int32_t iconFontSize;
    std::int32_t allowDoubleWidth;
    std::int32_t controlMethod;
    std::int32_t controlState;
    std::uint32_t fontColour;
    std::uint32_t alignment;
    std::int32_t sheet;
    std::int32_t sprite;
    std::int32_t fieldAC;
    std::uint32_t colour;
    float angle;
    std::int32_t inventoryIcon;
    std::int32_t window;
};
static_assert(sizeof(UIButtonIconWidget) == 0xc0);
static_assert(offsetof(UIButtonIconWidget, sheet) == 0xa4);
extern std::array<UIButtonIconWidget, 256> g_uiButtonIconWidgets;

// A TextBlock (view kind 33, 3 records): its Id, sprite sheet, 64 line texts and 64 three-word cells (reset to -1, -1, 0), LineCount,
// ColumnCount (also kept at +0x558), Size and its Panel's window.
struct UITextBlockCell {
    std::int32_t field0;
    std::int32_t field4;
    std::int32_t field8;
};
struct UITextBlock {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t sheet;
    std::array<const char *, 64> lines;
    std::int32_t field248;
    std::array<UITextBlockCell, 64> cells;
    std::int32_t field54C;
    std::int32_t lineCount;
    std::int32_t columnCount;
    std::int32_t field558;
    std::int32_t size;
    std::int32_t window;
    std::int32_t field564;
};
static_assert(sizeof(UITextBlock) == 0x568);
static_assert(offsetof(UITextBlock, cells) == 0x24c);
static_assert(offsetof(UITextBlock, lineCount) == 0x550);
extern std::array<UITextBlock, 3> g_uiTextBlocks;



struct UIButtonPanelEntry {
    std::int32_t field0;
    std::int32_t field4;
    std::array<char, 64> field8;
};
struct UIButtonPanel {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::array<char, 64> parentId;
    float buttonWidth;
    float buttonHeight;
    std::array<UIButtonPanelEntry, 5> entries;
    std::int32_t fontSize;
    std::int32_t iconFontSize;
};
static_assert(sizeof(UIButtonPanel) == 0x1fc);
static_assert(offsetof(UIButtonPanel, fontSize) == 0x1f4);
extern std::array<UIButtonPanel, 32> g_uiButtonPanels;

// A ButtonWheel item: its id (field0[0]) and skip flag (field0[1]: a nonzero item neither draws nor turns), its Labels and Descriptions
// tokens (the Main Menu's Information Bar shows the selected item's description), its image (field48[0]), two text pointers and two
// animation slots (its turn and its scale; reset to "" and -1), its angle (radians), its place on the wheel (index, and index84 for its Scale
// entry), Size, and the placement UI_PlaceButtonWheelItem gives it (the sine term, the cosine term and its scale). +0x80 and +0x88..+0x97
// other than the zeroed +0x90 are not traced yet.
struct UIButtonWheelItem {
    std::array<std::int32_t, 2> field0;
    d3t::UILabelToken label;
    d3t::UILabelToken description;
    std::array<std::int32_t, 2> field48;
    const char *field50;
    const char *field58;
    std::array<std::int32_t, 2> animSlots;
    float angle;
    std::int32_t index;
    std::int32_t size;
    float placeY;
    float placeX;
    float placeScale;
    std::int32_t field80;
    std::int32_t index84;
    std::int32_t field88;
    std::int32_t field8C;
    std::int32_t field90;
    std::int32_t field94;
};
static_assert(sizeof(UIButtonWheelItem) == 0x98);
static_assert(offsetof(UIButtonWheelItem, animSlots) == 0x60);
static_assert(offsetof(UIButtonWheelItem, placeScale) == 0x7c);

// A ButtonWheel (view kind 23, 3 records): its Id, eight items, Count, the placement origin (+0x508 Y, +0x50C X) and spread (+0x518 Y,
// +0x51C X) the reader leaves zero, Width and Height, the mouse callbacks its items run (the Main Menu wheel sets clicked), the Selected and
// Unselected images and colours (Selected also its Offset), Override, its Panel's window, Data binding, Shadow (image, Min, Max, Colour),
// and each item's Angles entry and Scale X and Y.
struct UIButtonWheel {
    std::array<char, 64> id;
    std::array<UIButtonWheelItem, 8> items;
    std::int32_t inUse;
    std::int32_t count;
    float originY;
    float originX;
    float width;
    float height;
    float spreadY;
    float spreadX;
    d3t::UIMouseCallbacks mouseCallbacks;
    std::array<std::int32_t, 3> field544;
    std::int32_t selectedImage;
    std::int32_t unselectedImage;
    std::uint32_t shadowColour;
    std::uint32_t selectedColour;
    std::uint32_t unselectedColour;
    float selectedOffset;
    std::int32_t override;
    std::int32_t window;
    std::int32_t dataBinding;
    std::int32_t selection;
    std::int32_t hasShadow;
    std::int32_t shadowImage;
    float shadowMin;
    float shadowMax;
    std::array<float, 8> angles;
    std::array<std::array<float, 2>, 8> scales;
};
static_assert(sizeof(UIButtonWheel) == 0x5e8);
static_assert(offsetof(UIButtonWheel, inUse) == 0x500);
static_assert(offsetof(UIButtonWheel, mouseCallbacks) == 0x520);
static_assert(offsetof(UIButtonWheel, selectedImage) == 0x550);
static_assert(offsetof(UIButtonWheel, angles) == 0x588);
extern std::array<UIButtonWheel, 3> g_uiButtonWheels;

// A queued mouse callback (UI_WindowMouseCallbacks): the element whose draw ran the hit area, the data the area's owner gave, the area's
// screen rectangle and the callback.
struct UIMouseCallbackEntry {
    std::int32_t element;
    void *data;
    std::array<float, 4> rect;
    d3t::UIMouseCallback callback;
};
static_assert(sizeof(UIMouseCallbackEntry) == 0x28);

// One of the four mouse callback queues (entered, hovered, left, clicked): UI_WindowMouseCallbacks fills up to 15 entries a frame (the
// sixteenth is never used), UI_UpdateElementEnableStatus empties them and UI_DispatchElementInput runs them.
constexpr std::int32_t kUIMouseCallbackQueueLimit = 15;
struct UIMouseCallbackQueue {
    std::int32_t count;
    std::array<UIMouseCallbackEntry, 16> entries;
};
static_assert(sizeof(UIMouseCallbackQueue) == 0x288);

// An AreaMap's map entry: set when read, its SpriteSheet name and sheet, Count, up to 16 Data pairs and Width and Height (fractions of
// 1920x1080). +0x04 is not traced yet.
struct UIAreaMapEntry {
    std::int32_t inUse;
    std::int32_t field4;
    const char *sheetName;
    std::int32_t sheet;
    std::int32_t count;
    std::array<std::array<float, 2>, 16> data;
    float width;
    float height;
};
static_assert(sizeof(UIAreaMapEntry) == 0xa0);




struct UIAreaMap {
    std::int32_t inUse;
    std::int32_t window;
    std::int32_t mapCount;
    std::int32_t fieldC;
    float field10;
    std::int32_t field14;
    std::array<UIAreaMapEntry, 3> maps;
    std::array<std::int32_t, 16> field1F8;
    std::array<float, 2> field238;
    float field240;
    float controlScale;
    std::array<float, 2> controlMin;
    std::array<float, 2> controlMax;
};
static_assert(sizeof(UIAreaMap) == 0x258);
static_assert(offsetof(UIAreaMap, maps) == 0x18);
static_assert(offsetof(UIAreaMap, field240) == 0x240);
extern std::array<UIAreaMap, 1> g_uiAreaMaps;

// A Selector (view kind 31, 10 records): its Id, Panel's window, Image and Colour, its Options (a flag and token for each of Up, Right, Down
// and Left), Pos (Up 0, Right 1, Down 2, Left 3, else -1), its Label (Font, token, Image, Width, Height), the Selected and Unselected images
// and colours, and Width and Height. +0x44, +0xF4 and +0x13C are not traced yet. InitializeUIReadState does not reset these records.
struct UISelector {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t field44;
    std::int32_t window;
    std::int32_t image;
    std::uint32_t colour;
    std::int32_t optionCount;
    std::array<std::int32_t, 4> hasOption;
    std::array<d3t::UILabelToken, 4> options;
    std::int32_t pos;
    std::int32_t hasLabel;
    std::int32_t labelFont;
    std::int32_t fieldF4;
    d3t::UILabelToken label;
    std::int32_t labelImage;
    float labelWidth;
    float labelHeight;
    std::int32_t selectedImage;
    std::int32_t unselectedImage;
    std::uint32_t selectedColour;
    std::uint32_t unselectedColour;
    float width;
    float height;
    std::int32_t field13C;
};
static_assert(sizeof(UISelector) == 0x140);
static_assert(offsetof(UISelector, options) == 0x68);
static_assert(offsetof(UISelector, label) == 0xf8);
static_assert(offsetof(UISelector, selectedImage) == 0x124);
extern std::array<UISelector, 10> g_uiSelectors;

// A list's scroll bar (ScrollList and Collection): its Panel's window, whether its Direction is Horizontal, and its Outline Image and Bar
// Image, Top and Bottom view images.
struct UIScrollBar {
    std::int32_t window;
    std::int32_t horizontal;
    std::int32_t outlineImage;
    std::int32_t barImage;
    std::int32_t topImage;
    std::int32_t bottomImage;
};
static_assert(sizeof(UIScrollBar) == 24);

// A Collection page's draw block: a Snapshot Draw's Background (Image, X, Y, Width, Height) and Main (X, Y, Width, Height) as fractions of
// 1920x1080. The other words are not traced yet.
struct UICollectionDraw {
    std::array<std::int32_t, 8> field0;
    float backgroundWidth;
    float backgroundHeight;
    float mainWidth;
    float mainHeight;
    std::array<std::int32_t, 12> field30;
    float backgroundX;
    float backgroundY;
    float mainX;
    float mainY;
    std::array<std::int32_t, 20> field70;
    std::int32_t backgroundImage;
    std::array<std::int32_t, 23> fieldC4;
};
static_assert(sizeof(UICollectionDraw) == 0x120);

// A Collection page: ParentID, set when read, Columns, Rows, ElementWidth and ElementHeight, Scale X and Y, the TexturePage record or
// sprite sheet it draws from (-1 for none), Override, its Draw type (0 for Snapshot) and draw block. +0x5C..+0x6B are not traced yet.
struct UICollectionPage {
    std::array<char, 64> parentId;
    std::int32_t inUse;
    std::int32_t columns;
    std::int32_t rows;
    float elementWidth;
    float elementHeight;
    float scaleX;
    float scaleY;
    std::int32_t field5C;
    std::array<std::int32_t, 2> field60;
    std::int32_t field68;
    std::int32_t texture;
    std::int32_t override;
    std::int32_t drawType;
    UICollectionDraw draw;
};
static_assert(sizeof(UICollectionPage) == 0x198);

// A Collection element; only its colour (white after a reset) is traced.
struct UICollectionElement {
    std::array<std::int32_t, 6> field0;
    std::uint32_t colour;
};
static_assert(sizeof(UICollectionElement) == 28);

// A Collection (view kind 25, 8 records): its Id, Data binding, Selected image and placement, page count, eight pages, 32 elements and its
// Bar. +0x44..+0x5F, +0x64..+0x77 (zeroed by the reset), +0x7C (cleared as a style item by the reset), +0x94 and +0x10F4 are not traced yet.
struct UICollection {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::array<std::int32_t, 7> field44;
    std::int32_t dataBinding;
    std::array<std::int32_t, 5> field64;
    std::int32_t selectedImage;
    std::int32_t field7C;
    float selectedX;
    float selectedY;
    float selectedWidth;
    float selectedHeight;
    std::int32_t pageCount;
    std::int32_t field94;
    std::array<UICollectionPage, 8> pages;
    std::array<UICollectionElement, 32> elements;
    std::int32_t hasBar;
    UIScrollBar bar;
    std::int32_t field10F4;
};
static_assert(sizeof(UICollection) == 0x10f8);
static_assert(offsetof(UICollection, dataBinding) == 0x60);
static_assert(offsetof(UICollection, pages) == 0x98);
static_assert(offsetof(UICollection, elements) == 0xd58);
static_assert(offsetof(UICollection, hasBar) == 0x10d8);
extern std::array<UICollection, 8> g_uiCollections;



inline constexpr std::array<const char *, 5> kUIOptionTypeNames{"Dropdown", "Slider", "Bar", "Button", "Key"};
inline constexpr std::array<const char *, 15> kUIElementTypeNames{
    "Box", "Slider", "SliderButton", "Bar", "Separator", "Confirm", "Cancel", "Event", "System", "Run", "Look", "Filter", "Snapshot", "Swap", "Pause"};

// An OptionsList option: set when read, +0x04 (1 when read), Tab, X, Y, Width and Height (fractions of 1920x1080), its Label token, a
// second text pointer ("" after a reset), its Type (an index into kUIOptionTypeNames, -1 for none), Value (-1 without one) and Action (an
// index into kInputActionNames). +0x1C, +0x3C, +0x50..+0x5F and +0x60 (zeroed by the reset) are not traced yet.
struct UIOptionsListOption {
    std::int32_t inUse;
    float field4;
    std::int32_t tab;
    float x;
    float y;
    float width;
    float height;
    std::int32_t field1C;
    d3t::UILabelToken label;
    const char *field40;
    std::int32_t type;
    std::int32_t value;
    std::array<std::int32_t, 4> field50;
    std::int32_t field60;
    std::int32_t action;
};
static_assert(sizeof(UIOptionsListOption) == 0x68);
static_assert(offsetof(UIOptionsListOption, label) == 0x20);
static_assert(offsetof(UIOptionsListOption, type) == 0x48);

// An OptionsList (view kind 26, 6 records): its Id, Panel's window, option count, Layout (Columns, Label and Box widths, Rows, Height),
// ScrollDownOnSelection and ScrollUpOnSelection (-1 without them), 32 options, per element type (kUIElementTypeNames) an image, size and
// position, Font, IconFontSize, the Selected, Unselected and Offset colours and SelectedImage. +0x48, +0x4C, +0x54, +0xEB4, +0xEC8 (zero
// when read) and +0xECC are not traced yet.
struct UIOptionsList {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t window;
    std::array<std::int32_t, 2> field48;
    std::int32_t optionCount;
    std::int32_t field54;
    std::int32_t columns;
    float labelWidth;
    float boxWidth;
    std::int32_t rows;
    float rowHeight;
    std::int32_t scrollDownOnSelection;
    std::int32_t scrollUpOnSelection;
    std::array<std::int32_t, 3> field74;
    std::array<UIOptionsListOption, 32> options;
    std::array<std::int32_t, 15> elementImages;
    std::array<std::array<float, 2>, 15> elementSizes;
    std::array<std::array<float, 2>, 15> elementPositions;
    std::int32_t font;
    std::int32_t iconFontSize;
    std::int32_t fieldEB4;
    std::uint32_t selectedColour;
    std::uint32_t unselectedColour;
    std::uint32_t offsetColour;
    std::int32_t selectedImage;
    std::int32_t fieldEC8;
    std::int32_t fieldECC;
};
static_assert(sizeof(UIOptionsList) == 0xed0);
static_assert(offsetof(UIOptionsList, options) == 0x80);
#if SHENMUE_IS_S2
static_assert(offsetof(UIOptionsList, field74) + sizeof(std::int32_t) == 0x78);
#endif
static_assert(offsetof(UIOptionsList, elementImages) == 0xd80);
static_assert(offsetof(UIOptionsList, elementPositions) == 0xe34);
static_assert(offsetof(UIOptionsList, font) == 0xeac);
extern std::array<UIOptionsList, 6> g_uiOptionsLists;





struct UIMenuBar {
    std::array<std::int32_t, 8> windows;
    std::array<std::int32_t, 2> extraWindows;
    std::int32_t field28;
    std::int32_t style;
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t optionCount;
    std::int32_t image;
    std::int32_t field7C;
    std::array<d3t::UILabelToken, 8> labels;
    std::int32_t font;
    std::int32_t field184;
    std::int32_t window;
    std::int32_t field18C;
    std::int32_t separatorImage;
    float separatorScaleX;
    float separatorScaleY;
    float separatorAlpha;
    std::int32_t hasButtonIcon;
    std::int32_t field1A4;
    std::array<char, 64> buttonIconImage;
    std::int32_t left;
    std::int32_t right;
    std::int32_t iconFontSize;
    float width;
    float height;
    float offsetX;
    float offsetY;
    float optionsWidth;
    float optionsHeight;
    std::array<std::array<float, 2>, 8> optionPositions;
    std::int32_t selectedImage;
    std::int32_t selectedOverride;
    std::array<float, 8> selectedX;
    std::array<float, 8> selectedY;
    std::array<float, 8> selectedWidth;
    std::array<float, 8> selectedHeight;
    std::uint32_t selectedColour;
    std::int32_t field2D8;
    std::int32_t field2DC;
};
static_assert(sizeof(UIMenuBar) == 0x2e0);
static_assert(offsetof(UIMenuBar, inUse) == 0x30);
static_assert(offsetof(UIMenuBar, labels) == 0x80);
static_assert(offsetof(UIMenuBar, buttonIconImage) == 0x1a8);
static_assert(offsetof(UIMenuBar, optionPositions) == 0x20c);
extern std::array<UIMenuBar, 10> g_uiMenuBars;

// A state style (128 records): the style items its Normal, Hover and Active members read.
struct UIStateStyle {
    std::int32_t inUse;
    std::int32_t normal;
    std::int32_t hover;
    std::int32_t active;
};
static_assert(sizeof(UIStateStyle) == 16);
extern std::array<UIStateStyle, 128> g_uiStateStyles;

// A ScrollList Draw entry. Its Method (1 Move, 2 Scroll, 3 Save, 4 Map, 5 Pawnshop, 6 Standard) decides what the shared words hold: +0x04 a
// sprite sheet or view image, +0x08..+0x1F positions and sizes, +0x20..+0x5F sizes (Save: its five Time positions), +0x60..+0x9F positions
// (Save: the Time entries' second positions), +0xA0..+0xBF Size indices, +0xC0..+0xDF view images, +0xE0..+0xFF colours, then the Selected and
// Unselected state styles, +0x108 (1 after a reset), the Font Selected and Unselected colours, an Offset colour and two Offset/Padding words.
struct UIScrollListDraw {
    std::int32_t method;
    std::int32_t image;
    std::array<std::array<float, 2>, 3> place08;
    std::array<std::array<float, 2>, 8> size20;
    std::array<std::array<float, 2>, 8> place60;
    std::array<std::int32_t, 8> sizes;
    std::array<std::int32_t, 8> images;
    std::array<std::uint32_t, 8> colours;
    std::int32_t selectedStyle;
    std::int32_t unselectedStyle;
    std::int32_t field108;
    std::uint32_t fontSelectedColour;
    std::uint32_t fontUnselectedColour;
    std::uint32_t offsetColour;
    float field118;
    float field11C;
};
static_assert(sizeof(UIScrollListDraw) == 0x120);
static_assert(offsetof(UIScrollListDraw, sizes) == 0xa0);
static_assert(offsetof(UIScrollListDraw, selectedStyle) == 0x100);

// A ScrollList (view kind 27, 8 records): its Id, Panel's window, List ColumnWidth and RowHeight, Rows (kept twice), Draw count, Font,
// IconFontSize, twelve text pointers and twelve words (reset to "" and -1), two Draw entries, Bar, and Selector (Width, Height, X, Y,
// Image). +0x44, +0x64, +0x6C, +0x74..+0x7B, +0x358..+0x387, +0x3A4..+0x3B7, +0x3CC and +0x3D4 are not traced yet.
struct UIScrollList {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t field44;
    std::int32_t window;
    d3t::UIWindowLength columnWidth;
    d3t::UIWindowLength rowHeight;
    std::int32_t field64;
    std::int32_t rows;
    std::int32_t field6C;
    std::int32_t drawCount;
    std::array<std::int32_t, 2> field74;
    std::int32_t field7C;
    std::int32_t font;
    std::int32_t iconFontSize;
    std::array<const char *, 12> texts;
    std::array<std::int32_t, 12> words;
    std::array<UIScrollListDraw, 2> draws;
    std::array<std::int32_t, 12> field358;
    std::int32_t hasBar;
    UIScrollBar bar;
    std::int32_t field3A4;
    std::int32_t field3A8;
    std::int32_t field3AC;
    std::array<std::int32_t, 2> field3B0;
    std::int32_t rowCount;
    float selectorWidth;
    float selectorHeight;
    float selectorX;
    float selectorY;
    std::int32_t field3CC;
    std::int32_t selectorImage;
    std::int32_t field3D4;
};
static_assert(sizeof(UIScrollList) == 0x3d8);
static_assert(offsetof(UIScrollList, texts) == 0x88);
static_assert(offsetof(UIScrollList, draws) == 0x118);
static_assert(offsetof(UIScrollList, hasBar) == 0x388);
static_assert(offsetof(UIScrollList, rowCount) == 0x3b8);
#if SHENMUE_IS_S2
static_assert(offsetof(UIScrollList, field3A4) == 0x3a4 && offsetof(UIScrollList, field3CC) == 0x3cc);
#endif
extern std::array<UIScrollList, 8> g_uiScrollLists;



struct UIImageFrame {
    std::int32_t inUse;
    std::int32_t hidden;
    std::int32_t window;
    std::int32_t background;
    std::array<std::uint16_t, 16> image;
};
static_assert(sizeof(UIImageFrame) == 0x30);
extern std::array<UIImageFrame, 3> g_uiImageFrames;



struct UITexturePage {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t count;
    std::array<std::int32_t, 8> images;
    std::array<std::int32_t, 8> min;
    std::array<std::int32_t, 8> max;
};
static_assert(sizeof(UITexturePage) == 0xa8);
extern std::array<UITexturePage, 5> g_uiTexturePages;


extern std::array<d3t::UIGaugeState, 5> g_fightGaugeWidgetStates;
extern std::array<d3t::UILabelWidget, d3t::kUILabelWidgetCount> g_uiLabelWidgets;
extern std::array<d3t::UIImageWidget, d3t::kUIImageWidgetCount> g_uiImageWidgets;


struct UILayoutRect {
    float x;
    float y;
    float width;
    float height;
};

// The font scale UI_FitLabelScale settles on for a label, and whether it splits the label over two lines.
struct UILabelFit {
    float scale;
    std::int32_t split;
};
static_assert(sizeof(UILabelFit) == 8);

// The record UI_DrawButtonIcon passes UI_DrawButtonIconLabel: the label's rectangle, its font width and height, and the widget's action,
// AllowDoubleWidth, IconFontSize, font colour, Alignment, control method and control state.
struct UIButtonIconLabel {
    UILayoutRect rect;
    float fontWidth;
    float fontHeight;
    std::int32_t action;
    std::int32_t doubleWidth;
    std::int32_t iconFontSize;
    std::uint32_t colour;
    std::uint32_t alignment;
    std::int32_t controlMethod;
    std::int32_t controlState;
};



struct UIAnimSlot {
    std::int32_t allocated;
    std::int32_t interpolation;
    std::int32_t status;
    std::array<std::byte, 12> reserved0c;
    std::array<float, 2> start;
    std::array<float, 2> current;
    std::array<float, 2> end;
    float duration;
    float delay;
    float elapsed;
    std::int32_t loopMode;
    std::int32_t reserved40;
    std::int32_t reserved44;
};
static_assert(sizeof(UIAnimSlot) == 0x48);
static_assert(offsetof(UIAnimSlot, status) == 0x08);
static_assert(offsetof(UIAnimSlot, start) == 0x18);
static_assert(offsetof(UIAnimSlot, current) == 0x20);
static_assert(offsetof(UIAnimSlot, end) == 0x28);
static_assert(offsetof(UIAnimSlot, duration) == 0x30);
static_assert(offsetof(UIAnimSlot, elapsed) == 0x38);
static_assert(offsetof(UIAnimSlot, reserved40) == 0x40);



struct UIWidgetBinding {
    std::int32_t allocated;
    std::int32_t animating;
    std::int32_t objectHandle;
    std::int32_t animSlot;
    std::array<float, 2> hidden;
    std::array<float, 2> shown;
    std::int32_t property;
    std::int32_t objectKind;
};
static_assert(sizeof(UIWidgetBinding) == 40);
static_assert(offsetof(UIWidgetBinding, hidden) == 0x10);
static_assert(offsetof(UIWidgetBinding, property) == 0x20);


struct UIFontFace {
    std::array<nk_font_atlas, 6> atlases;
    std::array<nk_font *, 6> fonts;
    std::array<struct nk_font_config, 6> configs;
    std::array<std::uint32_t, 6> textures;
    std::array<float, 6> sizes;
};
static_assert(sizeof(UIFontFace) == 3264 && offsetof(UIFontFace, fonts) == 2736 && offsetof(UIFontFace, configs) == 2784);
static_assert(offsetof(UIFontFace, textures) == 3216 && offsetof(UIFontFace, sizes) == 3240);
inline constexpr std::uint32_t kUiNavigateSound = 0x200abU;
inline constexpr std::uint32_t kUiRejectSound = 0x800abU;


inline constexpr std::array<const char *, 3> kPlatformButtonIconImageNames{"ButtonIcons", "ActiveButtonIcons", "GreyButtonIcons"};

extern std::array<std::array<UIFontFace, 4>, 7> g_uiFonts;
extern nk_draw_null_texture g_uiFontNullTexture;
extern std::int32_t g_uiFontSize;
extern std::int32_t g_uiFontFace;
extern float g_uiFontScale;
void SetupNuklear();
void FreeUIFontDefinition(std::int32_t language);
void FontLoader();
extern std::array<std::int32_t, 7> g_fontLanguageAvailable;
extern std::int32_t g_forceFontLanguageAvailable;
#if SHENMUE_IS_S2
void LoadUIFontLanguage(std::int32_t language);
extern std::int32_t g_pendingUIFontLanguageValid;
extern std::int32_t g_pendingUIFontLanguage;
#endif
inline constexpr std::array<const char *, 7> kUIFontDefinitionPaths{"/ui/font/english.fontdef", "/ui/font/Japanese.fontdef", "/ui/font/French.fontdef",
    "/ui/font/German.fontdef", "/ui/font/Korean.fontdef", "/ui/font/ChineseT.fontdef", "/ui/font/ChineseS.fontdef"};
#if SHENMUE_IS_S1
extern std::array<std::vector<std::byte>, 7> g_fontDefinitions;
void InstallLegacyUIFontRecord(std::uint32_t language, const std::uint32_t *header, const std::byte *record, const char *path, std::uint32_t renderHandle);
void RetireLegacyUIFont(std::int32_t language);
namespace d3t {
void LoadFont(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);
}
#else
extern std::int32_t g_nuklearVertexBuffer;
extern std::int32_t g_nuklearIndexBuffer;
extern std::uint8_t g_NuklearInitialized;
extern std::uint32_t g_uiScreenWidth;
extern std::uint32_t g_Nuklear_num_verts;
extern std::uint32_t g_Nuklear_num_indices;
extern d3t::D3TDeviceContext *g_nuklearRenderDeviceContext;
extern Vector4 g_currentUIViewportRect;
extern Vector2 g_currentUIViewportDepth;
extern bool g_bOrthographicRender;
extern Matrix4x4 g_notebookPageMatrix;
extern std::array<float, 2> g_nuklearArmedImageSize;
void CreateOrthographicViewMatrix(std::int32_t width, std::int32_t height, Matrix4x4 *matrix);
std::uint32_t InitNuklearScreenTexture();
void LoadUIFont(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);
#endif
extern std::int32_t g_bIs43AspectRatio;
extern std::int32_t g_currActiveUIIndex;
extern std::uint64_t g_uiLegacyDrawFrame;
extern std::array<d3t::UIViewImage *, 3> g_platformButtonIconImages;
#if SHENMUE_IS_S1
extern std::int64_t g_platformIconsSheet;
#else
extern std::int32_t g_platformIconsSheet;
#endif
extern struct nk_image g_uiDefaultImage;
extern std::array<UIMouseCallbackQueue, 4> g_uiMouseCallbackQueues;
extern std::int32_t g_mainMenuPulseSlot;
#if SHENMUE_IS_S1
inline constexpr std::size_t kInventoryIconSheetCount = 24;
#else
inline constexpr std::size_t kInventoryIconSheetCount = 32;
#endif
extern std::array<std::int32_t, kInventoryIconSheetCount> g_inventoryIconSheetImageIndices;
extern std::array<UIAnimSlot, 256> g_uiAnimSlots;
#if !SHENMUE_IS_S2
extern std::array<float *, 256> g_uiWidgetBindingProperties;
#endif
extern std::array<UIWidgetBinding, 256> g_uiWidgetBindings;
extern std::int32_t g_uiNavigateSoundFrame;

// The runtime's functions, in source order.
std::int32_t UIAnimAllocSlot();
void UIAnimConfigureSelectedChoice(std::int32_t slotIndex);
void UIAnim_Start(std::int32_t slotIndex, float from, float to, float duration, float delay);
void UIWidget_ApplyAnimations();
#if SHENMUE_IS_S2
void UIAnim_StartPair(std::int32_t slotIndex, std::array<float, 2> from, std::array<float, 2> to, float duration, float delay);
std::int32_t UIWidget_Bind(std::int32_t objectHandle, std::int32_t objectKind, std::int32_t property, std::array<float, 2> hidden, std::array<float, 2> shown);
#else
std::int32_t UIWidget_Bind(std::int32_t objectHandle, float *property, std::array<float, 2> hidden, std::array<float, 2> shown);
#endif
void UIWidget_Animate(std::int32_t index, std::int32_t direction, float duration, float delay);
void UIAnim_UpdateDeltaTime(float deltaTime);
void UIWidget_ClearBinding(std::int32_t index);
void UI_SelectFont();
std::int32_t d3t_GetAspectRatioMode();
void UI_ResetTextBlock(std::int32_t index);
void UI_ResetGaugeWidget(std::int32_t index);
void UI_ReleaseAnimSlot(std::int32_t index);
void UI_ResetButtonWheel(std::int32_t index);
void UI_ResetAreaMap(std::int32_t index);
void UI_ResetSelector(std::int32_t index);
void UI_ResetTexturePage(std::int32_t index);
void UI_ResetCollectionPage(std::int32_t collection, std::int32_t pageIndex);
void UI_ResetCollection(std::int32_t index);
void UI_ResetOptionsList(std::int32_t index);
void UI_ResetScrollListDraw(UIScrollListDraw &draw);
void UI_ResetScrollList(std::int32_t index);
void UI_ResetMenuBar(std::int32_t index);
void UI_ResetImageFrame(std::int32_t index);
void UI_FreeViewWidget(std::int32_t slotIndex, std::int32_t widget);
void ResetUIViewSlot(std::int32_t slotIndex);
void ResetUIWindow(std::int32_t index);
d3t::UIWindowLength UI_ReadWindowLength(const cJSON *length, cJSON *object, const char *imageKey, float reference);
std::int32_t UI_ReadWindow(cJSON *object);
void UI_ReadStyleColour(cJSON *item, std::array<std::uint32_t, 4> &rgba);
std::int32_t UI_ParseStyleItem(cJSON *item);
void UI_ReadFixedBackground(cJSON *item);
void UI_ReadBackground(cJSON *item);
void UI_ReadWindowStyle(cJSON *style);
void UI_ReadButtonStyle(cJSON *style);
void UI_ReadStyle(cJSON *style);
bool UI_ReadColour(cJSON *object, const char *name, std::uint32_t &colour);
std::int32_t UI_ReadFontName(const char *name);
std::int32_t UI_ReadDataBinding(cJSON *data);
void UI_ResetLabelWidget(std::int32_t index);
std::int32_t UI_ReadLabelObject(cJSON *label, std::int32_t window);
std::int32_t UI_LoadImageObject(cJSON *image, std::int32_t window);
std::uint32_t UI_ReadColour4(cJSON *colour);
std::int32_t UI_ReadButtonObject(cJSON *button, std::int32_t window);
std::int32_t UI_ReadColourImageObject(cJSON *image, std::int32_t window);
std::int32_t UI_ReadScissorObject(cJSON *scissor);
std::int32_t UI_ReadControlScheme(const char *name);
std::int32_t UI_AllocIconLabelWidget(const char *id, const char *text, const d3t::UILabelToken &token, std::uint32_t alignment, std::int32_t dataBinding,
    std::int32_t size, std::int32_t fontFamily, const std::array<std::int32_t, 4> &colour, std::int32_t hasStroke, std::uint32_t strokeColour,
    std::int32_t controlScheme, std::int32_t window, std::int32_t scaleVar, std::int32_t scaleVarAssign);
std::int32_t UI_ReadIconLabelObject(cJSON *iconLabel, std::int32_t window);
std::int32_t UI_ReadButtonIconObject(cJSON *object, std::int32_t window);
std::int32_t UI_ReadWidgetObject(cJSON *objects, std::int32_t &kind, std::int32_t window);
void UI_ReadPanel(cJSON *panel);
void UI_PlaceButtonWheelItem(
    float angle, float spreadY, float spreadX, float originY, float originX, float &placeY, float &placeX, float &scale, std::int32_t override);
void UI_SetLabelTokenKey(d3t::UILabelToken &token);
void UI_ReadButtonWheel(cJSON *buttonWheel);
void UI_ReadAreaMap(cJSON *areaMap);
void UI_ReadSelector(cJSON *selectorObject);
void UI_ReadTextBlock(cJSON *textBlock);
void UI_ReadButtonPanel(cJSON *buttonPanel);
void UI_ReadGaugeWidget(cJSON *gaugeObject);
std::int32_t UI_ReadTexturePage(cJSON *texturePage);
void UI_ReadImageFrame(cJSON *imageFrame);
void UI_ReadScrollBar(cJSON *barObject, UIScrollBar &bar);
void UI_ReadSnapshotDraw(cJSON *drawObject, UICollectionDraw &draw);
void UI_ReadCollectionDraw(std::int32_t collection, std::int32_t pageIndex, cJSON *drawObject);
void UI_ReadCollectionPage(cJSON *pageObject, std::int32_t collection, std::int32_t pageIndex);
void UI_ReadCollection(cJSON *collectionObject);
void UI_ReadOptionsList(cJSON *listObject);
std::int32_t UI_ReadStateStyle(cJSON *styleObject);
std::int32_t UI_FindSpriteSheet(const char *name);
float UI_ReadFraction(cJSON *object, const char *name, double reference);
std::int32_t UI_ReadViewImage(cJSON *object, const char *name);
std::int32_t UI_ReadSizeIndex(const cJSON *size);
void UI_ReadScrollListDraw(cJSON *drawObject, UIScrollListDraw &draw, std::int32_t window);
void UI_ReadScrollList(cJSON *listObject);
std::int32_t UI_TakeWindow();
void UI_ReadMenuBarWithoutPanel(cJSON *menuBar, UIMenuBar *bar);
void UI_ReadMenuBar(cJSON *menuBar);
void ReadUIView(cJSON *view);
void InitializeUIReadState();
void ReadImages(d3t::UIViewSlot<kUIViewImageCapacity> &slot);
void LoadUIFromMemoryIntoViewSlot(const char *memory, std::int32_t &slotIndex);
d3t::UIWindow *UI_GetWindow(std::int32_t index);
std::int32_t UI_FindViewWindow(std::uint32_t slotIndex, const char *id);
std::int32_t UI_FindViewLabel(std::uint32_t slotIndex, const char *id);
std::int32_t UI_FindViewColourImage(std::uint32_t slotIndex, const char *id);
std::int32_t UI_FindViewImageWidget(std::uint32_t slotIndex, const char *id);
#if SHENMUE_IS_S2
UIButtonIconWidget *UI_FindViewIconWidget(std::uint32_t slotIndex, const char *id, std::int32_t kind);
#endif
std::int32_t UI_FindViewButtonWheel(std::uint32_t slotIndex, const char *id);
d3t::UIWindow *UI_FindWindowById(const char *id);
float UI_SafeAreaScale();
std::int32_t UI_GetAspectWidth();
#if SHENMUE_IS_S2
float UI_GetFullReferenceWidth();
void UI_SetFontFamily(std::uint32_t family);
float UI_GetReferenceWidth();
float UI_GetReferenceHeight();
#endif
float WithinSafeArea(float length);
void AdjustForSafeArea(UILayoutRect &rect, float paddingX, float paddingY);
void UI_RootWindowArea(const d3t::UIWindow &window, float &width, float &height, float &aspectWidth);
void AdjustForDisplayAreaAndAspectRatio(const d3t::UIWindow &window, float &x, float &y);
void UI_ScaleSize(
    float &width, float &height, std::int32_t style, float availableWidth, float availableHeight, float srcWidth, float srcHeight, const d3t::UIWindow *parent);
void UI_WindowSize(const d3t::UIWindow &window, float &width, float &height, d3t::UIWindow *parent);
void UI_AlignInArea(std::int32_t alignX, std::int32_t alignY, float width, float height, float &x, float &y, float areaWidth, float areaHeight);
void UI_WindowPosition(const d3t::UIWindow &window, float width, float height, float &x, float &y, d3t::UIWindow *parent);
void UI_InvalidateWindowRect(d3t::UIWindow &window);
[[maybe_unused]] void UI_GetWindowRect(d3t::UIWindow *window, UILayoutRect &rect);
void UI_ResetNuklearStyle(nk_context &context);
nk_style_item UI_StyleItem(std::int32_t styleIndex);
UILayoutRect UI_SpacePushRect(const d3t::UILayoutItem &item, d3t::UIWindow *window);
void UI_SetTokenKeyFromString(d3t::UILabelToken &token, const char *text);
void UI_ResolveLabelText(d3t::UILabelWidget &label);
std::int32_t UI_FindLabelSplit(const char *text, std::int32_t length);
UILabelFit UI_FitLabelScale(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, float width, float height,
    std::int32_t allowSplit);
UILayoutRect UI_DrawLabelText(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, const UILabelFit &fit);
UILayoutRect UI_FitAndDrawLabelText(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, float width,
    float height, std::int32_t allowSplit);
#if SHENMUE_IS_S2
std::uint32_t UI_GetWindowColour(const d3t::UIWindow *window);
std::uint32_t UI_ModulateColour(std::uint32_t colour, std::uint32_t tint);
#endif
std::uint32_t ConstructColour(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha);
std::uint32_t UI_TintByWindow(const d3t::UIWindow &window, std::uint32_t colour);
void UI_DrawLabel(nk_context &context, std::int32_t labelIndex, std::int32_t widget);
std::int32_t UI_FindViewImage(std::uint32_t slot, const char *name);
std::int64_t PlatformIcons_IsSelected();
void RenderingRelatedInventory(struct nk_image &image, std::uint16_t item, std::int32_t mask);
std::int64_t GetIconSet();
std::int64_t SharedItems_IsPressed();
struct nk_image UI_GetSpriteImage(std::uint32_t sheet, std::int32_t frame, const d3t::UIViewImage *image);
void UI_DrawButtonIconLabel(nk_context &context, const UIButtonIconLabel &label);
float ScaleTo1080WithSafeArea(const struct nk_rect &rect, float length);
void UI_DrawButton(nk_context &context, std::int32_t index);
void UI_DrawImage(nk_context &context, std::int32_t index);
void UI_DrawScissor(nk_context &context, std::int32_t index);
void UI_DrawIconLabel(nk_context &context, std::int32_t index, std::int32_t widget);
void UI_WindowMouseCallbacks(d3t::UIMouseCallbacks &callbacks, std::int32_t element, const UILayoutRect &rect, void *data);
void UI_DrawButtonWheel(nk_context &context, std::int32_t index);
void UI_DrawSelector(nk_context &context, std::int32_t index);
void UI_DrawButtonPanel(nk_context &context, std::int32_t index);
struct nk_image UI_ReadyViewImage(std::int32_t image);
void UI_DrawCollectionCount(nk_context &context, const UILayoutRect &cell, std::int32_t count, const UICollectionPage &page);
void UI_DrawCollectionSprite(nk_context &context, const struct nk_image &image, nk_color colour, const UICollectionDraw &draw, const UILayoutRect &cell);
void UI_DrawScrollBar(nk_context &context, std::int32_t pageCount, std::int32_t page, const UIScrollBar &bar);
void UI_DrawOptionsList(nk_context &context, std::int32_t index);
void UI_DrawCollection(nk_context &context, std::int32_t index);
void UI_DrawMenuBar(nk_context &context, std::int32_t index);
std::int32_t UI_GetActionForId(std::int32_t id);
void UI_DrawTextBlock(nk_context &context, std::int32_t index);
void UI_DrawAreaMap(nk_context &context, std::int32_t index);
void UI_DrawImageFrame(nk_context &context, std::int32_t index);
void UI_DrawGauge(nk_context &context, std::int32_t index);
void UI_DrawColourImage(nk_context &context, std::int32_t index);
void UI_DrawSprite(nk_context &context, std::int32_t index);
void UI_DrawButtonIcon(nk_context &context, UIButtonIconWidget &widget, const UILayoutRect *rect);
void UI_DrawWindowBegin(nk_context &context, std::int32_t index);
void UI_InputHandler(nk_context &context, std::int32_t slot, std::int32_t first, std::int32_t end);
#if SHENMUE_IS_S2
nk_context *GetNativeUiContext();
std::int32_t ResolveButtonIconGlyph(std::int32_t action, std::int32_t controlMethod, std::int32_t inputType, std::int32_t doubleWidth);
d3t::UIViewSlot<kUIViewImageCapacity> *GetNuklearElementByIdx(std::uint32_t slot);
std::uint8_t GetIsNuklearActive();
void DrawUIEntity(nk_context &context, std::uint32_t slot, std::int32_t element);
void uiUpdateWithColour(nk_context &context, std::uint32_t slot);
#endif

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif

#include <cstdint>

#if SHENMUE_IS_S2
namespace shenmue::s2 {

// A 12-character string-table id.
struct SubtitleStringId {
    std::uint32_t word0;
    std::uint32_t word1;
    std::uint32_t word2;
};
static_assert(sizeof(SubtitleStringId) == 12);

const char *ReadStringFromStringTableById(const SubtitleStringId &id);

const char *GetStringForUILabel(const char *text);
} // namespace shenmue::s2
#endif
