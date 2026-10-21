#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

#if !SHENMUE_IS_S1

std::uint32_t ConstructColour(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha) {
    const auto clamp = [](std::int32_t component) { return static_cast<std::uint32_t>(component < 0 ? 0 : component > 255 ? 255 : component); };
    return clamp(red) | clamp(green) << 8 | clamp(blue) << 16 | clamp(alpha) << 24;
}


std::uint32_t ReadAlignmentValue(const char *alignment) {
    if (!alignment)
        return 18;
    if (!std::strcmp(alignment, "Centre"))
        return 18;
    if (!std::strcmp(alignment, "Left"))
        return 17;
    if (!std::strcmp(alignment, "Right"))
        return 20;
    if (!std::strcmp(alignment, "LeftTop"))
        return 9;
    if (!std::strcmp(alignment, "LeftDown"))
        return 33;
    if (!std::strcmp(alignment, "RightTop"))
        return 12;
    if (!std::strcmp(alignment, "RightDown"))
        return 36;
    if (!std::strcmp(alignment, "CentreUp"))
        return 10;
    if (!std::strcmp(alignment, "CentreDown"))
        return 34;
    return 18;
}

#endif



std::int32_t d3t_GetAspectRatioMode() {
    return g_bIs43AspectRatio;
}



void ResetUIWindow(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiWindows.size()))
        return;
    d3t::UIWindow &window = g_uiWindows[static_cast<std::size_t>(index)];
    window.inUse = 0;
    window.id[0] = 0;
    window.layout.width.fromImage = 0;
    window.layout.width.value.image = 0;
    window.layout.height.value.image = 0;
    window.layout.alignX = 0;
    window.layout.alignY = 0;
    window.layout.offsetX = 0.0f;
    window.layout.offsetY = 0.0f;
    window.layout.width.scale = 1.0f;
    window.layout.height.fromImage = 0;
    window.layout.height.scale = 1.0f;
    window.flags = 0;
    window.colour = 0xffffffffU;
    window.alpha = 1.0f;
    window.initialLayout = window.layout;
}



d3t::UIWindowLength UI_ReadWindowLength(const cJSON *length, cJSON *object, const char *imageKey, float reference) {
    d3t::UIWindowLength result{.fromImage = 0, .value = {.fraction = 0.0f}, .scale = 1.0f};
    if (length && length->type == cJSON_String) {
        result.fromImage = 1;
        result.value.image = 0;
        const cJSON *const image = cJSON_GetObjectItem(object, imageKey);
        const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
        for (std::size_t index = 0; image && image->valuestring && index < slot.images.size(); ++index) {
            if (!std::strcmp(slot.images[index].name, image->valuestring)) {
                result.value.image = static_cast<std::int32_t>(index);
                break;
            }
        }
    } else if (length) {
        result.value.fraction = static_cast<float>(length->valuedouble) / reference;
    }
    return result;
}





