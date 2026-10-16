#include "s2/extra_telop.hpp"

#include "s2/archive_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/conversation.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {
HLib::Task *g_extraTelopTask{};
std::int32_t g_extraTelopMode{};
std::int32_t g_extraTelopPhase{};
std::int32_t g_extraTelopRemainingFrames{};
std::int32_t g_extraTelopGlyphCount{};
std::int32_t g_extraTelopActiveGlyph{};
std::array<LegacyIconSprite, 8> g_extraTelopSprites{};
std::array<std::uint32_t, 8> g_extraTelopColours{};
std::array<float, 8> g_extraTelopOffsets{};
std::array<char, 24> g_extraTelopText{};
const char *g_extraTelopDisplayedText{};
namespace {

// @verified - p
void PublishExtraTelopGlyph(std::int32_t index, std::uint32_t colour) {
    const LegacyIconSprite &sprite = g_extraTelopSprites[index];
    if (sprite.icon == 0xffffU)
        return;
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    packet.colour = colour;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
    PublishBigKanjiGlyph(index, colour);
}

// @verified - p
void PositionExtraTelopGlyphs() {
    const float spacing = 640.0f / static_cast<float>(g_extraTelopGlyphCount);
    float x = spacing * 0.5f;
    for (std::int32_t i = 0; i < g_extraTelopGlyphCount; ++i) {
        LegacyIconSprite &sprite = g_extraTelopSprites[i];
        sprite.x = x;
        x += spacing;
        sprite.y = extraTelopRowPositions[g_extraTelopGlyphCount];
        sprite.scaleX = sprite.scaleY = 1.0f;
    }
}

// @verified - p
std::int32_t UpdateProgressiveExtraTelop() {
    if (g_extraTelopPhase == 0) {
        g_extraTelopRemainingFrames = 90;
        g_extraTelopPhase = 1;
    } else if (g_extraTelopPhase == 1) {
        for (std::int32_t i = 0; i < g_extraTelopGlyphCount; ++i) {
            if (g_extraTelopSprites[i].icon == 0xffffU)
                continue;
            const std::int32_t revealFrame = i * (60 / g_extraTelopGlyphCount);
            const std::int32_t elapsed = 90 - g_extraTelopRemainingFrames;
            if (elapsed < revealFrame)
                continue;
            if (elapsed == revealFrame)
                ExecuteAicaCommand(0x208abU, 0, 0);
            PublishExtraTelopGlyph(i, 0xffffffffU);
        }
        if (--g_extraTelopRemainingFrames == 0)
            return 1;
    }
    return 0;
}

// @verified - p
std::int32_t UpdateFadingExtraTelop() {
    switch (g_extraTelopPhase) {
    case 0:
        g_extraTelopPhase = 1;
        g_extraTelopRemainingFrames = 60;
        break;
    case 1: {
        const float fraction = static_cast<float>(60 - g_extraTelopRemainingFrames) / 60.0f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>(std::fmin(1.0f, fraction) * 255.0f));
        PublishExtraTelopGlyph(0, (static_cast<std::uint32_t>(alpha) << 24U) | 0xffffffU);
        if (--g_extraTelopRemainingFrames == 0) {
            g_extraTelopRemainingFrames = 20;
            g_extraTelopPhase = 2;
        } else if (g_extraTelopRemainingFrames == 50) {
            shenPlayAudioWithFlags("JINGURU01", 2);
        }
        break;
    }
    case 2: {
        const float fraction = static_cast<float>(g_extraTelopRemainingFrames) / 20.0f;
        const std::uint8_t alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>(std::fmin(1.0f, fraction) * 255.0f));
        PublishExtraTelopGlyph(0, (static_cast<std::uint32_t>(alpha) << 24U) | 0xffffffU);
        if (--g_extraTelopRemainingFrames == 0) {
            g_extraTelopRemainingFrames = 10;
            g_extraTelopPhase = 3;
        }
        break;
    }
    case 3:
        if (--g_extraTelopRemainingFrames == 0)
            return 1;
        break;
    }
    return 0;
}

