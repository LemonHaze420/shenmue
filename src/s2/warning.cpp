#include "s2/warning.hpp"

#include "s2/font.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"

#include <cstring>

namespace shenmue::s2 {
namespace {

// Offset of the DDS pixel block behind the PTX TEXN and DDS headers.
constexpr std::size_t WARNING_TEXT_PIXELS = 0xac;

std::uint32_t warnBaseColour{};
std::uint32_t warnColour{};
std::uint32_t warnTargetColour{};
std::int32_t warnId{};
std::int32_t warnValue{};
WarningState warnState{};
std::int32_t warnRetireFrames{};
// The longest line in characters during layout, then the centred left edge in pixels.
std::int32_t warnLeft{};
const char *warnMessage{};
std::array<char, 56> warnLine{};
std::int16_t warnLineSlot{};
char *warnCursor{};
std::int16_t warnRow{};
std::int16_t warnOpenFrame{};
// Frame 0 boxes the message, 1 and 2 the Yes and No choices. Layout records columns/rows; the second pass converts them to
// pixel origins and sizes (right and bottom become width and height).
std::array<std::int16_t, 3> warnFrameLeft{};
std::array<std::int16_t, 3> warnFrameTop{};
std::array<std::int16_t, 3> warnFrameRight{};
std::array<std::int16_t, 3> warnFrameBottom{};
std::int32_t warnBlink{};
std::int32_t reconnectRequest{};
std::byte *warnText{};
std::array<std::int16_t, WARNING_TEXT_SLOTS> warnTextures{};
std::int16_t reconnectLineSlot{};
const char *reconnectMessage{};
std::byte *reconnectText{};
std::array<std::int16_t, WARNING_TEXT_SLOTS> reconnectTextures{};
std::uint32_t reconnectBaseColour{};
std::array<char, 56> reconnectLine{};
// Cleared by AllocateWARP and never read.
std::uint32_t reconnectColour{};
char *reconnectCursor{};
std::uint32_t reconnectTargetColour{};
WarningState reconnectState{};
std::int32_t reconnectRetireFrames{};
std::int32_t reconnectLeft{};


std::uint32_t RasterizeKanjiLineNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) {
    return 0;
}



void AdmitWarningTextBank(std::byte *text, shen_name owner, std::array<std::int16_t, WARNING_TEXT_SLOTS> &textures) {
    ParseGuiPtx(text, WARNING_TEXT_WIDTH, WARNING_TEXT_HEIGHT, owner.name, WARNING_TEXT_TEXTURE_ID.name);
    textures[28] = static_cast<std::int16_t>(ReadTexnTexture(text));
    for (std::int32_t line = 0; line < WARNING_LINE_TEXTURES; ++line) {
        const std::uint32_t id = make_shen_name('0', '0', '0', static_cast<char>('0' + line));
        std::memcpy(text + 0x0c, &id, sizeof(id));
        textures[static_cast<std::size_t>(line) * 2] = static_cast<std::int16_t>(ReadTexnTexture(text));
    }
    std::memset(text + 0x04, 0, 4);
    std::memset(text + 0x28, 0, 2);
    std::memset(text + 0x20, 0, 4);
    std::memset(text + 0x0c, 0, 4);
}

std::uint32_t UploadTextLine(std::byte *text, const std::array<std::int16_t, WARNING_TEXT_SLOTS> &textures, std::int16_t slot, const char *line) {
    if (!text || textures[slot] < 0)
        return 0;
    const std::uint32_t result = RasterizeKanjiLineNullsub(text, WARNING_TEXT_WIDTH, line, 0, 0);
    ReplacePtxTexture(static_cast<std::uint32_t>(textures[slot]), text);
    if (slot < 27 && textures[slot + 1] >= 0) {
        std::memset(text + WARNING_TEXT_PIXELS, 0, 0x8000);
        ReplacePtxTexture(static_cast<std::uint32_t>(textures[slot + 1]), text);
    }
    return result;
}

// @verified - p
std::uint32_t WRN_UploadReconnectLine(std::int16_t slot) {
    return UploadTextLine(reconnectText, reconnectTextures, slot, reconnectLine.data());
}


// @verified - p
std::uint32_t WRN_BlendColour(std::int32_t weight, std::uint32_t from, std::uint32_t to) {
    const std::uint32_t clamped = static_cast<std::uint32_t>(weight < 0 ? 0 : weight > 16 ? 16 : weight);
    std::uint32_t result = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const std::uint32_t source = from >> shift & 0xffU;
        const std::uint32_t target = to >> shift & 0xffU;
        const std::uint32_t channel = source > target ? target + ((source - target) * (16 - clamped) >> 4) : source + ((target - source) * clamped >> 4);
        result |= (channel & 0xffU) << shift;
    }
    return result;
}