std::int32_t UI_ReadWindow(cJSON *object) {
    const char *parentId = "";
    const char *id = nullptr;
    d3t::UIWindowLength width{.fromImage = 0, .value = {.fraction = 0.0f}, .scale = 0.0f};
    d3t::UIWindowLength height = width;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float paddingX = 0.0f;
    float paddingY = 0.0f;
    float alpha = 1.0f;
    std::int32_t alignX = 0;
    std::int32_t alignY = 0;
    std::int32_t scaleStyle = 4;
    std::int32_t ignoreSafeArea = 0;
    std::int32_t flags = 0;
    if (object) {
        const cJSON *const parent = cJSON_GetObjectItem(object, "ParentID");
        if (parent && parent->valuestring && *parent->valuestring)
            parentId = parent->valuestring;
        width = UI_ReadWindowLength(cJSON_GetObjectItem(object, "Width"), object, "WImage", 1920.0f);
        height = UI_ReadWindowLength(cJSON_GetObjectItem(object, "Height"), object, "HImage", 1080.0f);
        if (cJSON *const x = cJSON_GetObjectItem(object, "X")) {
            alignX = d3t::ReadSpriteAlignmentValue(cJSON_GetObjectItem(x, "Align"));
            if (const cJSON *const offset = cJSON_GetObjectItem(x, "Offset"))
                offsetX = static_cast<float>(offset->valuedouble) / 1920.0f;
        }
        if (cJSON *const y = cJSON_GetObjectItem(object, "Y")) {
            alignY = d3t::ReadSpriteAlignmentValue(cJSON_GetObjectItem(y, "Align"));
            if (const cJSON *const offset = cJSON_GetObjectItem(y, "Offset"))
                offsetY = static_cast<float>(offset->valuedouble) / 1080.0f;
        }
        scaleStyle = d3t::ReadScaleStyle(object, 4);
        if (const cJSON *const ignore = cJSON_GetObjectItem(object, "IgnoreSafeArea"))
            ignoreSafeArea = ignore->valueint;
        if (cJSON *const scale = cJSON_GetObjectItem(object, "Scale")) {
            if (const cJSON *const x = cJSON_GetObjectItem(scale, "X"))
                width.scale = static_cast<float>(x->valuedouble);
            if (const cJSON *const y = cJSON_GetObjectItem(scale, "Y"))
                height.scale = static_cast<float>(y->valuedouble);
        } else {
            width.scale = 1.0f;
            height.scale = 1.0f;
        }
        if (cJSON *const padding = cJSON_GetObjectItem(object, "Padding")) {
            if (const cJSON *const x = cJSON_GetObjectItem(padding, "X"))
                paddingX = static_cast<float>(x->valuedouble);
            if (const cJSON *const y = cJSON_GetObjectItem(padding, "Y"))
                paddingY = static_cast<float>(y->valuedouble);
        }
        if (const cJSON *const alphaMember = cJSON_GetObjectItem(object, "Alpha"))
            alpha = static_cast<float>(alphaMember->valuedouble);
        flags = 32;
        if (const cJSON *const idMember = cJSON_GetObjectItem(object, "Id"))
            id = idMember->valuestring;
    }
    std::size_t index = 0;
    while (index < g_uiWindows.size() && g_uiWindows[index].inUse)
        ++index;
    if (index == g_uiWindows.size())
        return -1;
    d3t::UIWindow &window = g_uiWindows[index];
    window.inUse = 1;
    std::snprintf(window.id.data(), window.id.size(), "%s", id ? id : "");
    std::snprintf(window.layout.parentId.data(), window.layout.parentId.size(), "%s", parentId);
    window.layout.width = width;
    window.layout.height = height;
    window.layout.ignoreSafeArea = ignoreSafeArea;
    window.layout.offsetX = offsetX;
    window.layout.offsetY = offsetY;
    window.layout.paddingX = paddingX;
    window.layout.paddingY = paddingY;
    window.layout.scaleStyle = scaleStyle;
    window.layout.alignX = alignX;
    window.layout.alignY = alignY;
    window.drawCount = 0;
    window.flags = flags;
    window.initialLayout = window.layout;
    window.rectCached = 0;
    window.alpha = alpha;
    window.cachedRect = {};
    return static_cast<std::int32_t>(index);
}




void UI_ReadStyleColour(cJSON *item, std::array<std::uint32_t, 4> &rgba) {
    const cJSON *const format = cJSON_GetObjectItem(item, "Format");
    if (!format || !format->valuestring)
        return;
    const auto channel = [item](const char *name) {
        const cJSON *const member = cJSON_GetObjectItem(item, name);
        return member ? static_cast<std::uint32_t>(member->valueint) : 0U;
    };
    if (!std::strcmp(format->valuestring, "RGBA")) {
        const std::uint32_t value = channel("RGBA");
        rgba = {value, value, value, value};
    } else if (!std::strcmp(format->valuestring, "RGB")) {
        rgba[3] = 255;
        const std::uint32_t value = channel("RGB");
        rgba[0] = value;
        rgba[1] = value;
        rgba[2] = value;
    } else if (!std::strcmp(format->valuestring, "RGB_A")) {
        const std::uint32_t value = channel("RGB");
        rgba[0] = value;
        rgba[1] = value;
        rgba[2] = value;
        rgba[3] = channel("A");
    } else if (!std::strcmp(format->valuestring, "R_G_B_A")) {
        rgba = {channel("R"), channel("G"), channel("B"), channel("A")};
    }
}



std::int32_t UI_ParseStyleItem(cJSON *item) {
    const cJSON *const type = cJSON_GetObjectItem(item, "Type");
    std::array<std::uint32_t, 4> rgba{};
    std::int32_t isColour = 1;
    std::int32_t image = -1;
    if (type && type->valuestring && !std::strcmp(type->valuestring, "Colour")) {
        UI_ReadStyleColour(item, rgba);
    } else {
        isColour = 0;
        const cJSON *const name = cJSON_GetObjectItem(item, "Image");
        image = d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], name ? name->valuestring : nullptr);
    }
    for (std::size_t index = 0; index < g_uiStyleItems.size(); ++index) {
        UIStyleItem &style = g_uiStyleItems[index];
        if (style.inUse)
            continue;
        style.inUse = 1;
        style.isColour = isColour;
        if (isColour)
            style.colour = rgba;
        else
            style.image = image;
        return static_cast<std::int32_t>(index);
    }
    return -1;
}


void UI_ReadFixedBackground(cJSON *item) {
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 6, UI_ParseStyleItem(item));
}


