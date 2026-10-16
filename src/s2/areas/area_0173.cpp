#include "s2/debug.hpp"
#include "s2/areas/area_0173.hpp"
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

#include <bit>

#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
void E0173MainCallback(void *taskData);
void E0173LoopCallback(void *taskData);

// @verified - p
void E0173Init() {
}

// @verified - p
void E0173Main(std::uintptr_t) {
    const Vector3 center{270.79998779296875f, 114.5999984741211f, 279.70001220703125f};
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0173MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    if (npcSetAreaName(AREA_0173_ANP1, &center, 5.0f, 1) == 1)
        EV_debug_log("0173\\e0173_et_0173.c", 96, "NPC_AREA1 : TRUE\n");
    else
        EV_debug_log("0173\\e0173_et_0173.c", 98, "NPC_AREA1 : FALSE\n");
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0173A000");
    EV_FreeAuthorPak(1, "0173");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0173_CHARACTERS, parent->self);
}

void CleanupE0173(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0173_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0173");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0173A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    npcSetAreaName(AREA_0173_ANP1, nullptr, 0.0f, 0);
    if (g_currentMap.name == AREA_0173_WB00.name) {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {269.1000061035156f, 114.5999984741211f, 279.0f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 17021, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        BeginSecondarySampleExecution(0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0173Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0173LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
    child->state = 0;
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    BeginSecondarySampleExecution(0);
    EV_NPC_NewLoadSw(1);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0173_SYP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0173_SYP_, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0173_AREA.name));
    parent->state = 2;
}

// @verified - p
void E0173MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0173Loop(parent);
    else if (parent->state == 3)
        CleanupE0173(parent);
}

// @verified - p
void E0173LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0173_SYP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0173_SYP_, true);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace

const ScriptAreaDefinition area0173ScriptDefinition{.areaId = AREA_0173_AREA, .field04 = 0, .init = E0173Init, .main = E0173Main};

} // namespace shenmue::s2::areas
