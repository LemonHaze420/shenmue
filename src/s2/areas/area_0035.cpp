#include "s2/areas/area_0035.hpp"
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0035MainCallback(void *taskData);
void E0035LoopCallback(void *taskData);

// @verified - p
void E0035Init() {
}

// @verified - p
void E0035Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0035MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0035_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0035_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0035_CONVERSATION);
    EV_SetObjectFlagByIndex(2, 0);
}

// @verified - p
void SetupE0035Characters() {
    const Vector3 position{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    ResetAreaAuthorCharacter(AREA_AUTHOR_RYO, 0.0f);
    ResetAreaAuthorCharacter(AREA_0035_KUN_, 0.0f);
    EV_AT_SetPos(AREA_0035_AB04, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_AB04, false);
    EV_AT_SetPos(AREA_0035_A058, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_A058, false);
    EV_AT_SetPos(AREA_0035_A404, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_A404, false);
    EV_AT_SetPos(AREA_0035_A405, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_A405, false);
    EV_AT_SetPos(AREA_0035_A403, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_A403, false);
    EV_AT_SetPos(AREA_0035_A406, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0035_A406, false);
    for (const shen_name *character = AREA_0035_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_AT_SetPos(*character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0035Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0035_KUN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0035_KUN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_AB04, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_A058, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_A404, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_A405, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_A403, true);
    EV_SetCharacterAlphaEnabled(AREA_0035_A406, true);
    for (const shen_name *character = AREA_0035_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0035(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0035_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0035_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0035Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0035LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0035Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0035MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0035Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0035(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 4;
        } else if (input == 128 && ++parent->selection > 4)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 4) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0035_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0035LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0035Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0035ScriptDefinition{.areaId = AREA_0035_AREA, .field04 = 0, .init = E0035Init, .main = E0035Main};

} // namespace shenmue::s2::areas