void UI_ReadBackground(cJSON *item) {
    std::array<std::uint32_t, 4> rgba{};
    UI_ReadStyleColour(item, rgba);
    std::int32_t index = -1;
    for (std::size_t style = 0; style < g_uiStyleItems.size(); ++style) {
        if (!g_uiStyleItems[style].inUse) {
            index = static_cast<std::int32_t>(style);
            break;
        }
    }
    if (index != -1) {
        UIStyleItem &style = g_uiStyleItems[static_cast<std::size_t>(index)];
        style.colour = rgba;
        style.inUse = 1;
        style.isColour = 1;
    }
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 5, index);
}


void UI_ReadWindowStyle(cJSON *style) {
    for (cJSON *member = style->child; member; member = member->next) {
        if (!std::strcmp(member->string, "FixedBackground"))
            UI_ReadFixedBackground(member);
        else if (!std::strcmp(member->string, "Background"))
            UI_ReadBackground(member);
    }
}


void UI_ReadButtonStyle(cJSON *style) {
    for (cJSON *member = style->child; member; member = member->next) {
        const char *const name = member->string;
        const std::int32_t kind = !std::strcmp(name, "Normal") ? 7 : !std::strcmp(name, "Hover") ? 8 : !std::strcmp(name, "Active") ? 9 : 0;
        if (kind)
            d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], kind, UI_ParseStyleItem(member));
    }
}


void UI_ReadStyle(cJSON *style) {
    for (cJSON *member = style->child; member; member = member->next) {
        if (!std::strcmp(member->string, "Window"))
            UI_ReadWindowStyle(member);
        else if (!std::strcmp(member->string, "Button"))
            UI_ReadButtonStyle(member);
    }
}





bool UI_ReadColour(cJSON *object, const char *name, std::uint32_t &colour) {
    cJSON *const member = cJSON_GetObjectItem(object, name);
    if (!member)
        return false;
    std::array<std::uint8_t, 4> bytes{static_cast<std::uint8_t>(colour), static_cast<std::uint8_t>(colour >> 8U), static_cast<std::uint8_t>(colour >> 16U),
        static_cast<std::uint8_t>(colour >> 24U)};
    const int count = cJSON_GetArraySize(member);
    if (count != 3 && count != 4) {
        if (member->type != cJSON_String || !member->valuestring || member->valuestring[0] != '#')
            return false;
        std::array<char, 9> digits{};
        bool more = true;
        for (std::size_t index = 0; index < 8; ++index) {
            char digit = 'F';
            if (more) {
                const char character = member->valuestring[index + 1];
                if (!character)
                    more = false;
                else if ((character >= 'A' && character <= 'F') || (character >= '0' && character <= '9'))
                    digit = character;
                else if (character >= 'a' && character <= 'f')
                    digit = static_cast<char>(character - 32);
            }
            digits[index] = digit;
        }
        const std::uint32_t value = static_cast<std::uint32_t>(std::strtoul(digits.data(), nullptr, 16));
        colour = (value >> 24U) | (((value >> 16U) & 0xffU) << 8U) | (((value >> 8U) & 0xffU) << 16U) | ((value & 0xffU) << 24U);
        return true;
    }
    for (int channel = 0; channel < count; ++channel) {
        const cJSON *const value = cJSON_GetArrayItem(member, channel);
        std::int32_t byte = value->valueint;
        const double scaled = value->valuedouble * 255.0;
        bool saturate = false;
        if (scaled >= 255.0) {
            if (value->valuedouble == 1.0) {
                saturate = channel == 3;
                for (int other = 0; !saturate && other < 3; ++other)
                    saturate = other != channel && cJSON_GetArrayItem(member, other)->valuedouble * 255.0 < 255.0;
            }
        } else {
            byte = static_cast<std::int32_t>(scaled);
        }
        bytes[static_cast<std::size_t>(channel)] = saturate || byte > 255 ? 255 : byte < 0 ? 0 : static_cast<std::uint8_t>(byte);
    }
    if (count == 3)
        bytes[3] = 255;
    colour =
        bytes[0] | (static_cast<std::uint32_t>(bytes[1]) << 8U) | (static_cast<std::uint32_t>(bytes[2]) << 16U) | (static_cast<std::uint32_t>(bytes[3]) << 24U);
    return true;
}



std::int32_t UI_ReadFontName(const char *name) {
    constexpr std::array<const char *, 4> kFamilies{"UIStandardFont", "UIStandardFontBold", "UILoadScreenFont", "UINotePadFont"};
    for (std::size_t family = 0; name && family < kFamilies.size(); ++family) {
        std::size_t index = 0;
        while (name[index] && std::tolower(static_cast<unsigned char>(name[index])) == std::tolower(static_cast<unsigned char>(kFamilies[family][index])))
            ++index;
        if (!name[index] && !kFamilies[family][index])
            return static_cast<std::int32_t>(family);
    }
    return 0;
}



