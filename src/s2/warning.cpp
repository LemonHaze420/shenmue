#include "s2/warning.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/window.hpp"
#include "s2/font.hpp"
#include "s2/hlib.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "common/math.hpp"
#include <cstring>
#else
#include "s2/font.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"

#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Offset of the DDS pixel block behind the PTX TEXN and DDS headers.
constexpr std::size_t WARNING_TEXT_PIXELS = 0xac;

std::uint32_t warnBaseColour;

std::uint32_t warnColour{};

std::uint32_t warnTargetColour;

std::int32_t warnId;

std::int32_t warnValue;

WarningState warnState;

std::int32_t warnRetireFrames;

// The longest line in characters during layout, then the centred left edge in pixels.
std::int32_t warnLeft;

const char *warnMessage{};

std::array<char, 56> warnLine;

std::int16_t warnLineSlot;

char *warnCursor;

std::int16_t warnRow{};

std::int16_t warnOpenFrame;

// Frame 0 boxes the message, 1 and 2 the Yes and No choices. Layout records columns/rows; the second pass converts them to
// pixel origins and sizes (right and bottom become width and height).
std::array<std::int16_t, 3> warnFrameLeft;

std::array<std::int16_t, 3> warnFrameTop{};

std::array<std::int16_t, 3> warnFrameRight;

std::array<std::int16_t, 3> warnFrameBottom;

std::int32_t warnBlink{};

std::int32_t reconnectRequest{};

std::byte *warnText;

std::array<std::int16_t, WARNING_TEXT_SLOTS> warnTextures;

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

std::uint32_t WRN_BlendColour(std::int32_t weight, std::uint32_t from, std::uint32_t to);
void WRN_LayoutWarning(const char *text);
void WRN_DrawFrame(std::int32_t frame, std::uint32_t fill, std::uint32_t border);
void WRN_DrawWarning(void *);
void WRN_CtrlWarning_Callback(void *);

} // namespace