// @verified - p
std::int32_t UpdateSixGlyphExtraTelop() {
    std::uint8_t overlay = 0;
    switch (g_extraTelopPhase) {
    case 0:
        g_extraTelopRemainingFrames = 1;
        for (std::int32_t i = 0; i < 6; ++i)
            g_extraTelopColours[i] = 0x00ffffffU;
        break;
    case 1:
    case 3: {
        const std::int32_t frame = g_extraTelopRemainingFrames - 1;
        const std::int32_t index = g_extraTelopActiveGlyph;
        LegacyIconSprite &sprite = g_extraTelopSprites[index];
        g_extraTelopOffsets[index] *= 0.5f;
        sprite.y = (sprite.y + 340.0f) * 0.5f;
        sprite.rotation = 12288U * static_cast<std::uint32_t>(frame);
        g_extraTelopColours[index] = (0xff000000U - (static_cast<std::uint32_t>(frame) << 28U)) | 0xffffffU;
        sprite.scaleX = sprite.scaleY = static_cast<float>(frame) * 0.5f + 1.0f;
        break;
    }
    case 2:
    case 4:
    case 8: {
        float offset = static_cast<float>(g_extraTelopRemainingFrames) * 3.0f;
        if (g_extraTelopRemainingFrames & 1)
            offset = -offset;
        for (std::int32_t i = 0; i < 6; ++i)
            g_extraTelopOffsets[i] = offset;
        if (g_extraTelopPhase == 8)
            overlay = static_cast<std::uint8_t>(12 * g_extraTelopRemainingFrames);
        break;
    }
    case 5:
    case 6:
    case 7: {
        const float duration = static_cast<float>(extraTelopSixGlyphFrames[g_extraTelopPhase]);
        const float remaining = static_cast<float>(g_extraTelopRemainingFrames);
        g_extraTelopOffsets[0] = (127.0f / duration) * remaining;
        g_extraTelopOffsets[1] = (301.0f / duration) * remaining;
        for (std::int32_t i = 2; i < 6; ++i)
            g_extraTelopOffsets[i] = (735.0f / duration) * remaining;
        overlay = static_cast<std::uint8_t>(g_extraTelopPhase == 7 ? ~(31 * g_extraTelopRemainingFrames) : 95 - 11 * g_extraTelopRemainingFrames);
        break;
    }
    }
    for (std::int32_t i = 0; i < 6; ++i) {
        LegacyIconSprite &sprite = g_extraTelopSprites[i];
        const float x = sprite.x;
        sprite.x += g_extraTelopOffsets[i];
        PublishExtraTelopGlyph(i, g_extraTelopColours[i]);
        sprite.x = x;
    }
    vfSubmitQuad(0x01010101U * overlay, 1.0f);
    if (--g_extraTelopRemainingFrames != 0)
        return 0;
    switch (g_extraTelopPhase) {
    case 0:
        g_extraTelopSprites[0].x = 180.0f;
        g_extraTelopOffsets[0] = 540.0f;
        g_extraTelopSprites[0].y = -240.0f;
        break;
    case 1:
    case 3:
        g_extraTelopSprites[g_extraTelopActiveGlyph].y = 340.0f;
        ExecuteAicaCommand(0x208abU, 0, 0);
        break;
    case 2:
        ++g_extraTelopActiveGlyph;
        g_extraTelopSprites[1].x = 460.0f;
        g_extraTelopOffsets[1] = -540.0f;
        g_extraTelopSprites[1].y = -240.0f;
        break;
    case 4:
        PositionExtraTelopGlyphs();
        for (std::int32_t i = 2; i < 6; ++i)
            g_extraTelopColours[i] = 0xffffffffU;
        break;
    case 7:
        ExecuteAicaCommand(0x208abU, 0, 0);
        [[fallthrough]];
    case 5:
    case 6:
        ExecuteAicaCommand(0x1100abU, 0, 0);
        break;
    case 9:
        return 1;
    }
    ++g_extraTelopPhase;
    g_extraTelopRemainingFrames = extraTelopSixGlyphFrames[g_extraTelopPhase];
    return 0;
}