std::int32_t UI_ReadDataBinding(cJSON *data) {
    std::size_t index = 0;
    while (index < g_uiDataBindings.size() && g_uiDataBindings[index].inUse)
        ++index;
    if (index == g_uiDataBindings.size())
        return -1;
    UIDataBinding &binding = g_uiDataBindings[index];
    binding.inUse = 1;
    const cJSON *const method = cJSON_GetObjectItem(data, "Method");
    if (method && method->valuestring && !std::strcmp(method->valuestring, "Read"))
        binding.method = 0;
    else if (method && method->valuestring && !std::strcmp(method->valuestring, "Set"))
        binding.method = 1;
    const cJSON *const variable = cJSON_GetObjectItem(data, "Variable");
    binding.variable = -1;
    const std::array<d3t::UIViewVariable, d3t::kUIViewVariableCount> &variables = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)].variables;
    for (std::size_t slot = 0; variable && variable->valuestring && slot < variables.size() && variables[slot].used; ++slot) {
        if (!std::strcmp(variables[slot].name, variable->valuestring)) {
            binding.variable = static_cast<std::int32_t>(slot);
            break;
        }
    }
    return static_cast<std::int32_t>(index);
}




void UI_ReadPanel(cJSON *panel) {
    if (cJSON *const style = cJSON_GetObjectItem(panel, "Style"))
        UI_ReadStyle(style);
    const std::int32_t window = UI_ReadWindow(panel);
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    d3t::AddViewWidget(slot, 1, window);
    const cJSON *const flags = cJSON_GetObjectItem(panel, "Flags");
    for (cJSON *member = flags ? flags->next : nullptr; member; member = member->next) {
        if (!std::strcmp(member->string, "Layout")) {
            d3t::ReadLayout(member, window, g_uiLayoutItems, slot);
        } else if (!std::strcmp(member->string, "Objects")) {
            std::int32_t kind = 0;
            const std::int32_t index = UI_ReadWidgetObject(member, kind, window);
            if (index > -1)
                d3t::AddViewWidget(slot, kind, index);
        }
    }
    d3t::AddViewWidget(slot, 2, -1);
}


d3t::UIWindow *UI_GetWindow(std::int32_t index) {
    return static_cast<std::uint32_t>(index) > 0x3ffU ? nullptr : &g_uiWindows[static_cast<std::size_t>(index)];
}



std::int32_t UI_FindViewWindow(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75 || !id)
        return -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 1)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        const d3t::UIWindow *const window = UI_GetWindow(index);
        if (window && window->id[0] && !std::strcmp(window->id.data(), id) && index != -1)
            return index;
    }
    return -1;
}


std::int32_t UI_FindViewLabel(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75 || !id)
        return -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 15)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiLabelWidgets.size())
            continue;
        const char *const name = g_uiLabelWidgets[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}


std::int32_t UI_FindViewColourImage(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75 || !id)
        return -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 19)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiColourImageWidgets.size())
            continue;
        const char *const name = g_uiColourImageWidgets[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}



std::int32_t UI_FindViewImageWidget(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U || !id)
        return -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 18)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiImageWidgets.size())
            continue;
        const char *const name = g_uiImageWidgets[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}

#if SHENMUE_IS_S2
UIButtonIconWidget *UI_FindViewIconWidget(std::uint32_t slotIndex, const char *id, std::int32_t kind) {
    if (slotIndex > 75U || !id)
        return nullptr;
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != kind)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiButtonIconWidgets.size())
            continue;
        UIButtonIconWidget &icon = g_uiButtonIconWidgets[static_cast<std::size_t>(index)];
        if (icon.id[0] && !std::strcmp(icon.id.data(), id))
            return &icon;
    }
    return nullptr;
}
#endif


d3t::UIWindow *UI_FindWindowById(const char *id) {
    if (!id || !id[0])
        return nullptr;
    for (std::uint32_t slot = 0; slot < 76; ++slot) {
        const std::int32_t index = UI_FindViewWindow(slot, id);
        if (index != -1)
            return UI_GetWindow(index);
    }
    return nullptr;
}

// The UI safe-area scale, 0.8 at a UISafeArea of 0 up to 1.
float UI_SafeAreaScale() {
#if SHENMUE_IS_S1
    const float safeArea = g_currentSettings.display.uiDisplayArea;
    return (1.0f - safeArea) * 0.8f + safeArea;
#else
    return GetS2UserInterfaceSafeArea();
#endif
}



std::int32_t UI_GetAspectWidth() {
    const std::int32_t bufferWidth = static_cast<std::int32_t>(g_uiScreenWidth);
    const std::int32_t originalWidth = static_cast<std::int32_t>(g_originalScreenWidth);
    const std::int32_t screenWidth = originalWidth > bufferWidth ? bufferWidth : originalWidth;
    const float buffer = static_cast<float>(bufferWidth);
    const float screenHeight = static_cast<float>(static_cast<std::int32_t>(g_originalScreenHeight));
    const std::int32_t width = static_cast<std::int32_t>(buffer / ((1.0f / (1080.0f / buffer)) * (screenHeight / static_cast<float>(screenWidth))));
#if SHENMUE_IS_S1
    if (!g_getAspectRatioModeFunction)
        return width;
    return g_getAspectRatioModeFunction() == 0 ? width : 1439;
#else
    return GetAspectRatioModeShouldScale() ? 1439 : width;
#endif
}

