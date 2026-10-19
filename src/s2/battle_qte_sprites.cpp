#include "s2/battle_qte_sprites.hpp"

#include "s2/fight_stage.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/pak.hpp"
#include "s2/map_control.hpp"
#include "s2/save.hpp"
#include <bit>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
std::array<FightLoadSpriteRecord, 6> g_fightLoadSprites{};
std::array<std::int32_t, 24> g_fightLoadDigits{};
namespace {

std::array<std::uint32_t, 13> spriteTextures{0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU,
    0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU};
LegacyIconSprite eventSprite{};

void PrepareEventSprite_Callback(void *data) {


    PrepareDrawWithMode(
        *static_cast<const LegacyIconSprite *>(data), static_cast<ScreenMode>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data))));
}

std::int64_t LoadBattleSpriteBank(const char *filename, shen_name token, std::size_t first, std::size_t count) {
    for (std::size_t i = first; i < first + count; ++i)
        spriteTextures[i] = 0;
    void *allocation{};
    std::size_t size{};
    if (!LoadLegacySprite("sprite", filename, token, allocation, size))
        return 0;
    const std::byte *cursor = static_cast<const std::byte *>(allocation);
    const std::byte *const end = cursor + size;
    std::size_t index = first;
    while (end - cursor >= 8 && index < first + count) {
        std::uint32_t signature, chunkSize;
        std::memcpy(&signature, cursor, sizeof(signature));
        std::memcpy(&chunkSize, cursor + 4, sizeof(chunkSize));
        if (signature != SHEN_NAME(TEXN) || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        spriteTextures[index++] = static_cast<std::uint32_t>(ReadTexnTexture(cursor));
        cursor += chunkSize;
    }
    return HLib::FreeTaskData(allocation);
}
}

std::int64_t LoadFBQTE() {
    return LoadBattleSpriteBank("FBQTE.SPR", TASK_FQSP, 0, 6);
}
std::int64_t fbLoadGauge() {
    return LoadBattleSpriteBank("FBGAUGE.SPR", TASK_FGSP, 6, 7);
}

// @verified - p
void FreeFBQTE() {
    for (std::size_t i = 0; i < 6; ++i)
        if (spriteTextures[i] != 0)
            ReleaseNamedTexture(static_cast<std::int32_t>(spriteTextures[i]));
}

// @verified - p
void FreeBattleGaugeSprites() {
    for (std::size_t i = 6; i < 13; ++i)
        if (spriteTextures[i] != 0)
            ReleaseNamedTexture(static_cast<std::int32_t>(spriteTextures[i]));
}

// @verified - p
std::int64_t SetupLoadSprite() {
    for (FightLoadSpriteRecord &sprite : g_fightLoadSprites)
        sprite.textureIndex = -1;
    g_fightLoadSprites[0].width = 32;
    g_fightLoadSprites[0].height = 32;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    constexpr char glyph[]{static_cast<char>(0xa1), static_cast<char>(0xa7), 0};
    g_fightLoadSprites[0].textureIndex = static_cast<std::int32_t>(SetupUIElement(control->windowTask, glyph, 32, -1, static_cast<std::int16_t>(0xf000), 1));
    g_fightLoadDigits.fill(-1);
    void *allocation{};
    std::size_t size{};
    if (!LoadLegacySprite("sprite", "LOADNUM.GZ", SHEN_NAME(LDTD), allocation, size))
        return 0;
    const auto *cursor = static_cast<const std::byte *>(allocation);
    const auto *const end = cursor + size;
    std::size_t index = 0;
    while (end - cursor >= 8 && index < 10) {
        std::uint32_t signature, chunkSize;
        std::memcpy(&signature, cursor, sizeof(signature));
        std::memcpy(&chunkSize, cursor + 4, sizeof(chunkSize));
        if (signature != SHEN_NAME(TEXN) || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        g_fightLoadDigits[index++] = static_cast<std::int32_t>(ReadTexnTexture(cursor));
        cursor += chunkSize;
    }
    return HLib::FreeTaskData(allocation);
}

// @verified - p
void FreeLoadSprites() {
    for (FightLoadSpriteRecord &sprite : g_fightLoadSprites) {
        if (sprite.textureIndex == -1)
            continue;
        ReleaseNamedTexture(sprite.textureIndex);
        sprite.textureIndex = -1;
    }
    for (std::int32_t &textureIndex : g_fightLoadDigits) {
        if (textureIndex == -1)
            continue;
        ReleaseNamedTexture(textureIndex);
        textureIndex = -1;
    }
}

// @verified - p
void DrawFightLoadDigit(std::int32_t index) {
    LegacySpritePacket packet;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    packet.quad.flags = 0x400U;
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 264, 168, 32, 32, 0.8f, 0.8f, 0, ScreenMode::AdjustedWithOffset);
    if (g_fightLoadDigits[index] != -1)
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[index]), 1.0f);
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    packet.quad.flags = 0x400U;
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 312, 168, 32, 32, 1.6f, 0.8f, 0, ScreenMode::AdjustedWithOffset);
}

// @verified - p
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index) {
    if (mode == 1)
        FreeLoadSprites();
    else if (mode == 2)
        DrawFightLoadDigit(index);
}

// @verified - p
void DrawBattleQteSprite(
    std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth, ScreenMode mode) {
    VFMA_t *const stage = vfmaGetTaskParam();
    std::uint32_t colour = 0x00ffffffU;
    if (icon >= 14U) {
        colour = battleQteSpriteColours[icon - 14U];
        icon = 8;
    }
    const BattleQteSpriteDimensions &dimensions = battleQteSpriteDimensions[icon];
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    packet.reservedTail = 0;
    const float scaled = scale * dimensions.scale;
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, x, y, dimensions.width, dimensions.height, scaled, scaled, rotation, mode);
    packet.quad.flags |= static_cast<std::uint32_t>(flags) | 0x400U;
    const float alphaScaled = alpha * 255.0f;
    const std::int64_t alphaValue = alphaScaled >= -9223372036854775808.0f && alphaScaled < 9223372036854775808.0f ? static_cast<std::int64_t>(alphaScaled) :
                                                                                                                     std::numeric_limits<std::int64_t>::min();
    packet.colour = colour | static_cast<std::uint32_t>(static_cast<std::uint8_t>(alphaValue)) << 24U;
    if (stage && stage->spriteDepth != 0xffU)
        depth = static_cast<float>(stage->spriteDepth);
    else if (depth == 0.0f)
        depth = dimensions.depth;
    LegacySpriteNullsub(packet, spriteTextures[icon], depth);
}

void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags) {
    if (mode == 0) {
        DrawBattleQteSprite(static_cast<std::uint8_t>(icon), TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(y), std::bit_cast<float>(scaleBits), alpha,
            rotation, flags, 0.0f, ScreenMode::Centered);
    } else if (mode == 1) {
        eventSprite.x = x;
        eventSprite.y = y;
        eventSprite.scaleX = std::bit_cast<float>(scaleBits);
        eventSprite.scaleY = std::bit_cast<float>(scaleBits);
        eventSprite.depth = 3.0f;
        eventSprite.flags = 0;
        eventSprite.rotation = static_cast<std::uint32_t>(rotation);
        eventSprite.icon = static_cast<std::uint16_t>(icon);
        eventSprite.width = 32;
        eventSprite.height = 32;
        eventSprite.alignment = 5;
        HLib::ExecuteCallbackNow(PrepareEventSprite_Callback, &eventSprite);
    }
}

} // namespace shenmue::s2
