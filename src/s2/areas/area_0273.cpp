#include "s2/areas/area_0273.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {

// @verified - p
void Initialize0273() {
}

// @verified - p
void Start0273(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0273, 4, 0x0b, sizeof(Area0273Parameter), AUTHOR_VIEWER_TASK);
    Area0273Parameter *const parent = static_cast<Area0273Parameter *>(HLib::GetTaskParameter(task));
    parent->task = task;
    parent->author = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0273_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene(AREA_0273_SOUND);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0273_CHARACTERS, parent->task);
}

// @verified - p
void Update0273(void *taskData) {
    Area0273Parameter *const parent = static_cast<Area0273Parameter *>(taskData);
    if (parent->state == 1)
        Queue0273Author(parent);
    else if (parent->state == 3)
        Cleanup0273(parent);
}

// @verified - p
void Cleanup0273(Area0273Parameter *parent) {
    for (const shen_name *character = AREA_0273_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0273_ARCHIVE), 0);
    EV_FreeAuthorPak(0, AREA_0273_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->task);
    shen_name map = g_currentMap;
    std::uint32_t entry;
    if (map.name == AREA_0273_SOURCE.name) {
        scenSetTaskFlags(8);
        entry = 0;
        map = AREA_0273_DESTINATION;
    } else {
        entry = static_cast<std::uint32_t>(g_entryPoint);
    }
    EV_scenQueueLoad(g_actNumber, map, entry);
}

// @verified - p
std::int64_t Queue0273Author(Area0273Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0273AuthorTask, 4, 0x0b, sizeof(Area0273AuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area0273AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0273_JOP), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0273_JOP, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0273.name));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0273Author(Area0273AuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 3;
        mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0273_JOP), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0273_JOP, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0273AuthorTask(void *taskData) {
    Update0273Author(static_cast<Area0273AuthorParameter *>(taskData));
}

const ScriptAreaDefinition area0273ScriptDefinition{.areaId = AREA_0273, .field04 = 0, .init = Initialize0273, .main = Start0273};

} // namespace shenmue::s2::areas