// Native '%' / '@n' / '^' message layout into the WARN line textures and frame extents.
// @verified - p
void WRN_LayoutWarning(const char *text) {
    char character = *text;
    if (!character)
        return;
    char *cursor = warnCursor;
    std::int16_t row = warnRow;
    std::int16_t slot = warnLineSlot;
    do {
        ++text;
        if (character == '%') {
            *cursor = 0;
            const std::int16_t last = static_cast<std::int16_t>(std::strlen(warnLine.data()) - 1);
            if (warnLeft < last)
                warnLeft = last;
            WRN_UploadWarningLine(slot);
            row = warnRow;
            if (warnOpenFrame >= 0) {
                const std::size_t frame = static_cast<std::size_t>(warnOpenFrame);
                warnFrameLeft[frame] = 0;
                if (warnFrameRight[frame] < last)
                    warnFrameRight[frame] = last;
                warnFrameBottom[frame] = row;
            }
            slot = static_cast<std::int16_t>(warnLineSlot + 2);
            ++row;
            cursor = warnLine.data();
            warnCursor = cursor;
            warnRow = row;
            warnLineSlot = slot;
        } else if (character == '@') {
            const std::int8_t marker = static_cast<std::int8_t>(*text++ - '1');
            *cursor = 0;
            const std::int16_t length = static_cast<std::int16_t>(std::strlen(warnLine.data()));
            const std::size_t frame = static_cast<std::size_t>(static_cast<std::int16_t>(marker / 2));
            if (marker & 1) {
                const std::int16_t last = static_cast<std::int16_t>(length - 1);
                if (warnFrameRight[frame] < last)
                    warnFrameRight[frame] = last;
                warnFrameBottom[frame] = row;
                warnOpenFrame = -1;
            } else {
                warnFrameRight[frame] = length;
                warnFrameLeft[frame] = length;
                warnFrameBottom[frame] = row;
                warnFrameTop[frame] = row;
                warnOpenFrame = static_cast<std::int16_t>(frame);
            }
        } else if (character == '^') {
            std::int32_t remainder = warnValue;
            const std::int32_t hundreds = warnValue / 100;
            if (static_cast<std::int16_t>(hundreds) > 0) {
                *cursor++ = WARNING_DIGITS[static_cast<std::size_t>(static_cast<std::int16_t>(hundreds))];
                remainder = warnValue - 100 * static_cast<std::int16_t>(hundreds);
            }
            const std::int32_t tens = remainder / 10;
            if (static_cast<std::int16_t>(tens) > 0)
                *cursor++ = WARNING_DIGITS[static_cast<std::size_t>(static_cast<std::int16_t>(tens))];
            else if (hundreds > 0)
                *cursor++ = WARNING_DIGITS[0];
            const std::int16_t units = static_cast<std::int16_t>(remainder - static_cast<std::int16_t>(tens) * 10);
            *cursor++ = WARNING_DIGITS[static_cast<std::size_t>(units)];
            warnCursor = cursor;
        } else {
            *cursor++ = character;
            warnCursor = cursor;
        }
        character = *text;
    } while (character);
}

// Native reconnect layout: '@n' markers are dropped without frames and no value is inserted.
// @verified - p
void WRN_LayoutReconnect(const char *text) {
    char character = *text;
    if (!character)
        return;
    char *cursor = reconnectCursor;
    std::int16_t slot = reconnectLineSlot;
    do {
        ++text;
        if (character == '%') {
            *cursor = 0;
            const std::int32_t last = static_cast<std::int16_t>(std::strlen(reconnectLine.data()) - 1);
            if (reconnectLeft < last)
                reconnectLeft = last;
            UploadTextLine(reconnectText, reconnectTextures, slot, reconnectLine.data());
            slot = static_cast<std::int16_t>(reconnectLineSlot + 2);
            cursor = reconnectLine.data();
            reconnectLineSlot = slot;
            reconnectCursor = cursor;
        } else if (character == '@') {
            ++text;
            *cursor = 0;
        } else {
            *cursor++ = character;
            reconnectCursor = cursor;
        }
        character = *text;
    } while (character);
}


