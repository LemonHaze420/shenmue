#include "s2/areas/area_0034.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

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
#include "s2/telephone_cord.hpp"
#include "s2/ui.hpp"
#include "s2/character.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0034MainCallback(void *taskData);
void E0034LoopCallback(void *taskData);

// @verified - p
void E0034Init() {
}

// @verified - p
void ConfigureE0034Environment() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    SetRenderClockAndCompassTo(1);
    EV_FreeAuthorPak(1, AREA_0034_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0034A000");
    if (g_currentMap.name == AREA_0034_AREA.name) {
        SetMapModelDataDefaultVertexShader();
        if (g_entryPoint == 1) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0034_0000);
            PushGlobalLightingProgram();
        }
    }
}

// @verified - p
void E0034Main(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0034MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    ConfigureE0034Environment();
    parent->selection = g_currentMap.name == AREA_0034_WTA0.name ? *reinterpret_cast<const std::int16_t *>(argument) :
                                                                   static_cast<std::int16_t>(g_entryPoint > 1 ? 0 : g_entryPoint);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0034_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0034Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0034_KUN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0034_KUN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A047, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A044, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A046, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A050, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A051, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A052, false);
    EV_SetCharacterAlphaEnabled(AREA_0034_A053, false);
    if (g_currentMap.name != AREA_0034_WTA0.name)
        EV_SetCharacterAlphaEnabled(AREA_0034_AB27, false);
    EnqueueTelephoneCord();
}

// @verified - p
void RestoreE0034Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0034_KUN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0034_KUN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A047, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A044, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A046, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A050, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A051, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A052, true);
    EV_SetCharacterAlphaEnabled(AREA_0034_A053, true);
    if (g_currentMap.name != AREA_0034_WTA0.name)
        EV_SetCharacterAlphaEnabled(AREA_0034_AB27, true);
    CleanupTelephoneCord();
}

// @verified - p
void CleanupE0034(AreaImmediateAuthorTaskParameter *parent) {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 9;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    for (const shen_name *character = AREA_0034_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0034A"), 0);
    EV_FreeAuthorPak(0, AREA_0034_CONVERSATION);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name != AREA_0034_WTA0.name) {
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("WTA0"), 0) != 0)
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WTA0"), 0);
        else if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("WB00"), 0) != 0)
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WB00"), 0);
        if (g_entryPoint != 1) {
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
            return;
        }
    }
    EV_scenQueueLoad(g_actNumber, AREA_0034_WBBK, 1);
}

// @verified - p
void LoadE0034ScenePackages(std::int32_t selection) {
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory),
        reinterpret_cast<std::uintptr_t>(selection != 0 ? "WB00" : "WTA0"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    if (selection != 0) {
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(4, 0);
    } else {
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 1);
    }
}

// @verified - p
void E0034MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0034LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;

        SetupE0034Characters();
        if (g_currentMap.name == AREA_0034_AREA.name)
            LoadE0034ScenePackages(parent->selection);
        else if (parent->selection == 0)
            EV_DisableCharacterDisplay(AREA_0034_TELS);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection == 0 ? AREA_0034_AREA.name : parent->selection));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0034(parent);
}

// @verified - p
void E0034LoopCallback(void *taskData) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 1);
    } else if (frame == 221)
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    if (GetCurrentAuthorId().name == AREA_0034_AREA.name) {
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 253)
            snapSetDesc(0);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    RestoreE0034Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0034ScriptDefinition{.areaId = AREA_0034_AREA, .field04 = 0, .init = E0034Init, .main = E0034Main};

} // namespace shenmue::s2::areas