std::uint32_t WRN_UploadWarningLine(std::int16_t slot);
void *WRN_CtrlWarning_Enqueue();
void *AllocateWARP(std::int32_t textureSize);
void WRN_SetWarning(std::int32_t warning, std::int32_t value);
std::uint32_t WRN_SetBaseColour(std::uint32_t colour);
std::int16_t *WRN_GetWarningTextures();
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native XOR_EAX at the kanji rasterizer boundary: PC v1.07 draws no glyphs, so every line uploads the cleared pixels.
std::uint32_t RasterizeKanjiLineNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) {
    return 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native ParsePTX + TEXN_Reader admission of one 1024x16 text bank: the base '9999' texture into slot 28, fourteen line
// textures ('0000'..'000=') into the even slots, then the TEXN, PVRT, width and ID fields are cleared as native does.
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
std::uint32_t WRN_UploadReconnectLine(std::int16_t slot) {
    return UploadTextLine(reconnectText, reconnectTextures, slot, reconnectLine.data());
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint32_t WRN_BlendColour(std::int32_t weight, std::uint32_t from, std::uint32_t to);

void WRN_DrawFrame(std::int32_t frame, std::uint32_t fill, std::uint32_t border);

void WRN_DrawWarning(void *);

void WRN_LayoutWarning(const char *text);

void WRN_CtrlWarning_Callback(void *);

} // namespace

std::uint32_t warnBaseColour;

std::byte *warnText;

std::int16_t warnTextures[WARNING_TEXT_SLOTS];

struct WarningTextLine {
    std::uint32_t words[0x100];
};

namespace {

std::uint32_t warnColour = 0, warnTargetColour = 0;

std::int32_t warnId, warnValue;

WarningState warnState;

std::int32_t warnRetireFrames;

const char *warnMessage = 0;

std::int32_t warnLeft;

char warnLine[52];

char *warnCursor;

std::int16_t warnLineSlot, warnRow = 0, warnOpenFrame = 0;

std::int16_t warnFrameLeft[3], warnFrameTop[3] = {}, warnFrameRight[3] = {}, warnFrameBottom[3] = {};

std::uint32_t warnBlink = 0;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t WRN_UploadWarningLine(std::int16_t slot, const char *line) {
    std::int32_t result = 0;
    if (warnText && warnTextures[slot] >= 0) {
        result = RasterizeWindowTextLine4bpp(warnText + 0x2c, 1024, line, 1, 0);
        ReplacePtxTexture(warnTextures[slot], reinterpret_cast<const TextureChunk *>(warnText));
        if (slot <= 18 && warnTextures[slot + 1] >= 0) {
            *reinterpret_cast<WarningTextLine *>(warnText + 0x2c) = *reinterpret_cast<const WarningTextLine *>(warnText + 0x102c);
            ReplacePtxTexture(warnTextures[slot + 1], reinterpret_cast<const TextureChunk *>(warnText));
        }
    }
    return result;
}

#else

// @verified - p
std::uint32_t WRN_UploadWarningLine(std::int16_t slot) {
    return UploadTextLine(warnText, warnTextures, slot, warnLine.data());
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const std::int16_t *WRN_GetWarningTextures() {
    return warnTextures;
}

#else

// @verified - p
std::int16_t *WRN_GetWarningTextures() {
    return warnTextures.data();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static std::uint32_t WRN_BlendChannel(std::int32_t weight, std::uint32_t from, std::uint32_t to) {
    if (weight < 0)
        weight = 0;
    if (weight > 16)
        weight = 16;
    from = static_cast<std::uint8_t>(from);
    to = static_cast<std::uint8_t>(to);
    if (from > to)
        from = (from - to) * (16 - weight) >> 4;
    else
        to = (to - from) * weight >> 4;
    return (from + to) & 0xff;
}

namespace {

// @verified - d
std::uint32_t WRN_BlendColour(std::int32_t weight, std::uint32_t from, std::uint32_t to) {
    std::uint32_t result = WRN_BlendChannel(weight, from, to);
    result |= static_cast<std::uint32_t>(WRN_BlendChannel(weight, from >> 8, to >> 8)) << 8;
    result |= static_cast<std::uint32_t>(WRN_BlendChannel(weight, from >> 16, to >> 16)) << 16;
    result |= static_cast<std::uint32_t>(WRN_BlendChannel(weight, from >> 24, to >> 24)) << 24;
    return result;
}

} // namespace

#else

namespace {

// Native colour blend: each channel moves weight/16 (clamped 0..16) of the way from `from` to `to`.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void WRN_DrawFrame(std::int32_t frame, std::uint32_t fill, std::uint32_t border) {
    const std::int16_t x = static_cast<std::int16_t>(static_cast<std::int16_t>(warnLeft) + warnFrameLeft[frame] - 4);
    const std::int16_t top = static_cast<std::int16_t>(warnFrameTop[frame] + 43);
    const std::int16_t width = static_cast<std::int16_t>(warnFrameRight[frame] + 4);
    const std::int16_t height = static_cast<std::int16_t>(warnFrameBottom[frame] + 4);
    LegacyColourRect rect;
    rect.alignment = 0;
    rect.depth = 7.0f;
    rect.scaleX = rect.scaleY = 1.0f;
    rect.rotation = 0;
    rect.colour = fill;
    rect.x = static_cast<float>(x);
    rect.y = static_cast<float>(top);
    rect.width = width;
    rect.height = height;
    DrawLegacyColourRect(rect);
    rect.colour = border;
    rect.width = static_cast<std::int16_t>(rect.width + 2);
    rect.height = 2;
    rect.x -= 1.0f;
    rect.y -= 1.0f;
    DrawLegacyColourRect(rect);
    rect.y += static_cast<float>(height);
    DrawLegacyColourRect(rect);
    rect.width = 2;
    rect.height = static_cast<std::int16_t>(height + 2);
    rect.y = static_cast<float>(top - 1);
    DrawLegacyColourRect(rect);
    rect.x += static_cast<float>(width);
    DrawLegacyColourRect(rect);
}

} // namespace

#else

namespace {

// Native frame draw: a filled box with a two-pixel border, offset by the centred text origin.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void WRN_DrawWarning(void *) {
    std::int32_t index;
    const std::uint32_t counter = warnBlink + 1U;
    warnBlink = counter & 0x1fU;
    std::int32_t pulse = static_cast<std::int32_t>(warnBlink);
    if (counter & 0x10U)
        pulse = 32 - pulse;
    std::int32_t base = pulse - 8;
    pulse = base + pulse / 2;
    LegacyIconSprite text;
    text.flags = 0;
    text.alignment = 0;
    text.depth = 8.0f;
    text.scaleX = 1.0f;
    text.scaleY = 1.0f;
    text.rotation = 0;
    text.y = 48.0f;
    text.height = 32;
    const std::int16_t *textures = WRN_GetWarningTextures();
    for (index = 0; index < 10; ++index) {
        text.x = static_cast<float>(warnLeft);
        text.width = 512;
        text.icon = static_cast<std::uint16_t>(*textures++);
        if (static_cast<std::int16_t>(text.icon) >= 0)
            PrepareDrawTextureWithMode(text);
        text.x += 512.0f;
        text.width = 128;
        text.icon = static_cast<std::uint16_t>(*textures++);
        if (static_cast<std::int16_t>(text.icon) >= 0)
            PrepareDrawTextureWithMode(text);
        float rowHeight;
        rowHeight = 40.0f;
        rowHeight += text.y;
        text.y = rowHeight;
    }
    if (warnFrameRight[0] > 0)
        WRN_DrawFrame(0, 0x3b00008cU, 0xff4040c0U);
    index = warnValue ? 1 : 2;
    if (warnFrameRight[index] > 0)
        WRN_DrawFrame(index, 0x3b00008cU, 0xff4040c0U);
    index = 3 - index;
    if (warnFrameRight[index] > 0) {
        const std::uint32_t fill = WRN_BlendColour(pulse, 0x3b8c0000U, 0x3b460000U);
        const std::uint32_t border = WRN_BlendColour(pulse, 0xffc04040U, 0xff602020U);
        WRN_DrawFrame(index, fill, border);
    }
}

} // namespace

#else

namespace {

// Native warning draw: up to ten text rows, the message frame and the pulsing selected choice.
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native reconnect draw: its text rows and a fixed 500x165 bordered box.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void WRN_LayoutWarning(const char *text) {
    std::int16_t column;
    char character;
    while ((character = *text++) != 0) {
        if (character == '0') {
            *warnCursor = 0;
            column = static_cast<std::int16_t>(static_cast<std::int16_t>(std::strlen(warnLine) >> 1) - 1);
            if (warnLeft < column)
                warnLeft = column;
            WRN_UploadWarningLine(warnLineSlot, warnLine);
            if (warnOpenFrame >= 0) {
                warnFrameLeft[warnOpenFrame] = 0;
                if (warnFrameRight[warnOpenFrame] < column)
                    warnFrameRight[warnOpenFrame] = column;
                warnFrameBottom[warnOpenFrame] = warnRow;
            }
            warnCursor = warnLine;
            warnLineSlot = static_cast<std::int16_t>(warnLineSlot + 2);
            warnRow = static_cast<std::int16_t>(warnRow + 1);
        } else if (static_cast<std::uint8_t>(character - '1') < 6) {
            *warnCursor = 0;
            column = static_cast<std::int16_t>(std::strlen(warnLine) >> 1);
            const std::int8_t marker = static_cast<std::int8_t>(character - '1');
            std::int8_t frame = marker;
            frame /= 2;
            if (marker & 1) {
                column = static_cast<std::int16_t>(column - 1);
                if (warnFrameRight[frame] < column)
                    warnFrameRight[frame] = column;
                warnFrameBottom[frame] = warnRow;
                warnOpenFrame = -1;
            } else {
                warnFrameLeft[frame] = warnFrameRight[frame] = column;
                warnFrameTop[frame] = warnFrameBottom[frame] = warnRow;
                warnOpenFrame = static_cast<std::int16_t>(frame);
            }
        } else {
            if (character == '7') {
                const char *digit = WARNING_DIGITS + warnValue * 2;
                *warnCursor++ = *digit++;
                *warnCursor++ = *digit;
            } else {
                *warnCursor++ = character;
                *warnCursor++ = *text++;
            }
        }
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void WRN_CtrlWarning_Callback(void *) {
    std::int32_t frame;
    switch (warnState) {
    case WarningState_Idle:
        if (warnId > 0) {
            KanjiFontLoad("DC_KANJI.FON");
            wndSetFlag(0, 1);
            warnMessage = WARNING_MESSAGES[warnId - 1];
            warnState = WarningState_Layout;
            warnTargetColour = warnBaseColour;
        }
        break;
    case WarningState_Layout:
        warnLeft = 0;
        warnCursor = warnLine;
        warnLineSlot = 0;
        warnRow = 0;
        warnOpenFrame = -1;
        for (frame = 0; frame < 3; ++frame) {
            warnFrameLeft[frame] = warnFrameTop[frame] = 99;
            warnFrameRight[frame] = warnFrameBottom[frame] = -99;
        }
        WRN_LayoutWarning(warnMessage);
        for (; warnLineSlot < 20; warnLineSlot = static_cast<std::int16_t>(warnLineSlot + 2))
            WRN_UploadWarningLine(warnLineSlot, "");
        warnLeft = static_cast<std::int32_t>(620U - static_cast<std::uint32_t>(warnLeft) * 26U) >> 1;
        for (frame = 0; frame < 3; ++frame) {
            ++warnFrameRight[frame];
            warnFrameRight[frame] -= warnFrameLeft[frame];
            ++warnFrameBottom[frame];
            warnFrameBottom[frame] -= warnFrameTop[frame];
            warnFrameLeft[frame] = static_cast<std::int16_t>(warnFrameLeft[frame] * 26);
            warnFrameRight[frame] = static_cast<std::int16_t>(warnFrameRight[frame] * 26);
            warnFrameTop[frame] = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(warnFrameTop[frame]) * 40.0f));
            warnFrameBottom[frame] = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(warnFrameBottom[frame]) * 40.0f));
        }
        warnBlink = 0;
        warnState = WarningState_Shown;
        break;
    case WarningState_Shown:
        if (warnId > 0) {
            HLib::ExecuteCallbackNow(WRN_DrawWarning, 0);
        } else {
            warnState = WarningState_Retiring;
            warnRetireFrames = 4;
            warnTargetColour = 0;
        }
        break;
    case WarningState_Retiring:
        warnRetireFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(warnRetireFrames) - 1U);
        if (warnRetireFrames < 0)
            warnState = WarningState_Idle;
        break;
    }
    if (warnColour > warnTargetColour) {
        std::uint32_t difference = warnColour - warnTargetColour;
        if (difference > 0x08000000U)
            difference = 0x08000000U;
        warnColour -= difference;
    } else {
        std::uint32_t difference = warnTargetColour - warnColour;
        if (difference > 0x08000000U)
            difference = 0x08000000U;
        warnColour += difference;
    }
    if (warnColour)
        DrawMainIconOverlay(warnColour, 6.0f);
}

} // namespace

