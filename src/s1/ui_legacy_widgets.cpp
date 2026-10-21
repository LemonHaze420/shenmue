#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {

// UI_ScaleSize (Percent 0, Fill 1, Fit 2, Stretch 3, None 4) followed by the panel/object Scale, in the drawn space where one
// authored unit is `unit` wide.
void UIScaleLayout(const std::string &style, float authoredWidth, float authoredHeight, const UILayoutRect &parent, float referenceWidth, float referenceHeight,
    float unit, float &width, float &height) {
    width = authoredWidth * unit;
    height = authoredHeight * unit;
    if (style == "Stretch") {
        width = parent.width;
        height = parent.height;
    } else if ((style == "Fit" || style == "Fill") && authoredWidth > 0.0f && authoredHeight > 0.0f) {
        const float x = parent.width / authoredWidth;
        const float y = parent.height / authoredHeight;
        const float scale = style == "Fit" ? (x < y ? x : y) : (x > y ? x : y);
        width = authoredWidth * scale;
        height = authoredHeight * scale;
    } else if (style == "Percent") {
        width = referenceWidth != 0.0f ? authoredWidth * parent.width / referenceWidth : 0.0f;
        height = referenceHeight != 0.0f ? authoredHeight * parent.height / referenceHeight : 0.0f;
    }
}

UILayoutRect UIPanelLayoutRect(const UIPanelDefinition &panel, const UILayoutRect &parent, const UIPanelDefinition &parentPanel, float unit) {
    UILayoutRect rect{};
    UIScaleLayout(panel.scaleStyle, panel.width, panel.height, parent, parentPanel.width, parentPanel.height, unit, rect.width, rect.height);
    rect.width *= panel.scaleX;
    rect.height *= panel.scaleY;
    rect.x = UIAlignedPosition(panel.x.align, parent.x, parent.width, rect.width, panel.x.position * unit, panel.x.offset * unit);
    rect.y = UIAlignedPosition(panel.y.align, parent.y, parent.height, rect.height, panel.y.position * unit, panel.y.offset * unit);
    return rect;
}




UILayoutRect UIObjectLayoutRect(const UIObjectDefinition &object, const UIPanelDefinition &panel, const UILayoutRect &panelRect, float unit) {
    if (object.fullscreen) {
        float width = 1920.0f;
        float height = 1080.0f;
        if (object.width > 0.0f && object.height > 0.0f) {
            if (object.height / object.width < 1080.0f / 1920.0f)
                width = 1080.0f / object.height * object.width;
            else if (object.height / object.width > 1080.0f / 1920.0f)
                height = 1920.0f / object.width * object.height;
        }
        return {.x = 960.0f - width * 0.5f, .y = 540.0f - height * 0.5f, .width = width, .height = height};
    }
    const UILayoutRect content{.x = panelRect.x + panel.paddingX * unit,
        .y = panelRect.y + panel.paddingY * unit,
        .width = std::max(0.0f, panelRect.width - panel.paddingX * unit * 2.0f),
        .height = std::max(0.0f, panelRect.height - panel.paddingY * unit * 2.0f)};
    float width = 0.0f;
    float height = 0.0f;
    UIScaleLayout(object.scaleStyle, object.width, object.height, content, panel.width, panel.height, unit, width, height);
    const float x = UIAlignedPosition(object.x.align, content.x, content.width, width, 0.0f, 0.0f) + object.paddingX * unit +
        (object.x.position + object.x.offset) / 1920.0f * content.width;
    const float y = UIAlignedPosition(object.y.align, content.y, content.height, height, 0.0f, 0.0f) + object.paddingY * unit +
        (object.y.position + object.y.offset) / 1080.0f * content.height;
    width = std::max(0.0f, width - object.paddingX * unit * 2.0f);
    height = std::max(0.0f, height - object.paddingY * unit * 2.0f);
    return {.x = x + (width - width * object.scaleX) * 0.5f,
        .y = y + (height - height * object.scaleY) * 0.5f,
        .width = width * object.scaleX,
        .height = height * object.scaleY};
}

void UI_PlaySound(std::int32_t index) {
    if (index < 0 || static_cast<std::size_t>(index) >= kUiSoundCommands.size())
        return;
    std::int32_t &frame = g_uiSoundFrames[static_cast<std::size_t>(index)];
    PlayGatedUiSound(frame, kUiSoundCommands[static_cast<std::size_t>(index)]);
    }

