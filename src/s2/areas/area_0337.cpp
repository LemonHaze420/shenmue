#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0337.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Start0337(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0337, 4, 11, sizeof(Area0337Parameter), TASK_EVNT);
    Area0337Parameter *parent = (Area0337Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
    parent->selection = 0;
    Setup0337();
    Queue_EVT_UTL_LoadNPCChara_3(area0337Characters, parent->self);
}
#else
void Update0337(void *taskData)
{
    Area0337Parameter *parent = (Area0337Parameter *)taskData;
    switch (parent->state) {
    case 1:
        Queue0337Author(parent);
        break;
    case 3:
        Cleanup0337(parent);
        break;
    }
}

void Queue0337Author(Area0337Parameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0337AuthorTask, 4, 11, sizeof(Area0337AuthorParameter), TASK_EVNT);
    ((Area0337AuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
    Prepare0337Characters();
    FTBL.EV_authSceneCtrl(AREA_0337_AUTHOR);
    parent->state = 2;
}

void Update0337AuthorTask(void *taskData)
{
    Area0337AuthorParameter *child = (Area0337AuthorParameter *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        Restore0337Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Setup0337(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0337A", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "0337A", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0337");
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_lightLoad(AREA_0337_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.qronSetFloor(40);
    FTBL.SetAnimatedObjectState(AREA_0337_ELEVATOR, 40);
    FTBL.EV_LoadCutsceneFromCurrentScene("0337A000");
}

void Cleanup0337(Area0337Parameter *parent)
{
    EVT_UTL_FreeNPCChara(area0337Characters);
    FTBL.EV_ControlPackageResources(0, 6, "0337A", 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0337");
    FTBL.HLib_DestroyTask(parent->self);
    if (g_currentMap == AREA_0337_RETURN) {
        FTBL.EV_SetWeatherInstruction(12, 1);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        EVT_AT_SetPos(RYO_CHARACTER, INVALID_CHARACTER, 245.4f, 244.0f, 123.0f, 0);
        EVT_AT_SetAng(RYO_CHARACTER, INVALID_CHARACTER, 0.0f, 180.0f, 0.0f, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

shen_name area0337Characters[] = {AREA_0337_JOP, INVALID_CHARACTER};

void Prepare0337Characters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorCharacter(AREA_0337_JOP);
    HideAuthorProp(AREA_0337_a516);
    HideAuthorProp(AREA_0337_a512);
    HideAuthorProp(AREA_0337_a452);
}

void Restore0337Characters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorCharacter(AREA_0337_JOP);
    ShowAuthorProp(AREA_0337_a516);
    ShowAuthorProp(AREA_0337_a512);
    ShowAuthorProp(AREA_0337_a452);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"
#endif
#else
#include "s2/areas/area_0337.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/elevator.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0337() {
}

// @verified - p
void Start0337(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0337, 4, 0x0b, sizeof(Area0337Parameter), AUTHOR_VIEWER_TASK);
    Area0337Parameter *const parent = static_cast<Area0337Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_0337_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0337_LIGHT);
    PushGlobalLightingProgram();
    SetQronFloor(40);
    SetCurrentElevatorFloor(AREA_0337_ELEVATOR, 40);
    EV_LoadCutsceneFromCurrentScene("0337A000");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0337_CHARACTERS, parent->self);
}

// @verified - p
void Update0337(void *taskData) {
    Area0337Parameter *const parent = static_cast<Area0337Parameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0337AuthorTask, 4, 0x0b, sizeof(Area0337AuthorParameter), AUTHOR_VIEWER_TASK);
        static_cast<Area0337AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        Prepare0337Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0337_AUTHOR.name));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0337(parent);
    }
}

// @verified - p
void Cleanup0337(Area0337Parameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0337_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0337_ARCHIVE), 0);
    EV_FreeAuthorPak(0, AREA_0337_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0337_RETURN.name) {
        EV_SetWeatherInstruction(12, 1);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        sbtLoadProgram();
        EventSetPosition position{0, AREA_0337_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {245.39999389648438f, 244.0f, 123.0f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_0337_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x8000, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

// @verified - p
std::int64_t Update0337Author(Area0337AuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0337Characters();
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0337AuthorTask(void *taskData) {
    Update0337Author(static_cast<Area0337AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0337Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0337_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0337_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0337_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0337_JOP_, false);
    EV_SetCharacterAlphaEnabled(AREA_0337_ab12, false);
    EV_SetCharacterAlphaEnabled(AREA_0337_ab13, false);
    EV_SetCharacterAlphaEnabled(AREA_0337_a452, false);
}

// @verified - p
void Restore0337Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0337_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0337_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0337_JOP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0337_JOP_, true);
    EV_SetCharacterAlphaEnabled(AREA_0337_ab12, true);
    EV_SetCharacterAlphaEnabled(AREA_0337_ab13, true);
    EV_SetCharacterAlphaEnabled(AREA_0337_a452, true);
}

const ScriptAreaDefinition area0337ScriptDefinition{.areaId = AREA_0337, .field04 = 0, .init = Initialize0337, .main = Start0337};

} // namespace shenmue::s2::areas
#endif
