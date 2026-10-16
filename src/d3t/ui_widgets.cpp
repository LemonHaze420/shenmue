#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1 && defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif


void UI_ResetLabelWidget(std::int32_t index) {
    d3t::UILabelWidget &label = g_uiLabelWidgets[static_cast<std::size_t>(index)];
    if (label.inUse == 1 && label.dataBinding != -1) {
        UIDataBinding &binding = g_uiDataBindings[static_cast<std::size_t>(label.dataBinding)];
        binding.inUse = 0;
        binding.method = 0;
        binding.variable = -1;
    }
    label.text = "";
    label.token.token = "";
    label.colour = 0xffffffffU;
    label.inUse = 0;
    label.field44 = 1;
    label.token.key[0] = 0;
    label.token.key[1] = 0;
    label.token.key[2] = 0;
    label.token.key[3] = 0;
    label.token.alreadyLocalized = 0;
    label.alignment = 0;
    label.dataBinding = -1;
    label.size = 4;
    label.window = -1;
}

// UI_ReadWidgetObject's Label branch (kind 15): Text, Alignment, Data, Size, FontName, Colour, Stroke, Token (its first 12 characters
// as its key), AlreadyLocalized, Id, ScaleVar (the first of the view's leading used variables with that exact name) and
// ScaleVarAssign into a label record of `window`.
std::int32_t UI_ReadLabelObject(cJSON *label, std::int32_t window) {
    const cJSON *const text = cJSON_GetObjectItem(label, "Text");
    const cJSON *const alignment = cJSON_GetObjectItem(label, "Alignment");
    std::int32_t dataBinding = -1;
    if (cJSON *const data = cJSON_GetObjectItem(label, "Data"))
        dataBinding = UI_ReadDataBinding(data);
    std::int32_t size = 4;
    const cJSON *const sizeMember = cJSON_GetObjectItem(label, "Size");
    constexpr std::array<const char *, 6> kSizeNames{"H1", "H2", "H3", "BL", "B", "BS"};
    for (std::size_t index = 0; sizeMember && sizeMember->valuestring && index < kSizeNames.size(); ++index) {
        if (!std::strcmp(sizeMember->valuestring, kSizeNames[index])) {
            size = static_cast<std::int32_t>(index);
            break;
        }
    }
    const cJSON *const fontName = cJSON_GetObjectItem(label, "FontName");
    const std::int32_t fontFamily = fontName ? UI_ReadFontName(fontName->valuestring) : 0;
    std::uint32_t colour = 0xffffffffU;
    UI_ReadColour(label, "Colour", colour);
    std::uint32_t strokeColour = 0xff000000U;
    const bool hasStroke = UI_ReadColour(label, "Stroke", strokeColour);
    d3t::UILabelToken token{};
    if (const cJSON *const tokenMember = cJSON_GetObjectItem(label, "Token")) {
        token.token = tokenMember->valuestring;
        for (std::size_t index = 0; token.token && index < token.key.size() && token.token[index]; ++index)
            token.key[index] = token.token[index];
    }
    if (const cJSON *const localized = cJSON_GetObjectItem(label, "AlreadyLocalized"))
        token.alreadyLocalized = localized->valueint;
    const cJSON *const id = cJSON_GetObjectItem(label, "Id");
    std::int32_t scaleVar = -1;
    if (const cJSON *const scale = cJSON_GetObjectItem(label, "ScaleVar")) {
        const std::array<d3t::UIViewVariable, d3t::kUIViewVariableCount> &variables = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)].variables;
        for (std::size_t index = 0; scale->valuestring && index < variables.size() && variables[index].used; ++index) {
            if (!std::strcmp(variables[index].name, scale->valuestring)) {
                scaleVar = static_cast<std::int32_t>(index);
                break;
            }
        }
    }
    const cJSON *const assign = cJSON_GetObjectItem(label, "ScaleVarAssign");
    return d3t::AllocLabelWidget(g_uiLabelWidgets, id ? id->valuestring : nullptr, text && text->valuestring ? text->valuestring : "", token,
        ReadAlignmentValue(alignment ? alignment->valuestring : nullptr), dataBinding, size, fontFamily, colour, hasStroke ? 1 : 0, strokeColour, window,
        scaleVar, assign ? assign->valueint : 1);
}



std::int32_t UI_LoadImageObject(cJSON *image, std::int32_t window) {
    std::int32_t index = -1;
    const char *id = nullptr;
    float angle = 0.0f;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    if (image->child) {
        if (const cJSON *const name = cJSON_GetObjectItem(image, "Image"))
            index = d3t::FindViewImage(slot, name->valuestring);
        if (const cJSON *const idMember = cJSON_GetObjectItem(image, "Id"))
            id = idMember->valuestring;
        if (const cJSON *const rotate = cJSON_GetObjectItem(image, "Rotate"); rotate && rotate->valuedouble != 0.0)
            angle = static_cast<float>(
                static_cast<double>(static_cast<float>(static_cast<double>(static_cast<float>(rotate->valuedouble)) / 180.0)) * 3.14159265358979);
    } else {
        index = d3t::FindViewImage(slot, image->valuestring);
    }
    if (index == -1)
        return -1;
    return d3t::AllocImageWidget(g_uiImageWidgets, id, index, window, angle);
}


std::uint32_t UI_ReadColour4(cJSON *colour) {
    if (!colour || cJSON_GetArraySize(colour) != 4)
        return 0xffffffffU;
    return ConstructColour(cJSON_GetArrayItem(colour, 0)->valueint, cJSON_GetArrayItem(colour, 1)->valueint, cJSON_GetArrayItem(colour, 2)->valueint,
        cJSON_GetArrayItem(colour, 3)->valueint);
}