// Resolve an authored panel through its parents into the safe-area-mapped authored 1920x1080 space. Children lay out inside the
// parent's padded rect, as UI_GetPanelRect returns it. Submenu roots parent to GameMenu_New.ui panels, the Collection tabs' roots to
// SubMenu_Collection.ui panels and, in the frontend, the Options and Load Save roots to the Main Menu's MainMenu::ContentPanel (their
// GetContentPanel picks the parent), which are looked up in those definitions. An owner may displace one panel, and with it its
// children, by an animated offset.
UILayoutRect ResolveUIPanelRect(const UIDefinition &definition, const UIPanelDefinition &panel, std::int32_t depth, const UIPanelOffset *offset) {
    const float safeScale = 0.8f + std::clamp(g_currentSettings.display.uiDisplayArea, 0.0f, 1.0f) * 0.2f;
    const UIDefinition *parentDefinition = &definition;
    const UIPanelDefinition *parent = panel.parentId.empty() ? nullptr : definition.findPanel(panel.parentId);
    constexpr std::array<std::int32_t, 3> kParentOwners{kUiGameMenu, kUiSubMenuCollection, 4};
    for (std::size_t owner = 0; !parent && !panel.parentId.empty() && owner < kParentOwners.size(); ++owner) {
        parentDefinition = &g_parsedUIDefinitions[static_cast<std::size_t>(kParentOwners[owner])];
        parent = parentDefinition->findPanel(panel.parentId);
    }
    if (!parent || depth > 16) {
        UIPanelDefinition root{};
        root.width = 1920.0f;
        root.height = 1080.0f;
        const UILayoutRect screen{
            .x = 960.0f * (1.0f - safeScale), .y = 540.0f * (1.0f - safeScale), .width = 1920.0f * safeScale, .height = 1080.0f * safeScale};
        return UIPanelLayoutRect(panel, screen, root, safeScale);
    }
    const UILayoutRect parentRect = ResolveUIPanelRect(*parentDefinition, *parent, depth + 1, offset);
    const UILayoutRect content{.x = parentRect.x + parent->paddingX * safeScale,
        .y = parentRect.y + parent->paddingY * safeScale,
        .width = std::max(0.0f, parentRect.width - parent->paddingX * safeScale * 2.0f),
        .height = std::max(0.0f, parentRect.height - parent->paddingY * safeScale * 2.0f)};
    UILayoutRect rect = UIPanelLayoutRect(panel, content, *parent, safeScale);
    if (offset && offset->panel == &panel) {
        rect.x += offset->x;
        rect.y += offset->y;
    }
    return rect;
}

// The width of a line of text in the subtitle font at `scale`, in the authored 1920x1080 units the UI draws in.
float MeasureUILabelText(const SubtitleFontLayer &font, const char *text, float scale) {
    float lineWidth = 0.0f;
    while (text && *text) {
        if (const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, DecodeSubtitleCodepoint(text)))
            lineWidth += glyph->advance * scale;
    }
    return lineWidth;
}

