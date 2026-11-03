#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0362.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Start0362(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0362, 4, 11, sizeof(Area0273Parameter), TASK_EVNT);
    Area0273Parameter *parent = (Area0273Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->task = task;
    parent->state = 1;
    parent->author = 0;
    Setup0362();
    Queue_EVT_UTL_LoadNPCChara_3(area0362Characters, parent->task);
}
#else
void Update0362(void *taskData)
{
    Area0273Parameter *parent = (Area0273Parameter *)taskData;
    switch (parent->state) {
    case 1:
        Queue0362Author(parent);
        break;
    case 3:
        Cleanup0362(parent);
        break;
    }
}

void Queue0362Author(Area0273Parameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0362Author, 4, 11, sizeof(Area0362AuthorParameter), TASK_EVNT);
    Area0362AuthorParameter *child = (Area0362AuthorParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->frame = 0;
    Prepare0362Characters();
    FTBL.EV_authSceneCtrl(parent->author);
    parent->state = 2;
}

void Update0362Author(void *taskData)
{
    Area0362AuthorParameter *child = (Area0362AuthorParameter *)taskData;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        Restore0362Characters();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Setup0362(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0362");
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(AREA_0362_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
}

void Cleanup0362(Area0273Parameter *parent)
{
    EVT_UTL_FreeNPCChara(area0362Characters);
    FTBL.EV_FreeAuthorPak(0, "0362");
    FTBL.HLib_DestroyTask(parent->task);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0362_DESTINATION, 0);
}

const shen_name area0362Characters[] = {AREA_0362_KUN, INVALID_CHARACTER};

void Prepare0362Characters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorCharacter(AREA_0362_KUN);
    HideAuthorProp(AREA_0362_PROP_A047);
    HideAuthorProp(AREA_0362_PROP_AA76);
}

void Restore0362Characters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorCharacter(AREA_0362_KUN);
    ShowAuthorProp(AREA_0362_PROP_A047);
    ShowAuthorProp(AREA_0362_PROP_AA76);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
#include "s2/areas/area_0362.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {

// @verified - p
void Initialize0362() {
}

// @verified - p
void Start0362(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0362, 4, 0x0b, sizeof(Area0273Parameter), AUTHOR_VIEWER_TASK);
    Area0273Parameter *const parent = static_cast<Area0273Parameter *>(HLib::GetTaskParameter(task));
    parent->task = task;
    parent->author = 0;
    parent->state = 1;
    Setup0362();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0362_CHARACTERS, parent->task);
}

// @verified - p
void Update0362(void *taskData) {
    Area0273Parameter *const parent = static_cast<Area0273Parameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0362AuthorTask, 4, 0x0b, sizeof(Area0362AuthorParameter), AUTHOR_VIEWER_TASK);
        Area0362AuthorParameter *const child = static_cast<Area0362AuthorParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->frame = 0;
        Prepare0362Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0362(parent);
    }
}

// @verified - p
void Setup0362() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0362_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0362_LIGHT);
    PushGlobalLightingProgram();
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
}

// @verified - p
void Cleanup0362(Area0273Parameter *parent) {
    for (const shen_name *character = AREA_0362_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0362_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->task);
    EV_scenQueueLoad(g_actNumber, AREA_0362_DESTINATION, 0);
}

// @verified - p
std::int64_t Update0362Author(Area0362AuthorParameter *child) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        Restore0362Characters();
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0362AuthorTask(void *taskData) {
    Update0362Author(static_cast<Area0362AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0362Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0362_KUN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0362_KUN, false);
    EV_SetCharacterAlphaEnabled(AREA_0362_PROP_A047, false);
    EV_SetCharacterAlphaEnabled(AREA_0362_PROP_AA76, false);
}

// @verified - p
void Restore0362Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0362_KUN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0362_KUN, true);
    EV_SetCharacterAlphaEnabled(AREA_0362_PROP_A047, true);
    EV_SetCharacterAlphaEnabled(AREA_0362_PROP_AA76, true);
}

const ScriptAreaDefinition area0362ScriptDefinition{.areaId = AREA_0362, .field04 = 0, .init = Initialize0362, .main = Start0362};

} // namespace shenmue::s2::areas
#endif