std::int32_t UI_ReadButtonObject(cJSON *button, std::int32_t window) {
    std::size_t index = 0;
    while (index < g_uiButtonWidgets.size() && g_uiButtonWidgets[index].inUse)
        ++index;
    if (index == g_uiButtonWidgets.size())
        return -1;
    UIButtonWidget &widget = g_uiButtonWidgets[index];
    widget.inUse = 1;
    const cJSON *const id = cJSON_GetObjectItem(button, "Id");
    std::snprintf(widget.id.data(), widget.id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    widget.colour = UI_ReadColour4(cJSON_GetObjectItem(button, "Colour"));
    widget.rounding = 0.0f;
    if (const cJSON *const rounding = cJSON_GetObjectItem(button, "Rounding"))
        widget.rounding = static_cast<float>(rounding->valuedouble);
    widget.borderSize = 0.0f;
    if (const cJSON *const border = cJSON_GetObjectItem(button, "BorderSize"))
        widget.borderSize = static_cast<float>(border->valuedouble);
    cJSON *const borderColour = cJSON_GetObjectItem(button, "BorderColour");
    widget.borderColour = borderColour ? UI_ReadColour4(borderColour) : 0U;
    const cJSON *const alpha = cJSON_GetObjectItem(button, "Alpha");
    widget.alpha = alpha ? static_cast<float>(alpha->valuedouble) : 1.0f;
    widget.window = window;
    return static_cast<std::int32_t>(index);
}




std::int32_t UI_ReadColourImageObject(cJSON *image, std::int32_t window) {
    std::size_t index = 0;
    while (index < g_uiColourImageWidgets.size() && g_uiColourImageWidgets[index].inUse)
        ++index;
    if (index == g_uiColourImageWidgets.size())
        return -1;
    UIColourImageWidget &widget = g_uiColourImageWidgets[index];
    widget.inUse = 1;
    const cJSON *const name = cJSON_GetObjectItem(image, "Image");
    widget.image = d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], name ? name->valuestring : nullptr);
    widget.alpha = 1.0f;
    widget.colour = 0xffffffffU;
    if (cJSON *const colour = cJSON_GetObjectItem(image, "Colour")) {
        std::array<std::int32_t, 4> channels{};
        for (int channel = 0; channel < 4; ++channel) {
            const cJSON *const value = cJSON_GetArrayItem(colour, channel);
            channels[static_cast<std::size_t>(channel)] = value ? value->valueint : 0;
        }
        widget.colour = ConstructColour(channels[0], channels[1], channels[2], channels[3]);
    }
    if (const cJSON *const alpha = cJSON_GetObjectItem(image, "Alpha"))
        widget.alpha = static_cast<float>(alpha->valuedouble);
    if (const cJSON *const id = cJSON_GetObjectItem(image, "Id"); id && id->valuestring)
        std::snprintf(widget.id.data(), widget.id.size(), "%s", id->valuestring);
    if (const cJSON *const rotate = cJSON_GetObjectItem(image, "Rotate"); rotate && rotate->valuedouble != 0.0)
        widget.angle = static_cast<float>(
            static_cast<double>(static_cast<float>(static_cast<double>(static_cast<float>(rotate->valuedouble)) / 180.0)) * 3.14159265358979);
    widget.window = window;
    return static_cast<std::int32_t>(index);
}