// @verified - p
void WRN_DrawFrame(std::int32_t frame, std::uint32_t fill, std::uint32_t border) {
    const std::size_t index = static_cast<std::size_t>(frame);
    const std::int16_t width = static_cast<std::int16_t>(warnFrameRight[index] + 8);
    const std::int16_t height = static_cast<std::int16_t>(warnFrameBottom[index] + 1);
    const float top = static_cast<float>(static_cast<std::int16_t>(warnFrameTop[index] + 43));
    LegacyColourRect rect{.x = static_cast<float>(static_cast<std::int16_t>(warnFrameLeft[index] - 4 + static_cast<std::int16_t>(warnLeft))),
        .y = top,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = 7.0f,
        .colour = fill,
        .width = width,
        .height = height,
        .rotation = 0,
        .alignment = 0,
        .reserved21 = {}};
    DrawLegacyColourRect(rect);
    rect.x += -1.0f;
    rect.y += -1.0f;
    rect.width = static_cast<std::int16_t>(rect.width + 2);
    rect.colour = border;
    rect.height = 2;
    DrawLegacyColourRect(rect);
    rect.y += static_cast<float>(height);
    DrawLegacyColourRect(rect);
    rect.width = 2;
    rect.height = static_cast<std::int16_t>(height + 2);
    rect.y = top - 1.0f;
    DrawLegacyColourRect(rect);
    rect.x += static_cast<float>(width);
    DrawLegacyColourRect(rect);
}


// @verified - p
void WRN_DrawWarning(void *) {
    warnBlink = (warnBlink + 1) & 0x1f;
    std::int32_t pulse = warnBlink;
    if (pulse & 0x10)
        pulse = 32 - pulse;
    const std::int32_t weight = pulse + pulse / 2;
    LegacyIconSprite sprite{.x = 0.0f,
        .y = 48.0f,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = 8.0f,
        .flags = 0,
        .reserved16 = 0,
        .rotation = 0,
        .icon = 0,
        .width = 1024,
        .height = 32,
        .alignment = 0,
        .reserved23 = {}};
    for (std::size_t row = 0; row < 10; ++row) {
        sprite.icon = static_cast<std::uint16_t>(warnTextures[row * 2]);
        sprite.x = static_cast<float>(warnLeft);
        if (warnTextures[row * 2] >= 0)
            PrepareDrawTextureWithMode(sprite, ScreenMode::Centered);
        sprite.icon = static_cast<std::uint16_t>(warnTextures[row * 2 + 1]);
        sprite.x += 512.0f;
        if (warnTextures[row * 2 + 1] >= 0)
            PrepareDrawTextureWithMode(sprite, ScreenMode::Centered);
        sprite.y += 40.0f;
    }
    if (warnFrameRight[0] > 0)
        WRN_DrawFrame(0, 0x3b00008cU, 0xff4040c0U);
    const std::int32_t normal = warnValue != 0 ? 1 : 2;
    if (warnFrameRight[static_cast<std::size_t>(normal)] > 0)
        WRN_DrawFrame(normal, 0x3b00008cU, 0xff4040c0U);
    const std::int32_t selected = 3 - normal;
    if (warnFrameRight[static_cast<std::size_t>(selected)] > 0) {
        const std::uint32_t border = WRN_BlendColour(weight - 8, 0xffc04040U, 0xff602020U);
        const std::uint32_t fill = WRN_BlendColour(weight - 8, 0x3b8c0000U, 0x3b460000U);
        WRN_DrawFrame(selected, fill, border);
    }
}