#else

namespace {

// Native WRN_CtrlWarning_Callback: the warning and reconnect state machines, the reconnect dim and the eased warning dim.
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void WRN_CtrlWarning_Enqueue() {
    std::int32_t index;
    warnState = WarningState_Idle;
    warnId = 0;
    HLib::EnqueueTaskWithoutParameter(WRN_CtrlWarning_Callback, 15, 0, static_cast<std::int32_t>(SHEN_NAME4('W', 'A', 'R', 'N')));
    for (index = 0; index < static_cast<std::int32_t>(WARNING_TEXT_SLOTS); ++index)
        warnTextures[index] = -1;
    warnText = static_cast<std::byte *>(HLib::AllocateTaskData(0x2034, SHEN_NAME4('W', 'A', 'R', 'N')));
    if (warnText) {
        TextureChunk *texture = reinterpret_cast<TextureChunk *>(warnText);
        texture->name = SHEN_NAME4('T', 'E', 'X', 'N');
        texture->size = 0x202c;
        texture->globalIndexHeader[0] = SHEN_NAME4('G', 'B', 'I', 'X');
        texture->globalIndexHeader[1] = 4;
        texture->globalIndexHeader[2] = index = 0;
        warnText[28] = std::byte('P');
        warnText[29] = std::byte('V');
        warnText[30] = std::byte('R');
        warnText[31] = std::byte('T');
        TextureChunk *header = reinterpret_cast<TextureChunk *>(warnText);
        header->width = 1024;
        header->height = 16;
        header->imageSize = 0x2008;
        header->type = 0x502;
        header->nameLow = SHEN_NAME4('W', 'A', 'R', 'N');
        header->nameHigh = SHEN_NAME4('9', '9', '9', '9');
        std::memset(header + 1, 0xff, 0x2000);
        {
            std::uint32_t *tail = reinterpret_cast<std::uint32_t *>(warnText + 0x202c);
            tail[0] = 0;
            tail[1] = 0;
        }
        warnTextures[20] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(reinterpret_cast<const TextureChunk *>(warnText)));
        TextureChunk *half = reinterpret_cast<TextureChunk *>(warnText);
        half->size -= 0x1000;
        half->width = 512;
        half->imageSize = 0x1008;
        {
            std::uint32_t *tail = reinterpret_cast<std::uint32_t *>(warnText + 0x102c);
            tail[0] = 0;
            tail[1] = 0;
        }
        for (index = 0; index < 10; ++index) {
            TextureChunk *lineTexture = reinterpret_cast<TextureChunk *>(warnText);
            lineTexture->nameHigh = (static_cast<std::uint32_t>('0' + index) << 24) | SHEN_NAME4('0', '0', '0', '\0');
            warnTextures[index * 2] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(lineTexture));
        }
        TextureChunk *quarter = reinterpret_cast<TextureChunk *>(warnText);
        quarter->size -= 0x0c00;
        quarter->width = 128;
        quarter->imageSize = 0x0408;
        {
            std::uint32_t *tail = reinterpret_cast<std::uint32_t *>(warnText + 0x042c);
            tail[0] = 0;
            tail[1] = 0;
        }
        for (index = 0; index < 10; ++index) {
            TextureChunk *lineTexture = reinterpret_cast<TextureChunk *>(warnText);
            lineTexture->nameHigh = (static_cast<std::uint32_t>('0' + index) << 24) | SHEN_NAME4('0', '0', '1', '\0');
            warnTextures[index * 2 + 1] = static_cast<std::int16_t>(LoadAndFindTEXN_Wrapper(lineTexture));
        }
        TextureChunk *finalHeader = reinterpret_cast<TextureChunk *>(warnText);
        finalHeader->size = 0;
        finalHeader->width = 0;
        finalHeader->imageSize = 0;
        finalHeader->nameHigh = 0;
    }
    WRN_SetBaseColour(0x76000000U);
    warnColour = warnTargetColour = 0;
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void WRN_SetWarning(std::int32_t warning, std::int32_t value) {
    if (warning > 19)
        warning = 0;
    warnId = warning;
    warnValue = value;
}

#else

// @verified - p
void WRN_SetWarning(std::int32_t warning, std::int32_t value) {
    warnId = warning < 30 ? (warning < 0 ? 0 : warning) : 0;
    warnValue = value <= 99999 ? (value < 0 ? 0 : value) : 99999;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void WRN_RequestReconnectPrompt() {
    reconnectRequest = 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t WRN_SetBaseColour(std::uint32_t colour) {
    const std::uint32_t previous = warnBaseColour;
    warnBaseColour = colour;
    return previous;
}

#else

// @verified - p
std::uint32_t WRN_SetBaseColour(std::uint32_t colour) {
    const std::uint32_t previous = warnBaseColour;
    warnBaseColour = colour;
    return previous;
}

#endif

} // namespace s2
} // namespace shenmue
