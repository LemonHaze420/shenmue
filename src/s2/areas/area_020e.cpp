#include "s2/areas/area_020e.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E020EMainCallback(void *taskData);
void E020ELoopCallback(void *taskData);
void CleanupE020E(AreaAuthorTaskParameter *parent);

// @verified - p
void E020EInit() {
}

// @verified - p
void E020EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E020EMainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_020E_MOTION));
    parent->selection = static_cast<std::int16_t>(g_entryPoint);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_020E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    if (g_entryPoint == 99) {
        parent->selection = 0;
        parent->state = 0;
    } else {
        parent->selection = static_cast<std::int16_t>(g_entryPoint);
        parent->state = 1;
        if (parent->selection > 8)
            CleanupE020E(parent);
    }
    Queue_EVT_UTL_LoadNPCChara_3(AREA_020E_CHARACTERS, parent->self);
}

// @verified - p
void SetupE020ECharacters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    for (const shen_name *character = AREA_020E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE020ECharacters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    for (const shen_name *character = AREA_020E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE020E(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_020E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_020E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE020ELoop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E020ELoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E020EMainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE020ELoop(parent);
    } else if (parent->state == 3) {
        CleanupE020E(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 8;
        } else if (input == 128 && ++parent->selection > 8)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 8) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_020E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void E020ELoopCallback(void *taskData) {
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(taskData);
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        SetupE020ECharacters();
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    child->parent->state = g_entryPoint == 99 ? 0 : 3;
    RestoreE020ECharacters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area020EScriptDefinition{.areaId = AREA_020E_AREA, .field04 = 0, .init = E020EInit, .main = E020EMain};

} // namespace shenmue::s2::areas
