#include "s2/areas/area_0257.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
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
#include "s2/npc.hpp"
#include "s2/door.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/ui.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0257MainCallback(void *taskData);
void E0257LoopCallback(void *taskData);

// @verified - p
std::uint8_t e0257RewardIndex;
void E0257Init() {
    e0257RewardIndex = 0;
}

// @verified - p
void ConfigureE0257Environment() {
    const Vector3 center{116.51f, 92.7f, 170.56f};
    npcSetAreaName(AREA_0257_ANP1, &center, 5.0f, 1);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0257A000");
    EV_FreeAuthorPak(1, AREA_0257_CONVERSATION);
}

// @verified - p
void E0257Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0257MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0257Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0257_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0257Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0257_CHN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0257_CHN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0257_A428, false);
    EV_SetCharacterAlphaEnabled(AREA_0257_A438, false);
    EV_SetCharacterAlphaEnabled(AREA_0257_A534, false);
}

// @verified - p
void RestoreE0257Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0257_CHN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0257_CHN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0257_A428, true);
    EV_SetCharacterAlphaEnabled(AREA_0257_A438, true);
    EV_SetCharacterAlphaEnabled(AREA_0257_A534, true);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {100.1f, 90.8f, 173.1f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0x4000, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&rotation);
}

// @verified - p
void CleanupE0257(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0257_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, AREA_0257_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0257A"), 0);
    npcSetAreaName(AREA_0257_ANP1, nullptr, 0.0f, 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0257_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0257_AREA, g_entryPoint);
    else {
        ExecuteCleanupFuncForAreaWithCachedScene();
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        BeginSecondarySampleExecution(0);
    }
}

// @verified - p
void E0257MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0257LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->state = 0;
        child->parent = parent;
        EV_NPC_NewLoadSw(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetupE0257Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0257_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0257(parent);
    else if (parent->state == 4 && !GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage)) {
        const std::uint32_t item = AREA_0257_ITEMS[e0257RewardIndex];
        if (item == 0xffffffffU) {
            e0257RewardIndex = 0;
            parent->state = 3;
        } else {
            AddInventoryItem(item, 1, 1);
            ++e0257RewardIndex;
        }
    }
}

// @verified - p
void E0257LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 557) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0257Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 4;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0257ScriptDefinition{.areaId = AREA_0257_AREA, .field04 = 0, .init = E0257Init, .main = E0257Main};

} // namespace shenmue::s2::areas
