#include "s2/areas/area_0063.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
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

void E0063MainCallback(void *taskData);
void E0063LoopCallback(void *taskData);

// @verified - p
void E0063Init() {
}

// @verified - p
void E0063Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0063MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0063_MOTION));
    parent->motion = motion;
    parent->selection = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0063_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    EV_FreeAuthorPak(1, AREA_0063_CONVERSATION);
}

// @verified - p
void CleanupE0063(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0063_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_FreeAuthorPak(0, AREA_0063_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0063Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0063LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    EV_SetCharacterAlphaEnabled(AREA_0063_PROP, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0063MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0063Loop(parent);
    else if (parent->state == 3)
        CleanupE0063(parent);
}

// @verified - p
void E0063LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    EV_SetCharacterAlphaEnabled(AREA_0063_PROP, true);
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0063ScriptDefinition{.areaId = AREA_0063_AREA, .field04 = 0, .init = E0063Init, .main = E0063Main};

} // namespace shenmue::s2::areas