std::int32_t UI_ReadScissorObject(cJSON *scissor) {
    std::size_t index = 0;
    while (index < g_uiScissorWidgets.size() && g_uiScissorWidgets[index].inUse)
        ++index;
    if (index == g_uiScissorWidgets.size())
        return -1;
    UIScissorWidget &widget = g_uiScissorWidgets[index];
    widget.inUse = 1;
    const cJSON *const x = cJSON_GetObjectItem(scissor, "X");
    const cJSON *const y = cJSON_GetObjectItem(scissor, "Y");
    const cJSON *const width = cJSON_GetObjectItem(scissor, "Width");
    const cJSON *const height = cJSON_GetObjectItem(scissor, "Height");
    widget.x = (x ? static_cast<float>(x->valuedouble) : 0.0f) / 1920.0f;
    widget.y = (y ? static_cast<float>(y->valuedouble) : 0.0f) / 1080.0f;
    widget.width = (width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0f;
    widget.height = (height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0f;
    if (const cJSON *const align = cJSON_GetObjectItem(scissor, "Align"))
        widget.align = d3t::ReadSpriteAlignmentValue(align);
    return static_cast<std::int32_t>(index);
}

// The ControlScheme reader: its name compared without case: MainMenu 0, GameMenu 1, Game 2, Fight 3, QTE 4, Vehicle 6, anything else -1;
// no string is MainMenu.
std::int32_t UI_ReadControlScheme(const char *name) {
    if (!name)
        return 0;
    constexpr std::array<const char *, 6> kSchemes{"MainMenu", "GameMenu", "Game", "Fight", "QTE", "Vehicle"};
    constexpr std::array<std::int32_t, 6> kSchemeValues{0, 1, 2, 3, 4, 6};
    for (std::size_t scheme = 0; scheme < kSchemes.size(); ++scheme) {
        std::size_t index = 0;
        while (name[index] && std::tolower(static_cast<unsigned char>(name[index])) == std::tolower(static_cast<unsigned char>(kSchemes[scheme][index])))
            ++index;
        if (!name[index] && !kSchemes[scheme][index])
            return kSchemeValues[scheme];
    }
    return -1;
}




std::int32_t UI_AllocIconLabelWidget(const char *id, const char *text, const d3t::UILabelToken &token, std::uint32_t alignment, std::int32_t dataBinding,
    std::int32_t size, std::int32_t fontFamily, const std::array<std::int32_t, 4> &colour, std::int32_t hasStroke, std::uint32_t strokeColour,
    std::int32_t controlScheme, std::int32_t window, std::int32_t scaleVar, std::int32_t scaleVarAssign) {
    std::size_t index = 0;
    while (index < g_uiIconLabelWidgets.size() && g_uiIconLabelWidgets[index].label.inUse)
        ++index;
    if (index == g_uiIconLabelWidgets.size())
        return -1;
    UIIconLabelWidget &widget = g_uiIconLabelWidgets[index];
    widget.label.inUse = 1;
    std::snprintf(widget.id.data(), widget.id.size(), "%s", id ? id : "");
    widget.label.dataBinding = dataBinding;
    widget.label.size = size;
    widget.label.fontFamily = fontFamily;
    widget.label.token = token;
    widget.label.text = text;
    widget.label.alignment = alignment;
    widget.label.colour = ConstructColour(colour[0], colour[1], colour[2], colour[3]);
    widget.controlScheme = controlScheme;
    widget.label.window = window;
    widget.label.hasStroke = hasStroke;
    widget.label.scaleVar = scaleVar;
    widget.label.strokeColour = strokeColour;
    widget.label.scaleVarAssign = scaleVarAssign;
    return static_cast<std::int32_t>(index);
}




std::int32_t UI_ReadIconLabelObject(cJSON *iconLabel, std::int32_t window) {
    const cJSON *const text = cJSON_GetObjectItem(iconLabel, "Text");
    const cJSON *const alignment = cJSON_GetObjectItem(iconLabel, "Alignment");
    std::int32_t dataBinding = -1;
    if (cJSON *const data = cJSON_GetObjectItem(iconLabel, "Data"))
        dataBinding = UI_ReadDataBinding(data);
    std::int32_t size = 4;
    const cJSON *const sizeMember = cJSON_GetObjectItem(iconLabel, "Size");
    for (std::size_t index = 0; sizeMember && sizeMember->valuestring && index < kIconFontSizeNames.size(); ++index) {
        if (kIconFontSizeNames[index] == sizeMember->valuestring) {
            size = static_cast<std::int32_t>(index);
            break;
        }
    }
    const cJSON *const fontName = cJSON_GetObjectItem(iconLabel, "FontName");
    const std::int32_t fontFamily = fontName ? UI_ReadFontName(fontName->valuestring) : 0;
    const cJSON *const scheme = cJSON_GetObjectItem(iconLabel, "ControlScheme");
    const std::int32_t controlScheme = scheme ? UI_ReadControlScheme(scheme->valuestring) : -1;
    std::array<std::int32_t, 4> colour{255, 255, 255, 255};
    if (cJSON *const colourMember = cJSON_GetObjectItem(iconLabel, "Colour")) {
        for (int channel = 0; channel < 4; ++channel) {
            const cJSON *const value = cJSON_GetArrayItem(colourMember, channel);
            colour[static_cast<std::size_t>(channel)] = value ? value->valueint : 0;
        }
    }
    d3t::UILabelToken token{};
    if (const cJSON *const tokenMember = cJSON_GetObjectItem(iconLabel, "Token")) {
        token.token = tokenMember->valuestring;
        for (std::size_t index = 0; token.token && index < token.key.size() && token.token[index]; ++index)
            token.key[index] = token.token[index];
    }
    if (const cJSON *const localized = cJSON_GetObjectItem(iconLabel, "AlreadyLocalized"))
        token.alreadyLocalized = localized->valueint;
    const cJSON *const id = cJSON_GetObjectItem(iconLabel, "Id");
    std::uint32_t strokeColour = 0xff000000U;
    const bool hasStroke = UI_ReadColour(iconLabel, "Stroke", strokeColour);
    std::int32_t scaleVar = -1;
    if (const cJSON *const scale = cJSON_GetObjectItem(iconLabel, "ScaleVar")) {
        const std::array<d3t::UIViewVariable, d3t::kUIViewVariableCount> &variables = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)].variables;
        for (std::size_t index = 0; scale->valuestring && index < variables.size() && variables[index].used; ++index) {
            if (!std::strcmp(variables[index].name, scale->valuestring)) {
                scaleVar = static_cast<std::int32_t>(index);
                break;
            }
        }
    }
    const cJSON *const assign = cJSON_GetObjectItem(iconLabel, "ScaleVarAssign");
    return UI_AllocIconLabelWidget(id ? id->valuestring : nullptr, text && text->valuestring ? text->valuestring : "", token,
        ReadAlignmentValue(alignment ? alignment->valuestring : nullptr), dataBinding, size, fontFamily, colour, hasStroke ? 1 : 0, strokeColour, controlScheme,
        window, scaleVar, assign ? assign->valueint : 1);
}







