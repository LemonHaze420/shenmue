#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {



void GameMessage_ApplyPanelAnim(float progress) {
    const std::array<d3t::UIWindow *, 3> &windows = g_gameMessagePresentation.windows;
    if (!windows[0] || !windows[1] || !windows[2])
        return;
    UILayoutRect start{};
    UI_GetWindowRect(windows[1], start);
    UILayoutRect end{};
    UI_GetWindowRect(windows[2], end);
    const float remaining = 1.0f - progress;
    d3t::UIWindowLayout &layout = windows[0]->layout;
    layout.offsetX = (end.x * progress + -start.width * remaining) / static_cast<float>(UI_GetAspectWidth());
    layout.offsetY = (start.y * remaining + end.y * progress) / 1080.0f;
    layout.width.value.fraction = (end.width * progress + start.width * remaining) / 1920.0f;
    layout.height.value.fraction = (start.height * remaining + end.height * progress) / 1080.0f;
}




std::int64_t GameMessage_IsSelected() {
    UIGameMessagePresentation &message = g_gameMessagePresentation;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameMessage].config.uiHandleIndex);
    for (std::size_t window = 0; window < message.windows.size(); ++window)
        message.windows[window] = UI_GetWindow(UI_FindViewWindow(slot, kGameMessageWindowIds[window]));
    message.label = UI_LabelRecord(UI_FindViewLabel(slot, "UIGameMessage::Label"));
    message.placeholderSprite = UI_ButtonIconRecord(UI_FindViewIconWidget(20, slot, "UIGameMessage::PlaceholderSprite"));
    for (std::size_t image = 0; image < message.images.size(); ++image)
        message.images[image] = UI_ColourImageRecord(UI_FindViewColourImage(slot, kGameMessageImageIds[image]));
    if (message.label)
        message.label->text = message.labelText.data();
    message.animSlot = UIAnimAllocSlot();
    GameMessage_ApplyPanelAnim(0.0f);
    if (slot <= 75U)
        g_uiViewSlots[slot].keepLoaded = 1;
    return 1;
}



std::int64_t GameMessage_Tick() {
    const std::int32_t anim = g_gameMessagePresentation.animSlot;
    if (anim != -1)
        GameMessage_ApplyPanelAnim(g_uiAnimSlots[static_cast<std::size_t>(anim)].current[0]);
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameMessage].config.uiHandleIndex);
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}



std::int64_t GameMessage_Reset() {
    UIGameMessagePresentation &message = g_gameMessagePresentation;
    message.windows[0] = nullptr;
    message.windows[1] = nullptr;
    message.label = nullptr;
    message.images = {};
    return 0;
}

// STATIC_Game_Message_Render: the element's activation slide, 0 -> 1 over half a second.
void GameMessage_Render() {
    const std::int32_t slotIndex = g_gameMessagePresentation.animSlot;
    if (slotIndex == -1)
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
    slot.start[0] = 0.0f;
    slot.current[0] = 0.0f;
    slot.delay = 0.0f;
    slot.elapsed = 0.0f;
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 2;
    slot.end[0] = 1.0f;
    slot.duration = 0.5f;
    slot.reserved40 = -1;
    if (g_uiNavigateSoundFrame + 5 <= g_FrameCounter) {
        g_uiNavigateSoundFrame = g_FrameCounter;
        shenExecuteAICACommand(kUiNavigateSound, 0, 0);
    }
}


std::int64_t GameMessage_Destroy() {
    const std::int32_t slotIndex = g_gameMessagePresentation.animSlot;
    if (slotIndex != -1) {
        UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(slotIndex)];
        slot.end[0] = 0.0f;
        slot.delay = 0.0f;
        slot.elapsed = 0.0f;
        slot.allocated = 1;
        slot.status = 1;
        slot.interpolation = 2;
        slot.start[0] = 1.0f;
        slot.current[0] = 1.0f;
        slot.duration = 0.5f;
        slot.reserved40 = -1;
        if (g_uiNavigateSoundFrame + 5 <= g_FrameCounter) {
            g_uiNavigateSoundFrame = g_FrameCounter;
            shenExecuteAICACommand(kUiNavigateSound, 0, 0);
        }
    }
    return 1;
}

