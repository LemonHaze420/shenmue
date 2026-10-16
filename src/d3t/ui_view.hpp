#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

struct cJSON;

namespace shenmue::d3t {



constexpr std::size_t kUIViewSlotCount = 76;
constexpr std::size_t kUIViewVariableCount = 20;
constexpr std::size_t kUIViewWidgetCount = 512;
constexpr std::size_t kS1UIViewImageCapacity = 128;
constexpr std::size_t kS2UIViewImageCapacity = 80;

// One "Images" member: ReadImages stores its name, path and load request; TryFinalizeUIViewTextures fills the texture fields once it loads.
struct UIViewImage {
    std::int32_t ready;
    // The image's entry in the view texture table (slot * capacity + image).
    std::int32_t textureIndex;
    const char *name;
    const char *path;
    // The texture's width and height over a divisor the finalize pass takes (not traced yet).
    float widthScale;
    float heightScale;
    // The game's nk_image (0x20 bytes): the handle's id (-1 until loaded), the texture size, its full-texture region, and the game's two
    // additions after region, which its nk_draw_list_push_rect_uv reads: when rotate is 1 the quad turns by angle (radians) about its centre.
    std::int32_t textureHandle;
    std::int32_t textureHandleHigh;
    std::uint16_t width;
    std::uint16_t height;
    std::array<std::uint16_t, 4> region;
    std::int32_t rotate;
    float angle;
    std::int32_t imagePadding;
    std::uint32_t colour; // RGBA bytes, white when reset.
    std::int32_t field44;
};
static_assert(sizeof(UIViewImage) == 0x48);
static_assert(offsetof(UIViewImage, path) == 0x10);
static_assert(offsetof(UIViewImage, textureHandle) == 0x20);
static_assert(offsetof(UIViewImage, region) == 0x2c);
static_assert(offsetof(UIViewImage, colour) == 0x40);


enum class UIViewVariableType : std::int32_t {
    Int,
    Float,
    Char,
    Vec2,
    Float4,
};

union UIViewVariableValue {
    std::int32_t intValue;
    float floatValue;
    const char *stringValue;
    std::array<float, 2> vec2;
    std::array<float, 4> float4;
};
static_assert(sizeof(UIViewVariableValue) == 0x10);

// One "Variables" member; initialValue is ReadVariables' copy of value.
struct UIViewVariable {
    std::int32_t used;
    const char *name;
    UIViewVariableType type;
    UIViewVariableValue value;
    UIViewVariableValue initialValue;
};
static_assert(sizeof(UIViewVariable) == 0x38);
static_assert(offsetof(UIViewVariable, type) == 0x10);
static_assert(offsetof(UIViewVariable, value) == 0x18);
static_assert(offsetof(UIViewVariable, initialValue) == 0x28);

// A view slot: its images, variables and widgets (ReadUIView appends each widget's kind and its index in that kind's pool).
template <std::size_t ImageCapacity> struct UIViewSlot {
    std::int32_t inUse;
    std::int32_t imagesLoading;
    std::array<UIViewImage, ImageCapacity> images;
    std::array<UIViewVariable, kUIViewVariableCount> variables;
    std::array<std::int32_t, kUIViewWidgetCount> widgetKinds;
    std::array<std::int32_t, kUIViewWidgetCount> widgetPoolIndices;
    std::int32_t widgetCount;
    // Set while an owner keeps the view; ResetUIViewSlot then leaves it loaded.
    std::int32_t keepLoaded;
};
static_assert(sizeof(UIViewSlot<kS1UIViewImageCapacity>) == 0x3870);
static_assert(offsetof(UIViewSlot<kS1UIViewImageCapacity>, variables) == 0x2408);
static_assert(offsetof(UIViewSlot<kS1UIViewImageCapacity>, widgetKinds) == 0x2868);
static_assert(offsetof(UIViewSlot<kS1UIViewImageCapacity>, widgetPoolIndices) == 0x3068);
static_assert(offsetof(UIViewSlot<kS1UIViewImageCapacity>, keepLoaded) == 0x386c);
static_assert(sizeof(UIViewSlot<kS2UIViewImageCapacity>) == 0x2af0);
static_assert(offsetof(UIViewSlot<kS2UIViewImageCapacity>, variables) == 0x1688);
static_assert(offsetof(UIViewSlot<kS2UIViewImageCapacity>, keepLoaded) == 0x2aec);

constexpr std::size_t kUISpriteSheetCount = 56;
constexpr std::size_t kUISpriteSheetFrameCount = 64;

enum class UISpriteSheetType : std::int32_t {
    Static,
    Custom,
};



struct UISpriteSheet {
    std::int32_t used;
    const char *imageName;
    std::int32_t image;
    UISpriteSheetType type;
    float width;
    float height;
    std::array<std::array<float, 2>, kUISpriteSheetFrameCount> frameMin;
    std::array<std::array<float, 2>, kUISpriteSheetFrameCount> frameMax;
};
static_assert(sizeof(UISpriteSheet) == 0x420);
static_assert(offsetof(UISpriteSheet, image) == 0x10);
static_assert(offsetof(UISpriteSheet, width) == 0x18);
static_assert(offsetof(UISpriteSheet, frameMin) == 0x20);
static_assert(offsetof(UISpriteSheet, frameMax) == 0x220);

constexpr std::size_t kUIWindowCount = 1024;


constexpr std::array<const char *, 5> kUIScaleStyleNames{"Percent", "Fill", "Fit", "Stretch", "None"};

// A window's Width or Height: a fraction of the 1920x1080 screen, or (fromImage 1) the index of the view image its WImage or HImage names;
// and its Scale.
union UIWindowLengthValue {
    float fraction;
    std::int32_t image;
};

struct UIWindowLength {
    std::int32_t fromImage;
    UIWindowLengthValue value;
    float scale;
};
static_assert(sizeof(UIWindowLength) == 12);

// The part of a window the window reader lays out (X and Y as Offset fractions of 1920 and 1080 with their ReadSpriteAlignmentValue
// alignments), which the window also keeps as its initial layout.
struct UIWindowLayout {
    std::array<char, 64> parentId;
    float offsetX;
    float offsetY;
    float paddingX;
    float paddingY;
    UIWindowLength width;
    UIWindowLength height;
    std::int32_t scaleStyle;
    std::int32_t alignX;
    std::int32_t alignY;
    std::int32_t ignoreSafeArea;
};
static_assert(sizeof(UIWindowLayout) == 0x78);

// A hit area's mouse callback, called with the area's screen rectangle {x, y, width, height} and the data its owner gave with it.
using UIMouseCallback = void (*)(float *rect, void *data);

// The mouse callbacks UI_WindowMouseCallbacks queues for a hit area (a window's, a ButtonWheel's): entered, hovered, left and clicked, and
// whether the area was hovered when last run. 0x24 bytes: a window keeps its next field at +0x314.
#pragma pack(push, 4)
struct UIMouseCallbacks {
    UIMouseCallback entered;
    UIMouseCallback hovered;
    UIMouseCallback left;
    UIMouseCallback clicked;
    std::int32_t isHovered;
};
#pragma pack(pop)
static_assert(sizeof(UIMouseCallbacks) == 0x24);

// A UI window record (both games keep 1024; S1 HugeArray, S2 d3t::curr_ui_elems): its Id, its cached layout rectangle (UI_GetWindowRect,
// x, y, width and height, valid while rectCached is set), its layout and the layout the cache was computed for, its Nuklear window flags,
// the nk_panel UI_InputHandler begins its Nuklear window with (0x190 bytes, the game's Nuklear layout), its colour (R, G, B, A bytes) and
// alpha, the count of draws (part of each draw's unique Nuklear window name), and its mouse callbacks. +0x14C, +0x2EC and +0x314 are not
// traced yet.
struct UIWindow {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t rectCached;
    std::array<float, 4> cachedRect;
    UIWindowLayout layout;
    UIWindowLayout initialLayout;
    std::int32_t flags;
    std::int32_t field14C;
    alignas(8) std::array<std::byte, 0x190> panel;
    std::uint32_t colour;
    float alpha;
    std::int32_t drawCount;
    std::int32_t field2EC;
    UIMouseCallbacks mouseCallbacks;
    std::int32_t field314;
};
static_assert(sizeof(UIWindow) == 0x318);
static_assert(offsetof(UIWindow, layout) == 0x58);
static_assert(offsetof(UIWindow, initialLayout) == 0xd0);
static_assert(offsetof(UIWindow, flags) == 0x148);
static_assert(offsetof(UIWindow, colour) == 0x2e0);
static_assert(offsetof(UIWindow, alpha) == 0x2e4);
static_assert(offsetof(UIWindow, panel) == 0x150 && offsetof(UIWindow, mouseCallbacks) == 0x2f0 && offsetof(UIWindow, field314) == 0x314);




std::int32_t ReadSpriteAlignmentValue(const cJSON *align);



std::int32_t ReadScaleStyle(const cJSON *object, std::int32_t fallback);

constexpr std::size_t kUILayoutItemCount = 1024;

// A window's Layout entry (both games keep 1024): RowDynamic (kind 10: height and column count), RowStatic (11: also a width), SpaceBegin
// (12: height, unscaled, and count) or SpacePush (13: the space a following object fills: its X and Y Pos, Align and Offset, Width,
// Height, ScaleStyle, Padding, Fullscreen, Scale and Id). Positions, offsets and sizes are fractions of 1920 and 1080.
struct UILayoutItem {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t count;
    std::int32_t window;
    float width;
    float height;
    float scaleX;
    float scaleY;
    float posX;
    float posY;
    std::int32_t scaleStyle;
    std::int32_t alignX;
    std::int32_t alignY;
    float offsetX;
    float offsetY;
    float paddingX;
    float paddingY;
    std::int32_t fullscreen;
};
static_assert(sizeof(UILayoutItem) == 0x84);
static_assert(offsetof(UILayoutItem, count) == 0x44);
static_assert(offsetof(UILayoutItem, posX) == 0x5c);
static_assert(offsetof(UILayoutItem, alignX) == 0x68);
static_assert(offsetof(UILayoutItem, offsetX) == 0x70);
static_assert(offsetof(UILayoutItem, fullscreen) == 0x80);

// The layout pool as InitializeUIReadState resets it.
void ResetLayoutItems(std::array<UILayoutItem, kUILayoutItemCount> &items);




template <std::size_t ImageCapacity>
void ReadLayout(cJSON *layout, std::int32_t window, std::array<UILayoutItem, kUILayoutItemCount> &items, UIViewSlot<ImageCapacity> &slot);

constexpr std::size_t kUILabelWidgetCount = 512;



struct UILabelToken {
    std::array<char, 12> key;
    std::int32_t field0C;
    const char *token;
    std::int32_t alreadyLocalized;
    std::int32_t field14;
};
static_assert(sizeof(UILabelToken) == 0x20);

// The Label widget record (kind 15) UI_ReadWidgetObject allocates and UI_DrawLabelWidget draws: its Id, Token, the text it shows, its
// nk_text_align flags, its "Data" binding (an index into the data-binding pool, -1 for none), Size and FontName indices, Colour and Stroke,
// the ScaleVar binding with the safe-area screen size it was last fitted at, and its owning window.
struct UILabelWidget {
    std::array<char, 64> id;
    std::int32_t inUse;
    std::int32_t field44;
    UILabelToken token;
    const char *text;
    std::uint32_t alignment;
    std::int32_t dataBinding;
    std::int32_t size;
    std::int32_t fontFamily;
    std::int32_t hasStroke;
    std::uint32_t strokeColour;
    std::uint32_t colour;
    std::int32_t scaleVar;
    std::int32_t scaleVarAssign;
    std::int32_t scaleScreenWidth;
    std::int32_t scaleScreenHeight;
    std::int32_t window;
};
static_assert(sizeof(UILabelWidget) == 0xa0);
static_assert(offsetof(UILabelWidget, inUse) == 0x40);
static_assert(offsetof(UILabelWidget, text) == 0x68);
static_assert(offsetof(UILabelWidget, alignment) == 0x70);
static_assert(offsetof(UILabelWidget, colour) == 0x88);
static_assert(offsetof(UILabelWidget, scaleVar) == 0x8c);
static_assert(offsetof(UILabelWidget, window) == 0x9c);

constexpr std::size_t kUIImageWidgetCount = 512;

// The Image widget record (kind 18) d3t_LoadImage allocates: its Id, image (an index into its view's images), owning window, sprite frame
// (-1 for the whole image) and Rotate in radians. The use of +0x44 and of the record's tail is not traced yet.
struct UIImageWidget {
    std::int32_t inUse;
    std::array<char, 64> id;
    std::int32_t field44;
    std::int32_t window;
    std::int32_t frame;
    std::int32_t image;
    float angle;
    std::array<std::byte, 0x28> field58;
};
static_assert(sizeof(UIImageWidget) == 0x80);
static_assert(offsetof(UIImageWidget, field44) == 0x44);
static_assert(offsetof(UIImageWidget, image) == 0x50);
static_assert(offsetof(UIImageWidget, angle) == 0x54);



template <std::size_t ImageCapacity> std::int32_t AddViewWidget(UIViewSlot<ImageCapacity> &slot, std::int32_t kind, std::int32_t poolIndex);



std::int32_t AllocLabelWidget(std::array<UILabelWidget, kUILabelWidgetCount> &widgets, const char *id, const char *text, const UILabelToken &token,
    std::uint32_t alignment, std::int32_t dataBinding, std::int32_t size, std::int32_t fontFamily, std::uint32_t colour, std::int32_t hasStroke,
    std::uint32_t strokeColour, std::int32_t window, std::int32_t scaleVar, std::int32_t scaleVarAssign);


std::int32_t AllocImageWidget(std::array<UIImageWidget, kUIImageWidgetCount> &widgets, const char *id, std::int32_t image, std::int32_t window, float angle);



void ReadVariables(cJSON *document, std::array<UIViewVariable, kUIViewVariableCount> &variables);


template <std::size_t ImageCapacity> std::int32_t FindViewImage(const UIViewSlot<ImageCapacity> &slot, const char *name);

// The sprite-sheet pool as InitializeUIReadState resets it.
void ResetSpriteSheets(std::array<UISpriteSheet, kUISpriteSheetCount> &sheets);



template <std::size_t ImageCapacity>
void ReadSprites(cJSON *document, std::array<UISpriteSheet, kUISpriteSheetCount> &sheets, const UIViewSlot<ImageCapacity> &slot);

} // namespace shenmue::d3t
