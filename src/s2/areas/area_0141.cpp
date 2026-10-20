#include "s2/debug.hpp"
#include "s2/areas/area_0141.hpp"
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
#include "s2/inventory.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
std::int32_t e0141Choice = 2;
namespace {
void E0141MainCallback(void *taskData);
void E0141LoopCallback(void *taskData);

// @verified - p
void E0141Init() {
    e0141Choice = 2;
}

// @verified - p
void E0141Main(std::uintptr_t) {
    EV_stepSetFlags(0, 0x3da);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0141MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0141A000");
    EV_FreeAuthorPak(1, "0141");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0141_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0141Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0141_A542, false);
    EV_SetCharacterAlphaEnabled(AREA_0141_A544, false);
    EV_SetCharacterAlphaEnabled(AREA_0141_A048, false);
    EV_SetCharacterAlphaEnabled(AREA_0141_A256, false);
    for (const shen_name *character = AREA_0141_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0141Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0141_A542, true);
    EV_SetCharacterAlphaEnabled(AREA_0141_A544, true);
    EV_SetCharacterAlphaEnabled(AREA_0141_A048, true);
    EV_SetCharacterAlphaEnabled(AREA_0141_A256, true);
    for (const shen_name *character = AREA_0141_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

void CleanupE0141(AreaImmediateAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0141_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0141A"), 0);
    EV_FreeAuthorPak(0, "0141");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0141_WB00.name) {
        EV_NPC_NewLoadSw(1);
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {330.57000732421875f, 127.0f, 234.25f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 28253, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        BeginSecondarySampleExecution(0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

// @verified - p
void QueueE0141Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0141LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
    child->state = 0;
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio("0141A000", 0);
    BeginSecondarySampleExecution(0);
    SetupE0141Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0141_AREA.name));
    parent->state = 2;
}

// @verified - p
void E0141MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0141Loop(parent);
    else if (parent->state == 3)
        CleanupE0141(parent);
    else if (parent->state == 4 && !GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage))
        parent->state = 3;
}

// @verified - p
void E0141LoopCallback(void *taskData) {
    AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(taskData);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 3445 && child->state == 0) {
        std::int16_t choices[]{-1, 69, 68, -1};
        SetupControlSpeakAndUI(choices, 600, -1, 2);
        child->state = 1;
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
    } else if (frame == 3446) {
        const Vector3 rightPosition{0.122f, 0.023f, 0.059f};
        const Vector3 leftPosition{0.124f, -0.004f, -0.061f};
        const std::array<std::int32_t, 3> rightRotation{6551, 62892, 50970};
        const std::array<std::int32_t, 3> leftRotation{38229, 63879, 14195};
        if (e0141Choice != 0) {
            SetFixoDataForMotionWork(
                FindCharacterByScenarioName(AREA_0141_A542), FindCharacterByScenarioName(AREA_0141_KUN_), 21, rightPosition, rightRotation);
            SetFixoDataForMotionWork(FindCharacterByScenarioName(AREA_0141_A544), FindCharacterByScenarioName(AREA_0141_KUN_), 14, leftPosition, leftRotation);
        } else {
            SetFixoDataForMotionWork(FindCharacterByScenarioName(AREA_0141_A542), FindCharacterByScenarioName(AREA_0141_KUN_), 14, leftPosition, leftRotation);
            SetFixoDataForMotionWork(
                FindCharacterByScenarioName(AREA_0141_A544), FindCharacterByScenarioName(AREA_0141_KUN_), 21, rightPosition, rightRotation);
        }
    } else if (frame == 3717) {
        ResetCharaFixo(FindCharacterByScenarioName(e0141Choice ? AREA_0141_A544 : AREA_0141_A542));
    } else if (frame == 3725) {
        const Vector3 position{0.133f, 0.0f, -0.052f};
        const std::array<std::int32_t, 3> rotation{41852, 11616, 15654};
        SetFixoDataForMotionWork(
            FindCharacterByScenarioName(e0141Choice ? AREA_0141_A544 : AREA_0141_A542), FindCharacterByScenarioName(AREA_AUTHOR_RYO), 14, position, rotation);
    } else if (frame == 3895) {
        const shen_name object = e0141Choice ? AREA_0141_A544 : AREA_0141_A542;
        ResetCharaFixo(FindCharacterByScenarioName(object));
        DisableScenarioCharacterDisplay(object);
    }
    if (child->state == 1 && !GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        e0141Choice = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) != 69;
        EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
        child->state = -1;
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    std::uint32_t item;
    if (e0141Choice != 0) {
        if (e0141Choice != 1)
            EV_debug_log("0141\\e0141_et_0141.c", 278, "Author Skip!!\n");
        item = 8017;
    } else {
        item = 8016;
    }
    AddInventoryItem(item, 1, 1);
    RestoreE0141Characters();
    child->parent->state = 4;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
} // namespace

const ScriptAreaDefinition area0141ScriptDefinition{.areaId = AREA_0141_AREA, .field04 = 0, .init = E0141Init, .main = E0141Main};

} // namespace shenmue::s2::areas