// The GameMessage slide slot is allocated and idle, the condition the popup and cash overlay owners wait on.
bool GameMessageAnimIdle() {
    const std::int32_t slot = g_gameMessagePresentation.animSlot;
    return slot != -1 && g_uiAnimSlots[static_cast<std::size_t>(slot)].status == 0;
}


std::string FormatGroupedNumber(std::int32_t value) {
    if (value == 0)
        return "0";
    const char separator = g_uiLoadedLanguage == 2 || g_uiLoadedLanguage == 3 ? ' ' : ',';
    std::string reversed;
    std::int32_t digits = 0;
    for (std::int32_t remaining = value; remaining >= 1; remaining /= 10) {
        if (digits != 0 && digits % 3 == 0)
            reversed.push_back(separator);
        reversed.push_back(static_cast<char>('0' + remaining % 10));
        ++digits;
    }
    return std::string(reversed.rbegin(), reversed.rend());
}



void GameMessage_SetContent(UIGameMessageContentType type, std::int32_t value, const char *text) {
    UIGameMessagePresentation &message = g_gameMessagePresentation;
    if (message.images[0])
        message.images[0]->alpha = 0.0f;
    if (message.images[1])
        message.images[1]->alpha = 0.0f;
    if (type <= UIGameMessageContentType::Cassette) {
        message.spriteIndex = value;
        std::snprintf(message.labelText.data(), message.labelText.size(), "%s", text);
    } else {
        message.spriteIndex = -1;
        if (type == UIGameMessageContentType::Money) {
            if (message.images[1])
                message.images[1]->alpha = 1.0f;
            std::snprintf(message.labelText.data(), message.labelText.size(), "%s", FormatCashOverlayText(value).c_str());
        } else {
            if (message.images[0])
                message.images[0]->alpha = 1.0f;
            if (type == UIGameMessageContentType::Token)
                std::snprintf(message.labelText.data(), message.labelText.size(), "%s", FormatGroupedNumber(value).c_str());
        }
    }
    if (UIButtonIconWidget *const sprite = message.placeholderSprite) {
        if (message.spriteIndex == -1) {
            sprite->fieldAC = 1;
        } else {
            sprite->sprite = message.spriteIndex;
            sprite->inventoryIcon = 1;
            sprite->fieldAC = 0;
        }
    }
    message.contentType = type;
}


void InformationBar_Render() {
    d3t::CopyStringToUILabel(nullptr);
}



std::int32_t UI_FindViewIconLabel(std::uint32_t slotIndex, const char *id) {
    if (slotIndex > 75U || !id)
        return -1;
    const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &slot = g_uiViewSlots[slotIndex];
    for (std::int32_t widget = 0; widget < slot.widgetCount; ++widget) {
        if (slot.widgetKinds[static_cast<std::size_t>(widget)] != 16)
            continue;
        const std::int32_t index = slot.widgetPoolIndices[static_cast<std::size_t>(widget)];
        if (static_cast<std::uint32_t>(index) >= g_uiIconLabelWidgets.size())
            continue;
        const char *const name = g_uiIconLabelWidgets[static_cast<std::size_t>(index)].id.data();
        if (name[0] && !std::strcmp(name, id))
            return index;
    }
    return -1;
}



std::int64_t InformationBar_IsSelected() {
    InformationBarLabels &labels = g_informationBarLabels;
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameMenuInformationBar].config.uiHandleIndex);
    for (std::size_t window = 0; window < g_informationBarWindows.size(); ++window)
        g_informationBarWindows[window] = UI_GetWindow(UI_FindViewWindow(slot, kInformationBarWindowIds[window]));
    for (std::size_t label = 0; label < g_informationBarSideLabels.size(); ++label)
        g_informationBarSideLabels[label] = UI_LabelRecord(UI_FindViewLabel(slot, kInformationBarLabelIds[label]));
    for (std::size_t label = 0; label < g_informationBarTextLabels.size(); ++label) {
        const std::int32_t index = UI_FindViewIconLabel(slot, kInformationBarIconLabelIds[label]);
        g_informationBarTextLabels[label] = static_cast<std::uint32_t>(index) > 0x3fU ? nullptr : &g_uiIconLabelWidgets[static_cast<std::size_t>(index)];
    }
    const std::array<char *, 2> sideTexts{labels.rightText.data(), labels.leftText.data()};
    for (std::size_t label = 0; label < sideTexts.size(); ++label) {
        if (d3t::UILabelWidget *const record = g_informationBarSideLabels[label]) {
            record->text = sideTexts[label];
            record->token.alreadyLocalized = 1;
        }
    }
    const std::array<char *, 3> lineTexts{labels.singleLine.data(), labels.firstLine.data(), labels.secondLine.data()};
    for (std::size_t label = 0; label < lineTexts.size(); ++label) {
        if (UIIconLabelWidget *const record = g_informationBarTextLabels[label]) {
            record->label.text = lineTexts[label];
            record->label.token.alreadyLocalized = 1;
        }
    }
    for (std::size_t icon = 0; icon < g_informationBarIcons.size(); ++icon) {
        const std::int32_t index = UI_FindViewImageWidget(slot, kInformationBarImageIds[icon]);
        g_informationBarIcons[icon] = static_cast<std::uint32_t>(index) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(index)] : nullptr;
    }
    for (std::size_t image = 0; image < g_informationBarBackgroundImages.size(); ++image)
        g_informationBarBackgroundImages[image] = UI_FindViewImage(slot, kInformationBarBackgroundImageIds[image]);
    return 1;
}



