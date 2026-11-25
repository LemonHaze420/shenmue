#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/battle_qte_sprites.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/window.hpp"
#include "s2/fight.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pak_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/conversation_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include <cstring>
#include "s2/character_control_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/math.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/matrix.hpp"
#include <cstdio>
#include "s2/motion_registry_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/osage_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/conversation_fade_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include "s2/ui_state_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/camera_task_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_spline_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_draw_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include <math.h>
#include <float.h>
#include "s2/camera_conversation_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/npc_data_dc.hpp"
#endif
#else
#include "s2/battle_qte_sprites.hpp"

#include "s2/fight_stage.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/pak.hpp"
#include "s2/map_control.hpp"
#include "s2/save.hpp"
#include <bit>
#include <cstring>
#include <limits>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::array<FightLoadSpriteRecord, 6> g_fightLoadSprites;

std::array<std::int32_t, 24> g_fightLoadDigits;

namespace {

// Native FBQTE's six handles are immediately followed by FBGAUGE's seven; QTE icon indices address both banks.
std::array<std::uint32_t, 13> spriteTextures{0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU,
    0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU, 0xffffffffU};

LegacyIconSprite eventSprite{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int64_t LoadFBQTE();
std::int64_t fbLoadGauge();
void FreeFBQTE();
void FreeBattleGaugeSprites();
std::int64_t SetupLoadSprite();
void FreeLoadSprites();
void DrawFightLoadDigit(std::int32_t index);
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index);
void DrawBattleQteSprite(
    std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth, ScreenMode mode);
void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void PrepareEventSprite_Callback(void *data) {
    // ExecuteCallbackNow replaces RCX with the payload, retaining native
    // RDX as the second argument of PrepareDrawWithMode.
    PrepareDrawWithMode(
        *static_cast<const LegacyIconSprite *>(data), static_cast<ScreenMode>(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data))));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

FightRuntimePrefix *vfmaGetTaskParam();

FightRuntimePrefix *vfmaGetTaskParam();

FightRuntimePrefix *vfmaGetTaskParam();

FightRuntimePrefix *vfmaGetTaskParam();

FightRuntimePrefix *vfmaGetTaskParam();

FightRuntimePrefix *vfmaGetTaskParam();

void PrepareBattleEventSprite(void *parameter);

FightLoadSpriteRecord g_fightLoadSprites[6];

std::int32_t g_fightLoadDigits[24];

std::int32_t g_battleSpriteTextures[13] = {};

BattleQteSpriteDimensions battleQteSpriteDimensions[13] = {{64, 64, 1, 3}, {64, 64, 1, 3}, {64, 64, 1, 3}, {64, 64, 1, 3}, {64, 64, 1, 3}, {64, 64, 1, 4},
    {128, 128, 1, 1}, {16, 16, 1, 2.5f}, {16, 16, 1, 2}, {64, 128, 1, 1}, {16, 16, 1, 2}, {16, 16, 1, 2.5f}, {64, 64, 1, 2}};

std::uint32_t battleQteSpriteColours[11] = {0x4040ffU, 0x10c010U, 0, 0xf01010U, 0xc0c0c0U, 0xd0d010U, 0x9050ffU, 0xff9000U, 0x70e0ffU, 0xff90ffU, 0x40200000U};

LegacyIconSprite battleEventSprite;

std::uint8_t g_vfMotnNibbles[16];

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void LoadFBQTE();
void fbLoadGauge();
void FreeBattleGaugeSprites();
void SetupLoadSprite();
void FreeLoadSprites();
void DrawFightLoadDigit(std::int32_t index);
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index);
void FreeFBQTE();
void DrawBattleQteSprite(std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth);
void vfDrawBattleGaugeIcon(std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha);
void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags);
void FreeVFGW(FightActorRuntimePrefix *actor);
void vfMotnNibbleSet(std::uint8_t code, std::int32_t value);
#else
namespace {

void PrepareEventSprite_Callback(void *data);
std::int64_t LoadBattleSpriteBank(const char *filename, shen_name token, std::size_t first, std::size_t count);

} // namespace