// @verified - p
std::int32_t UpdateFiveGlyphExtraTelop() {
    const std::int32_t remaining = --g_extraTelopRemainingFrames;
    switch (g_extraTelopPhase) {
    case 0:
        for (std::int32_t i = 0; i < 5; ++i) {
            g_extraTelopColours[i] = 0;
            g_extraTelopOffsets[i] = 0;
            g_extraTelopSprites[i].x = extraTelopFiveGlyphPositions[i].x;
            g_extraTelopSprites[i].y = extraTelopFiveGlyphPositions[i].y;
        }
        g_extraTelopRemainingFrames = 60;
        g_extraTelopPhase = 1;
        break;
    case 1:
        if (remaining == 0) {
            g_extraTelopRemainingFrames = 16;
            g_extraTelopPhase = 2;
        }
        break;
    case 2: {
        const std::int32_t alpha = 255 - 16 * remaining;
        g_extraTelopColours[0] = (static_cast<std::uint32_t>(alpha < 0 ? 0 : alpha) << 24U) | 0xffffffU;
        g_extraTelopSprites[0].scaleX = g_extraTelopSprites[0].scaleY = static_cast<float>(remaining) * 0.80000001f + 1.5f;
        if (remaining == 0) {
            ExecuteAicaCommand(0x208abU, 0, 0);
            g_extraTelopRemainingFrames = 10;
            ++g_extraTelopPhase;
        }
        break;
    }
    case 3: {
        float offset = static_cast<float>(remaining) * 0.5f;
        if (remaining & 1)
            offset = -offset;
        g_extraTelopOffsets[0] = offset;
        if (remaining == 0) {
            g_extraTelopRemainingFrames = 230;
            g_extraTelopPhase = 4;
        }
        break;
    }
    case 4:
        if (remaining < 230) {
            const std::int32_t alpha = static_cast<std::int32_t>(g_extraTelopColours[0] >> 24U) - 16;
            g_extraTelopColours[0] = (static_cast<std::uint32_t>(alpha < 0 ? 0 : alpha) << 24U) | 0xffffffU;
        }
        if (remaining == 0) {
            g_extraTelopRemainingFrames = 15;
            g_extraTelopActiveGlyph = 1;
            g_extraTelopPhase = 5;
        }
        break;
    case 5:
        if (remaining == 0) {
            ExecuteAicaCommand(0x208abU, 0, 0);
            g_extraTelopColours[g_extraTelopActiveGlyph] = 0xffffffffU;
            if (++g_extraTelopActiveGlyph == 5) {
                g_extraTelopRemainingFrames = 45;
                ++g_extraTelopPhase;
            } else {
                g_extraTelopRemainingFrames = 15;
            }
        }
        break;
    case 6:
        if (remaining == 0)
            return 1;
        break;
    }
    for (std::int32_t i = 0; i < 5; ++i) {
        LegacyIconSprite &sprite = g_extraTelopSprites[i];
        const float y = sprite.y;
        sprite.y += g_extraTelopOffsets[i];
        PublishExtraTelopGlyph(i, g_extraTelopColours[i]);
        sprite.y = y;
    }
    return 0;
}

// @verified - p
void ExtraTelopControl_Callback(void *) {
    if (!(static_cast<std::uint32_t>(g_extraTelopMode) & 0x40000000U))
        vfSubmitQuad(0xa0000000U, 0.0f);
    std::int32_t complete = 1;
    switch (static_cast<std::uint8_t>(g_extraTelopMode)) {
    case 0:
        complete = UpdateProgressiveExtraTelop();
        break;
    case 1:
        complete = UpdateFadingExtraTelop();
        break;
    case 2:
        complete = UpdateSixGlyphExtraTelop();
        break;
    case 3:
        complete = UpdateFiveGlyphExtraTelop();
        break;
    }
    if (!complete)
        return;
    for (LegacyIconSprite &sprite : g_extraTelopSprites) {
        if (sprite.icon != 0xffffU)
            ReleaseNamedTexture(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = 0xffffU;
    }
    g_extraTelopDisplayedText = nullptr;
    if (g_extraTelopMode >= 0)
        RequestFightTelopResume();
    HLib::DestroyTaskInternal(g_extraTelopTask);
    g_extraTelopTask = nullptr;
    ReleaseBigKanjiText();
    DestroyElementByIndex(UIElement::BigKanjiText);
}

} // namespace