std::int32_t UI_ReadButtonIconObject(cJSON *object, std::int32_t window) {
    std::size_t index = 0;
    while (index < g_uiButtonIconWidgets.size() && g_uiButtonIconWidgets[index].inUse)
        ++index;
    if (index == g_uiButtonIconWidgets.size())
        return -1;
    UIButtonIconWidget &widget = g_uiButtonIconWidgets[index];
    widget.inUse = 1;
    widget.field88 = 1;
    widget.fontColour = 0xffffffffU;
    const cJSON *const image = cJSON_GetObjectItem(object, "Image");
    std::snprintf(widget.image.data(), widget.image.size(), "%s", image && image->valuestring ? image->valuestring : "");
    widget.iconFontSize = 5;
    if (const cJSON *const size = cJSON_GetObjectItem(object, "IconFontSize")) {
        widget.iconFontSize = 4;
        for (std::size_t name = 0; size->valuestring && name < kIconFontSizeNames.size(); ++name) {
            if (kIconFontSizeNames[name] == size->valuestring) {
                widget.iconFontSize = static_cast<std::int32_t>(name);
                break;
            }
        }
    }
    const cJSON *const id = cJSON_GetObjectItem(object, "Id");
    std::snprintf(widget.id.data(), widget.id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    if (!std::strcmp(object->string, "ButtonIcon")) {
        widget.controlMethod = -1;
        widget.controlState = -1;
        widget.sheet = -1;
        widget.action = 0;
        if (const cJSON *const action = cJSON_GetObjectItem(object, "Action"); action && action->valuestring) {
            for (std::size_t name = 0; name < kInputActionNames.size(); ++name) {
                if (!std::strncmp(action->valuestring, std::string_view(kInputActionNames[name]).data(), 64)) {
                    widget.action = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        widget.sprite = 0;
        UI_ReadColour(object, "FontColour", widget.fontColour);
        const cJSON *const alignment = cJSON_GetObjectItem(object, "Alignment");
        widget.alignment = ReadAlignmentValue(alignment ? alignment->valuestring : nullptr);
        const cJSON *const doubleWidth = cJSON_GetObjectItem(object, "AllowDoubleWidth");
        widget.allowDoubleWidth = doubleWidth ? doubleWidth->valueint : 1;
        if (const cJSON *const method = cJSON_GetObjectItem(object, "ControlMethod"))
            widget.controlMethod = method->valueint;
        if (const cJSON *const state = cJSON_GetObjectItem(object, "ControlState"))
            widget.controlState = state->valueint;
    } else {
        widget.sheet = -1;
        for (std::size_t sheet = 0; image && image->valuestring && sheet < g_uiSpriteSheets.size(); ++sheet) {
            const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
            if (candidate.imageName && candidate.used && !std::strcmp(candidate.imageName, image->valuestring)) {
                widget.sheet = static_cast<std::int32_t>(sheet);
                break;
            }
        }
        widget.action = -1;
        const cJSON *const sprite = cJSON_GetObjectItem(object, "Index");
        widget.sprite = sprite ? sprite->valueint : 0;
    }
    if (const cJSON *const rotate = cJSON_GetObjectItem(object, "Rotate"); rotate && rotate->valuedouble != 0.0)
        widget.angle = static_cast<float>(
            static_cast<double>(static_cast<float>(static_cast<double>(static_cast<float>(rotate->valuedouble)) / 180.0)) * 3.14159265358979);
    widget.fieldAC = 0;
    widget.colour = 0xffffffffU;
    if (cJSON *const colour = cJSON_GetObjectItem(object, "Colour")) {
        std::array<std::int32_t, 4> channels{};
        for (int channel = 0; channel < 4; ++channel) {
            const cJSON *const value = cJSON_GetArrayItem(colour, channel);
            channels[static_cast<std::size_t>(channel)] = value ? value->valueint : 0;
        }
        widget.colour = ConstructColour(channels[0], channels[1], channels[2], channels[3]);
    }
    widget.window = window;
    return static_cast<std::int32_t>(index);
}



struct nk_image UI_GetSpriteImage(std::uint32_t sheet, std::int32_t frame, const d3t::UIViewImage *image) {
    struct nk_image sprite {};
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || sheet > 55U)
        return sprite;
    const d3t::UISpriteSheet &sprites = g_uiSpriteSheets[sheet];
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    if (!image && static_cast<std::size_t>(sprites.image) < view.images.size())
        image = &view.images[static_cast<std::size_t>(sprites.image)];
    if (image)
        std::memcpy(&sprite, &image->textureHandle, sizeof(sprite));
    if (frame < 64) {
        const std::array<float, 2> &min = sprites.frameMin[static_cast<std::size_t>(frame)];
        const std::array<float, 2> &max = sprites.frameMax[static_cast<std::size_t>(frame)];
        sprite.region[0] = static_cast<unsigned short>(static_cast<std::int32_t>(min[0]));
        sprite.region[1] = static_cast<unsigned short>(static_cast<std::int32_t>(min[1]));
        sprite.region[2] = static_cast<unsigned short>(static_cast<std::int32_t>(max[0] - min[0]));
        sprite.region[3] = static_cast<unsigned short>(static_cast<std::int32_t>(max[1] - min[1]));
    } else {
        sprite.region[0] = sprite.region[1] = sprite.region[2] = sprite.region[3] = 0;
    }
    return sprite;
}









void UI_DrawButtonIconLabel(nk_context &context, const UIButtonIconLabel &label) {
    const std::int32_t glyph = ResolveButtonIconGlyph(label.action, label.controlMethod, label.controlState, label.doubleWidth);
    if (static_cast<std::uint32_t>(glyph - 41) <= 2U)
        return;
    const nk_color savedColour = context.style.text.color;
    std::memcpy(&context.style.text.color, &label.colour, sizeof(label.colour));
    const std::uint32_t savedSize = static_cast<std::uint32_t>(g_uiFontSize);
    const std::uint32_t savedFace = static_cast<std::uint32_t>(g_uiFontFace);
    if (g_uiFontFace) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    if (g_uiFontSize != label.iconFontSize && static_cast<std::uint32_t>(label.iconFontSize) < 6U) {
        g_uiFontSize = label.iconFontSize;
        UI_SelectFont();
    }
    const auto drawText = [&context](const char *text, nk_flags alignment, const UILayoutRect &rect, float fontWidth, float fontHeight) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
        const UILabelFit fit = UI_FitLabelScale(context, text, -1, alignment, rect, fontWidth, fontHeight, 1);
        UI_DrawLabelText(context, text, -1, alignment, rect, fit);
    };
    const UILayoutRect &rect = label.rect;
    if (static_cast<std::uint32_t>(glyph - 4) <= 6U) {
        context.style.text.color = nk_color{.r = 255, .g = 255, .b = 255, .a = static_cast<nk_byte>(label.colour >> 24)};
        switch (label.action) {
        case 10:
        case 11:
        case 12:
        case 13:
#if SHENMUE_IS_S1
        case 56:
        case 57:
        case 58:
#else
        case 57:
        case 58:
        case 59:
#endif
            drawText("R", label.alignment, rect, label.fontWidth, label.fontHeight);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
#if SHENMUE_IS_S1
        case 59:
        case 60:
        case 61:
#else
        case 60:
        case 61:
        case 62:
        case 67:
        case 68:
#endif
            drawText("L", label.alignment, rect, label.fontWidth, label.fontHeight);
            break;
        default:
            break;
        }
    }
    std::array<char, 64> name{};
    if (static_cast<std::uint32_t>(glyph - 26) > 2U) {
        if (glyph == 29 || glyph == 55) {
            if (FormatVirtualKeyString(13, name))
                drawText(name.data(), label.alignment, rect, label.fontWidth, label.fontHeight * 0.5f);
        } else if (static_cast<std::uint32_t>(glyph - 30) <= 5U) {
#if SHENMUE_IS_S1
            constexpr std::int32_t up = 'W';
            constexpr std::int32_t down = 'S';
            constexpr std::int32_t leftKey = 'A';
            constexpr std::int32_t rightKey = 'D';
#else
            const std::int32_t up = static_cast<std::int32_t>(GetMenuKeyMapping(14));
            const std::int32_t down = static_cast<std::int32_t>(GetMenuKeyMapping(15));
            const std::int32_t leftKey = static_cast<std::int32_t>(GetMenuKeyMapping(16));
            const std::int32_t rightKey = static_cast<std::int32_t>(GetMenuKeyMapping(17));
#endif
            const float side = rect.height * 0.6f;
            const float left = rect.width * 0.5f + rect.x - side * 0.5f;
            const float letterWidth = label.fontWidth * 0.6f;
            const float letterHeight = label.fontHeight * 0.6f;
            if (static_cast<std::uint32_t>(glyph - 33) <= 1U) {
                if (FormatVirtualKeyString(up, name)) {
                    const float x = glyph == 34 ? left - rect.width * 0.15f : left - rect.width * 0.08f;
                    drawText(name.data(), 18, {.x = x, .y = rect.y - side * 0.20000002f, .width = side, .height = side}, letterWidth, letterHeight);
                }
                if (FormatVirtualKeyString(down, name))
                    drawText(
                        name.data(), 18, {.x = left, .y = rect.height * 0.5f + rect.y - side * 0.0f, .width = side, .height = side}, letterWidth, letterHeight);
            }
            if (((glyph - 33) & ~2) == 0) {
                const float middle = rect.height * 0.5f + rect.y;
                const float y = glyph == 35 ? middle - side * 0.5f : middle - side * 0.0f;
                if (FormatVirtualKeyString(leftKey, name))
                    drawText(name.data(), 18, {.x = left - rect.width * 0.3f, .y = y, .width = side, .height = side}, letterWidth, letterHeight);
                if (FormatVirtualKeyString(rightKey, name))
                    drawText(name.data(), 18, {.x = left + rect.width * 0.3f, .y = y, .width = side, .height = side}, letterWidth, letterHeight);
            }
        }
    } else {
        std::int32_t key = 0;
#if SHENMUE_IS_S1
        switch (label.action) {
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
        case 64:
        case 65: {
            const std::int32_t type = label.controlState == -1 ? static_cast<std::int32_t>(GetCurrentInputType()) : label.controlState;
            const std::uint32_t mapping = static_cast<std::uint32_t>(label.action - 64);
            std::int32_t state = mapping ? 27 : 13;
            if (static_cast<std::uint32_t>(type) > static_cast<std::uint32_t>(InputMode::GameUi)) {
                switch (static_cast<InputMode>(type)) {
                case InputMode::Qte:
                case InputMode::Qte2:
                    state = UserMapping(18 + mapping);
                    break;
                case InputMode::Fight:
                    state = UserMapping(22 + mapping);
                    break;
                case InputMode::Vehicle:
                    state = UserMapping(32 + mapping);
                    break;
                default:
                    state = UserMapping(mapping);
                    break;
                }
            }
#if defined(_WIN32)
            key =
                static_cast<std::int32_t>(ExtendedMapVirtualKey(static_cast<std::uint32_t>(state), reinterpret_cast<void *>(0x08090809), GetKeyboardLayout(0)));
#else
            key = state;
#endif
            break;
        }
        default:
            key = label.action < 50 ? GetKeyForInputType(label.action, label.controlState) : 0;
            break;
        }
#else
        if (label.action < 50) {
            key = GetKeyForInputType(label.action, label.controlState);
        } else {
            switch (label.action) {
            case 50:
                key = static_cast<std::int32_t>(GetMenuKeyMapping(8));
                break;
            case 51:
                key = static_cast<std::int32_t>(GetMenuKeyMapping(9));
                break;
            case 52:
                key = static_cast<std::int32_t>(GetMenuKeyMapping(4));
                break;
            case 53:
                key = static_cast<std::int32_t>(GetMenuKeyMapping(5));
                break;
            case 54:
                key = static_cast<std::int32_t>(GetGameKeyMapping(16));
                break;
            case 55:
                key = static_cast<std::int32_t>(GetGameKeyMapping(17));
                break;
            case 56:
                key = static_cast<std::int32_t>(GetGameKeyMapping(8));
                break;
            case 65:
            case 66: {
                const std::int32_t mapping = label.action - 65;
                const std::int32_t type = label.controlState == -1 ? GetCurrentUIInputType() : label.controlState;
                const bool fightQte = type == static_cast<std::int32_t>(InputMode::Qte2) && IsGameUiSecondaryFlagSet(2) && GetBattleQteUIMode() != 2;
                std::uint32_t mapped = 0;
                if (type <= static_cast<std::int32_t>(InputMode::GameUi))
                    mapped = GetMenuKeyMapping(mapping);
                else if (fightQte || type == static_cast<std::int32_t>(InputMode::Fight))
                    mapped = GetFightKeyMapping(mapping);
                else if (type == static_cast<std::int32_t>(InputMode::Qte2) || type == static_cast<std::int32_t>(InputMode::Qte))
                    mapped = GetQteKeyMapping(mapping);
                else
                    mapped = GetGameKeyMapping(mapping);
                key = static_cast<std::int32_t>(MapInputKeyToCurrentLayout(mapped));
                break;
            }
            default:
                break;
            }
        }
#endif
        if (FormatVirtualKeyString(key, name))
            drawText(name.data(), label.alignment, rect, label.fontWidth, label.fontHeight);
    }
    context.style.text.color = savedColour;
    if (static_cast<std::uint32_t>(g_uiFontSize) != savedSize && savedSize < 6U) {
        g_uiFontSize = static_cast<std::int32_t>(savedSize);
        UI_SelectFont();
    }
    if (static_cast<std::uint32_t>(g_uiFontFace) != savedFace && savedFace < 4U) {
        g_uiFontFace = static_cast<std::int32_t>(savedFace);
        UI_SelectFont();
    }
}



float ScaleTo1080WithSafeArea(const struct nk_rect &rect, float length) {
    if (length == 0.0f)
        return 0.0f;
    return std::fminf(rect.h * 0.5f - 1.0f, std::fminf(rect.w * 0.5f - 1.0f, WithinSafeArea(length)));
}





void UI_DrawButton(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) >= g_uiButtonWidgets.size())
        return;
    const UIButtonWidget &widget = g_uiButtonWidgets[static_cast<std::size_t>(index)];
    const d3t::UIWindow *const window = UI_GetWindow(widget.window);
    context.style.button.rounding = context.current ? ScaleTo1080WithSafeArea(context.current->layout->row.item, widget.rounding) : 0.0f;
    context.style.button.border = widget.borderSize;
    std::memcpy(&context.style.button.border_color, &widget.borderColour, sizeof(widget.borderColour));
    const float alpha = static_cast<float>(widget.colour >> 24) * widget.alpha * (window ? window->alpha : 1.0f);
    if (g_waitingForNextTick != 1)
        return;
    const std::uint32_t colour = (widget.colour & 0x00ffffffU) | (static_cast<std::uint32_t>(static_cast<std::int32_t>(alpha)) & 0xffU) << 24;
    nk_color tint{};
    std::memcpy(&tint, &colour, sizeof(colour));
    nk_button_color(&context, tint);
}




