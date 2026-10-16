#include "s2/areas/area_192e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize192E() {
}

// @verified - p
void Start192E(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{});
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update192E, 4, 0x0b, sizeof(Area192EParameter), AUTHOR_VIEWER_TASK);
    Area192EParameter *const parent = static_cast<Area192EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = g_currentMap.name == AREA_192E_SOURCE.name;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_192E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_192E_CHARACTERS, parent->self);
}

// @verified - p
void Update192E(void *taskData) {
    Area192EParameter *const parent = static_cast<Area192EParameter *>(taskData);
    if (parent->state == 1) {
        Queue192EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup192E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
            Select192EPackage(parent->selection);
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 2;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 2)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_192E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup192E(Area192EParameter *parent) {
    for (const shen_name *character = AREA_192E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_192E_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_ARCHIVE), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_192E_SOURCE.name)
        EV_scenQueueLoad(g_actNumber, AREA_192E_RETURN, 1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue192EAuthor(Area192EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update192EAuthorTask, 4, 0x0b, sizeof(Area192EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area192EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_192E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_192E_SYE_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_192E_AUTHORS[parent->selection].name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update192EAuthor(Area192EAuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_currentMap.name == AREA_192E_SOURCE.name ? 3 : 0;
        mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_192E_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_192E_SYE_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_192E_SYE_, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update192EAuthorTask(void *taskData) {
    Update192EAuthor(static_cast<Area192EAuthorParameter *>(taskData));
}

// @verified - p
std::int64_t Select192EPackage(std::int32_t selection) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_192E_FIRST_PACKAGE), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_FIRST_PACKAGE), 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_192E_SECOND_PACKAGE), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_192E_SECOND_PACKAGE), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    const char *const package = selection ? AREA_192E_SECOND_PACKAGE : AREA_192E_FIRST_PACKAGE;
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    return EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

const ScriptAreaDefinition area192EScriptDefinition{.areaId = AREA_192E, .field04 = 0, .init = Initialize192E, .main = Start192E};

} // namespace shenmue::s2::areas
