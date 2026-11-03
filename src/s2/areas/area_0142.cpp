#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0142.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void E0142Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0142MainCallback, 4, 11, sizeof(E0142TaskParameter), TASK_EVNT);
    E0142TaskParameter *parent = (E0142TaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
    parent->selection = 0;
    SetupE0142Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0142Characters, parent->self);
}
#else
shen_name area0142Characters[] = {AREA_0142_KUN_, INVALID_CHARACTER};

void E0142MainCallback(void *taskData)
{
    E0142TaskParameter *parent = (E0142TaskParameter *)taskData;
    switch (parent->state) {
    case 1:
        QueueE0142Loop(parent);
        break;
    case 3:
        CleanupE0142(parent);
        break;
    }
}

void QueueE0142Loop(E0142TaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0142LoopCallback, 4, 11, sizeof(E0142LoopParameter), TASK_EVNT);
    E0142LoopParameter *child = (E0142LoopParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->frame = 0;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0142Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0142LoopCallback(void *taskData)
{
    E0142LoopParameter *child = (E0142LoopParameter *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTimedTask(child->parent->self, 180);
        RestoreE0142Characters();
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void SetupE0142Environment(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 22;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0142");
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_lightLoad(AREA_0142_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
}

void CleanupE0142(E0142TaskParameter *parent)
{
    EventDateTime time;
    EVT_UTL_FreeNPCChara(area0142Characters);
    FTBL.AddInventoryItem(0x1f4c, 1, 0);
    FTBL.EV_FreeAuthorPak(0, "0142");
    FTBL.HLib_DestroyTask(parent->self);
    AdvanceSleepToMorning();
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 8;
    time.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.SetSceneInfos(3, AREA_0142_NEXT_MAP, 0);
    FTBL.StartShowScreenOfId(4);
}

void SetupE0142Characters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorCharacter(AREA_0142_KUN_);
    HideAuthorProp(AREA_0142_A540);
}

void RestoreE0142Characters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorCharacter(AREA_0142_KUN_);
    ShowAuthorProp(AREA_0142_A540);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
#include "s2/areas/area_0142.hpp"
#include "s2/inventory.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
void E0142MainCallback(void *data);
void E0142LoopCallback(void *data);
// @verified - p
void E0142Init() {
}
// @verified - p
void E0142Main(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0142MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0142TaskParameter *const parent = static_cast<E0142TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    DateTime time;
    shenGetCurrentDateTime(time);
    time.hour = 22;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0142");
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0142_LIGHT);
    PushGlobalLightingProgram();
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0142_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0142Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0142_KUN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0142_KUN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0142_A540, false);
}
// @verified - p
void RestoreE0142Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0142_KUN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0142_KUN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0142_A540, true);
}
// @verified - p
void CleanupE0142(E0142TaskParameter *parent) {
    for (const shen_name *character = AREA_0142_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    AddInventoryItem(0x1f4c, 1, 0);
    EV_FreeAuthorPak(0, "0142");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    shenUpdateCurrentDateTime();
    DateTime time;
    shenGetCurrentDateTime(time);
    time.hour = 8;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetSceneInfos(3, AREA_0142_NEXT_MAP, 0);
    StartShowScreenOfId(4);
}
// @verified - p
void QueueE0142Loop(E0142TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0142LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0142LoopParameter *const child = static_cast<E0142LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0142Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void E0142MainCallback(void *data) {
    E0142TaskParameter *const parent = static_cast<E0142TaskParameter *>(data);
    if (parent->state == 1)
        QueueE0142Loop(parent);
    else if (parent->state == 3)
        CleanupE0142(parent);
}
// @verified - p
void E0142LoopCallback(void *data) {
    E0142LoopParameter *const child = static_cast<E0142LoopParameter *>(data);
    E0142TaskParameter *const parent = child->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTimedTask(parent->self, 180);
    RestoreE0142Characters();
    parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0142ScriptDefinition{.areaId = AREA_0142_AREA, .field04 = 0, .init = E0142Init, .main = E0142Main};
} // namespace shenmue::s2::areas
#endif