void UI_DrawImage(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || static_cast<std::uint32_t>(index) >= g_uiImageWidgets.size())
        return;
    const d3t::UIImageWidget &widget = g_uiImageWidgets[static_cast<std::size_t>(index)];
    std::int32_t slot = g_uiDrawViewSlot;
    if (widget.frame != -1) {
        if (static_cast<std::uint32_t>(widget.frame) > 0x4bU)
            return;
        slot = widget.frame;
    }
    d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(slot)];
    if (widget.image == -1 || !widget.field44 || static_cast<std::size_t>(widget.image) >= view.images.size())
        return;
    d3t::UIViewImage &image = view.images[static_cast<std::size_t>(widget.image)];
    if (widget.angle != 0.0f) {
        image.rotate = 1;
        image.angle = widget.angle;
    }
    const d3t::UIWindow *const window = UI_GetWindow(widget.window);
    if (!window || g_waitingForNextTick != 1)
        return;
    const std::uint32_t colour = UI_TintByWindow(*window, image.colour);
    nk_color tint{};
    std::memcpy(&tint, &colour, sizeof(colour));
    struct nk_image drawn {};
    std::memcpy(&drawn, &image.textureHandle, sizeof(drawn));
    nk_button_image_color(&context, drawn, tint);
}




void UI_DrawScissor(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 9U)
        return;
    const UIScissorWidget &widget = g_uiScissorWidgets[static_cast<std::size_t>(index)];
    float origin = 1920.0f;
    const float height = widget.height * 1080.0f;
    const float y = widget.y * 1080.0f;
    const float width = widget.width * 1920.0f;
    if (widget.align == 1) {
        if (g_bIs43AspectRatio)
            origin = 1439.9965f;
        origin = origin * 0.5f;
    } else if (widget.align == 8) {
        if (g_bIs43AspectRatio)
            origin = 1439.9965f;
    } else {
        origin = 0.0f;
    }
    const float x = widget.x * 1920.0f + origin;
    if (g_waitingForNextTick == 1 && context.current)
        nk_push_scissor(&context.current->buffer, nk_rect(x, y, width, height));
}








