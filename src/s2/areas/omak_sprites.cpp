#include "s2/areas/area_omak.hpp"
#include "s2/areas/omak_sprite_tables.hpp"
#include "s2/debug.hpp"
#include "s2/input.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"

namespace shenmue::s2::areas {
void DeleteOmakeSprites(OmakeWork *work) {
    for (std::int32_t index = 0; index < 512; ++index) {
        if (!work->sprites[index].name.name)
            continue;
        const shen_name name = work->sprites[index].name;
        for (std::int32_t match = 0; match < 512; ++match) {
            if (work->sprites[match].name.name == name.name) {
                work->sprites[match].name = shen_name{};
                break;
            }
        }
    }
}
OmakeSprite *FindOmakeSprite(OmakeWork *work, shen_name name) {
    for (std::int32_t index = 0; index < 512; ++index)
        if (work->sprites[index].name.name == name.name)
            return work->sprites + index;
    EV_debug_log("OMAK\\eOMAK_et_omak.c", 678, "evl_omak_Sprite_Search:Not Found\n");
    return work->sprites;
}
std::int32_t CreateOmakeSprite(OmakeWork *work, shen_name name) {
    std::int32_t index = 0;
    for (; index < 512; ++index)
        if (work->sprites[index].name.name == name.name)
            break;
    if (index < 512)
        EV_debug_log("OMAK\\eOMAK_et_omak.c", 637, "evl_omak_Sprite_Create:Already Created\n");
    else {
        for (index = 0; index < 512; ++index)
            if (!work->sprites[index].name.name)
                break;
        if (index >= 512)
            EV_debug_log("OMAK\\eOMAK_et_omak.c", 644, "evl_omak_Sprite_Create:Buffer Overflow\n");
    }
    work->sprites[index].name = name;
    OmakeSprite *sprite = FindOmakeSprite(work, name);
    sprite->packet.quad.flags = 0;
    sprite->packet.colour = 0xffffffff;
    sprite->packet.secondaryColour = 0;
    sprite->packet.uv = {0, 0, 1, 1};
    sprite->packet.reservedTail = 0;
    return static_cast<std::int32_t>(sprite - work->sprites);
}
void OmakeSpriteDrawNullsub(const LegacySpritePacket *, float) {
}
void OmakeSpriteDrawCallback(void *data) {
    if (omakeDrawEnabled)
        DrawOmakeSprites(*static_cast<OmakeWork **>(data));
}
void DrawOmakeSprites(OmakeWork *work) {
    for (std::int32_t index = 0; index < 512; ++index) {
        OmakeSprite *sprite = work->sprites + index;
        if (sprite->name.name)
            OmakeSpriteDrawNullsub(&sprite->packet, sprite->depth);
    }
    std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(1, 2));
    if (input & 0x800)
        work->debugSprite = ~work->debugSprite;
    if (work->debugSprite < 0)
        return;
    if (input & 0x80) {
        while (++work->debugSprite < 512)
            if (work->sprites[work->debugSprite].name.name)
                break;
        if (work->debugSprite >= 512)
            work->debugSprite = 0;
    }
    if (input & 0x40) {
        while (--work->debugSprite >= 0)
            if (work->sprites[work->debugSprite].name.name)
                break;
        if (work->debugSprite < 0)
            work->debugSprite = 511;
    }
    OmakeSprite *sprite = work->sprites + work->debugSprite;
    if (sprite->name.name) {
        const float *c = sprite->packet.quad.corners.data();
        std::int32_t x = TruncateSpriteCoordinate(((c[0] + c[2]) + c[4] + c[6]) * 0.25f) + (input & 0x8000 ? 1 : 0);
        std::int32_t y = TruncateSpriteCoordinate(((c[1] + c[3]) + c[5] + c[7]) * 0.25f) + (input & 0x2000 ? 1 : 0);
        std::int32_t width = TruncateSpriteCoordinate(((c[2] - c[0]) - c[4] + c[6]) * 0.5f) + (input & 0x200 ? 1 : 0);
        std::int32_t height = TruncateSpriteCoordinate(((c[5] - (c[3] + c[1])) + c[7]) * 0.5f) + (input & 2 ? 1 : 0);
        if (input & 0x4000)
            --x;
        if (input & 0x1000)
            --y;
        if (input & 4)
            --width;
        if (input & 0x400)
            --height;
        AdjustScreenAspectRatio(sprite->packet.quad, ScreenAlignment::HorizontalStretch, x, y, width, height, static_cast<ScreenMode>(0));
    }
    input = static_cast<std::uint16_t>(EVT_GetControllerInput(2, 2));
    if (input & 0x1000)
        ++work->animationFrames;
    if (input & 0x2000)
        --work->animationFrames;
    if (input & 2) {
        const std::uint32_t flags = omakeCategories[work->selection->category].entries[work->selection->selected[work->selection->category]].requiredFlags;
        for (std::uint8_t bit = 0; bit < 32; ++bit)
            if (flags & (1U << bit))
                SetCurrentGameFlag(bit, 1);
    }
}
bool HasOmakeText(OmakeWork *work, shen_name name) {
    for (std::int32_t index = 0; index < 128; ++index)
        if (work->textTextures[index].name.name == name.name)
            return true;
    return false;
}
std::int32_t FindOmakeText(OmakeWork *work, shen_name name) {
    for (std::int32_t index = 0; index < 128; ++index)
        if (work->textTextures[index].name.name == name.name)
            return work->textTextures[index].texture;
    EV_debug_log("OMAK\\eOMAK_et_omak.c", 918, "evl_omak_MojiTexture_Search:Not Found\n");
    return work->textTextures[0].texture;
}
std::int32_t CreateOmakeText(OmakeWork *work, shen_name name, const char *text, std::uint16_t colour) {
    std::int32_t index = 0;
    for (; index < 128; ++index)
        if (work->textTextures[index].name.name == name.name)
            break;
    if (index < 128)
        EV_debug_log("OMAK\\eOMAK_et_omak.c", 847, "evl_omak_MojiTexture_Create:Already Created\n");
    else {
        for (index = 0; index < 128; ++index)
            if (!work->textTextures[index].name.name)
                break;
        if (index >= 128)
            EV_debug_log("OMAK\\eOMAK_et_omak.c", 854, "evl_omak_MojiTexture_Create:Buffer Overflow\n");
    }
    work->textTextures[index].name = name;
    work->textTextures[index].texture = static_cast<std::int32_t>(SetupUIElement(nullptr, text, 512, static_cast<std::int16_t>(colour), -4096, 0));
    return work->textTextures[index].texture;
}
void OmakeTextDeleteCallback(void *data) {
    OmakeDeleteTextureWork *work = static_cast<OmakeDeleteTextureWork *>(data);
    const std::int32_t frames = work->frames--;
    if (frames <= 0) {
        ReleaseNamedTexture(work->texture);
        HLib::DestroyCurrentTask();
    }
}
void QueueOmakeTextDelete(OmakeWork *work, shen_name name) {
    std::int32_t index = 0;
    for (; index < 128; ++index)
        if (work->textTextures[index].name.name == name.name)
            break;
    if (index >= 128) {
        EV_debug_log("OMAK\\eOMAK_et_omak.c", 889, "evl_omak_MojiTexture_Delete:Not Found\n");
        return;
    }
    work->textTextures[index].name = shen_name{};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(OmakeTextDeleteCallback, 5, 0x0b, sizeof(OmakeDeleteTextureWork), OMAKE_EVENT);
    OmakeDeleteTextureWork *deletion = static_cast<OmakeDeleteTextureWork *>(HLib::GetTaskParameter(task));
    deletion->owner = work;
    deletion->frames = 2;
    deletion->texture = work->textTextures[index].texture;
}
void CreateOmakeMenuSprites(OmakeWork *work) {
    DeleteOmakeSprites(work);
    CreateOmakeText(work, OMAKE_UNKNOWN, "???", 0xffff);
    CreateOmakeText(work, OMAKE_SPACE, " ", 0xffff);
    for (const OmakeSpriteSetup &setup : OMAKE_SPRITE_SETUP) {
        CreateOmakeSprite(work, setup.name);
        OmakeSprite *sprite = FindOmakeSprite(work, setup.name);
        sprite->packet.quad.flags |= setup.flags;
        AdjustScreenAspectRatio(
            sprite->packet.quad, static_cast<ScreenAlignment>(setup.alignment), setup.x, setup.y, setup.width, setup.height, static_cast<ScreenMode>(0));
        sprite->texture = setup.texture < 0 ? FindOmakeText(work, OMAKE_UNKNOWN) : work->textures[setup.texture];
        sprite->depth = setup.depth;
        if (setup.name.name == OMAKE_MENU_ROWS[1].name || setup.name.name == OMAKE_MENU_ROWS[2].name || setup.name.name == OMAKE_MENU_ROWS[3].name)
            sprite->packet.secondaryColour = 0;
    }
}
} // namespace shenmue::s2::areas
