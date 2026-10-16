#include "s2/areas/area_0264.hpp"
#include "s2/sound_program.hpp"
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
#include "s2/event_queued.hpp"
#include "s2/ui.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0264MainCallback(void *taskData);
void E0264LoopCallback(void *taskData);

// @verified - p
void E0264Init() {
}

// @verified - p
void ConfigureE0264Environment() {
    const Vector3 center{116.51f, 92.7f, 170.56f};
    npcSetAreaName(AREA_0264_ANP1, &center, 5.0f, 1);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0264A000");
    EV_FreeAuthorPak(1, AREA_0264_CONVERSATION);
}

// @verified - p
void E0264Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0264MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0264Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0264_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0264Characters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_EnableCharacterDisplay(AREA_0264_CHN_);
    EV_SetCharaControllerFlags(AREA_0264_CHN_, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0264_CHN_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0264_CHN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0264_CHN_, false);
    EV_EnableCharacterDisplay(AREA_0264_A520);
    EV_SetCharaControllerFlags(AREA_0264_A520, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0264_A520, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0264_A520, false);
}

// @verified - p
void RestoreE0264Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_AUTHOR_RYO);
    EnableCharDisplayAndSetMotionworkData(AREA_0264_CHN_);
    EV_EnableCharacterDisplay(AREA_0264_A520);
    EV_SetCharaControllerFlags(AREA_0264_A520, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0264_A520, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0264_A520, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0264_A520, true);
    EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {100.1f, 90.8f, 173.1f}, 0.0f};
    EVT_AT_SetPosRot(&position);
    EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0x4000, 0}, 0.0f};
    EVT_AT_SetAng_Callback(&rotation);
}

// @verified - p
void CleanupE0264(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0264_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_npcLockUnlock(AREA_0264_SIK_);
    EV_npcLockUnlock(AREA_0264_SZY_);
    EV_npcLockUnlock(AREA_0264_TKY_);
    npcSetAreaName(AREA_0264_ANP1, nullptr, 0.0f, 0);
    EV_FreeAuthorPak(0, AREA_0264_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0264A"), 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0264_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0264_AREA, g_entryPoint);
    else
        EV_scenQueueLoad(g_actNumber, AREA_0264_0308, 0);
}

// @verified - p
void E0264MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0264LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        EV_ForceLoadNpc(AREA_0264_SZY_);
        EV_ForceLoadNpc(AREA_0264_TKY_);
        EV_ForceLoadNpc(AREA_0264_SIK_);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetupE0264Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0264_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0264(parent);
    else if (parent->state == 4 && !GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        parent->state = 3;
}

// @verified - p
void E0264LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 467) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0264Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 4;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    AddInventoryItem(0x1781, 1, 1);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0264ScriptDefinition{.areaId = AREA_0264_AREA, .field04 = 0, .init = E0264Init, .main = E0264Main};

} // namespace shenmue::s2::areas
