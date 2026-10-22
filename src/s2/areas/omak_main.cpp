#include "s2/areas/area_omak.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/area_jump.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/screen_film.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/title.hpp"
#include "s2/ui.hpp"
#include <cstring>

namespace shenmue::s2::areas {
void OmakeMainCallback(void *data) {
    OmakeWork *work = static_cast<OmakeWork *>(data);
    const std::uint32_t flags = static_cast<std::uint32_t>(GetCurrentGameFlags());
    if (GetDebugForceUIEnabled(UIElement::ShenmueCollection)) {
        if (omakeSelectedCategory != -1 && omakeSelectedEntry != -1) {
            Loop3DSoundEffect(0x3f7);
            SetPostFxFilterMode(1);
            ResetGlobalStateFlags();
            DestroyElementByIndex(UIElement::MainMenu);
            DestroyElementByIndex(UIElement::GameInfoBar);
            const OmakeEntry *entry = OMAKE_COLLECTION_TABLES[omakeSelectedCategory] + omakeSelectedEntry;
            entry->callback(work, &entry->command);
            omakeSelectedCategory = -1;
            omakeSelectedEntry = -1;
        }
        if (omakeBackRequested) {
            Loop3DSoundEffect(0x3f7);
            g_titleFlagShouldRun = 10;
            QueueOmakeModuleCall(work, &OMAKE_COLD_RESET);
            omakeBackRequested = 0;
        }
        return;
    }
    omakeSpriteAnimation = 0;
    omakeDrawEnabled = 1;
    if (work->delay > 0) {
        --work->delay;
        return;
    }
    const std::int32_t previousState = work->state;
    work->delay = 0;
    switch (previousState) {
    case 0: {
        work->loadedSprite = LoadLegacySceneFile("sprite", "OMAKE.SPR", OMAKE_AREA);
        const std::byte *texn = static_cast<const std::byte *>(work->loadedSprite);
        std::int32_t index = 0;
        for (;;) {
            std::uint32_t tag, size;
            std::memcpy(&tag, texn, 4);
            if (tag != SHEN_NAME(TEXN))
                break;
            std::memcpy(&size, texn + 4, 4);
            work->textures[index++] = static_cast<std::int32_t>(ReadTexnTexture(texn));
            texn += size;
        }
        work->state = 1;
        break;
    }
    case 1:
        HLib::FreeTaskData(work->loadedSprite);
        work->state = 2;
        break;
    case 2:
        CreateOmakeMenuSprites(work);
        EV_LoadAudio("0002S000", 0);
        SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
        ReleaseLoadTitleFade();
        RestoreOmakeArcadeSelection(work);
        work->state = 3;
        break;
    case 3: {
        std::uint16_t input = 0, trigger = 0;
        if (filmCheckFadeTrackColour())
            omakeSpriteAnimation = 10;
        else {
            FinishLoading();
            input = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_REPEAT));
            trigger = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_PRESSED));
        }
        OmakeSelection *selection = work->selection;
        if (trigger & 0x400) {
            const OmakeEntry *entry = omakeCategories[selection->category].entries + selection->selected[selection->category];
            if ((flags & entry->requiredFlags) == entry->requiredFlags) {
                Loop3DSoundEffect(0x3f7);
                SetPostFxFilterMode(1);
                ResetGlobalStateFlags();
                entry->callback(work, &entry->command);
                return;
            }
            Loop3DSoundEffect(0x3f6);
        }
        if (input & 0x200) {
            Loop3DSoundEffect(0x3f7);
            g_titleFlagShouldRun = 10;
            QueueOmakeModuleCall(work, &OMAKE_COLD_RESET);
        }
        bool categoryChanged = false;
        if (trigger & 0x4000) {
            Loop3DSoundEffect(0x3ef);
            --selection->category;
            work->animationDirection = 1;
            categoryChanged = true;
        } else if (trigger & 0x8000) {
            Loop3DSoundEffect(0x3ef);
            ++selection->category;
            work->animationDirection = 0;
            categoryChanged = true;
        }
        if (categoryChanged) {
            work->animationRemaining = work->animationFrames;
            work->refreshDescription = 1;
        }
        if (selection->category < 0) {
            selection->category = -1;
            while (omakeCategories[selection->category + 1].entries)
                ++selection->category;
        }
        if (!omakeCategories[selection->category].entries)
            selection->category = 0;
        if (input & 0x1000) {
            --selection->selected[selection->category];
            if (selection->selected[selection->category] < 0) {
                selection->selected[selection->category] = 0;
                goto update_menu;
            }
        } else if (input & 0x2000) {
            ++selection->selected[selection->category];
            if (!omakeCategories[selection->category].entries[selection->selected[selection->category]].name) {
                selection->selected[selection->category] = -1;
                while (omakeCategories[selection->category].entries[selection->selected[selection->category] + 1].name)
                    ++selection->selected[selection->category];
                goto update_menu;
            }
        } else
            goto update_menu;
        work->refreshDescription = 1;
        Loop3DSoundEffect(0x3ef);
    update_menu:
        std::int32_t previousCategory = selection->category - 1;
        if (previousCategory < 0) {
            previousCategory = -1;
            while (omakeCategories[previousCategory + 1].entries)
                ++previousCategory;
        }
        const std::int32_t nextCategory = omakeCategories[selection->category + 1].entries ? selection->category + 1 : 0;
        const std::int32_t categories[]{selection->category, previousCategory, nextCategory};
        for (std::int32_t index = 0; index < 3; ++index) {
            OmakeSprite *sprite = FindOmakeSprite(work, OMAKE_CATEGORY_ICONS[index]);
            sprite->texture = work->textures[omakeCategories[categories[index]].icon];
            sprite->depth = 2;
        }
        const std::int64_t relative = selection->selected[selection->category] - selection->scroll[selection->category];
        if (relative < 0)
            selection->scroll[selection->category] = selection->selected[selection->category];
        if (relative > 3)
            selection->scroll[selection->category] = selection->selected[selection->category] - 3;
        for (std::int32_t index = 0; index < 4; ++index) {
            OmakeSprite *sprite = FindOmakeSprite(work, OMAKE_MENU_BACKGROUNDS[index]);
            sprite->texture = work->textures[5];
            sprite->depth = 2;
        }
        const std::int64_t visible = selection->selected[selection->category] - selection->scroll[selection->category];
        if (visible >= 0 && visible <= 3) {
            OmakeSprite *sprite = FindOmakeSprite(work, OMAKE_MENU_BACKGROUNDS[visible]);
            sprite->texture = work->textures[21];
            sprite->depth = 2;
        }
        std::int32_t leftTexture = 1, rightTexture = 1;
        if (work->animationRemaining > 0) {
            --work->animationRemaining;
            if (work->animationDirection)
                leftTexture = 20;
            else
                rightTexture = 20;
        }
        OmakeSprite *left = FindOmakeSprite(work, OMAKE_LEFT_ARROW);
        left->texture = work->textures[leftTexture];
        left->depth = 1;
        OmakeSprite *right = FindOmakeSprite(work, OMAKE_RIGHT_ARROW);
        right->texture = work->textures[rightTexture];
        right->depth = 1;
        OmakeEntry *list = omakeCategories[selection->category].entries;
        std::int32_t count = 0;
        while (list[count + 1].name)
            ++count;
        if (!count)
            count = 1;
        OmakeSprite *scroll = FindOmakeSprite(work, OMAKE_SCROLL);
        AdjustScreenAspectRatio(scroll->packet.quad, ScreenAlignment::HorizontalStretch, 570,
            static_cast<std::int32_t>(123 * selection->selected[selection->category] / count) + 210, 128, 128, static_cast<ScreenMode>(0));
        const OmakeEntry *entry = list + selection->selected[selection->category];
        const bool unlocked = (flags & entry->requiredFlags) == entry->requiredFlags;
        OmakeSprite *photo = FindOmakeSprite(work, OMAKE_PHOTO);
        photo->texture = work->textures[unlocked ? entry->photoTexture : 9];
        photo->depth = 2;
        EV_EventSetUpHelpIcon_0(unlocked ? static_cast<std::uint8_t>(entry->helpTopic) : 0x82, 0);
        if (work->refreshDescription) {
            work->refreshDescription = 0;
            for (std::int32_t index = 0; index < 2; ++index) {
                const shen_name name = OMAKE_INSTRUCTIONS[index];
                if (HasOmakeText(work, name))
                    QueueOmakeTextDelete(work, name);
                CreateOmakeText(work, name, index ? entry->descriptionEnd : entry->description, 0xffff);
            }
            for (std::int32_t index = 0; index < 2; ++index) {
                OmakeSprite *sprite = FindOmakeSprite(work, OMAKE_INSTRUCTIONS[index]);
                sprite->texture = FindOmakeText(work, unlocked ? OMAKE_INSTRUCTIONS[index] : OMAKE_SPACE);
                sprite->depth = 2;
            }
            const bool twoLines = unlocked && *entry->descriptionEnd;
            AdjustScreenAspectRatio(FindOmakeSprite(work, OMAKE_INSTRUCTIONS[0])->packet.quad, ScreenAlignment::HorizontalStretch, 320, twoLines ? 387 : 407,
                512, 32, static_cast<ScreenMode>(0));
            AdjustScreenAspectRatio(FindOmakeSprite(work, OMAKE_INSTRUCTIONS[1])->packet.quad, ScreenAlignment::HorizontalStretch, 320, twoLines ? 420 : 407,
                512, 32, static_cast<ScreenMode>(0));
        }
        const std::int64_t first = selection->scroll[selection->category];
        if (categoryChanged)
            for (std::int32_t index = 0; index < 20; ++index) {
                const shen_name name{((static_cast<std::uint32_t>(first) + index) << 24) | 0x554e4dU};
                if (HasOmakeText(work, name))
                    QueueOmakeTextDelete(work, name);
            }
        for (const std::int32_t delta : {-1, 4}) {
            const shen_name name{((static_cast<std::uint32_t>(first) + static_cast<std::uint32_t>(delta)) << 24) | 0x554e4dU};
            if (HasOmakeText(work, name))
                QueueOmakeTextDelete(work, name);
        }
        for (std::int32_t index = 0; index < 4; ++index) {
            const shen_name name{((static_cast<std::uint32_t>(first) + index) << 24) | 0x554e4dU};
            const OmakeEntry *row = list + first + index;
            if (!HasOmakeText(work, name))
                CreateOmakeText(work, name, row->name, static_cast<std::uint16_t>(row->colour));
            OmakeSprite *sprite = FindOmakeSprite(work, OMAKE_MENU_ROWS[index]);
            sprite->texture = FindOmakeText(work, (flags & row->requiredFlags) == row->requiredFlags ? name : OMAKE_UNKNOWN);
            sprite->depth = 3;
        }
        break;
    }
    default:
        break;
    }
    if (previousState != work->state)
        EV_debug_log("OMAK\\eOMAK_et_omak.c", 1928, "state %i -> %i\n", previousState, work->state);
}
} // namespace shenmue::s2::areas