void UI_DrawIconLabel(nk_context &context, std::int32_t index, std::int32_t widget) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || static_cast<std::uint32_t>(index) > 0x3fU)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    UIIconLabelWidget &iconLabel = g_uiIconLabelWidgets[static_cast<std::size_t>(index)];
    d3t::UILabelWidget &label = iconLabel.label;
    d3t::UIWindow *const window = UI_GetWindow(label.window);
    if (!window)
        return;
    if (g_uiFontFace != label.fontFamily && static_cast<std::uint32_t>(label.fontFamily) < 4U) {
        g_uiFontFace = label.fontFamily;
        UI_SelectFont();
    }
    if (g_uiFontSize != label.size && static_cast<std::uint32_t>(label.size) < 6U) {
        g_uiFontSize = label.size;
        UI_SelectFont();
    }
    UI_ResolveLabelText(label);
    if (!label.text)
        return;
    const std::int32_t itemIndex = widget > 0 ? view.widgetPoolIndices[static_cast<std::size_t>(widget - 1)] : view.widgetKinds.back();
    if (static_cast<std::uint32_t>(itemIndex) > 0x3ffU)
        return;
    const d3t::UILayoutItem &item = g_uiLayoutItems[static_cast<std::size_t>(itemIndex)];
    d3t::UIWindow *const itemWindow = UI_GetWindow(item.window);
    if (!itemWindow)
        return;
    UILayoutRect windowRect{};
    UI_GetWindowRect(itemWindow, windowRect);
    const UILayoutRect rect = UI_SpacePushRect(item, itemWindow);
    const float fitWidth = itemWindow->layout.width.value.fraction * item.width * 1920.0f;
    const float fitHeight = itemWindow->layout.height.value.fraction * item.height * 1080.0f;
    const std::int32_t length = static_cast<std::int32_t>(std::strlen(label.text));
    if (length <= 0)
        return;
    struct Piece {
        const char *text;
        std::int32_t length;
        std::int32_t icon;
    };
    std::vector<Piece> pieces{{.text = label.text, .length = 0, .icon = 0}};
    std::string cut;
    for (std::int32_t at = 0; at < length; ++at) {
        const char c = label.text[at];
        if (!pieces.back().icon) {
            cut.push_back(c);
            if (c == '<')
                pieces.push_back({.text = label.text + at + 1, .length = -1, .icon = 1});
        } else if (c == '>') {
            cut.push_back('>');
            pieces.push_back({.text = label.text + at + 1, .length = -1, .icon = 0});
        }
        ++pieces.back().length;
    }
    const UILabelFit fit = UI_FitLabelScale(context, cut.c_str(), static_cast<std::int32_t>(cut.size()), label.alignment, rect, fitWidth, fitHeight, 1);
    const std::int32_t drawing = g_waitingForNextTick;
    g_waitingForNextTick = 0;
    UILayoutRect at = UI_DrawLabelText(context, cut.c_str(), static_cast<std::int32_t>(cut.size()), label.alignment, rect, fit);
    g_waitingForNextTick = drawing;
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(channels));
    const std::uint32_t windowColour =
        ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
    for (const Piece &piece : pieces) {
        if (piece.length <= 0)
            continue;
        if (piece.icon) {
            if (piece.length >= 20)
                continue;
            std::array<char, 20> name{};
            std::memcpy(name.data(), piece.text, static_cast<std::size_t>(piece.length));
            std::int32_t action = 0;
            for (std::size_t candidate = 0; candidate < kInputActionNames.size(); ++candidate) {
                if (!std::strncmp(name.data(), std::string_view(kInputActionNames[candidate]).data(), 64)) {
                    action = static_cast<std::int32_t>(candidate);
                    break;
                }
            }
            const d3t::UIViewImage *const image = g_platformButtonIconImages[0];
            const std::int32_t glyph = ResolveButtonIconGlyph(action, -1, iconLabel.controlScheme, 1);
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(g_platformIconsSheet), glyph, image);
            const std::uint32_t aspect = sprite.region[3] ? static_cast<std::uint32_t>(sprite.region[2]) / sprite.region[3] : 0U;
            const float iconWidth = at.height * static_cast<float>(aspect);
            const UILayoutRect cell{.x = at.x, .y = at.y, .width = iconWidth, .height = at.height};
            if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(cell.x, cell.y, cell.width, cell.height);
            nk_color tint{};
            std::memcpy(&tint, &windowColour, sizeof(windowColour));
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, tint);
            const UIButtonIconLabel keyLabel{.rect = cell,
                .fontWidth = iconWidth,
                .fontHeight = at.height,
                .action = action,
                .doubleWidth = 1,
                .iconFontSize = 5,
                .colour = 0xff000000U,
                .alignment = 18,
                .controlMethod = -1,
                .controlState = iconLabel.controlScheme};
            UI_DrawButtonIconLabel(context, keyLabel);
            at.x = at.x + iconWidth;
            at.width = at.width - iconWidth;
            continue;
        }
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(at.x, at.y, at.width, at.height);
        if (label.hasStroke == 1) {
            if (g_uiFontFace != 1) {
                g_uiFontFace = 1;
                UI_SelectFont();
            }
            const std::uint32_t stroke = UI_TintByWindow(*window, label.strokeColour);
            std::memcpy(&context.style.text.color, &stroke, sizeof(stroke));
            UI_DrawLabelText(context, piece.text, piece.length, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_TOP, at, fit);
            if (g_uiFontFace != 0) {
                g_uiFontFace = 0;
                UI_SelectFont();
            }
        }
        const std::uint32_t colour = UI_TintByWindow(*window, label.colour);
        std::memcpy(&context.style.text.color, &colour, sizeof(colour));
        const UILayoutRect drawn = UI_DrawLabelText(context, piece.text, piece.length, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_TOP, at, fit);
        at.x = at.x + drawn.width;
        at.width = at.width - drawn.width;
    }
}