#if SHENMUE_IS_S2
float UI_GetReferenceWidth() {
    if (ShouldScaleS2AspectRatio())
        return static_cast<float>(GetLogicalScreenHeight()) * 1.33333f;
    const float height = static_cast<float>(GetLogicalScreenHeight());
    return (1920.0f / std::fmax(height, 1080.0f)) * height;
}

float UI_GetReferenceHeight() {
    return static_cast<float>(GetLogicalScreenHeight());
}
#endif
float WithinSafeArea(float length) {
    if (length == 0.0f)
        return 0.0f;
    const float horizontal = (length / 1920.0f) * (static_cast<float>(UI_GetAspectWidth()) * UI_SafeAreaScale());
    const float vertical = UI_SafeAreaScale() * 1080.0f * (length / 1080.0f);
    return vertical <= horizontal ? vertical : horizontal;
}



std::int32_t UI_FindViewButtonWheel(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U)
        return -1;
#if SHENMUE_IS_S2
    if (!id)
        return -1;
#endif
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 23)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiButtonWheels.size())
            continue;
        const char *const name = g_uiButtonWheels[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}


void AdjustForSafeArea(UILayoutRect &rect, float paddingX, float paddingY) {
    const float x = WithinSafeArea(paddingX);
    const float y = WithinSafeArea(paddingY);
    rect.x = x + rect.x;
    rect.y = y + rect.y;
    rect.width = rect.width - x * 2.0f;
    rect.height = rect.height - y * 2.0f;
}

// The screen area a root window lays out in: 1080 high and the aspect width, less the margin outside the safe area unless the window
// ignores it; none when either side is 0.
void UI_RootWindowArea(const d3t::UIWindow &window, float &width, float &height, float &aspectWidth) {
    height = 1080.0f;
    aspectWidth = static_cast<float>(UI_GetAspectWidth());
    width = aspectWidth;
    if (!window.layout.ignoreSafeArea) {
        height = UI_SafeAreaScale() * 1080.0f;
        width = aspectWidth - (1080.0f - height);
    }
}


void AdjustForDisplayAreaAndAspectRatio(const d3t::UIWindow &window, float &x, float &y) {
    float width = 0.0f;
    float height = 0.0f;
    float aspectWidth = 0.0f;
    UI_RootWindowArea(window, width, height, aspectWidth);
    float areaWidth = 0.0f;
    float areaHeight = 0.0f;
    if (height != 0.0f && width != 0.0f) {
        areaWidth = width;
        areaHeight = height;
    }
    x = areaWidth * window.layout.offsetX + x;
    y = areaHeight * window.layout.offsetY + y;
}




void UI_ScaleSize(float &width, float &height, std::int32_t style, float availableWidth, float availableHeight, float srcWidth, float srcHeight,
    const d3t::UIWindow *parent) {
    width = 0.0f;
    height = 0.0f;
    if (availableHeight == 0.0f || availableWidth == 0.0f || srcWidth == 0.0f || srcHeight == 0.0f)
        return;
    const float availableAspect = availableHeight / availableWidth;
    const float srcAspect = srcHeight / srcWidth;
    // 0 the area, 1 fitted to its width, 2 fitted to its height.
    std::int32_t fit = 0;
    if (style == 2) {
        fit = availableAspect <= srcAspect ? (srcAspect <= availableAspect ? 0 : 2) : 1;
    } else if (style == 1) {
        fit = srcAspect <= availableAspect ? (availableAspect <= srcAspect ? 0 : 2) : 1;
    } else if (style == 4) {
        width = srcWidth;
        height = srcHeight;
        return;
    } else if (style == 0) {
        const float parentWidth = parent ? parent->layout.width.value.fraction * 1920.0f : 1920.0f;
        width = (srcWidth / parentWidth) * availableWidth;
        height = parent ? (srcHeight / (parent->layout.height.value.fraction * 1080.0f)) * availableHeight : (srcHeight / 1080.0f) * availableHeight;
        return;
    }
    if (fit == 1) {
        width = availableWidth;
        height = (availableWidth / srcWidth) * srcHeight;
    } else if (fit == 2) {
        height = availableHeight;
        width = (availableHeight / srcHeight) * srcWidth;
    } else {
        width = availableWidth;
        height = availableHeight;
    }
}

void UI_GetWindowRect(d3t::UIWindow *window, UILayoutRect &rect);



