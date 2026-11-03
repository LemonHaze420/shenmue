#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0252.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void E0252Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0252MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0252A.MOT");
    parent->state = 1;
    parent->selection = 0;
    ConfigureE0252Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0252Characters, parent->self);
}
#else
void E0252MainCallback(void *taskData)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)taskData;
    switch (parent->state) {
    case 1:
        QueueE0252Author(parent);
        break;
    case 3:
        CleanupE0252(parent);
        break;
    }
}

void QueueE0252Author(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0252LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0252Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0252LoopCallback(void *taskData)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)taskData;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        RestoreE0252Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ConfigureE0252Environment(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 11;
    time.minute = 30;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0252");
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(AREA_0252_L001);
    FTBL.PushGlobalLightingProgram();
}

void CleanupE0252(AreaAuthorTaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(area0252Characters);
    FTBL.EV_FreeAuthorPak(0, "0252");
    FTBL.DeleteMotion(parent->motion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0252_QR00, 97);
}

const shen_name area0252Characters[] = {AREA_0252_CLJ_, AREA_0252_WON_, AREA_0252_JOP_, AREA_0252_REN_, AREA_0252_CHO_, INVALID_CHARACTER};

void SetupE0252Characters(void)
{
    int i;
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorProp(AREA_0252_A209);
    HideAuthorProp(AREA_0252_A207);
    HideAuthorProp(AREA_0252_A208);
    HideAuthorProp(AREA_0252_A270);
    HideAuthorProp(AREA_0252_A326);
    HideAuthorProp(AREA_0252_A300);
    for (i = 0; area0252Characters[i] != INVALID_CHARACTER; i++)
        HideAuthorCharacter(area0252Characters[i]);
}

void RestoreE0252Characters(void)
{
    int i;
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0252_A209);
    ShowAuthorProp(AREA_0252_A207);
    ShowAuthorProp(AREA_0252_A208);
    ShowAuthorProp(AREA_0252_A270);
    ShowAuthorProp(AREA_0252_A326);
    ShowAuthorProp(AREA_0252_A300);
    for (i = 0; area0252Characters[i] != INVALID_CHARACTER; i++)
        ShowAuthorCharacter(area0252Characters[i]);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
#include "s2/areas/area_0252.hpp"
#include "s2/character.hpp"
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0252MainCallback(void *taskData);
void E0252LoopCallback(void *taskData);

// @verified - p
void E0252Init() {
}

// @verified - p
void ConfigureE0252Environment() {
    DateTime time;
    shenGetCurrentDateTime(time);
    time.hour = 11;
    time.minute = 30;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0252_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0252_L001);
    PushGlobalLightingProgram();
}

// @verified - p
void E0252Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0252MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0252_MOTION));
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0252Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0252_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0252Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A209, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A207, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A208, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A270, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A326, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A300, false);
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0252Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A209, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A207, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A208, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A270, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A326, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A300, true);
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0252(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0252_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0252_QR00, 97);
}

// @verified - p
void E0252MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0252LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        EV_spmBeginSampleExecute();
        SetupE0252Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0252(parent);
}

// @verified - p
void E0252LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0252Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0252ScriptDefinition{.areaId = AREA_0252_AREA, .field04 = 0, .init = E0252Init, .main = E0252Main};

} // namespace shenmue::s2::areas
#endif
