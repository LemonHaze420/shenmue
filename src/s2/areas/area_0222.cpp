#include "s2/areas/area_0222.hpp"
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

void E0222MainCallback(void *taskData);
void E0222LoopCallback(void *taskData);

// @verified - p
void E0222Init() {
}

// @verified - p
void ConfigureE0222Environment() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0222A000");
    EV_FreeAuthorPak(1, AREA_0222_CONVERSATION);
}

// @verified - p
void E0222Main(std::uintptr_t argument) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0222MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_currentMap.name == AREA_0222_AREA.name ? static_cast<std::int16_t>(g_entryPoint > 2 ? 0 : g_entryPoint) :
                                                                   *reinterpret_cast<const std::int16_t *>(argument);
    parent->state = 1;
    ConfigureE0222Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0222_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0222Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0222_YAN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0222_YAN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0222_AA22, false);
    if (selection != 0) {
        EV_SetCharacterAlphaEnabled(AREA_0222_A052, false);
        EV_SetCharacterAlphaEnabled(AREA_0222_A053, false);
        EV_SetCharacterAlphaEnabled(AREA_0222_A212, false);
        EV_SetCharacterAlphaEnabled(AREA_0222_A095, false);
        EV_SetCharacterAlphaEnabled(AREA_0222_A316, false);
    }
}

// @verified - p
void RestoreE0222Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0222_YAN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0222_YAN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0222_AA22, true);
    if (selection != 0) {
        EV_SetCharacterAlphaEnabled(AREA_0222_A052, true);
        EV_SetCharacterAlphaEnabled(AREA_0222_A053, true);
        EV_SetCharacterAlphaEnabled(AREA_0222_A212, true);
        EV_SetCharacterAlphaEnabled(AREA_0222_A095, true);
        EV_SetCharacterAlphaEnabled(AREA_0222_A316, true);
    }
}

// @verified - p
void CleanupE0222(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0222_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0222_CONVERSATION);
    const char *const package = g_currentMap.name == AREA_0222_AREA.name ? "0222A" : parent->selection == 0 ? "0222A1" : "0222A2";
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);

    EV_CleanupMapControlTask();
    if (g_currentMap.name == AREA_0222_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0222_AREA, g_entryPoint);
    else {
        const bool secondReturn = parent->selection != 0;
        HLib::DestroyTask(parent->self);
        if (secondReturn)
            EV_scenQueueLoad(g_actNumber, AREA_0222_QR00, 50);
        else
            EV_scenQueueLoad(g_actNumber, AREA_0222_QAAA, 1);
    }
}

// @verified - p
void E0222MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0222LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->state = 0;
        child->parent = parent;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetupE0222Characters(parent->selection);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0222_AUTHORS[parent->selection].name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0222(parent);
}

// @verified - p
void E0222LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent;
    if (parent->selection == 1) {
        parent->state = 1;
        ++parent->selection;
        return;
    }
    if (parent->selection != 0 && parent->selection != 2)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0222Characters(static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->selection);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0222ScriptDefinition{.areaId = AREA_0222_AREA, .field04 = 0, .init = E0222Init, .main = E0222Main};

} // namespace shenmue::s2::areas