void UI_WindowSize(const d3t::UIWindow &window, float &width, float &height, d3t::UIWindow *parent) {
    float availableWidth = 0.0f;
    float availableHeight = 0.0f;
    if (parent) {
        UILayoutRect parentRect{};
        UI_GetWindowRect(parent, parentRect);
        availableWidth = parentRect.width;
        availableHeight = parentRect.height;
    } else {
        float areaWidth = 0.0f;
        float areaHeight = 0.0f;
        float aspectWidth = 0.0f;
        UI_RootWindowArea(window, areaWidth, areaHeight, aspectWidth);
        if (areaHeight != 0.0f && areaWidth != 0.0f) {
            availableWidth = areaWidth;
            availableHeight = areaHeight;
        }
    }
    float scaledWidth = 0.0f;
    float scaledHeight = 0.0f;
    const d3t::UIWindowLength &authoredWidth = window.layout.width;
    const d3t::UIWindowLength &authoredHeight = window.layout.height;
    if (!authoredWidth.fromImage && !authoredHeight.fromImage) {
        UI_ScaleSize(scaledWidth, scaledHeight, window.layout.scaleStyle, availableWidth, availableHeight, authoredWidth.value.fraction * 1920.0f,
            authoredHeight.value.fraction * 1080.0f, parent);
    } else if (authoredWidth.fromImage == 1 && authoredHeight.fromImage == 1) {
        const d3t::UIViewImage &image = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)].images[static_cast<std::size_t>(authoredWidth.value.image)];
        UI_ScaleSize(scaledWidth, scaledHeight, window.layout.scaleStyle, availableWidth, availableHeight, image.widthScale * 1920.0f,
            image.heightScale * 1080.0f, parent);
    }
    width = scaledWidth * authoredWidth.scale;
    height = scaledHeight * authoredHeight.scale;
}




void UI_AlignInArea(std::int32_t alignX, std::int32_t alignY, float width, float height, float &x, float &y, float areaWidth, float areaHeight) {
    switch (alignX) {
    case 1:
        x = (areaWidth * 0.5f + x) - width * 0.5f;
        break;
    case 2:
        x = areaWidth * 0.5f + x;
        break;
    case 3:
        x = (areaWidth * 0.5f + x) - width;
        break;
    case 8:
        x = (areaWidth - width) + x;
        break;
    case 12:
        x = areaWidth + x;
        break;
    case 13:
        x = x - width;
        break;
    default:
        break;
    }
    switch (alignY) {
    case 1:
        y = (areaHeight * 0.5f + y) - height * 0.5f;
        break;
    case 5:
        y = areaHeight * 0.5f + y;
        break;
    case 4:
        y = (areaHeight * 0.5f + y) - height;
        break;
    case 7:
        y = (areaHeight - height) + y;
        break;
    case 11:
        y = areaHeight + y;
        break;
    case 10:
        y = y - height;
        break;
    default:
        break;
    }
}




void UI_WindowPosition(const d3t::UIWindow &window, float width, float height, float &x, float &y, d3t::UIWindow *parent) {
    float areaX = 0.0f;
    float areaY = 0.0f;
    float areaWidth = 0.0f;
    float areaHeight = 0.0f;
    if (parent) {
        UILayoutRect parentRect{};
        UI_GetWindowRect(parent, parentRect);
        areaX = parentRect.x;
        areaY = parentRect.y;
        areaWidth = parentRect.width;
        areaHeight = parentRect.height;
    } else {
        float rootWidth = 0.0f;
        float rootHeight = 0.0f;
        float aspectWidth = 0.0f;
        UI_RootWindowArea(window, rootWidth, rootHeight, aspectWidth);
        if (rootHeight != 0.0f && rootWidth != 0.0f) {
            const float aspect = rootHeight / rootWidth;
            const std::int32_t style = window.layout.scaleStyle;
            if (style == 4) {
                areaWidth = 1920.0f;
                areaHeight = 1080.0f;
            } else if ((style == 2 && aspect > 0.5625f) || (style == 1 && aspect < 0.5625f)) {
                areaHeight = (rootWidth / 1920.0f) * 1080.0f;
                areaWidth = rootWidth;
            } else if ((style == 2 && aspect < 0.5625f) || (style == 1 && aspect > 0.5625f)) {
                areaHeight = rootHeight;
                areaWidth = (rootHeight / 1080.0f) * 1920.0f;
            } else {
                areaHeight = rootHeight;
                areaWidth = rootWidth;
            }
        }
        areaX = (aspectWidth - rootWidth) * 0.5f;
        areaY = (1080.0f - rootHeight) * 0.5f;
    }
    x = areaX + x;
    y = areaY + y;
    UI_AlignInArea(window.layout.alignX, window.layout.alignY, width, height, x, y, areaWidth, areaHeight);
    const float scaleX = window.layout.width.scale;
    if (scaleX > 1.0f) {
        const float shrink = (width - width / scaleX) * 0.5f;
        if (window.layout.alignX == 8)
            x = shrink + x;
        else if (window.layout.alignX == 9)
            x = x - shrink;
    }
}



