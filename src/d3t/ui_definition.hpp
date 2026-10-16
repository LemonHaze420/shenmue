#pragma once

#include "d3t/ui_view.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace shenmue::d3t {

enum class UIAlign {
    Top,
    Centre,
    Bottom,
    BottomOut,
    TopOut,
    CentreRight,
    CentreLeft,
    CentreUp,
    CentreDown,
};

struct UIAxisDefinition {
    float position{};
    float offset{};
    UIAlign align{UIAlign::Centre};
};

struct UIImageDefinition {
    std::string name;
    std::string path;
    std::uint32_t textureHandle{};
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
    std::string imageName;
    std::string objectId;
    float imageOpacity{1.0f};


    std::string scaleStyle;
    float scaleX{1.0f};
    float scaleY{1.0f};
    float paddingX{};
    float paddingY{};
};

struct UILabelDefinition {
    std::string panelId;
    std::string layoutId;
    std::string text;
    std::string alignment;
    std::string scaleStyle;
    float width{};
    float height{};
    UIAxisDefinition x;
    UIAxisDefinition y;
    float scaleX{1.0f};
    float scaleY{1.0f};
    std::string id;
    std::string token;
    std::string fontName;
    std::string fontSize;
    std::uint32_t colour{0xffffffffU};
    std::uint32_t strokeColour{0xff000000U};
    bool hasStroke{};
    float textScale{1.0f};
    std::int32_t scaleVar{-1};
    std::int32_t panelIndex{-1};
    std::int32_t scaleVarAssign{1};
    mutable float scaleScreenWidth{};
    mutable float scaleScreenHeight{};
};

struct UISpriteFrame {
    float x{};
    float y{};
    float width{};
    float height{};
};

struct UISpriteSheetDefinition {
    std::string imageName;
    float width{};
    float height{};
    std::array<UISpriteFrame, 64> frames{};
};

enum class UITextureObjectKind {
    Image,
    Sprite,
    ButtonIcon,
    ColourButton,
};

struct UITextureObjectDefinition {
    UITextureObjectKind kind{UITextureObjectKind::Image};
    std::string id;
    std::string panelId;
    std::string layoutId;
    std::string imageName;
    std::string scaleStyle;
    UIAxisDefinition x;
    UIAxisDefinition y;
    float width{};
    float height{};
    float scaleX{1.0f};
    float scaleY{1.0f};
    float paddingX{};
    float paddingY{};
    std::int32_t frameIndex{};
    std::string action;
    std::string iconFontSize;
    std::string textAlignment;
    std::int32_t controlMethod{-1};
    std::int32_t controlState{-1};
    std::int32_t allowDoubleWidth{1};
    std::uint32_t fontColour{0xffffffffU};
    std::uint32_t colour{0xffffffffU}; // RGBA bytes, like UITexturedVertex.
    float rotation{};
    bool hidden{};
    float rounding{};
    bool fullscreen{};
    bool useShared{};
};


struct UIGaugeState {
    std::int32_t allocated{};
    std::int32_t panelIndex{-1};
    std::int32_t hidden{};
    std::int32_t transparentBackground{};
    std::array<std::int32_t, 9> images{};
    std::int32_t backgroundImage{};
    std::uint32_t colour{0xffffffffU};
    std::int32_t count{};
    std::int32_t filledCount{};
    std::array<std::array<float, 2>, 12> positions{};
    float width{};
    float height{};
    std::array<std::int32_t, 12> orbColours{};
    std::int32_t suppressEmptyOrbs{};
    std::int32_t translucentLastOrb{};
    float lastOrbAlpha{};
    std::int32_t fieldE8{};
};
static_assert(sizeof(UIGaugeState) == 0xec);
static_assert(offsetof(UIGaugeState, images) == 0x10);
static_assert(offsetof(UIGaugeState, filledCount) == 0x40);
static_assert(offsetof(UIGaugeState, positions) == 0x44);
static_assert(offsetof(UIGaugeState, orbColours) == 0xac);
static_assert(offsetof(UIGaugeState, lastOrbAlpha) == 0xe4);

struct UIGaugeDefinition {
    std::string panelId;
    UIGaugeState state;
};

class UIDefinition {
  public:
    bool parse(const std::byte *data, std::size_t size, bool includeColourButtons = false);
    std::shared_ptr<cJSON> document;
    std::array<UIViewVariable, kUIViewVariableCount> variables{};

    const UIPanelDefinition *findPanel(const std::string &id) const;
    const UIImageDefinition *findImage(const std::string &name) const;

    std::vector<UIImageDefinition> images;
    std::vector<UIPanelDefinition> panels;
    std::vector<UILabelDefinition> labels;


    std::vector<UITextureObjectDefinition> textureObjects;
    std::vector<UISpriteSheetDefinition> spriteSheets;
    std::vector<UIGaugeDefinition> gauges;
};

} // namespace shenmue::d3t

#include "shenmue.h"

#include <array>
#include <cstdint>

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

inline constexpr std::array<std::int8_t, 24> kTextBlockCellActions{59, 56, 0, 1, 2, 3, 8, 9, 7, -1, -1, 4, 5, 24, 22, 23, 25, 16, 17, 14, 15, 61, 60, 62};

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
