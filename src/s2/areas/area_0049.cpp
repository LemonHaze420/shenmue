#include "s2/areas/area_0049.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/particle_effects.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/door.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0049MainCallback(void *taskData);
void E0049LoopCallback(void *taskData);

// @verified - p
std::uint32_t e0049Leaves;
void E0049Init() {
    e0049Leaves = 0;
}

// @verified - p
void InitializeE0049FallingLeaves() {
    InitializeWarehouseFallingLeaves(&e0049Leaves);
}

// @verified - p
void ConfigureE0049Environment() {
    InitializeE0049FallingLeaves();
    SetPlayerControlEnabled(0);
    EV_LoadCutsceneFromCurrentScene("0049A000");
    EV_FreeAuthorPak(1, AREA_0049_CONVERSATION);
}

// @verified - p
void E0049Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0049MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0049Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0049_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0049Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0049_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0049_SYE_, false);
    EV_SetCharacterAlphaEnabled(AREA_0049_A092, false);
    LoadBlurModelForCharacter(AREA_0049_SYE_, "SYE_B.MT7", 1.0f);
}

// @verified - p
void CleanupE0049(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    FreeParticleEffect(3, e0049Leaves);
    for (const shen_name *character = AREA_0049_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0049_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0049A"), 0);
    SetNpcColdResetMode(0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0049_WB00.name) {
        EV_NPC_NewLoadSw(1);
        ExecuteCleanupFuncForAreaWithCachedScene();
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {335.6f, 127.0f, 280.1f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 17021, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        BeginSecondarySampleExecution(0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void E0049MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0049LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->state = 0;
        BeginSecondarySampleExecution(0);
        SetupE0049Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0049_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0049(parent);
}

// @verified - p
void E0049LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    HLib::SetupTimedTask(static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->self, 5);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0049_SYE_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0049_SYE_, true);
    EV_SetCharacterAlphaEnabled(AREA_0049_A092, true);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0049ScriptDefinition{.areaId = AREA_0049_AREA, .field04 = 0, .init = E0049Init, .main = E0049Main};

} // namespace shenmue::s2::areas
