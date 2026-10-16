#include "s2/areas/area_0080.hpp"
#include "s2/sound_program.hpp"
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0080MainCallback(void *taskData);
void E0080LoopCallback(void *taskData);

// @verified - p
void E0080Init() {
}

// @verified - p
void ConfigureE0080Environment() {
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_LoadCutsceneFromCurrentScene("0080A000");
    EV_FreeAuthorPak(1, AREA_0080_CONVERSATION);
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint8_t>(time.hour - 19) <= 5) {
        EV_lightLoad(AREA_0080_0000);
        PushGlobalLightingProgram();
        EV_SetObjectFlagByIndex(7, 1);
        EV_SetObjectFlagByIndex(6, 0);
    }
}

// @verified - p
void E0080Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{});
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0080MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0080Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0080_CHARACTERS, parent->self);
}

// @verified - p
void CleanupE0080(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0080_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_FreeAuthorPak(0, AREA_0080_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0080A"), 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0080_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0080_AREA, g_entryPoint);
    else
        EV_scenQueueLoad(g_actNumber, AREA_0080_AKY0, 0);
}

// @verified - p
void E0080MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0080LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;

        mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
        EV_SetCharacterAlphaEnabled(AREA_0080_AA48, false);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0080_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0080(parent);
}

// @verified - p
void E0080LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    EV_SetCharacterAlphaEnabled(AREA_0080_AA48, true);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0080ScriptDefinition{.areaId = AREA_0080_AREA, .field04 = 0, .init = E0080Init, .main = E0080Main};

} // namespace shenmue::s2::areas