std::int64_t InformationBar_Reset() {
    g_informationBarWindows.fill(nullptr);
    g_informationBarSideLabels.fill(nullptr);
    g_informationBarIcons.fill(nullptr);
    return 0;
}



std::int64_t InformationBar_Tick() {
    InformationBarLabels &labels = g_informationBarLabels;
    if (labels.message.noOutline == 1 && g_informationBarWindows[6])
        g_informationBarWindows[6]->alpha = 0.0f;
    const float remaining = labels.messageSeconds;
    if (remaining > 0.0f) {
        const float frame = g_bEnableFrameTimeSmoothing ? fFrameTimeMs : 0.033333335f;
        labels.messageSeconds = remaining - frame;
        if (remaining - frame <= 0.0f) {
            labels.savedMessage.textLiteral = 1;
            labels.savedMessage.text = labels.savedText.data();
            d3t::CopyStringToUILabel(&labels.savedMessage);
        }
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiGameMenuInformationBar].config.uiHandleIndex);
    if (slot <= 75U)
        UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}




std::int64_t InformationBar_GetContentPanel() {
    const std::array<d3t::UIWindow *, 7> &windows = g_informationBarWindows;
    const bool inGame = g_isInGame != 0;
    if (const d3t::UIWindow *const parent = windows[inGame ? 2 : 0]; windows[5] && parent)
        std::snprintf(windows[5]->layout.parentId.data(), windows[5]->layout.parentId.size(), "%s", parent->id.data());
    if (windows[1])
        windows[1]->alpha = inGame ? 0.0f : 1.0f;
    if (windows[4])
        windows[4]->alpha = inGame ? 1.0f : 0.0f;
    if (windows[3])
        windows[3]->alpha = inGame ? 1.0f : 0.0f;
    const std::uint32_t stroke = inGame ? 0xc8000000U : 0xff000000U;
    for (d3t::UILabelWidget *const label : g_informationBarSideLabels) {
        if (label)
            label->strokeColour = stroke;
    }
    for (UIIconLabelWidget *const label : g_informationBarTextLabels) {
        if (label)
            label->label.strokeColour = stroke;
    }
    return 0;
}



std::int32_t DecodeUITextCharacter(const char *text, std::uint32_t *codepoint, std::int32_t size) {
    constexpr std::array<std::uint8_t, 5> kMasks{0xc0, 0x80, 0xe0, 0xf0, 0xf8};
    constexpr std::array<std::uint8_t, 5> kPatterns{0x80, 0x00, 0xc0, 0xe0, 0xf0};
    constexpr std::array<std::uint32_t, 5> kMinimum{0, 0, 0x80, 0x800, 0x10000};
    constexpr std::array<std::uint32_t, 5> kMaximum{0, 0x7f, 0x7ff, 0xffff, 0x10ffff};
    if (!text || !codepoint || !size)
        return 0;
    *codepoint = 0xfffd;
    const auto kindOf = [&](std::uint8_t byte) {
        std::int32_t kind = 0;
        while (kind < 5 && (byte & kMasks[static_cast<std::size_t>(kind)]) != kPatterns[static_cast<std::size_t>(kind)])
            ++kind;
        return kind;
    };
    const auto lead = static_cast<std::uint8_t>(text[0]);
    const std::int32_t length = kindOf(lead);
    std::uint32_t value = length < 5 ? static_cast<std::uint8_t>(lead & ~kMasks[static_cast<std::size_t>(length)]) : 0U;
    if (static_cast<std::uint32_t>(length - 1) > 3U)
        return 1;
    std::int32_t read = 1;
    while (read < length) {
        if (read >= size)
            return 0;
        const auto next = static_cast<std::uint8_t>(text[read]);
        const std::int32_t kind = kindOf(next);
        value = (value << 6) | (kind < 5 ? static_cast<std::uint8_t>(next & ~kMasks[static_cast<std::size_t>(kind)]) : 0U);
        if (kind)
            return read;
        ++read;
    }
    if (kMinimum[static_cast<std::size_t>(length)] > value || value > kMaximum[static_cast<std::size_t>(length)] || value - 0xd800U <= 0x7ffU)
        value = 0xfffd;
    *codepoint = value;
    return value <= 0x7f ? 1 : value <= 0x7ff ? 2 : value <= 0xffff ? 3 : 4;
}


