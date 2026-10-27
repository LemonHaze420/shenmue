#include "s2/areas/area_0151.hpp"
#include "s2/pak.hpp"
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
#include "s2/npc.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0151MainCallback(void *taskData);
void E0151LoopCallback(void *taskData);

// @verified - p
void E0151Init() {
}

// @verified - p
void ConfigureE0151Environment() {
    const Vector3 center{335.84f, 126.0f, 275.82f};
    npcSetAreaName(AREA_0151_ANP1, &center, 4.0f, 1);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_0151_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0151A000");
}

// @verified - p
void E0151Main(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0151MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0151TaskParameter *const parent = static_cast<E0151TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection =
        g_currentMap.name == AREA_0151_WB00.name ? *reinterpret_cast<const std::uint32_t *>(argument) : static_cast<std::uint32_t>(g_entryPoint != 0);
    parent->state = 2;
    ConfigureE0151Environment();
}

// @verified - p
void CleanupE0151(E0151TaskParameter *parent) {
    EV_FreeAuthorPak(0, AREA_0151_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0151A"), 0);
    npcSetAreaName(AREA_0151_ANP1, nullptr, 0.0f, 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0151_WB00.name)
        SetPlayerControlEnabled(1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void E0151MainCallback(void *taskData) {
    E0151TaskParameter *const parent = static_cast<E0151TaskParameter *>(taskData);
    if (parent->state == 2) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0151LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        E0151LoopParameter *const child = static_cast<E0151LoopParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
        EV_NPC_NewLoadSw(1);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection == 0 ? AREA_0151_151A.name : AREA_0151_151B.name));
        parent->state = 3;
    } else if (parent->state == 4)
        CleanupE0151(parent);
    else if (parent->state == 1) {
        if (!filmCheckFadeTrackColour())
            parent->state = 4;
    } else if (parent->state == 0) {
        EV_NPC_NewLoadSw(1);
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        shenSetClockAndCompassRenderTo(0);
        parent->state = 1;
    }
}

// @verified - p
void E0151LoopCallback(void *taskData) {
    using enum CameraState;
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    static_cast<E0151LoopParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {335.85f, 127.0f, 275.0f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 64747, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&rotation);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0151ScriptDefinition{.areaId = AREA_0151_AREA, .field04 = 0, .init = E0151Init, .main = E0151Main};

} // namespace shenmue::s2::areas
