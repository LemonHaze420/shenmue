#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0221.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void E0221Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0221MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0221A.MOT");
    parent->state = 1;
    parent->selection = (unsigned int)g_entryPoint > 1 ? 0 : g_entryPoint;
    FTBL.EV_stepSetFlags(0x13e, 0);
    ConfigureE0221Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0221Characters, parent->self);
}
#else
void E0221MainCallback(void *taskData)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)taskData;
    EventDateTime time;
    switch (parent->state) {
    case 4:
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour = 12;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.Queue_ST_LoadTitleFadeIn(0);
        parent->state = 5;
        FTBL.HLib_SetupTimedTask(parent->self, 120);
        break;
    case 5:
        FTBL.ST_LoadTitleFade();
        parent->state = 1;
        FTBL.HLib_SetupTimedTask(parent->self, 60);
        break;
    case 1:
        QueueE0221Loop(parent);
        break;
    case 3:
        CleanupE0221(parent);
        break;
    }
}

void QueueE0221Loop(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0221LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    if (parent->selection == 0)
        FTBL.EV_cuffsAttach(RYO_CHARACTER, 1, AREA_0221_REN, 0, 0.3f);
    SetupE0221Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0221LoopCallback(void *taskData)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)taskData;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (child->parent->selection == 1 && frame == 997)
        FTBL.EV_cuffsSetFlags();
    if (!FTBL.EV_authSceneCtrl(-1)) {
        if (child->parent->selection == 0) {
            child->parent->selection++;
            child->parent->state = 4;
            FTBL.HLib_SetupTimedTask(child->parent->self, 60);
        } else {
            child->parent->state = 3;
        }
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreE0221Characters();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ConfigureE0221Environment(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 9;
    time.minute = 30;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0221");
}

void CleanupE0221(AreaAuthorTaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(area0221Characters);
    FTBL.EV_FreeAuthorPak(0, "0221");
    FTBL.DeleteMotion(parent->motion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0221_QR00, 97);
}

const shen_name area0221Characters[] = {AREA_0221_WON_, AREA_0221_REN, AREA_0221_CLJ_, INVALID_CHARACTER};

void SetupE0221Characters(void)
{
    int i;
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorProp(AREA_0221_A209);
    HideAuthorProp(AREA_0221_A326);
    HideAuthorProp(AREA_0221_A207);
    HideAuthorProp(AREA_0221_A208);
    for (i = 0; area0221Characters[i] != INVALID_CHARACTER; i++)
        HideAuthorCharacter(area0221Characters[i]);
}

void RestoreE0221Characters(void)
{
    int i;
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0221_A209);
    ShowAuthorProp(AREA_0221_A326);
    ShowAuthorProp(AREA_0221_A207);
    ShowAuthorProp(AREA_0221_A208);
    for (i = 0; area0221Characters[i] != INVALID_CHARACTER; i++)
        ShowAuthorCharacter(area0221Characters[i]);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
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
    shenGetCurrentDateTime(time);
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
        if (ctrlGetScenarioChara(*character)) {
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
        shenGetCurrentDateTime(time);
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
#endif
