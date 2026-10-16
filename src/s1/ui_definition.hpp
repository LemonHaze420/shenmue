#pragma once

#include "d3t/buffer.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_view.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;

enum class UIAlign {
    Top,
    Centre,
    Bottom,
    BottomOut,
    TopOut,
    Left,
    Right,
    LeftOut,
    RightOut,
    CentreLeft,
    CentreRight,
    CentreUp,
    CentreDown,
};

struct UIAxisDefinition {
    float position{};
    float offset{};
    UIAlign align{UIAlign::Centre};
};

float UIAlignedPosition(UIAlign align, float parentPosition, float parentSize, float size, float authoredPosition, float offset);


enum UITextAlign : std::uint32_t {
    UITextAlignLeft = 0x01,
    UITextAlignCentred = 0x02,
    UITextAlignRight = 0x04,
    UITextAlignTop = 0x08,
    UITextAlignMiddle = 0x10,
    UITextAlignBottom = 0x20,
};

std::uint32_t ReadAlignmentValue(const char *alignment);


inline constexpr std::array<std::string_view, 66> kInputActionNames{"Confirm", "Cancel", "Event", "System", "Filter", "Snapshot", "Swap", "Pause", "Look",
    "Run", "LookUp", "LookDown", "LookLeft", "LookRight", "MoveUp", "MoveDown", "MoveLeft", "MoveRight", "QTE1", "QTE2", "QTE3", "QTE4", "FightKick",
    "FightThrow", "FightPunch", "FightBlock", "FightUnused1", "FightUnused2", "FightUnused3", "FightUnused4", "FightAssignedMove", "FightRun",
    "VehicleRaiseLift", "VehicleCamera", "VehicleMap", "VehicleMenu", "VehicleUnused1", "VehicleUnused2", "VehicleUnused3", "VehicleUnused4", "VehicleAccel",
    "VehicleReverse", "VehicleUnused5", "VehicleUnused6", "VehicleUnused7", "VehicleUnused8", "VehicleUnused9", "VehicleUnused10", "VehicleLeft",
    "VehicleRight", "MenuBarLeft", "MenuBarRight", "SubBarLeft", "SubBarRight", "FightLeft", "FightRight", "LookAll", "LookVertical", "LookHorizontal",
    "MoveAll", "MoveVertical", "MoveHorizontal", "DpadAll", "PressStartButton", "GameHudConfirm", "GameHudCancel"};
// The IconFontSize names, in font order.
inline constexpr std::array<std::string_view, 6> kIconFontSizeNames{"H1", "H2", "H3", "BL", "B", "BS"};

std::uint32_t LoadTextureFromFilepath_Wrapper(const char *path, std::uint32_t renderHandle);

struct UIImageDefinition {
    std::string name;
    std::string path;
    std::uint32_t textureHandle{};
};
struct UISpriteSheetDefinition {
    std::string imageName;
    float textureWidth{};
    float textureHeight{};
    float cellWidth{};
    float cellHeight{};
    // "Custom" sheets list their frames as pixel rectangles {x, y, width, height} in UV key order.
    std::vector<std::array<float, 4>> frames;
};

struct UIPanelDefinition {
    std::string id;
    std::string parentId;
    float width{};
    float height{};
    UIAxisDefinition x;
    UIAxisDefinition y;
    float drawWidth{};
    float drawHeight{};
    UIAxisDefinition drawX;
    UIAxisDefinition drawY;
    // SpacePush "Fullscreen": the item covers the whole screen at its own aspect and is seen through the panel's window.
    bool drawFullscreen{};
    // SpacePush "ScaleStyle" (Percent, Fill, Fit, Stretch or None) sizing the item against the panel.
    std::string drawScaleStyle;
    std::string imageName;
    std::string objectId;
    std::int32_t spriteIndex{-1};
    bool buttonIcon{};
    float resolvedX{};
    float resolvedY{};
    bool layoutResolved{};
    std::string scaleStyle;
    float scaleX{1.0f};
    float scaleY{1.0f};
    float paddingX{};
    float paddingY{};

    float opacity{1.0f};


    std::uint32_t windowBackground{};
};

enum class UIObjectKind {
    Image,
    ColourImage,
    Sprite,
    Label,
    Button,
    ButtonIcon,
};

// One authored "Objects" entry, in panel source order, with the SpacePush layout that precedes it.
struct UIObjectDefinition {
    UIObjectKind kind{UIObjectKind::Image};
    std::string panelId;
    std::string id;
    std::string imageName;
    std::int32_t spriteIndex{-1};
    std::uint32_t colour{0xffffffffU}; // RGBA bytes, R in the low byte.
    std::string text;
    // A label's string-table token, resolved when the label is drawn.
    std::string token;

    std::uint32_t textAlignment{UITextAlignMiddle | UITextAlignCentred};


    std::string scaleVarName;
    std::int32_t scaleVar{-1};
    std::int32_t scaleVarAssign{1};
    mutable std::int32_t scaleScreenWidth{};
    mutable std::int32_t scaleScreenHeight{};
    std::string size;


    std::int32_t fontFamily{};
    std::int32_t fontSize{4};
    bool hasStroke{};
    std::uint32_t strokeColour{}; // RGBA bytes; authored as "#RRGGBB" or as an R/G/B/A object.
    std::string scaleStyle;
    float width{};
    float height{};
    UIAxisDefinition x;
    UIAxisDefinition y;
    float scaleX{1.0f};
    float scaleY{1.0f};
    float rotation{};
    // The SpacePush padding, its "Fullscreen" (the item covers the screen at its own aspect, seen through the panel) and the object's
    // "Alpha", which scales its colour's alpha.
    float paddingX{};
    float paddingY{};
    bool fullscreen{};
    float alpha{1.0f};



    std::int32_t action{};
    std::int32_t iconFontSize{5};
    std::int32_t allowDoubleWidth{1};
    std::int32_t controlMethod{-1};
    std::int32_t controlState{-1};
    std::uint32_t fontColour{0xffffffffU};
};



struct UIGaugeDefinition {
    std::string panelId;
    d3t::UIGaugeState state;
};





class UIDefinition {
  public:
    bool parse(const std::byte *data, std::size_t size);
    // Resolve every panel's position from its parent chain; parse runs it, and an owner that re-parents or resizes a panel reruns it.
    void resolveLayout();
    void requestTextures(std::uint32_t renderHandle);
    std::uint32_t draw(D3TDeviceContext *context, std::uint32_t renderTarget, float screenWidth, float screenHeight, float opacity, float offsetX = 0.0f,
        float offsetY = 0.0f) const;

    const UIPanelDefinition *findPanel(const std::string &id) const;
    const UIImageDefinition *findImage(const std::string &name) const;
    const UIObjectDefinition *findObject(const std::string &id) const;

    std::vector<UIImageDefinition> images;
    std::vector<UISpriteSheetDefinition> spriteSheets;
    std::vector<UIPanelDefinition> panels;
    std::vector<UIObjectDefinition> objects;

    std::vector<UIObjectDefinition> buttonIcons;
    std::vector<UIGaugeDefinition> gauges;

    std::int32_t viewSlot{-1};
};

} // namespace shenmue::s1