// A widget label's text in the subtitle font at `scale`, placed in `rect` by its Nuklear alignment (left 1, centred 2, right 4; middle
// 16 centres it vertically, bottom 32 sets it on the bottom edge, otherwise it sits at the top).
bool DrawUILabelText(const SubtitleFontLayer &font, const char *text, const UILayoutRect &rect, float scale, std::uint32_t align, std::uint32_t colour,
    std::uint32_t renderTarget) {
    const std::uint32_t texture = font.texture.load(std::memory_order_acquire);
    if (!texture || !text || !*text)
        return false;
    std::array<std::uint32_t, 256> codepoints{};
    std::size_t codepointCount = 0;
    float lineWidth = 0.0f;
    while (*text && codepointCount < codepoints.size()) {
        const std::uint32_t codepoint = DecodeSubtitleCodepoint(text);
        codepoints[codepointCount++] = codepoint;
        if (const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, codepoint))
            lineWidth += glyph->advance * scale;
    }
    const float toScreenX = static_cast<float>(g_uiScreenWidth) / 1920.0f;
    const float toScreenY = static_cast<float>(g_uiScreenHeight) / 1080.0f;
    float cursor = rect.x;
    if ((align & 2U) != 0)
        cursor = rect.x + (rect.width - lineWidth) * 0.5f;
    else if ((align & 4U) != 0)
        cursor = rect.x + rect.width - lineWidth;
    const float textHeight = font.lineHeight * scale;
    const float originY = (align & 16U) ? rect.y + (rect.height - textHeight) * 0.5f : (align & 32U) ? rect.y + rect.height - textHeight : rect.y;
    std::array<NuklearQuad, 256> quads{};
    std::size_t quadCount = 0;
    for (std::size_t index = 0; index < codepointCount; ++index) {
        const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, codepoints[index]);
        if (!glyph)
            continue;
        if (glyph->width > 0.0f && glyph->height > 0.0f) {
            quads[quadCount++] = {.x = (cursor + glyph->x0 * scale) * toScreenX,
                .y = (originY + glyph->y0 * scale) * toScreenY,
                .width = glyph->width * scale * toScreenX,
                .height = glyph->height * scale * toScreenY,
                .u0 = glyph->u0,
                .v0 = glyph->v0,
                .u1 = glyph->u1,
                .v1 = glyph->v1,
                .colour = colour};
        }
        cursor += glyph->advance * scale;
    }
    return quadCount && DrawNuklearQuads(g_RenderDeviceContext, renderTarget, texture, std::span{quads.data(), quadCount});
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {




std::int32_t GetKeyForInputType(std::int32_t action, std::int32_t inputType) {
    const std::int32_t type = inputType == -1 ? static_cast<std::int32_t>(GetCurrentInputType()) : inputType;
    std::int32_t key = -1;
    if (type > static_cast<std::int32_t>(InputMode::GameUi)) {
        switch (static_cast<InputMode>(type)) {
        case InputMode::Qte:
            if (static_cast<std::uint32_t>(action - 18) <= 3U)
                key = UserMapping(static_cast<std::uint32_t>(action));
            else
                key = UserMapping(static_cast<std::uint32_t>(action <= 3 ? 18 + action : action));
            break;
        case InputMode::Fight:
            key = UserMapping(static_cast<std::uint32_t>(action < 22 ? 22 + action : action));
            break;
        case InputMode::Qte2:
            key = UserMapping(static_cast<std::uint32_t>(action < 22 ? 18 + action : action));
            break;
        case InputMode::Vehicle:
            key = UserMapping(static_cast<std::uint32_t>(action < 22 ? 32 + action : action));
            break;
        default:
            key = UserMapping(static_cast<std::uint32_t>(action));
            break;
        }
    } else if (action < 50) {
        key = action < 18 ? kKeyboardKeys[static_cast<std::size_t>(action)] : UserMapping(static_cast<std::uint32_t>(action));
    }
#if defined(_WIN32)
    return static_cast<std::int32_t>(ExtendedMapVirtualKey(static_cast<std::uint32_t>(key), reinterpret_cast<void *>(0x08090809), GetKeyboardLayout(0)));
#else
    return key;
#endif
}




std::int32_t ResolvePressedKey(std::int32_t action, std::int32_t inputType, std::int32_t doubleWidth) {
    std::int32_t key = 0;
    if (action < 50) {
        key = GetKeyForInputType(action, inputType);
    } else {
        switch (action) {
        case 50:
            key = 'Q';
            break;
        case 51:
            key = 'E';
            break;
        case 52:
            key = 'Z';
            break;
        case 53:
            key = 'C';
            break;
        case 54:
            key = g_applicationSettings.savedInput.userMappedQteKeys[0];
            break;
        case 55:
            key = g_applicationSettings.savedInput.userMappedQteKeys[1];
            break;
        case 56:
            return 30;
        case 57:
            return 31;
        case 58:
            return 32;
        case 59:
        case 62:
            return 33;
        case 60:
            return 34;
        case 61:
            return 35;
        case 63:
            return 55;
        case 64:
        case 65: {
            const std::int32_t type = inputType == -1 ? static_cast<std::int32_t>(GetCurrentInputType()) : inputType;
            const std::uint32_t slot = action == 64 ? 0U : 1U;
            if (type <= static_cast<std::int32_t>(InputMode::GameUi))
                key = action == 64 ? 0x0d : 0x1b;
            else if (type == static_cast<std::int32_t>(InputMode::Qte) || type == static_cast<std::int32_t>(InputMode::Qte2))
                key = UserMapping(18U + slot);
            else if (type == static_cast<std::int32_t>(InputMode::Fight))
                key = UserMapping(22U + slot);
            else if (type == static_cast<std::int32_t>(InputMode::Vehicle))
                key = UserMapping(32U + slot);
            else
                key = UserMapping(slot);
#if defined(_WIN32)
            key = static_cast<std::int32_t>(ExtendedMapVirtualKey(static_cast<std::uint32_t>(key), reinterpret_cast<void *>(0x08090809), GetKeyboardLayout(0)));
#endif
            break;
        }
        default:
            break;
        }
    }
    if (key > 256)
        return key == 257 ? 43 : key == 258 ? 42 : 26;
    switch (key) {
    case 256:
        return 41;
    case -1:
        return 40;
    case 0x08:
    case 0x09:
    case 0x10:
    case 0x11:
    case 0x14:
    case 0x20:
    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
        return doubleWidth == 1 ? 27 : 26;
    case 0x0d:
        return doubleWidth == 1 ? 29 : 26;
    case 0x25:
        return 36;
    case 0x26:
        return 37;
    case 0x27:
        return 38;
    case 0x28:
        return 39;
    default:
        return 26;
    }
}

} // namespace shenmue::s1

