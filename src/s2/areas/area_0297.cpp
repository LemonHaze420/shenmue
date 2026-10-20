#include "s2/areas/area_0297.hpp"
#include "s2/author.hpp"
#include "s2/map_assets.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0297() {
}

// @verified - p
void Start0297(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0297, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    Setup0297Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0297_CHARACTERS, parent->self);
}

// @verified - p
void Update0297(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1)
        Queue0297Author(parent);
    else if (parent->state == 3)
        Cleanup0297(parent);
}

// @verified - p
void Cleanup0297(AuthorViewerTaskParameter *parent) {
    using enum CameraState;
    EventSetPosition position{0, AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {242.14999389648438f, 110.80000305175781f, 125.0f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle angle{0, AREA_0297_RYO_, AUTHOR_VIEWER_INVALID_CHARACTER, {0, 0x4000, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&angle);
    position.character = AREA_0297_REN_;
    position.position = {238.14999389648438f, 110.80000305175781f, 125.0f};
    EVT_AT_SetPosRot(&position);
    angle.character = AREA_0297_REN_;
    EVT_AT_SetAng_Callback(&angle);
    EV_npcLockUnlock(AREA_0297_REN_);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_FreeAuthorPak(0, AREA_0297_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0297A"), 0);
    if (g_currentMap.name == AREA_0297_RETURN.name) {
        LoadCutsceneAndSoundProgramForArea();
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
std::int64_t Queue0297Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0297AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0297Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0297_AUTHOR.name));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0297Author(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0297Characters();
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0297AuthorTask(void *taskData) {
    Update0297Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0297Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0297_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0297_REN_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_CHD_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0297_CHD_, false);
}

// @verified - p
void Restore0297Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0297_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_REN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0297_REN_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0297_CHD_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0297_CHD_, true);
}

// @verified - p
void Setup0297Scene() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    if (g_currentMap.name == AREA_0297.name) {
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        const Vector3 root{0.0f, 10.800000190734863f, 0.0f};
        SetLightRootPosition(root);
        for (std::uint32_t slot = 0; slot < 9; ++slot)
            SetMapModelSlotTransform(slot, &root, 0);
    }
    EV_FreeAuthorPak(1, AREA_0297_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0297A000");
}

const ScriptAreaDefinition area0297ScriptDefinition{.areaId = AREA_0297, .field04 = 0, .init = Initialize0297, .main = Start0297};

} // namespace shenmue::s2::areas
