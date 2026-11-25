#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_hud_text_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/title_sprite_dc.hpp"
#endif

#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/window.hpp"
#include <cmath>
#include <cstring>
#else
#include "s2/legacy_hud_text.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t references{};

std::array<std::int32_t, 24> digits = [] {
    std::array<std::int32_t, 24> result;
    result.fill(-1);
    return result;
}();

struct GlyphSlot {
    std::int32_t reserved{-1};
    std::int32_t texture{-1};
};

std::array<GlyphSlot, 16> glyphs{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitTIMR();
LegacyHudText *CreateLegacyHudText();
void DrawLegacyHudText(LegacyHudText *text);
std::int64_t LoadLegacyHudTextGlyph(const void *texn);
std::int64_t ReleaseLegacyHudText(LegacyHudText *text);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float Flush(float value) {
    return std::fabs(value) < 1.1920929e-7f ? 0.0f : value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ReleaseDigits() {
    for (auto &texture : digits)
        if (texture != -1) {
            ReleaseNamedTexture(texture);
            texture = -1;
        }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void LoadDigits() {
    void *allocation{};
    std::size_t size{};
    LoadLegacySprite("sprite", "LOADNUM.GZ", SHEN_NAME(TIMR), allocation, size);
    const auto *cursor = static_cast<const std::byte *>(allocation);
    const auto *end = cursor ? cursor + size : cursor;
    for (std::size_t i = 0; cursor && end - cursor >= 8 && i < 10; ++i) {
        std::uint32_t signature{}, chunkSize{};
        std::memcpy(&signature, cursor, 4);
        std::memcpy(&chunkSize, cursor + 4, 4);
        if (signature != SHEN_NAME(TEXN) || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        digits[i] = static_cast<std::int32_t>(ReadTexnTexture(cursor));
        cursor += chunkSize;
    }
    HLib::FreeTaskData(allocation);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ReleaseLegacyHudText(LegacyHudText *text);

std::int8_t LoadLegacyHudTextGlyph(const void *texture);

void DrawLegacyHudText(LegacyHudText *text);

std::int32_t g_legacyHudTextReferences = 0;

std::int32_t g_legacyHudDigitTextures[24];

LegacyHudGlyphSlot g_legacyHudGlyphs[16];

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void InitTIMR() {
    g_legacyHudTextReferences = 0;
    for (std::int32_t index = 0; index < 24; ++index)
        g_legacyHudDigitTextures[index] = -1;
    for (std::int32_t index = 0; index < 16; ++index) {
        g_legacyHudGlyphs[index].colourCode = 0;
        g_legacyHudGlyphs[index].texture = -1;
    }
}

#else

// Task-token allocation and shared LOADNUM bank admission.
// Native InitTIMR (CTRL construction): no references, no digit textures and empty glyph slots.
void InitTIMR() {
    references = 0;
    digits.fill(-1);
    for (GlyphSlot &glyph : glyphs)
        glyph = {.reserved = 0, .texture = -1};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyHudText *CreateLegacyHudText() {
    std::int32_t failed = 1;
    LegacyHudText *result = 0;
    LegacyHudText *text = 0;
    HLib::Task *task;
    shen_name token;
    std::uint32_t bytes;
    TextureChunk *allocation;
    float unit = 1.0f;
    goto allocate;
initialize:
    for (std::int32_t clearIndex = 0; clearIndex < static_cast<std::int32_t>(sizeof(*text)); ++clearIndex)
        reinterpret_cast<std::byte *>(text)[clearIndex] = 0;
    if (g_legacyHudTextReferences < 1) {
        bytes = shenOpenFile("sprite", "LOADNUM.SPR");
        allocation = static_cast<TextureChunk *>(HLib::AllocateTaskData(bytes, HUD_DIGIT_STORAGE));
        if (!allocation)
            goto done;
        shenLoadFile("sprite", "LOADNUM.SPR", allocation, bytes);
        ReadTitleSpriteTextures(allocation, bytes, g_legacyHudDigitTextures);
    }
    text->size = sizeof(*text);
    text->allocationToken = token;
    for (std::int32_t glyphIndex = 0; glyphIndex < 12; ++glyphIndex) {
        text->text[glyphIndex] = 0;
        text->glyphCache[glyphIndex] = 0;
        text->glyphTextures[glyphIndex] = -1;
    }
    text->depth = unit;
    text->colour = 0xffffffffU;
    text->scaleX = unit;
    text->scaleY = unit;
    text->x = 350.0f;
    text->y = 49.0f;
    text->spacing = 0.0f;
    text->release = ReleaseLegacyHudText;
    text->draw = DrawLegacyHudText;
    text->loadGlyph = LoadLegacyHudTextGlyph;
    ++g_legacyHudTextReferences;
    failed = 0;
    result = text;
    goto done;
allocate:
    task = HLib::GetCurrentTask();
    if (task) {
        token = task->taskToken;
        text = static_cast<LegacyHudText *>(HLib::AllocateTaskData(sizeof(*text), token));
        if (text)
            goto initialize;
    }
done:
    if (failed) {
        if (g_legacyHudTextReferences < 1) {
            for (std::int32_t digitIndex = 0; digitIndex < 24; ++digitIndex) {
                if (g_legacyHudDigitTextures[digitIndex] != -1) {
                    ReleaseNamedTexture(g_legacyHudDigitTextures[digitIndex]);
                    g_legacyHudDigitTextures[digitIndex] = -1;
                }
            }
        }
        if (text)
            HLib::FreeTaskData(text);
        result = 0;
    }
    return result;
}
#else
// @verified - d
LegacyHudText *CreateLegacyHudText() {
    LegacyHudText *text = 0;
    HLib::Task *const task = HLib::GetCurrentTask();
    if (task) {
        text = static_cast<LegacyHudText *>(HLib::AllocateTaskData(sizeof(LegacyHudText), shen_name(static_cast<std::uint32_t>(task->taskToken))));
        if (text) {
            std::memset(text, 0, sizeof(*text));
            if (g_legacyHudTextReferences < 1) {
                const std::uint32_t bytes = shenOpenFile("sprite", "LOADNUM.SPR");
                const TextureChunk *const allocation = static_cast<TextureChunk *>(HLib::AllocateTaskData(bytes, HUD_DIGIT_STORAGE));
                if (!allocation)
                    goto failure;
                shenLoadFile("sprite", "LOADNUM.SPR", const_cast<TextureChunk *>(allocation), bytes);
                const TextureChunk *chunk = allocation;
                const std::byte *const end = reinterpret_cast<const std::byte *>(allocation) + bytes;
                std::int32_t index = 0;
                while (reinterpret_cast<const std::byte *>(chunk) < end && chunk->name) {
                    g_legacyHudDigitTextures[index++] = LoadAndFindTEXN_Wrapper(chunk);
                    chunk = reinterpret_cast<const TextureChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->size);
                }
            }
            text->size = sizeof(*text);
            text->allocationToken = shen_name(static_cast<std::uint32_t>(task->taskToken));
            for (std::size_t rangeIndex99 = 0; rangeIndex99 < sizeof(text->glyphTextures) / sizeof((text->glyphTextures)[0]); ++rangeIndex99) {
                std::int32_t &texture = (text->glyphTextures)[rangeIndex99];
                texture = -1;
            }
            text->depth = 1.0f;
            text->colour = 0xffffffffU;
            text->scaleX = 1.0f;
            text->scaleY = 1.0f;
            text->x = 350.0f;
            text->y = 49.0f;
            text->spacing = 0.0f;
            text->release = ReleaseLegacyHudText;
            text->draw = DrawLegacyHudText;
            text->loadGlyph = LoadLegacyHudTextGlyph;
            ++g_legacyHudTextReferences;
            return text;
        }
    }
failure:
    if (g_legacyHudTextReferences < 1) {
        for (std::size_t rangeIndex100 = 0; rangeIndex100 < sizeof(g_legacyHudDigitTextures) / sizeof((g_legacyHudDigitTextures)[0]); ++rangeIndex100) {
            std::int32_t &texture = (g_legacyHudDigitTextures)[rangeIndex100];

            if (texture != -1) {
                ReleaseNamedTexture(texture);
                texture = -1;
            }
        }
    }
    if (text)
        HLib::FreeTaskData(text);
    return 0;
}

#endif

#else

LegacyHudText *CreateLegacyHudText() {
    auto *task = HLib::GetCurrentTask();
    auto *text =
        task ? static_cast<LegacyHudText *>(HLib::AllocateTaskData(sizeof(LegacyHudText), shen_name{static_cast<std::uint32_t>(task->taskToken)})) : nullptr;
    if (!text) {
        if (references <= 0)
            ReleaseDigits();
        return nullptr;
    }
    std::memset(text, 0, sizeof(*text));
    if (references <= 0)
        LoadDigits();
    text->size = sizeof(*text);
    text->allocationToken = shen_name{static_cast<std::uint32_t>(task->taskToken)};
    text->glyphTextures.fill(-1);
    text->depth = text->scaleX = text->scaleY = 1.0f;
    text->colour = 0xffffffffU;
    text->x = 350.0f;
    text->y = 50.0f;
    text->release = ReleaseLegacyHudText;
    text->draw = DrawLegacyHudText;
    text->loadGlyph = LoadLegacyHudTextGlyph;
    ++references;
    return text;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ReleaseLegacyHudText(LegacyHudText *text) {
    std::int32_t index;
    if (!text)
        return;
    std::int32_t *texture = text->glyphTextures;
    for (index = 0; index < 12; ++index) {
        if (*texture != -1) {
            ReleaseNamedTexture(*texture);
            *texture++ = -1;
        }
    }
    if (--g_legacyHudTextReferences <= 0) {
        for (index = 0; index < 24; ++index) {
            if (g_legacyHudDigitTextures[index] != -1) {
                ReleaseNamedTexture(g_legacyHudDigitTextures[index]);
                g_legacyHudDigitTextures[index] = -1;
            }
        }
        for (index = 0; index < 16; ++index) {
            if (g_legacyHudGlyphs[index].texture != -1) {
                ReleaseNamedTexture(g_legacyHudGlyphs[index].texture);
                g_legacyHudGlyphs[index].colourCode = 0;
                g_legacyHudGlyphs[index].texture = -1;
            }
        }
    }
    HLib::FreeTaskData(text);
}

#else

// The cursor advances only after a live slot.
std::int64_t ReleaseLegacyHudText(LegacyHudText *text) {
    if (!text)
        return 0;
    auto *cursor = text->glyphTextures.data();
    for (std::size_t i = 0; i < 12; ++i)
        if (*cursor != -1) {
            ReleaseNamedTexture(*cursor);
            *cursor++ = -1;
        }
    if (--references <= 0) {
        ReleaseDigits();
        for (auto &glyph : glyphs)
            if (glyph.texture != -1) {
                ReleaseNamedTexture(glyph.texture);
                glyph.reserved = 0;
                glyph.texture = -1;
            }
    }
    return HLib::FreeTaskData(text);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int8_t LoadLegacyHudTextGlyph(const void *texture) {
    std::int32_t index = 0;
    LegacyHudGlyphSlot *slots = g_legacyHudGlyphs;
    std::int32_t (*load)(const TextureChunk *) = LoadAndFindTEXN_Wrapper;
    std::int32_t limit = 15;
    std::int32_t handle;
    std::byte *textureBase;
    std::int32_t offset;
    goto test;
next:
    ++index;
test:
    if (index > limit)
        goto done;
    offset = index * sizeof(LegacyHudGlyphSlot);
    textureBase = reinterpret_cast<std::byte *>(slots) + offsetof(LegacyHudGlyphSlot, texture);
    if (*reinterpret_cast<std::int32_t *>(textureBase + offset) != -1)
        goto next;
    handle = load(reinterpret_cast<const TextureChunk *>(texture));
    if (handle != -1)
        *reinterpret_cast<std::int32_t *>(textureBase + offset) = handle;
    else
        index = -2;
done:
    if (index == 16)
        index = -2;
    return static_cast<std::int8_t>(index + 1);
}
#else
// @verified - d
std::int8_t LoadLegacyHudTextGlyph(const void *texture) {
    for (std::int32_t index = 0; index < 16; ++index) {
        LegacyHudGlyphSlot &slot = g_legacyHudGlyphs[index];
        if (slot.texture == -1) {
            const std::int32_t handle = LoadAndFindTEXN_Wrapper(reinterpret_cast<const TextureChunk *>(texture));
            if (handle == -1)
                return -1;
            slot.texture = handle;
            return static_cast<std::int8_t>(index + 1);
        }
    }
    return -1;
}

#endif

#else

// Sixteen process-owned glyph references, one-based byte identifiers.
std::int64_t LoadLegacyHudTextGlyph(const void *texn) {
    for (std::size_t i = 0; i < glyphs.size(); ++i)
        if (glyphs[i].texture == -1) {
            auto texture = static_cast<std::int32_t>(ReadTexnTexture(texn));
            if (texture == -1)
                return -1;
            glyphs[i].texture = texture;
            return static_cast<std::int64_t>(i + 1);
        }
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void DrawLegacyHudText(LegacyHudText *text) {
    if (!text)
        return;
    const char *string;
    std::int32_t *texture = text->glyphTextures;
    const std::uint16_t colour = static_cast<std::uint16_t>(
        ((text->colour & 0xf0000000U) >> 16) | ((text->colour & 0xf00000U) >> 12) | ((text->colour >> 8) & 0xf0) | ((text->colour & 0xf0) >> 4));
    for (std::int32_t index = 0; index < 12; ++index, ++texture) {
        if (*texture != -1 && (text->text[index] | (colour << 8)) != text->glyphCache[index]) {
            ReleaseNamedTexture(*texture);
            *texture = -1;
        }
    }
    string = text->text;
    std::int32_t count;
    for (count = 0; count < 12 && string[count]; ++count) {
    }
    if (!count)
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400;
    packet.colour = text->colour;
    float width = text->scaleX * 32.0f;
    float height = text->scaleY * 32.0f;
    float advance = width + text->spacing;
    if (std::fabs(width) < 1.1920928955078125e-07f)
        width = 0.0f;
    if (std::fabs(height) < 1.1920928955078125e-07f)
        height = 0.0f;
    if (std::fabs(advance) < 1.1920928955078125e-07f)
        advance = 0.0f;
    for (std::int32_t index = 0; index < count; ++index) {
        float x = static_cast<float>(count) * 0.5f;
        if (std::fabs(x) < 1.1920928955078125e-07f)
            x = 0.0f;
        x = static_cast<float>(index) - x;
        if (std::fabs(x) < 1.1920928955078125e-07f)
            x = 0.0f;
        x *= advance;
        if (std::fabs(x) < 1.1920928955078125e-07f)
            x = 0.0f;
        x += text->x;
        if (std::fabs(x) < 1.1920928955078125e-07f)
            x = 0.0f;
        std::int32_t glyph = string[index];
        std::int32_t handle;
        if (static_cast<std::uint8_t>(glyph - '0') <= 9) {
            handle = g_legacyHudDigitTextures[glyph - '0'];
            AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(text->y),
                static_cast<std::int32_t>(width), static_cast<std::int32_t>(height));
            DrawLegacySpritePacket(packet, handle, text->depth);
        } else if (glyph == ':') {
            if (text->glyphTextures[index] == -1) {
                const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                handle = SetupUIElement(control->retainedTaskD, "\241\247", 32, colour, 0, 1);
            } else {
                handle = text->glyphTextures[index];
            }
            if (handle != -1) {
                text->glyphTextures[index] = handle;
                LegacyIconSprite sprite;
                sprite.flags = 0;
                sprite.rotation = 0;
                sprite.scaleX = 1.0f;
                sprite.scaleY = 1.0f;
                sprite.depth = text->depth;
                sprite.alignment = 5;
                sprite.x = x;
                if (std::fabs(x) < 1.1920928955078125e-07f)
                    sprite.x = 0.0f;
                sprite.y = text->y;
                sprite.width = 32;
                sprite.height = 32;
                sprite.icon = static_cast<std::uint16_t>(handle);
                PrepareDrawTextureWithMode(sprite);
            }
        } else {
            glyph = static_cast<std::int8_t>(glyph - 1);
            if (static_cast<std::uint32_t>(glyph) <= 15 && g_legacyHudGlyphs[glyph].texture != -1) {
                AdjustScreenAspectRatio(packet.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(text->y),
                    static_cast<std::int32_t>(width), static_cast<std::int32_t>(height));
                DrawLegacySpritePacket(packet, g_legacyHudGlyphs[glyph].texture, text->depth);
            }
        }
        text->glyphCache[index] = glyph | (colour << 8);
    }
}

#else

// PC glyph submissions end at the native nullsub; colon texture allocation and screen transformation remain live.
void DrawLegacyHudText(LegacyHudText *text) {
    if (!text)
        return;
    const auto colour =
        static_cast<std::uint16_t>((text->colour >> 16 & 0xf000) | (text->colour >> 4 & 0xf) | (text->colour >> 8 & 0xf0) | (text->colour >> 12 & 0xf00));
    for (std::size_t i = 0; i < 12; ++i) {
        if (text->glyphTextures[i] != -1 && text->glyphCache[i] != ((static_cast<std::uint32_t>(colour) << 8) | static_cast<std::uint8_t>(text->text[i]))) {
            ReleaseNamedTexture(text->glyphTextures[i]);
            text->glyphTextures[i] = -1;
        }
    }
    std::size_t count{};
    while (count < 12 && text->text[count])
        ++count;
    const auto width = Flush(text->scaleX * 32.0f);
    const auto height = Flush(text->scaleY * 32.0f);
    const auto advance = Flush(text->scaleX * 32.0f + text->spacing);
    LegacySpritePacket packet{};
    packet.quad.flags = 1024;
    packet.colour = text->colour;
    packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    for (std::size_t i = 0; i < count; ++i) {
        const auto x = Flush(Flush(Flush(static_cast<float>(i) - Flush(static_cast<float>(count) * 0.5f)) * advance) + text->x);
        auto glyph = static_cast<std::uint8_t>(text->text[i]);
        if (glyph >= '0' && glyph <= '9') {
            TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(text->y),
                TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(digits[glyph - '0']), text->depth);
        } else if (glyph == ':') {
            auto &texture = text->glyphTextures[i];
            if (texture == -1) {
                constexpr char colonGlyph[]{static_cast<char>(0xa1), static_cast<char>(0xa7), 0};
                const MapControlData *control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                texture = static_cast<std::int32_t>(SetupUIElement(control->windowTask, colonGlyph, 32, static_cast<std::int16_t>(colour), 0, 1));
            }
            if (texture != -1) {
                LegacyIconSprite sprite{};
                sprite.x = x;
                sprite.y = text->y;
                sprite.scaleX = sprite.scaleY = 1.0f;
                sprite.depth = text->depth;
                sprite.icon = static_cast<std::uint16_t>(texture);
                sprite.width = sprite.height = 32;
                sprite.alignment = 5;
                PrepareDrawTextureWithMode(sprite, ScreenMode::AdjustedWithOffset);
            }
        } else {
            --glyph;
            if (glyph < 16 && glyphs[glyph].texture != -1) {
                TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(text->y),
                    TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1.0f, 1.0f, 0, ScreenMode::AdjustedWithOffset);
                LegacySpriteNullsub(packet, static_cast<std::uint32_t>(glyphs[glyph].texture), text->depth);
            }
        }
        text->glyphCache[i] = (static_cast<std::uint32_t>(colour) << 8) | glyph;
    }
}

#endif

} // namespace s2
} // namespace shenmue
