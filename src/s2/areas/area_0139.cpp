#include "s2/areas/area_0139.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
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
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0139MainCallback(void *taskData);
void E0139LoopCallback(void *taskData);

// @verified - p
void E0139Init() {
}

// @verified - p
void ConfigureE0139Environment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0139_CONVERSATION);
    if (g_currentMap.name == AREA_0139_AREA.name) {
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0139_0000);
        PushGlobalLightingProgram();
    }
}

// @verified - p
void E0139Main(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0139MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0139Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0139_CHARACTERS, parent->self);
}

// @verified - p
void CleanupE0139(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0139_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0139A"), 0);
    EV_FreeAuthorPak(0, AREA_0139_CONVERSATION);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0139_AR03.name) {
        EV_NPC_NewLoadSw(1);
        EV_SetWeatherInstruction(12, 1);
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {289.4f, 100.0f, 484.9f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 33223, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
        doorLoadAuthorWithAudio(shen_name{});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        BeginSecondarySampleExecution(0);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void E0139MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        EV_LoadCutsceneFromCurrentScene("0139A000");
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0139LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->state = 0;
        child->parent = parent;

        mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0139_SYN_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0139_SYN_, false);
        EV_SetCharacterAlphaEnabled(AREA_0139_A450, false);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0139_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0139(parent);
}

// @verified - p
void E0139LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    AddInventoryItem(0x1f52, 1, 1);
    HLib::SetupTimedTask(static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->self, 130);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0139_SYN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0139_SYN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0139_A450, true);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0139ScriptDefinition{.areaId = AREA_0139_AREA, .field04 = 0, .init = E0139Init, .main = E0139Main};

} // namespace shenmue::s2::areas