std::int64_t LoadFBQTE();
std::int64_t fbLoadGauge();
void FreeBattleGaugeSprites();
std::int64_t SetupLoadSprite();
void FreeLoadSprites();
void DrawFightLoadDigit(std::int32_t index);
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index);
void FreeFBQTE();
void DrawBattleQteSprite(
    std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth, ScreenMode mode);
void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetupLoadSprite() {
    char glyph[] = "\241\247";
    for (std::int32_t index = 0; index < 6; ++index)
        g_fightLoadSprites[index].textureIndex = -1;
    SetFightLoadSprite(0, glyph, 32, 32);
    for (std::int32_t index = 0; index < 24; ++index)
        g_fightLoadDigits[index] = -1;
    const std::uint32_t size = shenOpenFile("sprite", "LOADNUM.SPR");
    void *const allocation = HLib::AllocateTaskData(size, SHEN_NAME(LDTD));
    shenLoadFile("sprite", "LOADNUM.SPR", allocation, size);
    ReadTitleSpriteTextures(allocation, size, g_fightLoadDigits);
    HLib::FreeTaskData(allocation);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void ReleaseNamedTexture_Wrapper(std::int32_t index);

// @verified - d
void FreeLoadSprites() {
    std::int32_t index;
    for (index = 0; index < 6; ++index) {
        if (g_fightLoadSprites[index].textureIndex == -1)
            continue;
        ReleaseNamedTexture_Wrapper(g_fightLoadSprites[index].textureIndex);
        g_fightLoadSprites[index].textureIndex = -1;
    }
    for (index = 0; index < 24; ++index) {
        if (g_fightLoadDigits[index] == -1)
            continue;
        ReleaseNamedTexture_Wrapper(g_fightLoadDigits[index]);
        g_fightLoadDigits[index] = -1;
    }
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void CreateFightLoadMessage() {
    char message[] = "\310\257";
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_vfStrikesLeftLabelSprite = SetupUIElement(control->retainedTaskD, message, 64, 65535, 0xf000, 1);
}

// @verified - d
void DrawFightLoadDigit(std::int32_t index) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    TransformScreenSpace(packet.quad, HorizontalStretch, 264, 168, 32, 32, 0.8f, 0.8f, 0);
    if (g_fightLoadDigits[index] != -1)
        DrawLegacySpritePacket(packet, g_fightLoadDigits[index], 1.0f);
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    TransformScreenSpace(packet.quad, HorizontalStretch, 312, 168, 32, 32, 1.6f, 0.8f, 0);
    DrawLegacySpritePacket(packet, g_vfStrikesLeftLabelSprite, 1.0f);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index) {
    switch (mode) {
    case 0:
        CreateFightLoadMessage();
        break;
    case 1:
        FreeFightLoadSprites();
        break;
    case 2:
        DrawFightLoadDigit(index);
        break;
    }
}

#else

// @verified - p
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index) {
    if (mode == 1)
        FreeLoadSprites();
    else if (mode == 2)
        DrawFightLoadDigit(index);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FreeVFGW(FightActorRuntimePrefix *actor) {
    if (actor->gauge) {
        HLib::ClearTaskDataMark(actor->gauge);
        HLib::FreeTaskData(actor->gauge);
        actor->gauge = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfMotnNibbleSet(std::uint8_t code, std::int32_t value) {
    const std::uint32_t index = code & 0x3fU;
    if (index >= 16)
        return;
    if (value < 0)
        value = 0;
    if (code & 0x40U)
        g_vfMotnNibbles[index] = static_cast<std::uint8_t>((g_vfMotnNibbles[index] & 0xf0U) | static_cast<std::uint8_t>(value));
    else if (code & 0x80U)
        g_vfMotnNibbles[index] = static_cast<std::uint8_t>((g_vfMotnNibbles[index] & 0xfU) | (static_cast<std::uint32_t>(value) << 4));
    else
        g_vfMotnNibbles[index] = static_cast<std::uint8_t>(value);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void fbLoadGauge() {
    for (std::int32_t index = 6; index < 13; ++index)
        g_battleSpriteTextures[index] = 0;
    const std::uint32_t size = shenOpenFile("sprite", "FBGAUGE.SPR");
    void *const allocation = shenLoadFile("sprite", "FBGAUGE.SPR", SHEN_NAME(FGSP));
    ReadTitleSpriteTextures(allocation, size, g_battleSpriteTextures + 6);
    HLib::FreeTaskData(allocation);
}

#else

std::int64_t fbLoadGauge() {
    return LoadBattleSpriteBank("FBGAUGE.SPR", TASK_FGSP, 6, 7);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void LoadFBQTE() {
    for (std::int32_t index = 0; index < 6; ++index)
        g_battleSpriteTextures[index] = 0;
    const std::uint32_t size = shenOpenFile("sprite", "FBQTE.SPR");
    void *const allocation = shenLoadFile("sprite", "FBQTE.SPR", SHEN_NAME(FQSP));
    ReadTitleSpriteTextures(allocation, size, g_battleSpriteTextures);
    HLib::FreeTaskData(allocation);
}

#else

std::int64_t LoadFBQTE() {
    return LoadBattleSpriteBank("FBQTE.SPR", TASK_FQSP, 0, 6);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FreeBattleGaugeSprites() {
    for (std::int32_t index = 6; index < 13; ++index)
        if (g_battleSpriteTextures[index])
            ReleaseNamedTexture_Wrapper(g_battleSpriteTextures[index]);
}

#else

// @verified - p
void FreeBattleGaugeSprites() {
    for (std::size_t i = 6; i < 13; ++i)
        if (spriteTextures[i] != 0)
            ReleaseNamedTexture(static_cast<std::int32_t>(spriteTextures[i]));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FreeFBQTE() {
    for (std::int32_t index = 0; index < 6; ++index)
        if (g_battleSpriteTextures[index])
            ReleaseNamedTexture_Wrapper(g_battleSpriteTextures[index]);
}

#else

// @verified - p
void FreeFBQTE() {
    for (std::size_t i = 0; i < 6; ++i)
        if (spriteTextures[i] != 0)
            ReleaseNamedTexture(static_cast<std::int32_t>(spriteTextures[i]));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfDrawBattleGaugeIcon(std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha) {
    DrawBattleQteSprite(icon, x, y, scale, alpha, 0, 0, 0.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void DrawBattleQteSprite(std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    FightRuntimePrefix *game = 0;
    if (task)
        game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    const std::uint8_t opacity = static_cast<std::uint8_t>(static_cast<std::int32_t>(255.0f * alpha));
    std::uint32_t colour = 0x00ffffffU;
    std::uint32_t kind = icon;
    if (kind >= 14) {
        colour = battleQteSpriteColours[kind - 14];
        kind = 8;
    }
    const BattleQteSpriteDimensions *const dimensions = &battleQteSpriteDimensions[kind];
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    TransformScreenSpace(
        packet.quad, HorizontalStretch, x, y, dimensions->width, dimensions->height, scale * dimensions->scale, scale * dimensions->scale, rotation);
    packet.quad.flags |= 0x400U | static_cast<std::uint32_t>(flags);
    packet.colour = (static_cast<std::uint32_t>(opacity) << 24U) | colour;
    float drawDepth;
    if (!game || game->spriteDepth == 255) {
        if (depth != 0.0f)
            drawDepth = depth;
        else
            drawDepth = dimensions->depth;
    } else
        drawDepth = static_cast<float>(game->spriteDepth);
    DrawLegacySpritePacket(packet, g_battleSpriteTextures[kind], drawDepth);
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags) {
    if (!mode)
        DrawBattleQteSprite(static_cast<std::uint8_t>(icon), TruncateToInt32(x), TruncateToInt32(y), u32AsFloat(scaleBits), alpha, rotation, flags, 0.0f);
    else if (mode == 1) {
        battleEventSprite.x = x;
        battleEventSprite.y = y;
        battleEventSprite.scaleX = u32AsFloat(scaleBits);
        battleEventSprite.scaleY = u32AsFloat(scaleBits);
        battleEventSprite.depth = 3.0f;
        battleEventSprite.flags = 0;
        battleEventSprite.rotation = static_cast<std::uint32_t>(rotation);
        battleEventSprite.icon = static_cast<std::uint16_t>(icon);
        battleEventSprite.width = 32;
        battleEventSprite.height = 32;
        battleEventSprite.alignment = 5;
        HLib::ExecuteCallbackNow(PrepareBattleEventSprite, &battleEventSprite);
    }
}

#else

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

#endif

} // namespace s2
} // namespace shenmue