namespace shenmue::s1::ui_runtime_detail {


std::int32_t ResolveDirectionalGlyph(std::int32_t action) {
    switch (action) {
    case 10:
    case 14:
        return 7;
    case 11:
    case 15:
        return 5;
    case 12:
    case 16:
        return 8;
    case 13:
    case 17:
        return 6;
    case 56:
    case 59:
        return 4;
    case 57:
    case 60:
        return 10;
    case 58:
    case 61:
        return 9;
    case 62:
        return 15;
    default:
        return -1;
    }
}

// The pad glyph tables' A/B context: Steam Input's game layout (or its debug allowance) with the keyboard idle, and whether the cinema
// bars or the skip prompt (UI element 74 ticked this frame, or in the previous frame for the swap context) flip the confirm and cancel faces.
bool PadLayoutActive() {
    return g_keyboardActive != 1 && (g_steamInputGameLayoutState == 1 || g_debugAllowInputGameLayout != 0);
}

bool PadFacesSwappedByScene() {
    return g_cinemaBars[1].isVisible != 0 || (g_uiRefreshedElements[2] & 0x400U) != 0;
}


std::int32_t ResolveXboxGlyph(std::int32_t action) {
    const bool swapTriggers = g_applicationSettings.savedInput.swapTriggers != 0;
    switch (action) {
    case 0:
        return static_cast<std::int32_t>(ResolveCurrentInputForGame(-1)) + 18;
    case 1:
        return 19 - static_cast<std::int32_t>(ResolveCurrentInputForGame(-1));
    case 2:
    case 20:
    case 24:
        return 20;
    case 3:
    case 21:
    case 25:
        return 21;
    case 4:
    case 52:
        return 2;
    case 5:
    case 53:
        return 3;
    case 8:
    case 50:
        return 0;
    case 9:
    case 51:
        return 1;
    case 18:
        if (!PadLayoutActive())
            return 18;
        return PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0 ? 19 : 18;
    case 19:
        if (!PadLayoutActive())
            return 19;
        return PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0 ? 18 : 19;
    case 22:
        return PadLayoutActive() && (PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0) ? 19 : 18;
    case 23:
        return PadLayoutActive() && (PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0) ? 18 : 19;
    case 30:
        return swapTriggers ? 1 : 0;
    case 31:
        return swapTriggers ? 0 : 1;
    case 54:
        return 11;
    case 55:
        return 13;
    case 63:
        return PadLayoutActive() ? 19 : 18;
    case 65:
        return 19;
    default:
        return 18;
    }
}



std::int32_t ResolvePlayStationGlyph(std::int32_t action) {
    const bool swapTriggers = g_applicationSettings.savedInput.swapTriggers != 0;
    switch (action) {
    case 0:
        return static_cast<std::int32_t>(ResolveCurrentInputForGame(-1)) + 22;
    case 1:
        return 23 - static_cast<std::int32_t>(ResolveCurrentInputForGame(-1));
    case 2:
    case 20:
    case 24:
        return 24;
    case 3:
    case 21:
    case 25:
        return 25;
    case 4:
    case 52:
        return 46;
    case 5:
    case 53:
        return 47;
    case 8:
    case 50:
        return 44;
    case 9:
    case 51:
        return 45;
    case 18:
        if (!PadLayoutActive())
            return 22;
        return PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0 ? 23 : 22;
    case 19:
        if (!PadLayoutActive())
            return 23;
        return PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0 ? 22 : 23;
    case 22:
        return PadLayoutActive() && (PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0) ? 23 : 22;
    case 23:
        return PadLayoutActive() && (PadFacesSwappedByScene() || (g_uiPreviousRefreshedElements[2] & 0x400U) != 0) ? 22 : 23;
    case 30:
        return swapTriggers ? 45 : 44;
    case 31:
        return swapTriggers ? 44 : 45;
    case 54:
        return 48;
    case 55:
        return 50;
    case 63:
        return PadLayoutActive() ? 23 : 22;
    case 65:
        return 23;
    default:
        return 22;
    }
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {





std::int32_t ResolveButtonIconGlyph(std::int32_t action, std::int32_t controlMethod, std::int32_t inputType, std::int32_t doubleWidth) {
    if (controlMethod == -1)
        controlMethod = g_keyboardActive;
    const std::int32_t platform = g_debugUnknown0;
    if (platform == 1 || (static_cast<std::uint32_t>(platform - 2) > 1U && controlMethod == 1))
        return ResolvePressedKey(action, inputType, doubleWidth);
    if (const std::int32_t directional = ResolveDirectionalGlyph(action); directional != -1)
        return directional;
    return platform == 3 ? ResolvePlayStationGlyph(action) : ResolveXboxGlyph(action);
}

} // namespace shenmue::s1

namespace shenmue::s1::d3t {



bool FormatVirtualKeyString(std::int32_t key, std::array<char, 64> &name) {
    name[0] = '\0';
    if (key >= 256) {
        if (key > 260)
            return false;
        std::snprintf(name.data(), name.size(), "M %d", key - 256);
        return true;
    }
#if defined(_WIN32)
    UINT scan = MapVirtualKeyA(static_cast<UINT>(key), MAPVK_VK_TO_VSC);
    if (scan >= 2 && scan <= 11) {
        name[0] = scan == 11 ? '0' : static_cast<char>('0' + scan - 1);
        name[1] = '\0';
        return true;
    }
    switch (key) {
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 45:
    case 46:
    case 111:
    case 144:
        scan |= 0x100U;
        break;
    default:
        break;
    }
    std::array<char, 64> text{};
    if (GetKeyNameTextA(static_cast<LONG>(scan << 16U), text.data(), static_cast<int>(text.size())) <= 0)
        return false;
    std::snprintf(name.data(), name.size(), "%s", text.data());
    return true;
#else
    return false;
#endif
}

} // namespace shenmue::s1::d3t

namespace shenmue::s1::ui_runtime_detail {






bool UI_DrawButtonIcon(const UIDefinition &definition, const UIObjectDefinition &widget, const UILayoutRect &bounds, std::int32_t action, float alpha,
    std::uint32_t renderTarget) {
    const std::int32_t glyph = ResolveButtonIconGlyph(action, widget.controlMethod, widget.controlState, widget.allowDoubleWidth);
    const UIDefinition &platformIcons = g_parsedUIDefinitions[kUiPlatformIcons];
    if (glyph < 0 || platformIcons.spriteSheets.empty() || static_cast<std::size_t>(glyph) >= platformIcons.spriteSheets[0].frames.size())
        return false;
    const UIImageDefinition *image = definition.findImage(widget.imageName);
    if (!image || !image->textureHandle)
        image = platformIcons.findImage("ButtonIcons");
    std::uint32_t textureWidth = 0;
    std::uint32_t textureHeight = 0;
    if (!image || !image->textureHandle || !GetTexture2DDimensions(image->textureHandle, &textureWidth, &textureHeight) || !textureWidth || !textureHeight)
        return false;
    const auto withAlpha = [alpha](std::uint32_t colour) {
        return (colour & 0x00ffffffU) | (static_cast<std::uint32_t>(static_cast<float>(colour >> 24U) * alpha) << 24U);
    };
    const std::array<float, 4> &cell = platformIcons.spriteSheets[0].frames[static_cast<std::size_t>(glyph)];
    const float toScreenX = static_cast<float>(g_uiScreenWidth) / 1920.0f;
    const float toScreenY = static_cast<float>(g_uiScreenHeight) / 1080.0f;
    const NuklearQuad quad{.x = bounds.x * toScreenX,
        .y = bounds.y * toScreenY,
        .width = bounds.width * toScreenX,
        .height = bounds.height * toScreenY,
        .u0 = cell[0] / static_cast<float>(textureWidth),
        .v0 = cell[1] / static_cast<float>(textureHeight),
        .u1 = (cell[0] + cell[2]) / static_cast<float>(textureWidth),
        .v1 = (cell[1] + cell[3]) / static_cast<float>(textureHeight),
        .colour = withAlpha(widget.colour)};
    bool drew = DrawNuklearQuads(g_RenderDeviceContext, renderTarget, image->textureHandle, std::span{&quad, 1});
    const bool enterCap = glyph == 29 || glyph == 55;
    if ((glyph < 26 || glyph > 28) && !enterCap)
        return drew;
    std::int32_t key = 0x0d;
    if (!enterCap) {
        switch (action) {
        case 50:
            key = 'Q';
            break;
        case 51:
            key = 'E';
            break;
        case 52:
            key = 'Z';
            break;
        case 53:
            key = 'C';
            break;
        case 54:
            key = g_applicationSettings.savedInput.userMappedQteKeys[0];
            break;
        case 55:
            key = g_applicationSettings.savedInput.userMappedQteKeys[1];
            break;
        default:
            key = action < 50 ? GetKeyForInputType(action, widget.controlState) : 0;
            break;
        }
    }
    std::array<char, 64> name{};
    if (!d3t::FormatVirtualKeyString(key, name) || g_uiLoadedLanguage < 0 || g_uiLoadedLanguage >= static_cast<std::int32_t>(g_subtitleFonts.size()))
        return drew;
    const UILayoutRect label{
        .x = bounds.x + bounds.width * 0.125f, .y = bounds.y + bounds.height * 0.125f, .width = bounds.width * 0.75f, .height = bounds.height * 0.75f};
    const std::uint32_t fontColour = withAlpha(widget.fontColour);
    const std::array<SubtitleFontLayer, 2> &fonts = g_subtitleFonts[static_cast<std::size_t>(g_uiLoadedLanguage)];
    const SubtitleFontLayer &font = fonts[(fontColour & 0x00ffffffU) == 0 ? 1 : 0];
    const float lineHeight = fonts[0].lineHeight;

    const auto fit = [&font, lineHeight](const char *text, const UILayoutRect &rect) {
        float scale = 1.0f;
        const float width = MeasureUILabelText(font, text, 1.0f);
        if (width > rect.width)
            scale = rect.width / width;
        if (lineHeight > rect.height && scale > rect.height / lineHeight)
            scale *= rect.height / lineHeight;
        return std::max(scale, 0.33f);
    };
    const float scale = fit(name.data(), label);
    const std::string_view text{name.data()};
    // Below half size it splits at the space nearest the text's middle (UI_FindLabelSplit) into two half-height lines at the smaller fit.
    std::size_t split = std::string_view::npos;
    if (scale < 0.5f) {
        std::size_t nearest = text.size();
        for (std::size_t at = 0; at < text.size(); ++at) {
            const std::size_t distance = at > text.size() / 2 ? at - text.size() / 2 : text.size() / 2 - at;
            if (text[at] == ' ' && distance < nearest) {
                split = at;
                nearest = distance;
            }
        }
    }
    if (split == std::string_view::npos)
        return drew | DrawUILabelText(font, name.data(), label, scale, widget.textAlignment, fontColour, renderTarget);
    const std::string first{text.substr(0, split)};
    const std::string second{text.substr(split + 1)};
    const UILayoutRect top{.x = label.x, .y = label.y, .width = label.width, .height = label.height * 0.5f};
    const UILayoutRect bottom{.x = label.x, .y = label.y + label.height * 0.5f, .width = label.width, .height = label.height * 0.5f};
    const float lineScale = std::min(fit(first.c_str(), top), fit(second.c_str(), bottom));
    drew |= DrawUILabelText(font, first.c_str(), top, lineScale, widget.textAlignment, fontColour, renderTarget);
    drew |= DrawUILabelText(font, second.c_str(), bottom, lineScale, widget.textAlignment, fontColour, renderTarget);
    return drew;
}



bool UI_DrawWidgetObject(const UIDefinition &definition, const UIObjectDefinition &object, const UIPanelDefinition &panel, const UILayoutRect &panelRect,
    std::uint32_t colour, const char *text, std::uint32_t renderTarget, std::uint32_t textAlign = 0) {
    const float safeScale = 0.8f + std::clamp(g_currentSettings.display.uiDisplayArea, 0.0f, 1.0f) * 0.2f;
    const float toScreenX = static_cast<float>(g_uiScreenWidth) / 1920.0f;
    const float toScreenY = static_cast<float>(g_uiScreenHeight) / 1080.0f;
    const auto alpha = static_cast<std::uint32_t>(static_cast<float>(colour >> 24U) * object.alpha);
    if (!alpha)
        return false;
    colour = (colour & 0x00ffffffU) | (alpha << 24U);
    const UILayoutRect bounds = UIObjectLayoutRect(object, panel, panelRect, safeScale);
    if (object.kind == UIObjectKind::Label) {
        if (!text || g_uiLoadedLanguage < 0 || g_uiLoadedLanguage >= static_cast<std::int32_t>(g_subtitleFonts.size()))
            return false;



        const std::array<SubtitleFontLayer, 2> &fonts = g_subtitleFonts[static_cast<std::size_t>(g_uiLoadedLanguage)];
        const SubtitleFontLayer &font = fonts[(colour & 0x00ffffffU) == 0 ? 1 : 0];
        const float pixelSize = g_uiFontPixelSizes[static_cast<std::size_t>(g_uiLoadedLanguage)][static_cast<std::size_t>(object.fontFamily & 3)]
                                                  [static_cast<std::size_t>(std::clamp(object.fontSize, 0, 5))];
        const float lineHeight = pixelSize > 0.0f ? pixelSize : fonts[0].lineHeight;
        const float baseScale = fonts[0].lineHeight > 0.0f ? lineHeight / fonts[0].lineHeight : 1.0f;
        float fit = 1.0f;
        const float width = MeasureUILabelText(font, text, baseScale);
        if (width > bounds.width && width > 0.0f)
            fit = bounds.width / width;
        if (lineHeight > bounds.height && fit > bounds.height / lineHeight)
            fit *= bounds.height / lineHeight;
        fit = std::max(fit, 0.33f);


        const float area = g_currentSettings.display.uiDisplayArea;
        const std::int32_t screenHeight = static_cast<std::int32_t>(((1.0f - area) * 0.8f + area) * 1080.0f);
        const std::int32_t screenWidth = static_cast<std::int32_t>(1920.0f - (1080.0f - static_cast<float>(screenHeight)));
        if (object.scaleVar != -1 && definition.viewSlot != -1) {
            d3t::UIViewVariable &variable = g_uiViewSlots[static_cast<std::size_t>(definition.viewSlot)].variables[static_cast<std::size_t>(object.scaleVar)];
            if (screenWidth != object.scaleScreenWidth || screenHeight != object.scaleScreenHeight)
                variable.value = variable.initialValue;
            if (variable.value.floatValue > fit && object.scaleVarAssign == 1)
                variable.value.floatValue = fit;
            else
                fit = variable.value.floatValue;
        }
        object.scaleScreenWidth = screenWidth;
        object.scaleScreenHeight = screenHeight;
        const float scale = baseScale * fit;
        const std::uint32_t align = textAlign ? textAlign : object.textAlignment;
        return DrawUILabelText(font, text, bounds, scale, align, colour, renderTarget);
    }
    std::uint32_t texture = g_rendererWhiteTexture;
    if (object.kind != UIObjectKind::Button) {
        const UIImageDefinition *const image = definition.findImage(object.imageName);
        texture = image ? image->textureHandle : 0;
    }
    if (!texture)
        return false;
    const NuklearQuad quad{.x = bounds.x * toScreenX,
        .y = bounds.y * toScreenY,
        .width = bounds.width * toScreenX,
        .height = bounds.height * toScreenY,
        .u0 = 0.0f,
        .v0 = 0.0f,
        .u1 = 1.0f,
        .v1 = 1.0f,
        .colour = colour};
    const float clipX0 = panelRect.x * toScreenX;
    const float clipY0 = panelRect.y * toScreenY;
    const float clipX1 = (panelRect.x + panelRect.width) * toScreenX;
    const float clipY1 = (panelRect.y + panelRect.height) * toScreenY;
    const float x0 = std::max(quad.x, clipX0);
    const float y0 = std::max(quad.y, clipY0);
    const float x1 = std::min(quad.x + quad.width, clipX1);
    const float y1 = std::min(quad.y + quad.height, clipY1);
    if (x1 <= x0 || y1 <= y0 || quad.width <= 0.0f || quad.height <= 0.0f)
        return false;
    const NuklearQuad clipped{.x = x0,
        .y = y0,
        .width = x1 - x0,
        .height = y1 - y0,
        .u0 = (x0 - quad.x) / quad.width,
        .v0 = (y0 - quad.y) / quad.height,
        .u1 = (x1 - quad.x) / quad.width,
        .v1 = (y1 - quad.y) / quad.height,
        .colour = colour};
    return DrawNuklearQuads(g_RenderDeviceContext, renderTarget, texture, std::span{&clipped, 1});
}



bool UI_DrawWindowBackground(const UIDefinition &definition, const UIPanelDefinition &panel, float opacity, std::uint32_t renderTarget) {
    const auto alpha = static_cast<std::uint32_t>(static_cast<float>(panel.windowBackground >> 24U) * panel.opacity * opacity);
    if (!alpha || !g_rendererWhiteTexture)
        return false;
    const float toScreenX = static_cast<float>(g_uiScreenWidth) / 1920.0f;
    const float toScreenY = static_cast<float>(g_uiScreenHeight) / 1080.0f;
    const UILayoutRect rect = ResolveUIPanelRect(definition, panel, 0, nullptr);
    const NuklearQuad quad{.x = rect.x * toScreenX,
        .y = rect.y * toScreenY,
        .width = rect.width * toScreenX,
        .height = rect.height * toScreenY,
        .u0 = 0.0f,
        .v0 = 0.0f,
        .u1 = 1.0f,
        .v1 = 1.0f,
        .colour = (panel.windowBackground & 0x00ffffffU) | (alpha << 24U)};
    return DrawNuklearQuads(g_RenderDeviceContext, renderTarget, g_rendererWhiteTexture, std::span{&quad, 1});
}






bool DrawDefinitionWidgets(std::int32_t elementIndex, float opacity, std::uint32_t renderTarget) {
    const UIDefinition &definition = g_parsedUIDefinitions[static_cast<std::size_t>(elementIndex)];
    const float safeScale = 0.8f + std::clamp(g_currentSettings.display.uiDisplayArea, 0.0f, 1.0f) * 0.2f;
    bool drew = false;
    for (const UIPanelDefinition &panel : definition.panels)
        drew |= UI_DrawWindowBackground(definition, panel, opacity, renderTarget);
    for (const UIObjectDefinition &object : definition.objects) {
        if (object.kind != UIObjectKind::Image && object.kind != UIObjectKind::ColourImage)
            continue;
        const UIPanelDefinition *const panel = definition.findPanel(object.panelId);
        if (!panel || panel->opacity * opacity <= 0.0f)
            continue;
        const std::uint32_t alpha = static_cast<std::uint32_t>(static_cast<float>(object.colour >> 24U) * panel->opacity * opacity);
        const UILayoutRect panelRect = ResolveUIPanelRect(definition, *panel, 0, nullptr);
        drew |= UI_DrawWidgetObject(definition, object, *panel, panelRect, (object.colour & 0x00ffffffU) | (alpha << 24U), nullptr, renderTarget);
    }
    for (const UIObjectDefinition &widget : definition.buttonIcons) {
        const UIPanelDefinition *const panel = definition.findPanel(widget.panelId);
        if (!panel || panel->opacity * opacity * widget.alpha <= 0.0f)
            continue;
        const UILayoutRect bounds = UIObjectLayoutRect(widget, *panel, ResolveUIPanelRect(definition, *panel, 0, nullptr), safeScale);
        drew |= UI_DrawButtonIcon(definition, widget, bounds, widget.action, panel->opacity * opacity * widget.alpha, renderTarget);
    }
    return drew;
}


d3t::UILabelWidget *UI_LabelRecord(std::int32_t index) {
    return static_cast<std::uint32_t>(index) > 0x1ffU ? nullptr : &g_uiLabelWidgets[static_cast<std::size_t>(index)];
}


UIColourImageWidget *UI_ColourImageRecord(std::int32_t index) {
    return static_cast<std::uint32_t>(index) > 0xffU ? nullptr : &g_uiColourImageWidgets[static_cast<std::size_t>(index)];
}


UIButtonIconWidget *UI_ButtonIconRecord(std::int32_t index) {
    return static_cast<std::uint32_t>(index) > 0xffU ? nullptr : &g_uiButtonIconWidgets[static_cast<std::size_t>(index)];
}

} // namespace shenmue::s1::ui_runtime_detail
