#include "s2/areas/area_0069.hpp"
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
#include "s2/blur_figure_pose.hpp"
#include "s2/load_title.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0069MainCallback(void *taskData);
void E0069LoopCallback(void *taskData);

// @verified - p
void E0069Init() {
}

// @verified - p
void E0069Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0069MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0069_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0069_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(4, 0);
    EV_FreeAuthorPak(1, AREA_0069_CONVERSATION);
}

// @verified - p
void SetupE0069Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0069_KNK_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0069_KNK_, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A250, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A266, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A251, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A264, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A252, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A265, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A253, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A263, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A254, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A262, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A255, false);
    EV_SetCharacterAlphaEnabled(AREA_0069_A261, false);
    LoadBlurModelForCharacter(AREA_0069_KNK_, "KNK_B.MT7", 1.0f);
}

// @verified - p
void RestoreE0069Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0069_KNK_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0069_KNK_, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A250, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A266, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A251, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A264, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A252, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A265, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A253, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A263, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A254, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A262, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A255, true);
    EV_SetCharacterAlphaEnabled(AREA_0069_A261, true);
}

// @verified - p
void CleanupE0069(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0069_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0069_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    scenSetTaskFlags(9);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0069Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0069LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0069Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0069MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0069Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0069(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 6;
        } else if (input == 128 && ++parent->selection > 6)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 6) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0069_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void E0069LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0069Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0069ScriptDefinition{.areaId = AREA_0069_AREA, .field04 = 0, .init = E0069Init, .main = E0069Main};

} // namespace shenmue::s2::areas