std::int32_t FillStringData(const char *text, std::int32_t size) {
    if (!text || !size)
        return 0;
    std::uint32_t codepoint = 0;
    std::int32_t offset = 0;
    std::int32_t count = 0;
    for (std::int32_t length = DecodeUITextCharacter(text, &codepoint, size); length && offset < size;
         length = DecodeUITextCharacter(text + offset, &codepoint, size - offset)) {
        offset += length;
        ++count;
    }
    return count;
}


char *ParseGameTagsForBuffer(char *buffer, std::int32_t size, const char *tag) {
    const std::size_t tagLength = std::strlen(tag);
    char *match = buffer;
    char *cursor = buffer;
    std::size_t matched = 0;
    if (!*buffer)
        return nullptr;
    while (true) {
        std::uint32_t codepoint = 0;
        const std::int32_t length = DecodeUITextCharacter(cursor, &codepoint, size);
        if (codepoint != static_cast<std::uint32_t>(static_cast<std::int8_t>(tag[matched])))
            matched = 0;
        if (codepoint == static_cast<std::uint32_t>(static_cast<std::int8_t>(tag[matched]))) {
            if (++matched == 1)
                match = cursor;
            else if (matched == tagLength)
                return match;
        }
        cursor += length;
        if (!*cursor)
            return nullptr;
    }
}


std::int32_t TextFormat(char *buffer, std::int32_t size, const char *tag, const char *replacement) {
    char *const found = ParseGameTagsForBuffer(buffer, size, tag);
    if (!found)
        return 0;
    std::vector<char> copy(static_cast<std::size_t>(size));
    std::snprintf(copy.data(), copy.size(), "%s", buffer);
    const std::int32_t space = FillStringData(buffer, size) + static_cast<std::int32_t>(buffer - found);
    std::snprintf(found, static_cast<std::size_t>(space), "%s%s", replacement, copy.data() + (found - buffer) + std::strlen(tag));
    return 1;
}

