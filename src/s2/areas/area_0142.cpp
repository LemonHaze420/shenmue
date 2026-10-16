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
    GetCurrentDateTime(time);
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
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    AddInventoryItem(0x1f4c, 1, 0);
    EV_FreeAuthorPak(0, "0142");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    shenUpdateCurrentDateTime();
    DateTime time;
    GetCurrentDateTime(time);
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
