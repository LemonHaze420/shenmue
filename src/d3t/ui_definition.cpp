#include "d3t/ui_definition.hpp"
#include "d3t/ui_view.hpp"

#include <cJSON.h>
#include <json/json.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <limits>
#include <string>
#include <string_view>

namespace shenmue::d3t {
namespace {

bool ParseJson(std::string_view text, Json::Value &value) {
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    builder["rejectDupKeys"] = false;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errors;
    return reader->parse(text.data(), text.data() + text.size(), &value, &errors);
}

std::size_t FindObjectEnd(std::string_view text, std::size_t begin) {
    std::size_t depth = 0;
    bool quoted = false;
    bool escaped = false;
    for (std::size_t index = begin; index < text.size(); ++index) {
        const char character = text[index];
        if (quoted) {
            if (escaped) {
                escaped = false;
            } else if (character == '\\') {
                escaped = true;
            } else if (character == '"') {
                quoted = false;
            }
            continue;
        }
        if (character == '"') {
            quoted = true;
        } else if (character == '{') {
            ++depth;
        } else if (character == '}' && --depth == 0) {
            return index + 1;
        }
    }
    return std::string_view::npos;
}

const Json::Value &FindAuthoredMember(const Json::Value &object, std::string_view name) {
    for (Json::ValueConstIterator member = object.begin(); member != object.end(); ++member) {
        const std::string key = member.name();
        if (key.size() != name.size())
            continue;
        bool equal = true;
        for (std::size_t index = 0; index < name.size(); ++index) {
            const char a = key[index] >= 'A' && key[index] <= 'Z' ? key[index] + ('a' - 'A') : key[index];
            const char b = name[index] >= 'A' && name[index] <= 'Z' ? name[index] + ('a' - 'A') : name[index];
            if (a != b) {
                equal = false;
                break;
            }
        }
        if (equal)
            return *member;
    }
    return Json::Value::nullSingleton();
}

UIAlign ParseAlign(const Json::Value &axis) {
    const std::string value = axis.get("Align", "Centre").asString();
    if (value == "Top" || value == "Left" || value == "None")
        return UIAlign::Top;
    if (value == "Bottom" || value == "Right")
        return UIAlign::Bottom;
    if (value == "BottomOut" || value == "RightOut")
        return UIAlign::BottomOut;
    if (value == "TopOut" || value == "LeftOut")
        return UIAlign::TopOut;
    if (value == "CentreRight")
        return UIAlign::CentreRight;
    if (value == "CentreLeft")
        return UIAlign::CentreLeft;
    if (value == "CentreUp")
        return UIAlign::CentreUp;
    if (value == "CentreDown")
        return UIAlign::CentreDown;
    return UIAlign::Centre;
}

UIAxisDefinition ParseAxis(const Json::Value &axis) {
    return {axis.get("Pos", 0.0).asFloat(), axis.get("Offset", 0.0).asFloat(), ParseAlign(axis)};
}

std::uint32_t ReadAuthoredFontColour(const Json::Value &value, std::uint32_t colour) {
    const std::size_t count = value.isArray() || value.isObject() ? value.size() : 0;
    if (count == 3 || count == 4) {
        std::array<const Json::Value *, 4> components{};
        std::size_t used = 0;
        for (const Json::Value &component : value) {
            std::size_t index = used++;
            while (index > 0 && components[index - 1]->getOffsetStart() > component.getOffsetStart()) {
                components[index] = components[index - 1];
                --index;
            }
            components[index] = &component;
        }
        std::array<double, 4> values{};
        for (std::size_t index = 0; index < count; ++index)
            values[index] = components[index]->isNumeric() ? components[index]->asDouble() : 0.0;
        colour = 0xff000000U;
        for (std::size_t index = 0; index < count; ++index) {
            const double value255 = values[index] * 255.0;
            double channel = value255 < 255.0 ? value255 : values[index];
            if (values[index] == 1.0) {
                bool normalized = index == 3;
                for (std::size_t other = 0; other < 3; ++other)
                    if (other != index && values[other] * 255.0 < 255.0)
                        normalized = true;
                if (normalized)
                    channel = 255.0;
            }
            const std::uint32_t byte = channel < 0.0 ? 0U : channel > 255.0 ? 255U : static_cast<std::uint32_t>(channel);
            const std::uint32_t shift = static_cast<std::uint32_t>(index * 8U);
            colour = (colour & ~(0xffU << shift)) | (byte << shift);
        }
        return colour;
    }
    if (!value.isString())
        return colour;
    const std::string text = value.asString();
    if (text.empty() || text[0] != '#')
        return colour;
    std::array<char, 9> hex{};
    for (std::size_t index = 0; index < 8; ++index) {
        const char c = index + 1 < text.size() ? text[index + 1] : '\0';
        hex[index] = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ? c : 'F';
    }
    const std::uint32_t rgba = static_cast<std::uint32_t>(std::strtoul(hex.data(), nullptr, 16));
    return ((rgba & 0xff000000U) >> 24U) | ((rgba & 0x00ff0000U) >> 8U) | ((rgba & 0x0000ff00U) << 8U) | ((rgba & 0x000000ffU) << 24U);
}

void FindRenderable(const Json::Value &value, UIPanelDefinition &panel) {
    if (!value.isObject())
        return;
    if (value.isMember("ButtonIcon"))
        return;
    if (value.isMember("ColourImage")) {
        const Json::Value &image = value["ColourImage"];
        panel.imageName = image.get("Image", "").asString();
        panel.objectId = image.get("Id", "").asString();
        const Json::Value &colour = image["Colour"];
        panel.imageOpacity = static_cast<float>(std::clamp(colour.get("A", 255).asInt(), 0, 255)) / 255.0f;
        return;
    }
    if (value.isMember("Image")) {
        const Json::Value &image = value["Image"];
        if (image.isString()) {
            panel.imageName = image.asString();
        } else if (image.isObject()) {
            panel.imageName = image.get("Image", "").asString();
            panel.objectId = image.get("Id", "").asString();
        }
        if (!panel.imageName.empty())
            return;
    }
    for (const std::string &name : value.getMemberNames()) {
        FindRenderable(value[name], panel);
        if (!panel.imageName.empty())
            return;
    }
}

void ReadSpriteSheets(std::string_view text, const Json::Value &root, std::vector<UISpriteSheetDefinition> &sheets) {
    const Json::Value &table = root["SpriteSheets"];
    for (const auto &value : table) {
        UISpriteSheetDefinition sheet;
        sheet.imageName = value.get("Image", "").asString();
        sheet.width = value.get("Width", 0.0).asFloat();
        sheet.height = value.get("Height", 0.0).asFloat();
        if (value.get("Type", "").asString() == "Static") {
            const float width = value.get("X", 0.0).asFloat();
            const float height = value.get("Y", 0.0).asFloat();
            if (width > 0.0f && height > 0.0f) {
                const auto columns = static_cast<std::int32_t>(std::clamp(sheet.width / width, 0.0f, 64.0f));
                const auto rows = static_cast<std::int32_t>(std::clamp(sheet.height / height, 0.0f, 64.0f));
                std::size_t index = 0;
                for (std::int32_t y = 0; y < rows && index < 64; ++y) {
                    for (std::int32_t x = 0; x < columns && index < 64; ++x) {
                        sheet.frames[index++] = {x * width, y * height, width, height};
                    }
                }
            }
        } else if (value.get("Type", "").asString() == "Custom") {
            const auto count = static_cast<std::size_t>(std::clamp(value.get("Count", 0).asInt(), 0, 64));


            const Json::Value &uv = value["UV"];
            std::size_t cursor = static_cast<std::size_t>(uv.getOffsetStart()) + 1;
            const std::size_t limit = static_cast<std::size_t>(uv.getOffsetLimit());
            std::size_t index = 0;
            bool quoted = false;
            while (cursor < limit && cursor < text.size() && index < count) {
                const char character = text[cursor++];
                if (quoted && character == '\\') {
                    ++cursor;
                    continue;
                }
                if (character == '"') {
                    quoted = !quoted;
                    continue;
                }
                if (quoted || character != '{')
                    continue;
                const std::size_t begin = cursor - 1;
                const std::size_t end = FindObjectEnd(text, begin);
                if (end == std::string_view::npos || end > limit)
                    break;
                Json::Value frame;
                if (!ParseJson(text.substr(begin, end - begin), frame))
                    break;
                cursor = end;
                sheet.frames[index++] = {
                    frame.get("X", 0.0).asFloat(), frame.get("Y", 0.0).asFloat(), frame.get("Width", 0.0).asFloat(), frame.get("Height", 0.0).asFloat()};
            }
        }
        sheets.push_back(std::move(sheet));
    }
}

void ReadTextureObjects(std::string_view text, const UIPanelDefinition &panel, std::vector<UITextureObjectDefinition> &objects, bool includeColourButtons) {
    constexpr std::array<std::string_view, 5> keys{"\"Sprite\"", "\"ColourImage\"", "\"Image\"", "\"ButtonIcon\"", "\"Button\""};
    std::size_t cursor = 0;
    while ((cursor = text.find("\"Objects\"", cursor)) != std::string_view::npos) {
        const auto begin = text.find('{', cursor);
        if (begin == std::string_view::npos)
            break;
        const auto end = FindObjectEnd(text, begin);
        if (end == std::string_view::npos)
            break;
        Json::Value layout;
        const auto push = text.rfind("\"SpacePush\"", cursor);
        if (push != std::string_view::npos) {
            const auto pushBegin = text.find('{', push);
            const auto pushEnd = FindObjectEnd(text, pushBegin);
            if (pushEnd != std::string_view::npos)
                ParseJson(text.substr(pushBegin, pushEnd - pushBegin), layout);
        }
        std::size_t itemCursor = begin;
        while (itemCursor < end) {
            auto at = std::string_view::npos;
            std::size_t kind = 0;
            for (std::size_t index = 0; index < (includeColourButtons ? keys.size() : keys.size() - 1); ++index) {
                const auto candidate = text.find(keys[index], itemCursor);
                if (candidate < at) {
                    at = candidate;
                    kind = index;
                }
            }
            if (at >= end)
                break;
            const auto colon = text.find(':', at + keys[kind].size());
            const auto valueBegin = text.find_first_not_of(" \t\r\n", colon + 1);
            if (valueBegin >= end)
                break;
            auto valueEnd = valueBegin + 1;
            if (text[valueBegin] == '{') {
                valueEnd = FindObjectEnd(text, valueBegin);
            } else if (text[valueBegin] == '"') {
                bool escaped = false;
                for (; valueEnd < end; ++valueEnd) {
                    if (!escaped && text[valueEnd] == '"') {
                        ++valueEnd;
                        break;
                    }
                    escaped = !escaped && text[valueEnd] == '\\';
                }
            }
            if (valueEnd > end)
                break;
            Json::Value value;
            if (ParseJson(text.substr(valueBegin, valueEnd - valueBegin), value) && (value.isObject() || value.isString())) {
                UITextureObjectDefinition object;
                object.kind = kind == 0 ? UITextureObjectKind::Sprite :
                    kind == 3           ? UITextureObjectKind::ButtonIcon :
                    kind == 4           ? UITextureObjectKind::ColourButton :
                                          UITextureObjectKind::Image;
                object.panelId = panel.id;
                if (value.isString()) {
                    object.imageName = value.asString();
                } else {
                    object.id = value.get("Id", "").asString();
                    object.imageName = value.get("Image", "").asString();
                    object.frameIndex = value.get("Index", 0).asInt();
                    object.useShared = value.get("UseShared", 0).asInt() > 0;
                    if (object.kind == UITextureObjectKind::ButtonIcon) {
                        object.action = value.get("Action", "").asString();
                        object.iconFontSize = value.get("IconFontSize", "BS").asString();
                        object.textAlignment = value.get("Alignment", "Centre").asString();
                        object.controlMethod = value.get("ControlMethod", -1).asInt();
                        object.controlState = value.get("ControlState", -1).asInt();
                        object.allowDoubleWidth = value.get("AllowDoubleWidth", 1).asInt();
                        object.fontColour = ReadAuthoredFontColour(value["FontColour"], object.fontColour);
                    }
                    object.rotation = value.get("Rotate", 0.0).asFloat() * (3.1415927f / 180.0f);
                    object.rounding = value.get("Rounding", 0.0).asFloat();
                    const Json::Value &colour = value["Colour"];
                    if (!colour.isNull()) {
                        constexpr std::array<const char *, 4> channels{"R", "G", "B", "A"};
                        object.colour = 0;
                        for (Json::ArrayIndex channel = 0; channel < 4; ++channel) {
                            const auto component =
                                static_cast<std::uint8_t>(colour.isArray() ? colour[channel].asInt() : colour.get(channels[channel], 255).asInt());
                            object.colour |= static_cast<std::uint32_t>(component) << (channel * 8U);
                        }
                    }
                }
                object.width = layout.get("Width", panel.width).asFloat();
                object.layoutId = layout.get("Id", "").asString();
                object.height = layout.get("Height", panel.height).asFloat();
                object.x = ParseAxis(layout["X"]);
                object.y = ParseAxis(layout["Y"]);
                object.scaleStyle = layout.get("ScaleStyle", "").asString();
                object.scaleX = layout["Scale"].get("X", 1.0).asFloat();
                object.fullscreen = layout.get("Fullscreen", 0).asInt() != 0;
                object.scaleY = layout["Scale"].get("Y", 1.0).asFloat();
                object.paddingX = layout["Padding"].get("X", 0.0).asFloat();
                object.paddingY = layout["Padding"].get("Y", 0.0).asFloat();
                objects.push_back(std::move(object));
            }
            itemCursor = valueEnd;
        }
        cursor = end;
    }
}

template <std::size_t Capacity> std::array<const Json::Value *, Capacity> OrderedChildren(const Json::Value &object) {
    std::array<const Json::Value *, Capacity> children{};
    std::size_t count = 0;
    for (const Json::Value &child : object) {
        if (count == Capacity)
            break;
        children[count++] = &child;
    }
    std::sort(children.begin(), children.begin() + count,
        [](const Json::Value *first, const Json::Value *second) { return first->getOffsetStart() < second->getOffsetStart(); });
    return children;
}

bool ReadGaugeDefinitions(std::string_view text, const Json::Value &root, UIDefinition &definition) {
    const auto imageSlot = [&](const std::string &name) -> std::int32_t {
        for (std::size_t index = 0; index < definition.images.size(); ++index)
            if (definition.images[index].name == name)
                return static_cast<std::int32_t>(index);
        return -1;
    };


    std::string backgroundName;
    Json::Value::Int64 firstOffset = std::numeric_limits<Json::Value::Int64>::max();
    for (const auto &name : root["Images"].getMemberNames()) {
        const auto offset = root["Images"][name].getOffsetStart();
        if (offset < firstOffset) {
            firstOffset = offset;
            backgroundName = name;
        }
    }
    std::size_t cursor = 0;
    while ((cursor = text.find("\"Gauge\"", cursor)) != std::string_view::npos) {
        cursor += 7;
        const auto colon = text.find(':', cursor);
        if (colon == std::string_view::npos)
            break;
        const auto begin = text.find_first_not_of(" \t\r\n", colon + 1);
        if (begin == std::string_view::npos)
            break;
        if (text[begin] != '{')
            continue;
        const auto end = FindObjectEnd(text, begin);
        if (end == std::string_view::npos)
            return false;
        Json::Value object;
        if (!ParseJson(text.substr(begin, end - begin), object))
            return false;
        cursor = end;
        if (definition.gauges.size() == 5)
            return false;
        UIGaugeDefinition gauge;
        gauge.panelId = object["Panel"].get("Id", "").asString();
        const auto *panel = definition.findPanel(gauge.panelId);
        if (!panel)
            return false;
        auto &state = gauge.state;
        state.allocated = 1;
        state.panelIndex = static_cast<std::int32_t>(panel - definition.panels.data());
        state.images.fill(-1);
        state.backgroundImage = imageSlot(backgroundName);
        state.count = object.get("Count", 0).asInt();
        if (state.count < 0 || state.count > 12 || object["X"].size() > 12 || object["Y"].size() > 12)
            return false;
        // 401CC0 uses linked-list child ordinal (34C5F0), not numeric keys.
        // GameHudGauge.ui deliberately lists the keys 11 down to 0.
        const auto x = OrderedChildren<12>(object["X"]);
        const auto y = OrderedChildren<12>(object["Y"]);
        for (std::int32_t index = 0; index < state.count; ++index) {
            if (!x[index] || !y[index])
                return false;
            state.positions[index] = {x[index]->asFloat() / panel->width, y[index]->asFloat() / panel->height};
        }
        if (object["Image"].size() == 9) {
            const auto images = OrderedChildren<9>(object["Image"]);
            for (std::size_t index = 0; index < images.size(); ++index)
                state.images[index] = imageSlot(images[index]->asString());
        }
        state.width = object.get("Width", 0.0).asFloat() / panel->width;
        state.height = object.get("Height", 0.0).asFloat() / panel->height;
        const auto &colour = object["Colour"];
        state.colour = (colour.get("R", 255).asUInt() & 255U) | ((colour.get("G", 255).asUInt() & 255U) << 8U) |
            ((colour.get("B", 255).asUInt() & 255U) << 16U) | ((colour.get("A", 255).asUInt() & 255U) << 24U);
        for (std::size_t index = 0; index < state.orbColours.size(); ++index)
            state.orbColours[index] = index < 8 ? 2 : index < 11 ? 3 : 1;
        definition.gauges.push_back(std::move(gauge));
    }
    return true;
}

} // namespace

bool UIDefinition::parse(const std::byte *data, std::size_t size, bool includeColourButtons) {
    images.clear();
    document.reset();
    variables = {};
    panels.clear();
    labels.clear();
    textureObjects.clear();
    spriteSheets.clear();
    gauges.clear();
    if (!data || size == 0)
        return false;

    const std::string_view text(reinterpret_cast<const char *>(data), size);
    Json::Value root;
    if (!ParseJson(text, root))
        return false;
    if (includeColourButtons) {
        const std::string terminatedText(text);
        document = std::shared_ptr<cJSON>(cJSON_Parse(terminatedText.c_str()), cJSON_Delete);
        if (!document)
            return false;
        ReadVariables(document.get(), variables);
    }

    const Json::Value &imageTable = root["Images"];
    std::vector<std::string> imageNames = imageTable.getMemberNames();
    if (includeColourButtons) {
        for (std::size_t index = 1; index < imageNames.size(); ++index) {
            std::size_t before = index;
            while (before && imageTable[imageNames[before]].getOffsetStart() < imageTable[imageNames[before - 1]].getOffsetStart()) {
                imageNames[before].swap(imageNames[before - 1]);
                --before;
            }
        }
    }
    for (const std::string &name : imageNames) {
        if (imageTable[name].isString()) {
            images.push_back({name, imageTable[name].asString(), 0});
        }
    }
    ReadSpriteSheets(text, root, spriteSheets);

    constexpr std::string_view panelKey = "\"Panel\"";
    std::size_t cursor = 0;
    while ((cursor = text.find(panelKey, cursor)) != std::string_view::npos) {
        const std::size_t begin = text.find('{', cursor + panelKey.size());
        if (begin == std::string_view::npos)
            break;
        const std::size_t end = FindObjectEnd(text, begin);
        if (end == std::string_view::npos)
            break;

        Json::Value object;
        if (ParseJson(text.substr(begin, end - begin), object)) {
            UIPanelDefinition panel;
            panel.id = object.get("Id", "").asString();
            panel.parentId = includeColourButtons ? FindAuthoredMember(object, "ParentID").asString() : object.get("ParentID", "").asString();
            panel.width = object.get("Width", 0.0).asFloat();
            panel.height = object.get("Height", 0.0).asFloat();
            panel.x = ParseAxis(object["X"]);
            panel.y = ParseAxis(object["Y"]);
            panel.scaleStyle = object.get("ScaleStyle", "").asString();
            panel.scaleX = object["Scale"].get("X", 1.0).asFloat();
            panel.scaleY = object["Scale"].get("Y", 1.0).asFloat();
            panel.paddingX = object["Padding"].get("X", 0.0).asFloat();
            panel.paddingY = object["Padding"].get("Y", 0.0).asFloat();
            FindRenderable(object["Objects"], panel);

            const std::string_view panelText = text.substr(begin, end - begin);
            const std::size_t objectsAt = panelText.find("\"Objects\"");
            const std::size_t pushAt = objectsAt == std::string_view::npos ? std::string_view::npos : panelText.rfind("\"SpacePush\"", objectsAt);
            if (pushAt != std::string_view::npos) {
                const std::size_t pushBegin = panelText.find('{', pushAt);
                const std::size_t pushEnd = FindObjectEnd(panelText, pushBegin);
                Json::Value push;
                if (pushEnd != std::string_view::npos && ParseJson(panelText.substr(pushBegin, pushEnd - pushBegin), push)) {
                    panel.drawWidth = push.get("Width", panel.width).asFloat();
                    panel.drawHeight = push.get("Height", panel.height).asFloat();
                    panel.drawX = ParseAxis(push["X"]);
                    panel.drawY = ParseAxis(push["Y"]);
                }
            }




            std::size_t labelCursor = 0;
            while ((labelCursor = panelText.find("\"Label\"", labelCursor)) != std::string_view::npos) {
                const std::size_t labelBegin = panelText.find('{', labelCursor);
                if (labelBegin == std::string_view::npos)
                    break;
                const std::size_t labelEnd = FindObjectEnd(panelText, labelBegin);
                if (labelEnd == std::string_view::npos)
                    break;
                Json::Value labelObject;
                if (ParseJson(panelText.substr(labelBegin, labelEnd - labelBegin), labelObject)) {
                    UILabelDefinition label;
                    label.panelId = panel.id;
                    label.panelIndex = static_cast<std::int32_t>(panels.size());
                    label.text = labelObject.get("Text", "").asString();
                    label.alignment = labelObject.get("Alignment", "").asString();
                    label.id = labelObject.get("Id", "").asString();
                    label.token = labelObject.get("Token", "").asString();
                    label.fontName = labelObject.get("FontName", "").asString();
                    label.fontSize = labelObject.get("Size", "").asString();
                    label.colour = ReadAuthoredFontColour(labelObject["Colour"], label.colour);
                    label.hasStroke = !labelObject["Stroke"].isNull();
                    label.strokeColour = ReadAuthoredFontColour(labelObject["Stroke"], label.strokeColour);
                    const std::string scaleVar = labelObject.get("ScaleVar", "").asString();
                    if (!scaleVar.empty())
                        label.textScale = root["Variables"][scaleVar].get("Value", 1.0).asFloat();
                    if (includeColourButtons) {
                        label.scaleVarAssign = labelObject.get("ScaleVarAssign", 1).asInt();
                        for (std::size_t index = 0; index < variables.size(); ++index)
                            if (variables[index].used && scaleVar == variables[index].name) {
                                label.scaleVar = static_cast<std::int32_t>(index);
                                break;
                            }
                    }
                    label.width = panel.width;
                    label.height = panel.height;
                    const std::size_t layoutAt = panelText.rfind("\"SpacePush\"", labelCursor);
                    if (layoutAt != std::string_view::npos) {
                        const std::size_t layoutBegin = panelText.find('{', layoutAt);
                        const std::size_t layoutEnd = FindObjectEnd(panelText, layoutBegin);
                        Json::Value layout;
                        if (layoutEnd != std::string_view::npos && ParseJson(panelText.substr(layoutBegin, layoutEnd - layoutBegin), layout)) {
                            label.layoutId = layout.get("Id", "").asString();
                            label.width = layout.get("Width", panel.width).asFloat();
                            label.height = layout.get("Height", panel.height).asFloat();
                            label.x = ParseAxis(layout["X"]);
                            label.y = ParseAxis(layout["Y"]);
                            label.scaleStyle = layout.get("ScaleStyle", "").asString();
                            label.scaleX = layout["Scale"].get("X", 1.0).asFloat();
                            label.scaleY = layout["Scale"].get("Y", 1.0).asFloat();
                        }
                    }
                    labels.push_back(std::move(label));
                }
                labelCursor = labelEnd;
            }
            ReadTextureObjects(panelText, panel, textureObjects, includeColourButtons);
            panels.push_back(std::move(panel));
        }
        cursor = end;
    }
    return ReadGaugeDefinitions(text, root, *this) && !panels.empty();
}

const UIPanelDefinition *UIDefinition::findPanel(const std::string &id) const {
    const auto found = std::find_if(panels.begin(), panels.end(), [&id](const UIPanelDefinition &panel) { return panel.id == id; });
    return found == panels.end() ? nullptr : &*found;
}

const UIImageDefinition *UIDefinition::findImage(const std::string &name) const {
    const auto found = std::find_if(images.begin(), images.end(), [&name](const UIImageDefinition &image) { return image.name == name; });
    return found == images.end() ? nullptr : &*found;
}

} // namespace shenmue::d3t