void UI_InvalidateWindowRect(d3t::UIWindow &window) {
    if (window.rectCached != 1)
        return;
    for (d3t::UIWindow &other : g_uiWindows) {
        if (!std::strcmp(window.id.data(), other.layout.parentId.data()) || !std::strcmp(window.id.data(), other.initialLayout.parentId.data()))
            UI_InvalidateWindowRect(other);
    }
    window.initialLayout = window.layout;
    window.rectCached = 0;
}




[[maybe_unused]] void UI_GetWindowRect(d3t::UIWindow *window, UILayoutRect &rect) {
    if (!window)
        return;
    if (std::memcmp(&window->layout, &window->initialLayout, sizeof(window->layout)) != 0)
        UI_InvalidateWindowRect(*window);
    if (window->rectCached) {
        rect = {.x = window->cachedRect[0], .y = window->cachedRect[1], .width = window->cachedRect[2], .height = window->cachedRect[3]};
        return;
    }
    d3t::UIWindow *const parent = UI_FindWindowById(window->layout.parentId.data());
    UI_WindowSize(*window, rect.width, rect.height, parent);
    UI_WindowPosition(*window, rect.width, rect.height, rect.x, rect.y, parent);
    AdjustForDisplayAreaAndAspectRatio(*window, rect.x, rect.y);
    AdjustForSafeArea(rect, window->layout.paddingX, window->layout.paddingY);
    window->rectCached = 1;
    window->cachedRect = {rect.x, rect.y, rect.width, rect.height};
}

static_assert(offsetof(nk_context, style.text.padding) == 0x1a8 && offsetof(nk_context, style.button.border) == 0x240);
static_assert(offsetof(nk_context, style.contextual_button.border) == 0x308 && offsetof(nk_context, style.menu_button.border) == 0x3d0);
static_assert(offsetof(nk_context, style.option.padding) == 0x4ec && offsetof(nk_context, style.checkbox.padding) == 0x604);
static_assert(offsetof(nk_context, style.selectable.padding) == 0x74c && offsetof(nk_context, style.slider.border) == 0x888);
static_assert(offsetof(nk_context, style.window.border) == 0x2298 && offsetof(nk_context, style.window.spacing) == 0x22b8);
static_assert(offsetof(nk_context, style.window.padding) == 0x22d0 && offsetof(nk_window, flags) == 0x48 && offsetof(nk_window, layout) == 0xa0);
static_assert(offsetof(nk_panel, row.item_width) == 0x78 && offsetof(nk_panel, row.item_offset) == 0x80 && offsetof(nk_panel, row.item) == 0x88);




void UI_ResetNuklearStyle(nk_context &context) {
    nk_style &style = context.style;
    for (nk_style_button *const button : {&style.button, &style.contextual_button, &style.menu_button}) {
        button->border = 0.0f;
        button->rounding = 0.0f;
        button->padding = nk_vec2(0.0f, 0.0f);
        button->image_padding = nk_vec2(0.0f, 0.0f);
        button->touch_padding = nk_vec2(0.0f, 0.0f);
    }
    style.text.padding = nk_vec2(0.0f, 0.0f);
    for (nk_style_toggle *const toggle : {&style.option, &style.checkbox}) {
        toggle->padding = nk_vec2(0.0f, 0.0f);
        toggle->touch_padding = nk_vec2(0.0f, 0.0f);
        toggle->spacing = 0.0f;
        toggle->border = 0.0f;
    }
    style.selectable.padding = nk_vec2(0.0f, 0.0f);
    style.selectable.touch_padding = nk_vec2(0.0f, 0.0f);
    style.selectable.image_padding = nk_vec2(0.0f, 0.0f);
    style.slider.border = 0.0f;
    style.slider.padding = nk_vec2(0.0f, 0.0f);
    style.slider.spacing = nk_vec2(0.0f, 0.0f);
    nk_style_window &window = style.window;
    window.border = 0.0f;
    window.combo_border = 0.0f;
    window.contextual_border = 0.0f;
    window.menu_border = 0.0f;
    window.group_border = 0.0f;
    window.tooltip_border = 0.0f;
    window.popup_border = 0.0f;
    window.rounding = 0.0f;
    window.spacing = nk_vec2(0.0f, 0.0f);
    for (struct nk_vec2 *const padding : {&window.padding, &window.group_padding, &window.popup_padding, &window.combo_padding, &window.contextual_padding,
             &window.menu_padding, &window.tooltip_padding})
        *padding = nk_vec2(0.0f, 0.0f);
}



