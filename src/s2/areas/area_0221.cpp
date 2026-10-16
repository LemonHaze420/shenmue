#include "s2/areas/area_0221.hpp"
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
#include "s2/load_title.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0221MainCallback(void *taskData);
void E0221LoopCallback(void *taskData);

// @verified - p
void E0221Init() {
}

// @verified - p
void E0221Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0221MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0221_MOTION));
    parent->selection = static_cast<std::uint32_t>(g_entryPoint) > 1 ? 0 : g_entryPoint;
    parent->state = 1;
    EV_stepSetFlags(0, 0x13e);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 9;
    time.minute = 30;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0221_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0221_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0221Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0221_A209, false);
    EV_SetCharacterAlphaEnabled(AREA_0221_A326, false);
    EV_SetCharacterAlphaEnabled(AREA_0221_A207, false);
    EV_SetCharacterAlphaEnabled(AREA_0221_A208, false);
    for (const shen_name *character = AREA_0221_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0221Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0221_A209, true);
    EV_SetCharacterAlphaEnabled(AREA_0221_A326, true);
    EV_SetCharacterAlphaEnabled(AREA_0221_A207, true);
    EV_SetCharacterAlphaEnabled(AREA_0221_A208, true);
    for (const shen_name *character = AREA_0221_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0221(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0221_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0221_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0221_QR00, 97);
}

// @verified - p
void QueueE0221Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0221LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection == 0)
        EV_cuffsAttach(AREA_AUTHOR_RYO, 1, AREA_0221_REN, 0, 0.3f);
    SetupE0221Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0221MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    switch (parent->state) {
    case 1:
        QueueE0221Loop(parent);
        break;
    case 3:
        CleanupE0221(parent);
        break;
    case 4: {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 12;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        Queue_ST_LoadTitleFadeIn(shen_name{});
        parent->state = 5;
        HLib::SetupTimedTask(parent->self, 120);
        break;
    }
    case 5:
        ST_LoadTitleFade();
        parent->state = 1;
        HLib::SetupTimedTask(parent->self, 60);
        break;
    }
}

// @verified - p
void E0221LoopCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (parent->selection == 1 && frame == 997)
        EV_cuffsSetFlags();
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (parent->selection != 0)
        parent->state = 3;
    else {
        parent->selection = 1;
        parent->state = 4;
        HLib::SetupTimedTask(parent->self, 60);
    }
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0221Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0221ScriptDefinition{.areaId = AREA_0221_AREA, .field04 = 0, .init = E0221Init, .main = E0221Main};

} // namespace shenmue::s2::areas