// @verified - p
void WRN_DrawReconnect(void *) {
    warnBlink = (warnBlink + 1) & 0x1f;
    LegacyIconSprite sprite{.x = 118.0f,
        .y = 155.0f,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = 16.0f,
        .flags = 0,
        .reserved16 = 0,
        .rotation = 0,
        .icon = 0,
        .width = 1024,
        .height = 32,
        .alignment = 0,
        .reserved23 = {}};
    for (std::size_t row = 0; row < 10; ++row) {
        sprite.x = 118.0f;
        sprite.icon = static_cast<std::uint16_t>(reconnectTextures[row * 2]);
        if (reconnectTextures[row * 2] >= 0)
            PrepareDrawTextureWithMode(sprite, ScreenMode::AdjustedWithOffset);
        sprite.icon = static_cast<std::uint16_t>(reconnectTextures[row * 2 + 1]);
        sprite.x += 512.0f;
        if (reconnectTextures[row * 2 + 1] >= 0)
            PrepareDrawTextureWithMode(sprite, ScreenMode::AdjustedWithOffset);
        sprite.y += 59.0f;
    }
    LegacyColourRect rect{.x = 80.0f,
        .y = 153.0f,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = 15.0f,
        .colour = 0x3b8c0000U,
        .width = 500,
        .height = 165,
        .rotation = 0,
        .alignment = 0,
        .reserved21 = {}};
    DrawLegacyColourRect(rect);
    rect.x += -1.0f;
    rect.y += -1.0f;
    rect.width = static_cast<std::int16_t>(rect.width + 2);
    rect.colour = 0xffc04040U;
    rect.height = 2;
    DrawLegacyColourRect(rect);
    rect.y += 165.0f;
    DrawLegacyColourRect(rect);
    rect.y = 152.0f;
    rect.width = 2;
    rect.height = 167;
    DrawLegacyColourRect(rect);
    rect.x += 500.0f;
    DrawLegacyColourRect(rect);
}


// @verified - p
void WRN_CtrlWarning_Callback(void *) {
    switch (warnState) {
    case WarningState::Idle:
        if (warnId > 0) {
            KanjiFontLoad("DC_KANJI.FON");
            wndSetFlag(nullptr, 1);
            warnState = WarningState::Layout;
            warnMessage = WARNING_MESSAGES[static_cast<std::size_t>(warnId - 1)];
            warnTargetColour = warnBaseColour;
        }
        break;
    case WarningState::Layout:
        warnCursor = warnLine.data();
        warnOpenFrame = -1;
        warnFrameRight[2] = -99;
        warnFrameBottom[2] = -99;
        warnFrameLeft[2] = 99;
        warnFrameTop[2] = 99;
        warnLeft = 0;
        warnLineSlot = 0;
        warnRow = 0;
        warnFrameRight[0] = warnFrameRight[1] = -99;
        warnFrameBottom[0] = warnFrameBottom[1] = -99;
        warnFrameLeft[0] = warnFrameLeft[1] = 99;
        warnFrameTop[0] = warnFrameTop[1] = 99;
        WRN_LayoutWarning(warnMessage);
        for (; warnLineSlot < 29; warnLineSlot = static_cast<std::int16_t>(warnLineSlot + 2))
            WRN_UploadWarningLine(warnLineSlot);
        warnLeft = (620 - 14 * warnLeft) / 2;
        for (std::size_t frame = 0; frame < 3; ++frame) {
            const std::int16_t top = warnFrameTop[frame];
            const std::int16_t left = warnFrameLeft[frame];
            warnFrameLeft[frame] = static_cast<std::int16_t>(left * 14);
            warnFrameRight[frame] = static_cast<std::int16_t>(static_cast<std::int16_t>(warnFrameRight[frame] - left + 1) * 14);
            warnFrameTop[frame] = static_cast<std::int16_t>(top * 40);
            warnFrameBottom[frame] = static_cast<std::int16_t>(static_cast<std::int16_t>(warnFrameBottom[frame] - top + 1) * 40);
        }
        warnBlink = 0;
        warnState = WarningState::Shown;
        break;
    case WarningState::Shown:
        if (warnId <= 0) {
            warnState = WarningState::Retiring;
            warnRetireFrames = 4;
            warnTargetColour = 0;
        } else {
            HLib::ExecuteCallbackNow(WRN_DrawWarning, nullptr);
        }
        break;
    case WarningState::Retiring:
        if (--warnRetireFrames < 0)
            warnState = WarningState::Idle;
        break;
    }
    bool dim = true;
    switch (reconnectState) {
    case WarningState::Idle:
        if (!reconnectRequest) {
            dim = false;
            break;
        }
        if (warnId <= 0) {
            KanjiFontLoad("DC_KANJI.FON");
            wndSetFlag(nullptr, 1);
        }
        reconnectMessage = RECONNECT_MESSAGE;
        reconnectTargetColour = reconnectBaseColour;
        reconnectState = WarningState::Layout;
        break;
    case WarningState::Layout:
        reconnectCursor = reconnectLine.data();
        reconnectLeft = 0;
        reconnectLineSlot = 0;
        WRN_LayoutReconnect(reconnectMessage);
        for (; reconnectLineSlot < 29; reconnectLineSlot = static_cast<std::int16_t>(reconnectLineSlot + 2)) {
            WRN_UploadReconnectLine(reconnectLineSlot);
        }
        reconnectState = WarningState::Shown;
        break;
    case WarningState::Shown:
        if (!reconnectRequest) {
            reconnectState = WarningState::Retiring;
            reconnectRetireFrames = 4;
            reconnectTargetColour = 0;
            dim = false;
            break;
        }
        HLib::ExecuteCallbackNow(WRN_DrawReconnect, nullptr);
        break;
    case WarningState::Retiring:
        if (--reconnectRetireFrames < 0)
            reconnectState = WarningState::Idle;
        break;
    }
    if (dim && reconnectRequest)
        vfSubmitQuad(RECONNECT_BASE_COLOUR, 14.0f);
    std::uint32_t colour = warnColour;
    reconnectRequest = 0;
    if (colour > warnTargetColour) {
        const std::uint32_t step = colour - warnTargetColour;
        colour -= step > WARNING_COLOUR_STEP ? WARNING_COLOUR_STEP : step;
    } else {
        const std::uint32_t step = warnTargetColour - colour;
        colour += step > WARNING_COLOUR_STEP ? WARNING_COLOUR_STEP : step;
    }
    warnColour = colour;
    if (colour)
        vfSubmitQuad(colour, 6.0f);
}

} // namespace

