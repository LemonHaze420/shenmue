#include "s2/areas/area_0066.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/inventory.hpp"
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

void E0066MainCallback(void *taskData);
void E0066LoopCallback(void *taskData);

// @verified - p
void E0066Init() {
}

// @verified - p
void ConfigureE0066Environment() {
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0066_L001);
    PushGlobalLightingProgram();
    if (g_currentMap.name == AREA_0066_WTA0.name)
        EV_DisableCharacterDisplay(AREA_0066_0001);
    EV_FreeAuthorPak(1, AREA_0066_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0066A000");
}

// @verified - p
void E0066Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0066MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0066Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0066_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0066Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0066_KUN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0066_KUN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A047, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A890, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A540, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A044, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A045, false);
    EV_SetCharacterAlphaEnabled(AREA_0066_A046, false);
}

// @verified - p
void RestoreE0066Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0066_KUN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0066_KUN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A047, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A890, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A540, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A044, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A045, true);
    EV_SetCharacterAlphaEnabled(AREA_0066_A046, true);
}

// @verified - p
void CleanupE0066(AreaImmediateAuthorTaskParameter *parent) {
    AddInventoryItem(0x1f4c, 1, 0);
    AddInventoryItem(0x0d, 1, 0);
    for (const shen_name *character = AREA_0066_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_NPC_NewLoadSw(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0066A"), 0);
    EV_FreeAuthorPak(0, AREA_0066_CONVERSATION);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0066_WTA0.name)
        EV_scenQueueLoad(g_actNumber, AREA_0066_WT00, 80);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void E0066MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0066LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        EV_spmBeginSampleExecute();
        SetupE0066Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0066_AREA.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0066(parent);
}

// @verified - p
void E0066LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    RestoreE0066Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0066ScriptDefinition{.areaId = AREA_0066_AREA, .field04 = 0, .init = E0066Init, .main = E0066Main};

} // namespace shenmue::s2::areas