// d3t_CopyStringToUILabel's reading of a message text: unless its literal byte is set it is a string id of up to 12 characters, localized once.
const char *LocalizeInformationBarText(const char *text, std::uint8_t &literal, std::array<char, 512> &storage) {
    if (literal)
        return text;
    SubtitleStringId id{};
    std::size_t length = 0;
    while (length < sizeof(id) && text[length])
        ++length;
    std::memcpy(&id, text, length);
    std::snprintf(storage.data(), storage.size(), "%s", ReadStringFromStringTableById(id));
    literal = 1;
    return storage.data();
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1::d3t {






void CopyStringToUILabel(const InformationBarMessage *message) {
    InformationBarLabels &labels = g_informationBarLabels;
    InformationBarMessage &current = labels.message;
    labels.messageSeconds = 0.0f;
    std::uint32_t colour = g_informationBarTextColour;
    if (message && message->colourOverride)
        std::memcpy(&colour, message->colour.data(), sizeof(colour));
    for (d3t::UILabelWidget *const label : g_informationBarSideLabels) {
        if (label)
            label->colour = colour;
    }
    for (UIIconLabelWidget *const label : g_informationBarTextLabels) {
        if (label)
            label->label.colour = colour;
    }
    d3t::UIWindow *const labelsPanel = g_informationBarWindows[5];
    if (message) {
        current = *message;
        if (labelsPanel)
            labelsPanel->alpha = 1.0f;
    } else {
        current.outlinePanel = -1;
        current.iconAction = -1;
        current.iconIndex = -1;
        current.text = nullptr;
        if (labelsPanel)
            labelsPanel->alpha = 0.0f;
    }
    labels.rightText[0] = 0;
    labels.leftText[0] = 0;
    labels.singleLine[0] = 0;
    labels.firstLine[0] = 0;
    labels.secondLine[0] = 0;
    if (current.text) {
        current.text = LocalizeInformationBarText(current.text, current.textLiteral, g_informationBarLocalizedTexts[0]);
        const char *const lineBreak = std::strstr(current.text, "\xa1\xf5");
        if (lineBreak) {
            const std::size_t first = static_cast<std::size_t>(lineBreak - current.text);
            std::snprintf(labels.firstLine.data(), labels.firstLine.size(), "%.*s", static_cast<int>(first), current.text);
            std::snprintf(labels.secondLine.data(), labels.secondLine.size(), "%.*s", static_cast<int>(std::strlen(current.text) - first - 2), lineBreak + 2);
        } else {
            std::snprintf(labels.singleLine.data(), labels.singleLine.size(), "%s", current.text);
        }
    }
    if (current.leftText) {
        current.leftText = LocalizeInformationBarText(current.leftText, current.leftTextLiteral, g_informationBarLocalizedTexts[1]);
        std::snprintf(labels.leftText.data(), labels.leftText.size(), "%s", current.leftText);
    }
    if (current.rightText) {
        current.rightText = LocalizeInformationBarText(current.rightText, current.rightTextLiteral, g_informationBarLocalizedTexts[2]);
        std::snprintf(labels.rightText.data(), labels.rightText.size(), "%s", current.rightText);
    }
    for (std::size_t side = 0; side < 2; ++side) {
        d3t::UIImageWidget *const icon = g_informationBarIcons[side];
        if (!icon)
            continue;
        icon->frame = current.iconAction;
        icon->image = current.iconIndex;
        if (icon->frame == -1 || current.iconIndex == -1)
            icon->field44 = 0;
    }
    d3t::UIWindow *const outline = g_informationBarWindows[6];
    if (!outline)
        return;
    outline->alpha = 0.0f;
    if (!message || current.noOutline)
        return;
    if (current.outlinePanel == -1) {
        if (current.outline[0] > 0.0f && current.outline[1] > 0.0f && current.outline[2] > 0.0f && current.outline[3] > 0.0f) {
            outline->layout.parentId[0] = 0;
            outline->alpha = 1.0f;
            outline->layout.scaleStyle = 4;
            outline->layout.alignX = 9;
            outline->layout.alignY = 6;
            outline->layout.offsetX = current.outline[0] / static_cast<float>(UI_GetAspectWidth());
            outline->layout.offsetY = current.outline[1] / 1080.0f;
            outline->layout.width.value.fraction = current.outline[2] / 1920.0f;
            outline->layout.height.value.fraction = current.outline[3] / 1080.0f;
        }
    } else if (static_cast<std::uint32_t>(current.outlinePanel) <= 0x3ffU) {
        const d3t::UIWindow &panel = g_uiWindows[static_cast<std::size_t>(current.outlinePanel)];
        std::snprintf(outline->layout.parentId.data(), outline->layout.parentId.size(), "%s", panel.id.data());
        outline->alpha = 1.0f;
        outline->layout.scaleStyle = 3;
        outline->layout.alignX = 1;
        outline->layout.alignY = 1;
        outline->layout.offsetX = 0.0f;
        outline->layout.offsetY = 0.0f;
    }
}



void ShowMessageStr(const InformationBarMessage *message, float seconds) {
    if (!message)
        return;
    InformationBarLabels &labels = g_informationBarLabels;
    if (labels.messageSeconds <= 0.0f) {
        labels.savedMessage = labels.message;
        if (labels.firstLine[0] && labels.secondLine[0])
            std::snprintf(labels.savedText.data(), labels.savedText.size(), "%s\xa1\xf5%s", labels.firstLine.data(), labels.secondLine.data());
        else
            std::snprintf(labels.savedText.data(), labels.savedText.size(), "%s", labels.singleLine.data());
    }
    d3t::CopyStringToUILabel(message);
    labels.messageSeconds = seconds;
}

} // namespace shenmue::s1::d3t
