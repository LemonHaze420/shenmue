#include "s1/ui_definition.hpp"
#include "s1/archive_data.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/d3t/settings.hpp"
#include "logging.hpp"

#include "s1/mip_streaming.hpp"
#include "s1/input.hpp"
#include "s1/render_passes.hpp"

#include <json/json.h>

#include <algorithm>
#include <cctype>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string_view>

namespace shenmue::s1 {
namespace {

bool parseJson(std::string_view text, Json::Value &value) {
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    builder["rejectDupKeys"] = false;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    std::string errors;
    return reader->parse(text.data(), text.data() + text.size(), &value, &errors);
}


const Json::Value &memberNoCase(const Json::Value &object, const char *name) {
    for (const std::string &member : object.getMemberNames()) {
        std::size_t index = 0;
        while (member[index] && name[index] && std::tolower(static_cast<unsigned char>(member[index])) == std::tolower(static_cast<unsigned char>(name[index])))
            ++index;
        if (!member[index] && !name[index])
            return object[member];
    }
    return Json::Value::nullSingleton();
}

std::size_t objectEnd(std::string_view text, std::size_t begin) {
    std::size_t depth = 0;
    bool quoted = false;
    bool escaped = false;
    for (std::size_t i = begin; i < text.size(); ++i) {
        const char c = text[i];
        if (quoted) {
            if (escaped)
                escaped = false;
            else if (c == '\\')
                escaped = true;
            else if (c == '"')
                quoted = false;
            continue;
        }
        if (c == '"')
            quoted = true;
        else if (c == '{')
            ++depth;
        else if (c == '}' && --depth == 0)
            return i + 1;
    }
    return std::string_view::npos;
}



std::vector<std::pair<std::string_view, std::size_t>> orderedMembers(std::string_view text, std::size_t begin) {
    std::vector<std::pair<std::string_view, std::size_t>> members;
    if (begin >= text.size() || text[begin] != '{')
        return members;
    std::size_t depth = 0;
    for (std::size_t i = begin; i < text.size(); ++i) {
        const char c = text[i];
        if (c == '"') {
            std::size_t end = i + 1;
            while (end < text.size() && text[end] != '"')
                end += text[end] == '\\' ? 2 : 1;
            std::size_t colon = end + 1;
            while (colon < text.size() && (text[colon] == ' ' || text[colon] == '\t' || text[colon] == '\r' || text[colon] == '\n'))
                ++colon;
            if (depth == 1 && colon < text.size() && text[colon] == ':')
                members.emplace_back(text.substr(i + 1, end - i - 1), colon + 1);
            i = end;
        } else if (c == '{' || c == '[') {
            ++depth;
        } else if (c == '}' || c == ']') {
            if (--depth == 0)
                break;
        }
    }
    return members;
}

// The offset of the object value of the member named key in the object at begin, or npos.
std::size_t memberObject(std::string_view text, std::size_t begin, std::string_view key) {
    for (const std::pair<std::string_view, std::size_t> &member : orderedMembers(text, begin)) {
        if (member.first == key) {
            const std::size_t value = text.find_first_not_of(" \t\r\n", member.second);
            return value != std::string_view::npos && text[value] == '{' ? value : std::string_view::npos;
        }
    }
    return std::string_view::npos;
}

UIAlign parseAlign(const Json::Value &axis) {
    const std::string value = axis.get("Align", "Centre").asString();
    if (value == "Top")
        return UIAlign::Top;
    if (value == "Bottom")
        return UIAlign::Bottom;
    if (value == "BottomOut")
        return UIAlign::BottomOut;
    if (value == "TopOut")
        return UIAlign::TopOut;
    if (value == "Left")
        return UIAlign::Left;
    if (value == "Right")
        return UIAlign::Right;
    if (value == "LeftOut")
        return UIAlign::LeftOut;
    if (value == "RightOut")
        return UIAlign::RightOut;
    if (value == "CentreLeft")
        return UIAlign::CentreLeft;
    if (value == "CentreRight")
        return UIAlign::CentreRight;
    if (value == "CentreUp")
        return UIAlign::CentreUp;
    if (value == "CentreDown")
        return UIAlign::CentreDown;
    if (value == "Centre")
        return UIAlign::Centre;


    return UIAlign::Left;
}

UIAxisDefinition parseAxis(const Json::Value &axis) {
    return {axis.get("Pos", 0.0).asFloat(), axis.get("Offset", 0.0).asFloat(), parseAlign(axis)};
}

void findRenderable(const Json::Value &value, UIPanelDefinition &panel) {
    if (!value.isObject())
        return;
    if (value.isMember("ColourImage")) {
        const Json::Value &image = value["ColourImage"];
        panel.imageName = image.get("Image", "").asString();
        panel.objectId = image.get("Id", "").asString();
        return;
    }
    if (value.isMember("Image")) {
        const Json::Value &image = value["Image"];
        if (image.isString()) {
            panel.imageName = image.asString();
            panel.objectId = value.get("Id", "").asString();
            panel.spriteIndex = value.get("Index", -1).asInt();
            panel.buttonIcon = value.isMember("Action");
        } else if (image.isObject()) {
            panel.imageName = image.get("Image", "").asString();
            panel.objectId = image.get("Id", "").asString();
            panel.spriteIndex = image.get("Index", -1).asInt();
        }
        if (!panel.imageName.empty())
            return;
    }
    for (const std::string &name : value.getMemberNames()) {
        findRenderable(value[name], panel);
        if (!panel.imageName.empty())
            return;
    }
}


std::string stringMember(const Json::Value &object, const char *name) {
    if (!object.isObject())
        return {};
    const Json::Value &value = object[name];
    return value.isString() ? value.asString() : std::string{};
}

float floatMember(const Json::Value &object, const char *name, float fallback) {
    if (!object.isObject())
        return fallback;
    const Json::Value &value = object[name];
    return value.isNumeric() ? value.asFloat() : fallback;
}

std::int32_t intMember(const Json::Value &object, const char *name, std::int32_t fallback) {
    if (!object.isObject())
        return fallback;
    const Json::Value &value = object[name];
    return value.isNumeric() ? static_cast<std::int32_t>(value.asDouble()) : fallback;
}

UIAxisDefinition axisMember(const Json::Value &object, const char *name) {
    const Json::Value &axis = object.isObject() ? object[name] : Json::Value::nullSingleton();
    Json::Value align(Json::objectValue);
    align["Align"] = stringMember(axis, "Align");
    if (align["Align"].asString().empty())
        align["Align"] = "Centre";
    return {floatMember(axis, "Pos", 0.0f), floatMember(axis, "Offset", 0.0f), parseAlign(align)};
}

std::uint32_t parseColour(const Json::Value &colour) {
    constexpr std::array<const char *, 4> channels{"R", "G", "B", "A"};
    std::uint32_t packed = 0;
    for (std::size_t index = 0; index < channels.size(); ++index)
        packed |= (static_cast<std::uint32_t>(intMember(colour, channels[index], 255)) & 0xffU) << (8U * index);
    return packed;
}



std::uint32_t parseHexColour(const std::string &text) {
    std::array<char, 9> digits{};
    bool more = true;
    for (std::size_t index = 0; index < 8; ++index) {
        char digit = 'F';
        if (more) {
            const char character = index + 1 < text.size() ? text[index + 1] : '\0';
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
    return (value >> 24U) | (((value >> 16U) & 0xffU) << 8U) | (((value >> 8U) & 0xffU) << 16U) | ((value & 0xffU) << 24U);
}




std::uint32_t parseColourArray(const Json::Value &colour) {
    std::array<std::uint32_t, 4> bytes{255, 255, 255, 255};
    for (Json::ArrayIndex channel = 0; channel < colour.size() && channel < 4; ++channel) {
        const double value = colour[channel].asDouble();
        std::int32_t byte = static_cast<std::int32_t>(value);
        bool saturate = false;
        if (value * 255.0 >= 255.0) {
            if (value == 1.0) {
                saturate = channel == 3;
                for (Json::ArrayIndex other = 0; !saturate && other < 3; ++other)
                    saturate = other != channel && colour[other].asDouble() * 255.0 < 255.0;
            }
        } else {
            byte = static_cast<std::int32_t>(value * 255.0);
        }
        bytes[channel] = saturate || byte > 255 ? 255U : byte < 0 ? 0U : static_cast<std::uint32_t>(byte);
    }
    return bytes[0] | (bytes[1] << 8U) | (bytes[2] << 16U) | (bytes[3] << 24U);
}




std::uint32_t parseStyleColour(const Json::Value &item) {
    if (!item.isObject() || memberNoCase(item, "Type").asString() != "Colour")
        return 0;
    const Json::Value &formatValue = memberNoCase(item, "Format");
    const std::string format = formatValue.isString() ? formatValue.asString() : std::string{};
    const auto channel = [&](const char *name) {
        const Json::Value &value = memberNoCase(item, name);
        return value.isNumeric() ? static_cast<std::uint32_t>(static_cast<std::int32_t>(value.asDouble())) : 0U;
    };
    std::array<std::uint32_t, 4> rgba{};
    if (format == "RGBA") {
        rgba = {channel("RGBA"), channel("RGBA"), channel("RGBA"), channel("RGBA")};
    } else if (format == "RGB") {
        rgba = {channel("RGB"), channel("RGB"), channel("RGB"), 255};
    } else if (format == "RGB_A") {
        rgba = {channel("RGB"), channel("RGB"), channel("RGB"), channel("A")};
    } else if (format == "R_G_B_A") {
        rgba = {channel("R"), channel("G"), channel("B"), channel("A")};
    }
    return (rgba[0] & 0xffU) | ((rgba[1] & 0xffU) << 8U) | ((rgba[2] & 0xffU) << 16U) | ((rgba[3] & 0xffU) << 24U);
}

void parseObjects(
    std::string_view panelText, const std::string &panelId, std::vector<UIObjectDefinition> &objects, std::vector<UIObjectDefinition> &buttonIcons) {
    constexpr std::string_view objectsKey = "\"Objects\"";
    constexpr std::string_view pushKey = "\"SpacePush\"";
    std::size_t cursor = 0;
    std::size_t previousEnd = 0;
    while ((cursor = panelText.find(objectsKey, cursor)) != std::string_view::npos) {
        const std::size_t begin = panelText.find('{', cursor + objectsKey.size());
        const std::size_t end = begin == std::string_view::npos ? std::string_view::npos : objectEnd(panelText, begin);
        if (end == std::string_view::npos)
            break;
        UIObjectDefinition layout;
        layout.panelId = panelId;
        const std::size_t pushAt = panelText.rfind(pushKey, cursor);
        if (pushAt != std::string_view::npos && pushAt >= previousEnd) {
            const std::size_t pushBegin = panelText.find('{', pushAt);
            const std::size_t pushEnd = pushBegin == std::string_view::npos ? std::string_view::npos : objectEnd(panelText, pushBegin);
            Json::Value push;
            if (pushEnd != std::string_view::npos && parseJson(panelText.substr(pushBegin, pushEnd - pushBegin), push)) {
                layout.scaleStyle = stringMember(push, "ScaleStyle");
                layout.width = floatMember(push, "Width", 0.0f);
                layout.height = floatMember(push, "Height", 0.0f);
                layout.x = axisMember(push, "X");
                layout.y = axisMember(push, "Y");
                layout.scaleX = floatMember(push["Scale"], "X", 1.0f);
                layout.scaleY = floatMember(push["Scale"], "Y", 1.0f);
                layout.paddingX = floatMember(push["Padding"], "X", 0.0f);
                layout.paddingY = floatMember(push["Padding"], "Y", 0.0f);
                layout.fullscreen = intMember(push, "Fullscreen", 0) != 0;
            }
        }
        Json::Value members;
        if (parseJson(panelText.substr(begin, end - begin), members) && members.isObject()) {
            for (const std::string &name : members.getMemberNames()) {
                const Json::Value &value = members[name];
                UIObjectDefinition object = layout;
                if (name == "Image" && value.isString()) {
                    object.imageName = value.asString();
                } else if (name == "Image" || name == "Sprite" || name == "ColourImage") {
                    object.kind = name == "Sprite" ? UIObjectKind::Sprite : name == "ColourImage" ? UIObjectKind::ColourImage : UIObjectKind::Image;
                    object.imageName = stringMember(value, "Image");
                    object.id = stringMember(value, "Id");
                    object.spriteIndex = intMember(value, "Index", -1);
                    object.rotation = floatMember(value, "Rotate", 0.0f);
                    object.alpha = floatMember(value, "Alpha", 1.0f);
                    if (value.isObject() && value["Colour"].isObject())
                        object.colour = parseColour(value["Colour"]);
                } else if (name == "Label" || name == "IconLabel") {
                    object.kind = UIObjectKind::Label;
                    object.id = stringMember(value, "Id");
                    object.text = stringMember(value, "Text");
                    object.token = stringMember(value, "Token");
                    const Json::Value &alignment = value.isObject() ? memberNoCase(value, "Alignment") : Json::Value::nullSingleton();
                    object.textAlignment = ReadAlignmentValue(alignment.isString() ? alignment.asCString() : nullptr);


                    if (name == "Label" && value.isObject()) {
                        const Json::Value &scaleVar = memberNoCase(value, "ScaleVar");
                        if (scaleVar.isString())
                            object.scaleVarName = scaleVar.asString();
                        const Json::Value &scaleVarAssign = memberNoCase(value, "ScaleVarAssign");
                        if (scaleVarAssign.isNumeric())
                            object.scaleVarAssign = static_cast<std::int32_t>(scaleVarAssign.asDouble());
                    }
                    object.size = stringMember(value, "Size");
                    constexpr std::array<std::string_view, 6> kLabelSizeNames{"H1", "H2", "H3", "BL", "B", "BS"};
                    for (std::size_t index = 0; index < kLabelSizeNames.size(); ++index) {
                        if (object.size == kLabelSizeNames[index])
                            object.fontSize = static_cast<std::int32_t>(index);
                    }

                    std::string fontName = stringMember(value, "FontName");
                    for (char &character : fontName)
                        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
                    object.fontFamily = fontName == "uistandardfontbold" ? 1 : fontName == "uiloadscreenfont" ? 2 : fontName == "uinotepadfont" ? 3 : 0;
                    const Json::Value &labelColour = value.isObject() ? value["Colour"] : Json::Value::nullSingleton();
                    if (labelColour.isObject())
                        object.colour = parseColour(labelColour);
                    else if (labelColour.isArray() && (labelColour.size() == 3 || labelColour.size() == 4))
                        object.colour = parseColourArray(labelColour);
                    else if (name == "Label" && labelColour.isString() && labelColour.asString()[0] == '#')
                        object.colour = parseHexColour(labelColour.asString());
                    const Json::Value &stroke = value.isObject() ? value["Stroke"] : Json::Value::nullSingleton();
                    if (stroke.isObject()) {
                        object.hasStroke = true;
                        object.strokeColour = parseColour(stroke);
                    } else if (stroke.isString() && stroke.asString()[0] == '#') {
                        object.hasStroke = true;
                        object.strokeColour = parseHexColour(stroke.asString());
                    }
                } else if (name == "Button") {
                    object.kind = UIObjectKind::Button;
                    object.id = stringMember(value, "Id");
                    object.alpha = floatMember(value, "Alpha", 1.0f);
                    if (value.isObject() && value["Colour"].isObject())
                        object.colour = parseColour(value["Colour"]);
                } else if (name == "ButtonIcon") {
                    object.kind = UIObjectKind::ButtonIcon;
                    object.imageName = stringMember(value, "Image");
                    object.id = stringMember(value, "Id");
                    const std::string action = stringMember(value, "Action");
                    for (std::size_t index = 0; index < kInputActionNames.size(); ++index) {
                        if (action == kInputActionNames[index])
                            object.action = static_cast<std::int32_t>(index);
                    }
                    if (value.isObject() && value.isMember("IconFontSize")) {
                        const std::string size = stringMember(value, "IconFontSize");
                        object.iconFontSize = 4;
                        for (std::size_t index = 0; index < kIconFontSizeNames.size(); ++index) {
                            if (size == kIconFontSizeNames[index])
                                object.iconFontSize = static_cast<std::int32_t>(index);
                        }
                    }

                    const Json::Value &fontColour = value.isObject() ? value["FontColour"] : Json::Value::nullSingleton();
                    if (fontColour.isObject())
                        object.fontColour = parseColour(fontColour);
                    else if (fontColour.isString() && fontColour.asString()[0] == '#')
                        object.fontColour = parseHexColour(fontColour.asString());
                    const Json::Value &alignment = value.isObject() ? memberNoCase(value, "Alignment") : Json::Value::nullSingleton();
                    object.textAlignment = ReadAlignmentValue(alignment.isString() ? alignment.asCString() : nullptr);
                    object.allowDoubleWidth = intMember(value, "AllowDoubleWidth", 1);
                    object.controlMethod = intMember(value, "ControlMethod", -1);
                    object.controlState = intMember(value, "ControlState", -1);
                    object.rotation = floatMember(value, "Rotate", 0.0f);
                    if (value.isObject() && value["Colour"].isObject())
                        object.colour = parseColour(value["Colour"]);
                    buttonIcons.push_back(std::move(object));
                    continue;
                } else {
                    continue;
                }
                objects.push_back(std::move(object));
            }
        }
        previousEnd = end;
        cursor = end;
    }
}

} // namespace



std::uint32_t ReadAlignmentValue(const char *alignment) {
    if (!alignment)
        return UITextAlignMiddle | UITextAlignCentred;
    if (!std::strcmp(alignment, "Centre"))
        return UITextAlignMiddle | UITextAlignCentred;
    if (!std::strcmp(alignment, "Left"))
        return UITextAlignMiddle | UITextAlignLeft;
    if (!std::strcmp(alignment, "Right"))
        return UITextAlignMiddle | UITextAlignRight;
    if (!std::strcmp(alignment, "LeftTop"))
        return UITextAlignTop | UITextAlignLeft;
    if (!std::strcmp(alignment, "LeftDown"))
        return UITextAlignBottom | UITextAlignLeft;
    if (!std::strcmp(alignment, "RightTop"))
        return UITextAlignTop | UITextAlignRight;
    if (!std::strcmp(alignment, "RightDown"))
        return UITextAlignBottom | UITextAlignRight;
    return UITextAlignMiddle | UITextAlignCentred;
}

float UIAlignedPosition(UIAlign align, float parentPosition, float parentSize, float size, float authoredPosition, float offset) {
    switch (align) {
    case UIAlign::Top:
    case UIAlign::Left:
        return parentPosition + authoredPosition + offset;
    case UIAlign::Bottom:
    case UIAlign::Right:
        return parentPosition + parentSize - size + authoredPosition + offset;
    case UIAlign::BottomOut:
    case UIAlign::RightOut:
        return parentPosition + parentSize + authoredPosition + offset;
    case UIAlign::TopOut:
    case UIAlign::LeftOut:
        return parentPosition - size + authoredPosition + offset;
    case UIAlign::CentreLeft:
    case UIAlign::CentreUp:
        return parentPosition + parentSize * 0.5f - size + authoredPosition + offset;
    case UIAlign::CentreRight:
    case UIAlign::CentreDown:
        return parentPosition + parentSize * 0.5f + authoredPosition + offset;
    case UIAlign::Centre:
    default:
        return parentPosition + (parentSize - size) * 0.5f + authoredPosition + offset;
    }
}

bool UIDefinition::parse(const std::byte *data, std::size_t size) {
    images.clear();
    spriteSheets.clear();
    panels.clear();
    objects.clear();
    buttonIcons.clear();
    if (!data || !size)
        return false;
    const std::string_view text(reinterpret_cast<const char *>(data), size);

    Json::Value root;
    if (!parseJson(text, root))
        return false;
    const Json::Value &imageTable = root["Images"];
    for (const std::string &name : imageTable.getMemberNames()) {
        if (imageTable[name].isString()) {
            images.push_back({name, imageTable[name].asString(), 0});
        }
    }
    const Json::Value &spriteSheetTable = root["SpriteSheets"];
    const std::size_t sheetsAt = text.find("\"SpriteSheets\"");
    const std::size_t sheetsBegin = sheetsAt == std::string_view::npos ? std::string_view::npos : text.find('{', sheetsAt);
    for (const std::string &name : spriteSheetTable.getMemberNames()) {
        const Json::Value &sheet = spriteSheetTable[name];
        if (!sheet.isObject())
            continue;
        UISpriteSheetDefinition definition;
        definition.imageName = sheet.get("Image", "").asString();
        definition.textureWidth = sheet.get("Width", 0.0).asFloat();
        definition.textureHeight = sheet.get("Height", 0.0).asFloat();
        definition.cellWidth = sheet.get("X", 0.0).asFloat();
        definition.cellHeight = sheet.get("Y", 0.0).asFloat();

        const Json::Value &uv = sheet["UV"];
        if (uv.isObject()) {
            const std::size_t uvBegin = memberObject(text, memberObject(text, sheetsBegin, name), "UV");
            const std::vector<std::pair<std::string_view, std::size_t>> frameNames = orderedMembers(text, uvBegin);
            const std::size_t count = sheet.isMember("Count") ? sheet["Count"].asUInt() : frameNames.size();
            for (std::size_t index = 0; index < count && index < frameNames.size() && index < 64; ++index) {
                const Json::Value &frame = uv[std::string(frameNames[index].first)];
                definition.frames.push_back(
                    {floatMember(frame, "X", 0.0f), floatMember(frame, "Y", 0.0f), floatMember(frame, "Width", 0.0f), floatMember(frame, "Height", 0.0f)});
            }
        }
        if (!definition.imageName.empty() && definition.textureWidth > 0.0f && definition.textureHeight > 0.0f &&
            ((definition.cellWidth > 0.0f && definition.cellHeight > 0.0f) || !definition.frames.empty())) {
            spriteSheets.push_back(std::move(definition));
        }
    }

    constexpr std::string_view key = "\"Panel\"";
    std::size_t cursor = 0;
    while ((cursor = text.find(key, cursor)) != std::string_view::npos) {
        const std::size_t begin = text.find('{', cursor + key.size());
        if (begin == std::string_view::npos)
            break;
        const std::size_t end = objectEnd(text, begin);
        if (end == std::string_view::npos)
            break;
        Json::Value object;
        if (parseJson(text.substr(begin, end - begin), object)) {
            UIPanelDefinition panel;
            panel.id = object.get("Id", "").asString();
            panel.parentId = memberNoCase(object, "ParentID").asString();
            panel.width = object.get("Width", 0.0).asFloat();
            panel.height = object.get("Height", 0.0).asFloat();
            panel.x = parseAxis(object["X"]);
            panel.y = parseAxis(object["Y"]);
            panel.scaleStyle = stringMember(object, "ScaleStyle");
            panel.scaleX = floatMember(object["Scale"], "X", 1.0f);
            panel.scaleY = floatMember(object["Scale"], "Y", 1.0f);
            panel.paddingX = floatMember(object["Padding"], "X", 0.0f);
            panel.paddingY = floatMember(object["Padding"], "Y", 0.0f);
            panel.windowBackground = parseStyleColour(memberNoCase(memberNoCase(memberNoCase(object, "Style"), "Window"), "FixedBackground"));
            findRenderable(object["Objects"], panel);
            const std::string_view panelText = text.substr(begin, end - begin);
            parseObjects(panelText, panel.id, objects, buttonIcons);
            const std::size_t objectsAt = panelText.find("\"Objects\"");
            const std::size_t pushAt = objectsAt == std::string_view::npos ? std::string_view::npos : panelText.rfind("\"SpacePush\"", objectsAt);
            if (pushAt != std::string_view::npos) {
                const std::size_t pushBegin = panelText.find('{', pushAt);
                const std::size_t pushEnd = objectEnd(panelText, pushBegin);
                Json::Value push;
                if (pushEnd != std::string_view::npos && parseJson(panelText.substr(pushBegin, pushEnd - pushBegin), push)) {
                    panel.drawWidth = push.get("Width", panel.width).asFloat();
                    panel.drawHeight = push.get("Height", panel.height).asFloat();
                    panel.drawX = parseAxis(push["X"]);
                    panel.drawY = parseAxis(push["Y"]);
                    panel.drawFullscreen = intMember(push, "Fullscreen", 0) != 0;
                    panel.drawScaleStyle = stringMember(push, "ScaleStyle");
                }
            }
            panels.push_back(std::move(panel));
        }
        cursor = end;
    }
    if (std::any_of(panels.begin(), panels.end(), [](const UIPanelDefinition &panel) { return panel.buttonIcon && panel.imageName == "ButtonIcons"; }) &&
        !findImage("ButtonIcons")) {
        images.push_back({
            "ButtonIcons",
            "/ui/common/buttons/DefaultButtonSpriteSheet.tga",
            0,
        });
    }



    gauges.clear();
    const std::size_t imagesAt = text.find("\"Images\"");
    const std::vector<std::pair<std::string_view, std::size_t>> authoredImages =
        orderedMembers(text, imagesAt == std::string_view::npos ? std::string_view::npos : text.find('{', imagesAt));
    const auto imageSlot = [&](std::string_view name) {
        for (std::size_t index = 0; index < images.size(); ++index)
            if (images[index].name == name)
                return static_cast<std::int32_t>(index);
        return -1;
    };
    constexpr std::string_view gaugeKey = "\"Gauge\"";
    for (std::size_t at = 0; (at = text.find(gaugeKey, at)) != std::string_view::npos && gauges.size() < 5;) {
        at += gaugeKey.size();
        const std::size_t colon = text.find_first_not_of(" \t\r\n", at);
        const std::size_t begin = colon != std::string_view::npos && text[colon] == ':' ? text.find_first_not_of(" \t\r\n", colon + 1) : colon;
        if (begin == std::string_view::npos || text[begin] != '{')
            continue;
        const std::size_t end = objectEnd(text, begin);
        Json::Value object;
        if (end == std::string_view::npos || !parseJson(text.substr(begin, end - begin), object))
            break;
        at = end;
        UIGaugeDefinition gauge{.panelId = object["Panel"].get("Id", "").asString()};
        const UIPanelDefinition *const panel = findPanel(gauge.panelId);
        d3t::UIGaugeState &state = gauge.state;
        state.allocated = 1;
        state.panelIndex = panel ? static_cast<std::int32_t>(panel - panels.data()) : -1;
        state.backgroundImage = authoredImages.empty() ? -1 : imageSlot(authoredImages.front().first);
        state.count = intMember(object, "Count", 0);
        const float width = panel ? panel->width : 1920.0f;
        const float height = panel ? panel->height : 1080.0f;
        const std::vector<std::pair<std::string_view, std::size_t>> xs = orderedMembers(text, memberObject(text, begin, "X"));
        const std::vector<std::pair<std::string_view, std::size_t>> ys = orderedMembers(text, memberObject(text, begin, "Y"));
        for (std::int32_t index = 0; index < state.count && index < 12; ++index) {
            const std::size_t slot = static_cast<std::size_t>(index);
            if (slot < xs.size())
                state.positions[slot][0] = object["X"][std::string(xs[slot].first)].asFloat() / width;
            if (slot < ys.size())
                state.positions[slot][1] = object["Y"][std::string(ys[slot].first)].asFloat() / height;
        }
        const std::vector<std::pair<std::string_view, std::size_t>> gaugeImages = orderedMembers(text, memberObject(text, begin, "Image"));
        if (gaugeImages.size() == 9)
            for (std::size_t index = 0; index < 9; ++index)
                state.images[index] = imageSlot(object["Image"][std::string(gaugeImages[index].first)].asString());
        state.width = floatMember(object, "Width", 0.0f) / width;
        state.height = floatMember(object, "Height", 0.0f) / height;
        state.colour = parseColour(object["Colour"]);
        gauges.push_back(std::move(gauge));
    }
    resolveLayout();
    return !panels.empty();
}

void UIDefinition::resolveLayout() {
    std::unordered_map<std::string_view, std::size_t> panelIndices;
    panelIndices.reserve(panels.size());
    for (std::size_t index = 0; index < panels.size(); ++index) {
        if (!panels[index].id.empty()) {
            panelIndices.emplace(panels[index].id, index);
        }
    }
    std::vector<std::uint8_t> layoutState(panels.size());
    const auto resolvePanel = [&](const auto &self, std::size_t index) -> void {
        if (layoutState[index] == 2)
            return;
        if (layoutState[index] == 1) {
            layoutState[index] = 2;
            return;
        }
        layoutState[index] = 1;
        float parentX = 0.0f;
        float parentY = 0.0f;
        float parentWidth = 1920.0f;
        float parentHeight = 1080.0f;
        UIPanelDefinition &panel = panels[index];
        const auto parent = panelIndices.find(panel.parentId);
        if (parent != panelIndices.end() && parent->second != index) {
            self(self, parent->second);
            const UIPanelDefinition &parentPanel = panels[parent->second];
            parentX = parentPanel.resolvedX;
            parentY = parentPanel.resolvedY;
            parentWidth = parentPanel.width;
            parentHeight = parentPanel.height;
        }
        panel.resolvedX = UIAlignedPosition(panel.x.align, parentX, parentWidth, panel.width, panel.x.position, panel.x.offset);
        panel.resolvedY = UIAlignedPosition(panel.y.align, parentY, parentHeight, panel.height, panel.y.position, panel.y.offset);
        panel.layoutResolved = true;
        layoutState[index] = 2;
    };
    for (std::size_t index = 0; index < panels.size(); ++index) {
        resolvePanel(resolvePanel, index);
    }
}



std::uint32_t LoadTextureFromFilepath_Wrapper(const char *path, std::uint32_t renderHandle) {
    const std::string authored(path);
    std::string requestPath = authored.find('?') == std::string::npos ? authored + "?usage=0" : authored;
    if (g_dbg_is_jp_ver != 0) {
        const std::size_t query = requestPath.find('?');
        const std::size_t extension = requestPath.rfind('.', query);
        if (extension != std::string::npos && extension > 0) {
            const std::string localizedAsset =
                requestPath.substr(0, extension) + "_jp" + requestPath.substr(extension, query == std::string::npos ? std::string::npos : query - extension);
            if (ArchiveData_GetFileSize(localizedAsset.c_str()) != 0)
                requestPath = localizedAsset + "?usage=0";
        }
    }
    std::uint32_t textureHandle = 0;
    d3t::LoadTextureFromFilepath(requestPath.c_str(), 0, &textureHandle, renderHandle);
    return textureHandle;
}

void UIDefinition::requestTextures(std::uint32_t renderHandle) {
    for (UIImageDefinition &image : images) {
        if (!image.textureHandle && !image.path.empty())
            image.textureHandle = LoadTextureFromFilepath_Wrapper(image.path.c_str(), renderHandle);
    }
}

const UIPanelDefinition *UIDefinition::findPanel(const std::string &id) const {
    const auto it = std::find_if(panels.begin(), panels.end(), [&id](const UIPanelDefinition &panel) { return panel.id == id; });
    return it == panels.end() ? nullptr : &*it;
}

const UIObjectDefinition *UIDefinition::findObject(const std::string &id) const {
    for (const UIObjectDefinition &object : objects) {
        if (object.id == id)
            return &object;
    }
    return nullptr;
}

const UIImageDefinition *UIDefinition::findImage(const std::string &name) const {
    const auto it = std::find_if(images.begin(), images.end(), [&name](const UIImageDefinition &image) { return image.name == name; });
    return it == images.end() ? nullptr : &*it;
}

std::uint32_t UIDefinition::draw(
    D3TDeviceContext *context, std::uint32_t renderTarget, float screenWidth, float screenHeight, float opacity, float offsetX, float offsetY) const {
    const float safeScale = 0.8f + std::clamp(g_currentSettings.display.uiDisplayArea, 0.0f, 1.0f) * 0.2f;
    const float scaleX = screenWidth * safeScale / 1920.0f;
    const float scaleY = screenHeight * safeScale / 1080.0f;
    const float originX = screenWidth * (1.0f - safeScale) * 0.5f;
    const float originY = screenHeight * (1.0f - safeScale) * 0.5f;
    struct Rect {
        float x;
        float y;
        float w;
        float h;
    };
    std::uint32_t submitted = 0;
    for (const UIPanelDefinition &panel : panels) {
        const Rect rect{panel.resolvedX, panel.resolvedY, panel.width, panel.height};
        Rect drawRect = rect;
        if (panel.drawWidth > 0.0f && panel.drawHeight > 0.0f) {
            drawRect.w = panel.drawWidth;
            drawRect.h = panel.drawHeight;


            if (rect.w > 0.0f && rect.h > 0.0f && (panel.drawScaleStyle == "Fill" || panel.drawScaleStyle == "Fit")) {
                const float panelAspect = rect.h / rect.w;
                const float itemAspect = panel.drawHeight / panel.drawWidth;
                const bool heightBound = panel.drawScaleStyle == "Fit" ? panelAspect < itemAspect : itemAspect < panelAspect;
                if (panelAspect == itemAspect) {
                    drawRect.w = rect.w;
                    drawRect.h = rect.h;
                } else if (heightBound) {
                    drawRect.h = rect.h;
                    drawRect.w = rect.h / panel.drawHeight * panel.drawWidth;
                } else {
                    drawRect.w = rect.w;
                    drawRect.h = rect.w / panel.drawWidth * panel.drawHeight;
                }
            } else if (panel.drawScaleStyle == "Stretch") {
                drawRect.w = rect.w;
                drawRect.h = rect.h;
            }
            drawRect.x = UIAlignedPosition(panel.drawX.align, rect.x, rect.w, drawRect.w, panel.drawX.position, panel.drawX.offset);
            drawRect.y = UIAlignedPosition(panel.drawY.align, rect.y, rect.h, drawRect.h, panel.drawY.position, panel.drawY.offset);
        }
        drawRect.x += offsetX;
        drawRect.y += offsetY;
        const UIImageDefinition *image = findImage(panel.imageName);
        if (!image || !image->textureHandle)
            continue;
        const std::uint32_t alpha = static_cast<std::uint32_t>(std::clamp(opacity * panel.opacity, 0.0f, 1.0f) * 255.0f + 0.5f);
        NuklearQuad quad{originX + drawRect.x * scaleX, originY + drawRect.y * scaleY, drawRect.w * scaleX, drawRect.h * scaleY, 0.0f, 0.0f, 1.0f, 1.0f,
            (alpha << 24U) | 0x00ffffffU};
        if (panel.buttonIcon) {
            const bool keyboard = g_keyboardActive == 1;
            const float u0 = keyboard ? 640.0f / 1024.0f : 0.0f;
            const float v0 = keyboard ? 384.0f / 1024.0f : 128.0f / 1024.0f;
            if (keyboard) {
                quad.y += quad.height * 0.25f;
                quad.height *= 0.5f;
            }
            // The icon art keeps its texture colour; the authored FontColour belongs to the key label drawn over it.
            quad.u0 = u0;
            quad.v0 = v0;
            quad.u1 = u0 + 0.125f;
            quad.v1 = v0 + 0.125f;
        } else if (panel.spriteIndex >= 0) {
            const auto sheet = std::find_if(spriteSheets.begin(), spriteSheets.end(),
                [&panel](const UISpriteSheetDefinition &candidate) { return candidate.imageName == panel.imageName; });
            if (sheet == spriteSheets.end() || sheet->cellWidth <= 0.0f)
                continue;
            const std::int32_t columns = static_cast<std::int32_t>(sheet->textureWidth / sheet->cellWidth);
            if (columns <= 0)
                continue;
            const std::int32_t column = panel.spriteIndex % columns;
            const std::int32_t row = panel.spriteIndex / columns;
            quad.u0 = static_cast<float>(column) * sheet->cellWidth / sheet->textureWidth;
            quad.v0 = static_cast<float>(row) * sheet->cellHeight / sheet->textureHeight;
            quad.u1 = quad.u0 + sheet->cellWidth / sheet->textureWidth;
            quad.v1 = quad.v0 + sheet->cellHeight / sheet->textureHeight;
        } else if (panel.drawFullscreen && panel.drawWidth > 0.0f && panel.drawHeight > 0.0f) {

            quad.width = screenWidth;
            quad.height = screenHeight;
            if (panel.drawHeight / panel.drawWidth < screenHeight / screenWidth)
                quad.width = screenHeight / panel.drawHeight * panel.drawWidth;
            else if (panel.drawHeight / panel.drawWidth > screenHeight / screenWidth)
                quad.height = screenWidth / panel.drawWidth * panel.drawHeight;
            quad.x = (screenWidth - quad.width) * 0.5f;
            quad.y = (screenHeight - quad.height) * 0.5f;
        }
        // Nuklear scissors a window's contents to its padded bounds.
        const float clipX0 = originX + (rect.x + offsetX + panel.paddingX) * scaleX;
        const float clipY0 = originY + (rect.y + offsetY + panel.paddingY) * scaleY;
        const float clipX1 = originX + (rect.x + offsetX + rect.w - panel.paddingX) * scaleX;
        const float clipY1 = originY + (rect.y + offsetY + rect.h - panel.paddingY) * scaleY;
        const float x0 = std::max(quad.x, clipX0);
        const float y0 = std::max(quad.y, clipY0);
        const float x1 = std::min(quad.x + quad.width, clipX1);
        const float y1 = std::min(quad.y + quad.height, clipY1);
        if (x1 <= x0 || y1 <= y0 || quad.width <= 0.0f || quad.height <= 0.0f)
            continue;
        const float uPerPixel = (quad.u1 - quad.u0) / quad.width;
        const float vPerPixel = (quad.v1 - quad.v0) / quad.height;
        const NuklearQuad clipped{x0, y0, x1 - x0, y1 - y0, quad.u0 + (x0 - quad.x) * uPerPixel, quad.v0 + (y0 - quad.y) * vPerPixel,
            quad.u0 + (x1 - quad.x) * uPerPixel, quad.v0 + (y1 - quad.y) * vPerPixel, quad.colour};
        const bool drew = DrawNuklearQuads(context, renderTarget, image->textureHandle, std::span{&clipped, 1});
        submitted += drew ? 1U : 0U;
    }
    return submitted;
}

} // namespace shenmue::s1