void UI_DrawColourImage(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 0xffU)
        return;
    const UIColourImageWidget &widget = g_uiColourImageWidgets[static_cast<std::size_t>(index)];
    const d3t::UIWindow *const window = UI_GetWindow(widget.window);
    d3t::UIViewSlot<kUIViewImageCapacity> *const view =
        static_cast<std::uint32_t>(g_uiDrawViewSlot) <= 0x4bU ? &g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)] : nullptr;
    d3t::UIViewImage *const image =
        view && static_cast<std::size_t>(widget.image) < view->images.size() ? &view->images[static_cast<std::size_t>(widget.image)] : nullptr;
    if (widget.angle != 0.0f && image) {
        image->rotate = 1;
        image->angle = widget.angle;
    }
    if (!window || !view)
        return;
    std::uint32_t colour = UI_TintByWindow(*window, widget.colour);
    const std::uint32_t alpha = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(colour >> 24) * widget.alpha)) & 0xffU;
    colour = (colour & 0x00ffffffU) | alpha << 24;
    nk_color tint{};
    std::memcpy(&tint, &colour, sizeof(colour));
    if (g_waitingForNextTick != 1)
        return;
    struct nk_image drawn = g_uiDefaultImage;
    if (image && image->ready == 1)
        std::memcpy(&drawn, &image->textureHandle, sizeof(drawn));
    nk_image_color(&context, drawn, tint);
}







