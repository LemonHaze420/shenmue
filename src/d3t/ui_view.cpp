#include "d3t/ui_view.hpp"

#include <cJSON.h>

#include <cstdio>
#include <cstring>

namespace shenmue::d3t {

void ReadVariables(cJSON *document, std::array<UIViewVariable, kUIViewVariableCount> &variables) {
    cJSON *const table = cJSON_GetObjectItem(document, "Variables");
    if (!table)
        return;
    const int count = cJSON_GetArraySize(table);
    for (int index = 0; index < count; ++index) {
        cJSON *const member = cJSON_GetArrayItem(table, index);
        UIViewVariable &variable = variables[static_cast<std::size_t>(index)];
        variable.used = 1;
        variable.name = member->string;
        const cJSON *const type = cJSON_GetObjectItem(member, "Type");
        cJSON *const value = cJSON_GetObjectItem(member, "Value");
        if (!std::strcmp(type->valuestring, "int")) {
            variable.value.intValue = value->valueint;
            variable.type = UIViewVariableType::Int;
        } else if (!std::strcmp(type->valuestring, "float4")) {
            for (int component = 0; component < 4; ++component)
                variable.value.float4[static_cast<std::size_t>(component)] = static_cast<float>(cJSON_GetArrayItem(value, component)->valuedouble);
            variable.type = UIViewVariableType::Float4;
        } else if (!std::strcmp(type->valuestring, "float")) {
            variable.value.floatValue = static_cast<float>(value->valuedouble);
            variable.type = UIViewVariableType::Float;
        } else if (!std::strcmp(type->valuestring, "char")) {
            variable.value.stringValue = value->valuestring;
            variable.type = UIViewVariableType::Char;
        } else if (!std::strcmp(type->valuestring, "vec2")) {
            for (int component = 0; component < 2; ++component)
                variable.value.vec2[static_cast<std::size_t>(component)] = static_cast<float>(cJSON_GetArrayItem(value, component)->valuedouble);
            variable.type = UIViewVariableType::Vec2;
        }
        variable.initialValue = variable.value;
    }
}

template <std::size_t ImageCapacity> std::int32_t FindViewImage(const UIViewSlot<ImageCapacity> &slot, const char *name) {
    if (!name || !*name)
        return -1;
    for (std::size_t index = 0; index < ImageCapacity; ++index) {
        if (!std::strcmp(slot.images[index].name, name))
            return static_cast<std::int32_t>(index);
    }
    return -1;
}

void ResetSpriteSheets(std::array<UISpriteSheet, kUISpriteSheetCount> &sheets) {
    for (UISpriteSheet &sheet : sheets) {
        sheet.used = 0;
        sheet.imageName = "";
        sheet.image = -1;
        sheet.type = UISpriteSheetType::Static;
        sheet.width = 0.0f;
        sheet.height = 0.0f;
        for (std::size_t frame = 0; frame < kUISpriteSheetFrameCount; ++frame) {
            sheet.frameMin[frame] = {};
            sheet.frameMax[frame] = {};
        }
    }
}

template <std::size_t ImageCapacity>
void ReadSprites(cJSON *document, std::array<UISpriteSheet, kUISpriteSheetCount> &sheets, const UIViewSlot<ImageCapacity> &slot) {
    cJSON *const table = cJSON_GetObjectItem(document, "SpriteSheets");
    if (!table)
        return;
    const int count = cJSON_GetArraySize(table);
    for (int index = 0; index < count; ++index) {
        cJSON *const member = cJSON_GetArrayItem(table, index);
        std::size_t record = 0;
        while (record < sheets.size() && sheets[record].used)
            ++record;

        if (record == sheets.size())
            continue;
        UISpriteSheet &sheet = sheets[record];
        sheet.used = 1;
        const cJSON *const image = cJSON_GetObjectItem(member, "Image");
        sheet.image = FindViewImage(slot, image->valuestring);
        sheet.imageName = image->valuestring;
        const float width = static_cast<float>(cJSON_GetObjectItem(member, "Width")->valuedouble);
        sheet.width = width;
        const float height = static_cast<float>(cJSON_GetObjectItem(member, "Height")->valuedouble);
        sheet.height = height;
        const char *const type = cJSON_GetObjectItem(member, "Type")->valuestring;
        if (!std::strcmp(type, "Static")) {
            sheet.type = UISpriteSheetType::Static;
            const float cellWidth = static_cast<float>(cJSON_GetObjectItem(member, "X")->valuedouble);
            const float cellHeight = static_cast<float>(cJSON_GetObjectItem(member, "Y")->valuedouble);
            const int columns = static_cast<int>(width / cellWidth);
            const int rows = static_cast<int>(height / cellHeight);
            std::size_t first = 0;
            for (int row = 0; row < rows; ++row) {
                const float y = static_cast<float>(row) * cellHeight;
                for (int column = 0; column < columns; ++column) {
                    const float x = static_cast<float>(column) * cellWidth;
                    sheet.frameMin[first + static_cast<std::size_t>(column)] = {x, y};
                    sheet.frameMax[first + static_cast<std::size_t>(column)] = {x + cellWidth, y + cellHeight};
                }
                first += static_cast<std::size_t>(columns);
            }
        } else if (!std::strcmp(type, "Custom")) {
            sheet.type = UISpriteSheetType::Custom;
            const int frames = cJSON_GetObjectItem(member, "Count")->valueint;
            cJSON *const uv = cJSON_GetObjectItem(member, "UV");
            for (int frame = 0; frame < frames; ++frame) {
                cJSON *const rect = cJSON_GetArrayItem(uv, frame);
                if (!rect)
                    continue;
                const float x = static_cast<float>(cJSON_GetObjectItem(rect, "X")->valuedouble);
                const float y = static_cast<float>(cJSON_GetObjectItem(rect, "Y")->valuedouble);
                sheet.frameMin[static_cast<std::size_t>(frame)] = {x, y};
                sheet.frameMax[static_cast<std::size_t>(frame)] = {static_cast<float>(cJSON_GetObjectItem(rect, "Width")->valuedouble) + x,
                    static_cast<float>(cJSON_GetObjectItem(rect, "Height")->valuedouble) + y};
            }
        }
    }
}

std::int32_t ReadSpriteAlignmentValue(const cJSON *align) {
    if (!align || !align->valuestring)
        return 0;
    constexpr std::array<const char *, 13> kNames{
        "Centre", "CentreRight", "CentreLeft", "CentreUp", "CentreDown", "TopOut", "Top", "BottomOut", "Bottom", "RightOut", "Right", "LeftOut", "Left"};
    constexpr std::array<std::int32_t, 13> kValues{1, 2, 3, 4, 5, 10, 6, 11, 7, 12, 8, 13, 9};
    for (std::size_t index = 0; index < kNames.size(); ++index) {
        if (!std::strcmp(align->valuestring, kNames[index]))
            return kValues[index];
    }
    return 0;
}

std::int32_t ReadScaleStyle(const cJSON *object, std::int32_t fallback) {
    const cJSON *const style = cJSON_GetObjectItem(const_cast<cJSON *>(object), "ScaleStyle");
    if (!style || !style->valuestring)
        return fallback;
    for (std::size_t index = 0; index < kUIScaleStyleNames.size(); ++index) {
        if (!std::strncmp(style->valuestring, kUIScaleStyleNames[index], 64))
            return static_cast<std::int32_t>(index);
    }
    return fallback;
}

template <std::size_t ImageCapacity> std::int32_t AddViewWidget(UIViewSlot<ImageCapacity> &slot, std::int32_t kind, std::int32_t poolIndex) {
    const std::int32_t count = slot.widgetCount;
    if (count < static_cast<std::int32_t>(kUIViewWidgetCount)) {
        slot.widgetKinds[static_cast<std::size_t>(count)] = kind;
        slot.widgetPoolIndices[static_cast<std::size_t>(count)] = poolIndex;
        ++slot.widgetCount;
    }
    return count;
}

std::int32_t AllocLabelWidget(std::array<UILabelWidget, kUILabelWidgetCount> &widgets, const char *id, const char *text, const UILabelToken &token,
    std::uint32_t alignment, std::int32_t dataBinding, std::int32_t size, std::int32_t fontFamily, std::uint32_t colour, std::int32_t hasStroke,
    std::uint32_t strokeColour, std::int32_t window, std::int32_t scaleVar, std::int32_t scaleVarAssign) {
    std::size_t index = 0;
    while (index < widgets.size() && widgets[index].inUse)
        ++index;
    if (index == widgets.size())
        return -1;
    UILabelWidget &widget = widgets[index];
    widget.inUse = 1;
    std::snprintf(widget.id.data(), widget.id.size(), "%s", id ? id : "");
    widget.text = text;
    widget.token = token;
    widget.dataBinding = dataBinding;
    widget.size = size;
    widget.strokeColour = strokeColour;
    widget.fontFamily = fontFamily;
    widget.scaleVar = scaleVar;
    widget.colour = colour;
    widget.scaleVarAssign = scaleVarAssign;
    widget.hasStroke = hasStroke;
    widget.alignment = alignment;
    widget.window = window;
    return static_cast<std::int32_t>(index);
}

std::int32_t AllocImageWidget(std::array<UIImageWidget, kUIImageWidgetCount> &widgets, const char *id, std::int32_t image, std::int32_t window, float angle) {
    std::size_t index = 0;
    while (index < widgets.size() && widgets[index].inUse)
        ++index;
    if (index == widgets.size())
        return -1;
    UIImageWidget &widget = widgets[index];
    widget.inUse = 1;
    std::snprintf(widget.id.data(), widget.id.size(), "%s", id ? id : "");
    widget.angle = angle;
    widget.frame = -1;
    widget.image = image;
    widget.window = window;
    widget.field44 = 1;
    return static_cast<std::int32_t>(index);
}

void ResetLayoutItems(std::array<UILayoutItem, kUILayoutItemCount> &items) {
    for (UILayoutItem &item : items) {
        item.inUse = 0;
        item.id[0] = 0;
        item.count = 0;
        item.window = 0;
        item.width = 0.0f;
        item.height = 0.0f;
    }
}

namespace {

double MemberNumber(cJSON *object, const char *name) {
    const cJSON *const member = object ? cJSON_GetObjectItem(object, name) : nullptr;
    return member ? member->valuedouble : 0.0;
}

std::int32_t MemberInt(cJSON *object, const char *name) {
    const cJSON *const member = object ? cJSON_GetObjectItem(object, name) : nullptr;
    return member ? member->valueint : 0;
}

UILayoutItem *AllocLayoutItem(std::array<UILayoutItem, kUILayoutItemCount> &items, std::int32_t &index) {
    for (std::size_t item = 0; item < items.size(); ++item) {
        if (!items[item].inUse) {
            items[item].inUse = 1;
            index = static_cast<std::int32_t>(item);
            return &items[item];
        }
    }
    index = -1;
    return nullptr;
}


void ReadLayoutAxis(cJSON *axis, float &pos, std::int32_t &align, float &offset) {
    pos = static_cast<float>(MemberNumber(axis, "Pos"));
    align = ReadSpriteAlignmentValue(axis ? cJSON_GetObjectItem(axis, "Align") : nullptr);
    offset = static_cast<float>(MemberNumber(axis, "Offset"));
}

} // namespace

template <std::size_t ImageCapacity>
void ReadLayout(cJSON *layout, std::int32_t window, std::array<UILayoutItem, kUILayoutItemCount> &items, UIViewSlot<ImageCapacity> &slot) {
    for (cJSON *member = layout->child; member; member = member->next) {
        const char *const name = member->string;
        std::int32_t index = -1;
        if (!std::strcmp(name, "RowDynamic")) {
            const float height = static_cast<float>(MemberNumber(member, "Height")) / 1080.0f;
            const std::int32_t columns = MemberInt(member, "Columns");
            if (UILayoutItem *const item = AllocLayoutItem(items, index)) {
                item->height = height;
                item->count = columns;
                item->width = 0.0f;
                item->window = window;
            }
            AddViewWidget(slot, 10, index);
        } else if (!std::strcmp(name, "RowStatic")) {
            const float height = static_cast<float>(MemberNumber(member, "Height")) / 1080.0f;
            const float width = static_cast<float>(MemberNumber(member, "Width")) / 1920.0f;
            const std::int32_t columns = MemberInt(member, "Columns");
            if (UILayoutItem *const item = AllocLayoutItem(items, index)) {
                item->width = width;
                item->height = height;
                item->count = columns;
                item->window = window;
            }
            AddViewWidget(slot, 11, index);
        } else if (!std::strcmp(name, "SpaceBegin")) {
            if (UILayoutItem *const item = AllocLayoutItem(items, index)) {
                item->height = static_cast<float>(MemberNumber(member, "Height"));
                item->count = MemberInt(member, "Count");
            }
            AddViewWidget(slot, 12, index);
        } else if (!std::strcmp(name, "SpacePush")) {
            if (UILayoutItem *const item = AllocLayoutItem(items, index)) {
                ReadLayoutAxis(cJSON_GetObjectItem(member, "X"), item->posX, item->alignX, item->offsetX);
                item->posX /= 1920.0f;
                item->offsetX /= 1920.0f;
                ReadLayoutAxis(cJSON_GetObjectItem(member, "Y"), item->posY, item->alignY, item->offsetY);
                item->posY /= 1080.0f;
                item->offsetY /= 1080.0f;
                if (const cJSON *const width = cJSON_GetObjectItem(member, "Width"))
                    item->width = static_cast<float>(width->valuedouble) / 1920.0f;
                if (const cJSON *const height = cJSON_GetObjectItem(member, "Height"))
                    item->height = static_cast<float>(height->valuedouble) / 1080.0f;
                item->scaleStyle = ReadScaleStyle(member, 4);
                if (cJSON *const padding = cJSON_GetObjectItem(member, "Padding")) {
                    if (const cJSON *const x = cJSON_GetObjectItem(padding, "X"))
                        item->paddingX = static_cast<float>(x->valuedouble);
                    if (const cJSON *const y = cJSON_GetObjectItem(padding, "Y"))
                        item->paddingY = static_cast<float>(y->valuedouble);
                } else {
                    item->paddingX = 0.0f;
                    item->paddingY = 0.0f;
                }
                const cJSON *const fullscreen = cJSON_GetObjectItem(member, "Fullscreen");
                item->fullscreen = fullscreen ? fullscreen->valueint : 0;
                if (cJSON *const scale = cJSON_GetObjectItem(member, "Scale")) {
                    if (const cJSON *const x = cJSON_GetObjectItem(scale, "X"))
                        item->scaleX = static_cast<float>(x->valuedouble);
                    if (const cJSON *const y = cJSON_GetObjectItem(scale, "Y"))
                        item->scaleY = static_cast<float>(y->valuedouble);
                } else {
                    item->scaleX = 1.0f;
                    item->scaleY = 1.0f;
                }
                const cJSON *const id = cJSON_GetObjectItem(member, "Id");
                std::snprintf(item->id.data(), item->id.size(), "%s", id && id->valuestring ? id->valuestring : "");
                item->window = window;
            }
            AddViewWidget(slot, 13, index);
        } else if (!std::strcmp(name, "SpaceEnd")) {
            AddViewWidget(slot, 14, -1);
        }
    }
}

template void ReadLayout(cJSON *, std::int32_t, std::array<UILayoutItem, kUILayoutItemCount> &, UIViewSlot<kS1UIViewImageCapacity> &);
template void ReadLayout(cJSON *, std::int32_t, std::array<UILayoutItem, kUILayoutItemCount> &, UIViewSlot<kS2UIViewImageCapacity> &);
template std::int32_t AddViewWidget(UIViewSlot<kS1UIViewImageCapacity> &, std::int32_t, std::int32_t);
template std::int32_t AddViewWidget(UIViewSlot<kS2UIViewImageCapacity> &, std::int32_t, std::int32_t);
template std::int32_t FindViewImage(const UIViewSlot<kS1UIViewImageCapacity> &, const char *);
template std::int32_t FindViewImage(const UIViewSlot<kS2UIViewImageCapacity> &, const char *);
template void ReadSprites(cJSON *, std::array<UISpriteSheet, kUISpriteSheetCount> &, const UIViewSlot<kS1UIViewImageCapacity> &);
template void ReadSprites(cJSON *, std::array<UISpriteSheet, kUISpriteSheetCount> &, const UIViewSlot<kS2UIViewImageCapacity> &);

} // namespace shenmue::d3t
