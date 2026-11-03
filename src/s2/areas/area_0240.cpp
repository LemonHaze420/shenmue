#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0240.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void E0240Main(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0240MainCallback, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0240A.MOT");
    parent->state = 4;
    parent->selection = 0;
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    ConfigureE0240Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0240Characters, parent->self);
}
#else
void E0240MainCallback(void *taskData)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)taskData;
    switch (parent->state) {
    case 4:
        FTBL.SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.SetIsSkySetup(0);
        FTBL.RefreshPadTaskData(0);
        FTBL.FinishLoading();
        parent->state = 5;
        break;
    case 5:
        if (!FTBL.IsItemSelectable()) {
            FTBL.SetRenderClockAndCompassTo(1);
            FTBL.SetLoadingBusy();
            parent->state = 1;
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    case 1:
        FTBL.SetIsSkySetup(1);
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        QueueE0240Loop(parent);
        break;
    case 3:
        CleanupE0240(parent);
        break;
    }
}

void QueueE0240Loop(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(E0240LoopCallback, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.FreeWAIR();
    FTBL.UpdateWeather();
    SetupE0240Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void E0240LoopCallback(void *taskData)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)taskData;
    if (FTBL.EV_authSceneCtrl(-6) == 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.AddInventoryItem(72, 1, 1);
        FTBL.HLib_SetupTimedTask(child->parent->self, 130);
        RestoreE0240Characters();
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ConfigureE0240Environment(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0240");
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(AREA_0240_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_DisableCharacterDisplay(RYO_CHARACTER);
}

const shen_name area0240Characters[] = {AREA_0240_REN_, AREA_0240_CHO_, AREA_0240_CLJ_, AREA_0240_JOP_, INVALID_CHARACTER};

void CleanupE0240(AreaAuthorTaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(area0240Characters);
    FTBL.EV_stepSetFlags(0x119, 0);
    FTBL.EV_FreeAuthorPak(0, "0240");
    FTBL.DeleteMotion(parent->motion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0240_QR00, 99);
}

void SetupE0240Characters(void)
{
    int i;
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorProp(AREA_0240_A207);
    HideAuthorProp(AREA_0240_A209);
    HideAuthorProp(AREA_0240_A014);
    HideAuthorProp(AREA_0240_A326);
    HideAuthorProp(AREA_0240_A045);
    HideAuthorProp(AREA_0240_A208);
    HideAuthorProp(AREA_0240_A270);
    HideAuthorProp(AREA_0240_A806);
    HideAuthorProp(AREA_0240_A341);
    for (i = 0; area0240Characters[i] != INVALID_CHARACTER; i++)
        HideAuthorCharacter(area0240Characters[i]);
}

void RestoreE0240Characters(void)
{
    int i;
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0240_A207);
    ShowAuthorProp(AREA_0240_A209);
    ShowAuthorProp(AREA_0240_A014);
    ShowAuthorProp(AREA_0240_A326);
    ShowAuthorProp(AREA_0240_A045);
    ShowAuthorProp(AREA_0240_A208);
    ShowAuthorProp(AREA_0240_A270);
    ShowAuthorProp(AREA_0240_A806);
    ShowAuthorProp(AREA_0240_A341);
    for (i = 0; area0240Characters[i] != INVALID_CHARACTER; i++)
        ShowAuthorCharacter(area0240Characters[i]);
}

void HideAuthorCharacter(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowAuthorCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 1);
}
#endif
#else
#include "s2/areas/area_0240.hpp"
#include "s2/character.hpp"
#include "s2/inventory.hpp"

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
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/memo.hpp"
#include "s2/ui.hpp"
#include "s2/pause.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0240MainCallback(void *taskData);
void E0240LoopCallback(void *taskData);

// @verified - p
void E0240Init() {
}

// @verified - p
void E0240Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0240MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0240_MOTION));
    parent->selection = 0;
    parent->state = 4;
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0240_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0240_LIGHT);
    PushGlobalLightingProgram();
    EV_DisableCharacterDisplay(AREA_AUTHOR_RYO);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0240_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0240Characters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A207, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A209, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A014, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A326, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A045, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A208, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A270, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A806, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A341, false);
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0240Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A207, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A209, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A014, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A326, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A045, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A208, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A270, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A806, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A341, true);
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0240(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_stepSetFlags(0, 0x119);
    EV_FreeAuthorPak(0, AREA_0240_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0240_QR00, 99);
}

// @verified - p
void QueueE0240Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0240LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    RefreshWeatherForCurrentTime(0);
    EV_FreeWAIR();
    EV_UpdateWeather();
    SetupE0240Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0240ItemReadyNullsub() {
}

// @verified - p
void E0240MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    switch (parent->state) {
    case 1:
        SetIsSkySetup(1);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        QueueE0240Loop(parent);
        break;
    case 3:
        CleanupE0240(parent);
        break;
    case 4:
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        SetIsSkySetup(0);
        RefreshPadTask(2, 1);
        FinishLoading();
        parent->state = 5;
        break;
    case 5:
        if (!IsItemSelectable()) {
            shenSetClockAndCompassRenderTo(1);
            E0240ItemReadyNullsub();
            parent->state = 1;
            HLib::SetupTaskDelay(60);
        }
        break;
    }
}

// @verified - p
void E0240LoopCallback(void *taskData) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    AddInventoryItem(0x48, 1, 1);
    HLib::SetupTimedTask(static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->self, 130);
    RestoreE0240Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0240ScriptDefinition{.areaId = AREA_0240_AREA, .field04 = 0, .init = E0240Init, .main = E0240Main};

} // namespace shenmue::s2::areas
#endif