void UI_DrawSprite(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 0xffU || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 75U)
        return;
    const UIButtonIconWidget &widget = g_uiButtonIconWidgets[static_cast<std::size_t>(index)];
    if (!context.current || widget.sprite == -1)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    const d3t::UIViewImage *image = nullptr;
    struct nk_image sprite {};
    if (widget.inventoryIcon) {
        const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiSharedItems].config.uiHandleIndex);
        const std::size_t sheet = static_cast<std::uint8_t>(widget.sprite >> 8);
        const std::int32_t found = sheet < g_inventoryIconSheetImageIndices.size() ? g_inventoryIconSheetImageIndices[sheet] : -1;
        if (slot <= 75U && static_cast<std::uint32_t>(found) < kUIViewImageCapacity)
            image = &g_uiViewSlots[slot].images[static_cast<std::size_t>(found)];
        RenderingRelatedInventory(sprite, static_cast<std::uint16_t>(widget.sprite), 0);
    } else if (static_cast<std::uint32_t>(widget.sheet) <= 0x37U) {
        const d3t::UISpriteSheet &sheet = g_uiSpriteSheets[static_cast<std::size_t>(widget.sheet)];
        const std::int32_t found = sheet.imageName ? UI_FindViewImage(static_cast<std::uint32_t>(g_uiDrawViewSlot), sheet.imageName) : sheet.image;
        if (static_cast<std::size_t>(found) < view.images.size())
            image = &view.images[static_cast<std::size_t>(found)];
        if (image)
            sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(widget.sheet), widget.sprite, image);
    }
    if (widget.fieldAC)
        return;
    const d3t::UIWindow *const window = UI_GetWindow(widget.window);
    if (!window)
        return;
    const std::uint32_t colour = UI_TintByWindow(*window, widget.colour);
    nk_color tint{};
    std::memcpy(&tint, &colour, sizeof(colour));
    sprite.rotate = widget.angle != 0.0f;
    sprite.angle = widget.angle;
    if (g_waitingForNextTick == 1)
        nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, tint);
}







void UI_DrawButtonIcon(nk_context &context, UIButtonIconWidget &widget, const UILayoutRect *rect) {
    if (static_cast<std::uint32_t>(widget.window) > 0x3ffU)
        return;
    d3t::UIWindow &window = g_uiWindows[static_cast<std::size_t>(widget.window)];
    widget.sprite = ResolveButtonIconGlyph(widget.action, widget.controlMethod, widget.controlState, widget.allowDoubleWidth);
    if (widget.sprite <= -1) {
        const std::uint32_t colour = UI_TintByWindow(window, widget.colour);
        nk_color tint{};
        std::memcpy(&tint, &colour, sizeof(colour));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, g_uiDefaultImage, tint);
        return;
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiDrawViewSlot);
    const std::int32_t found = UI_FindViewImage(slot, widget.image.data());
    const d3t::UIViewImage *image = found == -1 || slot > 0x4bU ? nullptr : &g_uiViewSlots[slot].images[static_cast<std::size_t>(found)];
    if (!image)
        image = widget.field88 == 1 ? g_platformButtonIconImages[0] : g_platformButtonIconImages[2];
    if (widget.sheet == -1)
        widget.sheet = static_cast<std::int32_t>(g_platformIconsSheet);
    const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(widget.sheet), widget.sprite, image);
    UILayoutRect windowRect{};
    std::memcpy(&windowRect, &sprite, sizeof(windowRect));
    UI_GetWindowRect(&window, windowRect);
    UILayoutRect bounds{};
    if (rect) {
        bounds = *rect;
    } else if (context.current) {
        nk_panel &layout = *context.current->layout;
        const float left = layout.row.item.x + layout.at_x;
        if (layout.row.item.w + left > layout.max_x)
            layout.max_x = layout.row.item.w + left;
        bounds = {.x = left - static_cast<float>(layout.offset->x) - windowRect.x,
            .y = layout.row.item.y + layout.at_y - static_cast<float>(layout.offset->y) - windowRect.y,
            .width = layout.row.item.w,
            .height = layout.row.item.h};
    } else {
        bounds = {.x = 0.0f - windowRect.x, .y = 0.0f - windowRect.y, .width = 0.0f, .height = 0.0f};
    }
    const float labelWidth = bounds.width * 0.75f;
    const float labelHeight = bounds.height * 0.75f;
    UIButtonIconLabel label{.rect = {.x = bounds.x + (bounds.width - labelWidth) * 0.5f,
                                .y = bounds.y + (bounds.height - labelHeight) * 0.5f,
                                .width = labelWidth,
                                .height = labelHeight},
        .fontWidth = labelWidth,
        .fontHeight = labelHeight,
        .action = widget.action,
        .doubleWidth = widget.allowDoubleWidth,
        .iconFontSize = widget.iconFontSize,
        .colour = widget.fontColour,
        .alignment = widget.alignment,
        .controlMethod = widget.controlMethod,
        .controlState = widget.controlState};
    if (!widget.fieldAC) {
        const std::uint32_t colour = UI_TintByWindow(window, widget.colour);
        nk_color tint{};
        std::memcpy(&tint, &colour, sizeof(colour));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, tint);
        label.colour = UI_TintByWindow(window, widget.fontColour);
    }
    UI_DrawButtonIconLabel(context, label);
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