std::int64_t ShowBigKanjiText(const char *text) {
    g_extraTelopGlyphCount = static_cast<std::int32_t>(std::strlen(text) / 2);
    if (g_extraTelopDisplayedText)
        return 0;
    const char *spriteName = nullptr;
    std::int16_t size = 32;
    for (const ExtraTelopDefinition &definition : extraTelopDefinitions) {
        if (!definition.text)
            break;
        if (std::strcmp(text, definition.text) != 0)
            continue;
        spriteName = definition.spriteName;
        size = definition.glyphSize;
        break;
    }
    for (std::int32_t i = 0; i < g_extraTelopGlyphCount; ++i) {
        LegacyIconSprite &sprite = g_extraTelopSprites[i];
        sprite.alignment = 5;
        sprite.depth = 0.5f;
        sprite.rotation = 0;
        sprite.height = sprite.width = size;
    }
    if (!spriteName)
        return 0;
    char filename[256];
    std::snprintf(filename, sizeof(filename), "%s.spr", spriteName);
    char path[260];
    std::snprintf(path, sizeof(path), "/sprite/%s", filename);
    const std::uint32_t resourceSize = static_cast<std::uint32_t>(ArchiveData_GetFileSize(path));
    const std::uint32_t npcLoadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const std::uint32_t allocationSize = static_cast<std::uint32_t>(ArchiveData_GetFileSize(path));
    void *allocation = nullptr;
    if (allocationSize && HLib::WouldAllocationFail(0x20000U, allocationSize) == 0) {
        allocation = HLib::AllocateTaskData(allocationSize, RESOURCE_TLPF);
        if (allocation && !ArchiveData_Read(path, 0, allocation, allocationSize)) {
            HLib::FreeTaskData(allocation);
            allocation = nullptr;
        }
    }
    EV_NPC_NewLoadSw(npcLoadState);
    std::array<std::int32_t, 8> handles;
    TEXNReader(allocation, resourceSize, handles.data());
    HLib::FreeTaskData(allocation);
    std::strcpy(g_extraTelopText.data(), text);
    g_extraTelopDisplayedText = g_extraTelopText.data();
    for (std::int32_t i = 0; i < g_extraTelopGlyphCount; ++i)
        g_extraTelopSprites[i].icon = static_cast<std::uint16_t>(handles[i]);
    PublishBigKanjiText(spriteName, g_extraTelopSprites.data(), g_extraTelopGlyphCount);
    ShowElementByIndex(UIElement::BigKanjiText);
    return 1;
}

// @verified - p
void QueueExtraTelop(const char *text, std::int32_t mode) {
    if (g_extraTelopTask)
        return;
    if (!g_extraTelopDisplayedText) {
        for (LegacyIconSprite &sprite : g_extraTelopSprites)
            sprite.icon = 0xffffU;
    }
    if (mode == 99) {
        g_extraTelopDisplayedText = nullptr;
        ShowBigKanjiText(text);
    } else {
        SetNewEwzoTypeForMoveLearned(1, 0, 0);
        g_extraTelopTask = HLib::EnqueueTaskWithoutParameter(ExtraTelopControl_Callback, 15, 0, static_cast<std::int32_t>(TASK_XTLP.name));
        g_extraTelopMode = mode;
        g_extraTelopPhase = 0;
        g_extraTelopRemainingFrames = 0;
        g_extraTelopActiveGlyph = 0;
        if (mode >= 0)
            RequestFightTelopPause();
        ShowBigKanjiText(text);
        PositionExtraTelopGlyphs();
    }
}

} // namespace shenmue::s2
