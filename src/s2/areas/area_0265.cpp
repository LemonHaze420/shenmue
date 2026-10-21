#include "s2/areas/area_0265.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"

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
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/fog.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0265MainCallback(void *taskData);
void E0265LoopCallback(void *taskData);

// @verified - p
void E0265Init() {
}

// @verified - p
void ConfigureE0265Environment() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0265_L001);
    PushGlobalLightingProgram();
    EV_LoadCutsceneFromCurrentScene("0265A000");
    EV_FreeAuthorPak(1, AREA_0265_CONVERSATION);
}

// @verified - p
void E0265Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0265MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0265Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0265_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0265Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0265_CHN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0265_CHN_, false);
}

// @verified - p
void RestoreE0265Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0265_CHN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0265_CHN_, true);
}

// @verified - p
void CleanupE0265(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0265_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0265_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0265A"), 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0265_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0265_AREA, g_entryPoint);
    else {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {151.26f, 92.8f, 178.9f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0265_0000);
        PushGlobalLightingProgram();
        SetAreaFogParams();
        EV_NPC_NewLoadSw(1);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
void E0265MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0265LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;

        SetupE0265Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0265_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0265(parent);
}

// @verified - p
void E0265LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0265Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0265ScriptDefinition{.areaId = AREA_0265_AREA, .field04 = 0, .init = E0265Init, .main = E0265Main};

} // namespace shenmue::s2::areas