nk_style_item UI_StyleItem(std::int32_t styleIndex) {
    nk_style_item item{};
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 75U || static_cast<std::uint32_t>(styleIndex) > 0x3ffU)
        return item;
    const UIStyleItem &record = g_uiStyleItems[static_cast<std::size_t>(styleIndex)];
    if (record.isColour == 1) {
        const std::uint32_t colour = ConstructColour(static_cast<std::int32_t>(record.colour[0]), static_cast<std::int32_t>(record.colour[1]),
            static_cast<std::int32_t>(record.colour[2]), static_cast<std::int32_t>(record.colour[3]));
        item.type = NK_STYLE_ITEM_COLOR;
        std::memcpy(&item.data.color, &colour, sizeof(colour));
    } else if (record.isColour == 0) {
        const d3t::UIViewImage &image = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)].images[static_cast<std::size_t>(record.image)];
        struct nk_image nkImage {};
        std::memcpy(&nkImage, &image.textureHandle, sizeof(nkImage));
        const unsigned short width = static_cast<unsigned short>(static_cast<std::int32_t>(image.widthScale * 1920.0f));
        const unsigned short height = static_cast<unsigned short>(static_cast<std::int32_t>(image.heightScale * 1080.0f));
        nkImage.w = width;
        nkImage.h = height;
        nkImage.region[2] = width;
        nkImage.region[3] = height;
        item.type = NK_STYLE_ITEM_IMAGE;
        item.data.image = nkImage;
    }
    return item;
}




UILayoutRect UI_SpacePushRect(const d3t::UILayoutItem &item, d3t::UIWindow *window) {
    UILayoutRect rect{};
    UILayoutRect windowRect{};
    UI_GetWindowRect(window, windowRect);
    if (item.fullscreen) {
        const std::int32_t aspectWidth = UI_GetAspectWidth();
        float areaWidth = 0.0f;
        float areaHeight = 0.0f;
        if (static_cast<float>(aspectWidth) != 0.0f) {
            areaWidth = static_cast<float>(aspectWidth);
            areaHeight = 1080.0f;
        }
        const float width = item.width * 1920.0f;
        const float height = item.height * 1080.0f;
        if (areaHeight != 0.0f && areaWidth != 0.0f && width != 0.0f && height != 0.0f) {
            if (height / width <= areaHeight / areaWidth) {
                if (areaHeight / areaWidth <= height / width) {
                    rect.width = areaWidth;
                    rect.height = areaHeight;
                } else {
                    rect.height = areaHeight;
                    rect.width = (areaHeight / height) * width;
                }
            } else {
                rect.width = areaWidth;
                rect.height = (areaWidth / width) * height;
            }
        }
        const float left = (static_cast<float>(aspectWidth) - static_cast<float>(aspectWidth)) * 0.5f - windowRect.x;
        rect.y = 0.0f - windowRect.y;
        rect.x = (left + areaWidth * 0.5f) - rect.width * 0.5f;
        rect.y = (areaHeight * 0.5f + rect.y) - rect.height * 0.5f;
        return rect;
    }
    UI_ScaleSize(rect.width, rect.height, item.scaleStyle, windowRect.width, windowRect.height, item.width * 1920.0f, item.height * 1080.0f, window);
    UI_AlignInArea(item.alignX, item.alignY, rect.width, rect.height, rect.x, rect.y, windowRect.width, windowRect.height);
    AdjustForSafeArea(rect, item.paddingX, item.paddingY);
    rect.x = (item.offsetX + item.posX) * windowRect.width + rect.x;
    rect.y = (item.offsetY + item.posY) * windowRect.height + rect.y;
    const float width = rect.width;
    const float height = rect.height;
    rect.x = (width - width * item.scaleX) * 0.5f + rect.x;
    rect.y = (height - height * item.scaleY) * 0.5f + rect.y;
    rect.width = width * item.scaleX;
    rect.height = height * item.scaleY;
    return rect;
}





void UI_DrawWindowBegin(nk_context &context, std::int32_t index) {
    d3t::UIWindow *const window = UI_GetWindow(index);
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 75U || !window)
        return;
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(channels));
    const std::int32_t alpha = static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha);
    const std::uint32_t colour = ConstructColour(channels[0], channels[1], channels[2], alpha);
    std::memcpy(&context.style.window.background, &colour, sizeof(colour));
    nk_byte &fixedAlpha = context.style.window.fixed_background.data.color.a;
    fixedAlpha = static_cast<nk_byte>(static_cast<std::int32_t>(static_cast<float>(fixedAlpha) * window->alpha));
    std::array<char, 256> name{};
    std::snprintf(name.data(), name.size(), "s%d p%d - %s: %i", g_uiDrawViewSlot, index, window->id.data(), window->drawCount);
    ++window->drawCount;
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(rect.x, rect.y, rect.width, rect.height), static_cast<nk_flags>(window->flags));
    }
    if (window->alpha > 0.0f)
        UI_WindowMouseCallbacks(window->mouseCallbacks, g_currActiveUIIndex, {.x = 0.0f, .y = 0.0f, .width = rect.width, .height = rect.height}, window);
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