// @verified - p
std::uint32_t WRN_UploadWarningLine(std::int16_t slot) {
    return UploadTextLine(warnText, warnTextures, slot, warnLine.data());
}

// @verified - p
void *WRN_CtrlWarning_Enqueue() {
    warnState = WarningState::Idle;
    warnId = 0;
    HLib::EnqueueTaskWithoutParameter(WRN_CtrlWarning_Callback, 15, 0, static_cast<std::int32_t>(WARNING_TASK.name));
    warnTextures.fill(-1);
    const std::int32_t textureSize = static_cast<std::int32_t>(GetTextureStorageSize(WARNING_TEXT_FORMAT, WARNING_TEXT_WIDTH, WARNING_TEXT_HEIGHT));
    warnText = static_cast<std::byte *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(textureSize + 48), WARNING_TEXT_TEXTURE));
    if (warnText) {
        std::memset(warnText, 0, static_cast<std::size_t>(textureSize + 48));
        AdmitWarningTextBank(warnText, WARNING_TEXT_TEXTURE, warnTextures);
    }
    warnBaseColour = WARNING_BASE_COLOUR;
    warnTargetColour = 0;
    warnColour = 0;
    return AllocateWARP(textureSize);
}

// @verified - p
void *AllocateWARP(std::int32_t textureSize) {
    reconnectTextures.fill(-1);
    reconnectText = static_cast<std::byte *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(textureSize + 48), RECONNECT_TEXT_TEXTURE));
    if (reconnectText) {
        std::memset(reconnectText, 0, static_cast<std::size_t>(textureSize + 48));
        AdmitWarningTextBank(reconnectText, RECONNECT_TEXT_TEXTURE, reconnectTextures);
    }
    reconnectTargetColour = 0;
    reconnectColour = 0;
    reconnectState = WarningState::Idle;
    reconnectRequest = 0;
    reconnectBaseColour = RECONNECT_BASE_COLOUR;
    return reconnectText;
}

// @verified - p
void WRN_SetWarning(std::int32_t warning, std::int32_t value) {
    warnId = warning < 30 ? (warning < 0 ? 0 : warning) : 0;
    warnValue = value <= 99999 ? (value < 0 ? 0 : value) : 99999;
}

// @verified - p
void WRN_RequestReconnectPrompt() {
    reconnectRequest = 1;
}

// @verified - p
std::uint32_t WRN_SetBaseColour(std::uint32_t colour) {
    const std::uint32_t previous = warnBaseColour;
    warnBaseColour = colour;
    return previous;
}

// @verified - p
std::int16_t *WRN_GetWarningTextures() {
    return warnTextures.data();
}

} // namespace shenmue::s2
