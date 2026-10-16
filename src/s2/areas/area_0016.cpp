#include "s2/areas/area_0016.hpp"
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

void E0016MainCallback(void *taskData);
void E0016LoopCallback(void *taskData);

// @verified - p
void E0016Init() {
}

// @verified - p
void E0016Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0016MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0016_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0016_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0016_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0016Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0016_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0016_JOP_, false);
    EV_SetCharacterAlphaEnabled(AREA_0016_A396, false);
    EV_SetCharacterAlphaEnabled(AREA_0016_A718, false);
    EV_SetCharacterAlphaEnabled(AREA_0016_A298, false);
    EV_SetCharacterAlphaEnabled(AREA_0016_A714, false);
}

// @verified - p
void RestoreE0016Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0016_JOP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0016_JOP_, true);
    EV_SetCharacterAlphaEnabled(AREA_0016_A396, true);
    EV_SetCharacterAlphaEnabled(AREA_0016_A718, true);
    EV_SetCharacterAlphaEnabled(AREA_0016_A298, true);
    EV_SetCharacterAlphaEnabled(AREA_0016_A714, true);
}

// @verified - p
void CleanupE0016(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0016_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0016_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0016Loop(AreaAuthorTaskParameter *parent) {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    dateTime.hour = static_cast<std::uint32_t>(parent->selection) > 2 ? 8 : 22;
    dateTime.minute = 0;
    SetCurrentDateTimeTo(dateTime);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0016LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0016Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0016MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0016Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0016(parent);
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
            EV_debug_log_to_screen(AREA_0016_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0016LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    RestoreE0016Characters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0016ScriptDefinition{.areaId = AREA_0016_AREA, .field04 = 0, .init = E0016Init, .main = E0016Main};

} // namespace shenmue::s2::areas
