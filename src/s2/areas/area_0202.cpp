#include "s2/areas/area_0202.hpp"
#include "s2/sound_program.hpp"

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
#include "s2/telephone_cord.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0202MainCallback(void *taskData);
void E0202LoopCallback(void *taskData);

// @verified - p
void E0202Init() {
}

// @verified - p
void E0202Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0202MainCallback, 4, 0x0b, 0x18, AREA_AUTHOR_TASK);
    E0202TaskParameter *const parent = static_cast<E0202TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0202_MOTION));
    parent->secondMotion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0202_SECOND_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, "0202");
    EV_FreeAuthorPak(1, "0203");
    EnqueueTelephoneCord();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0202_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0202Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0202_YAN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0202_YAN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_A052, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_A053, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_A212, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_A057, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_AA21, false);
    EV_SetCharacterAlphaEnabled(AREA_0202_AA22, false);
}

// @verified - p
void RestoreE0202Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0202_YAN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0202_YAN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_A052, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_A053, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_A212, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_A057, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_AA21, true);
    EV_SetCharacterAlphaEnabled(AREA_0202_AA22, true);
}

// @verified - p
void CleanupE0202(E0202TaskParameter *parent) {
    for (const shen_name *character = AREA_0202_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    CleanupTelephoneCord();
    EV_FreeAuthorPak(0, AREA_0202_CONVERSATION);
    EV_FreeAuthorPak(0, AREA_0202_SECOND_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    DeleteMotion(std::bit_cast<std::int32_t>(parent->secondMotion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0202Loop(E0202TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0202LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0202LoopParameter *const child = static_cast<E0202LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection != 0)
        EV_LoadCutsceneFromCurrentScene("0203A000");
    else {
        EV_LoadAudio("0202A000", 0);
        EV_LoadCutsceneFromCurrentScene("0202A000");
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0202Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0202MainCallback(void *taskData) {
    E0202TaskParameter *const parent = static_cast<E0202TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0202Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0202(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0202_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0202LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<E0202LoopParameter *>(taskData)->parent->state = 0;
    RestoreE0202Characters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0202ScriptDefinition{.areaId = AREA_0202_AREA, .field04 = 0, .init = E0202Init, .main = E0202Main};

} // namespace shenmue::s2::areas
