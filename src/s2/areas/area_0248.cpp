#include "s2/areas/area_0248.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
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

void E0248MainCallback(void *taskData);
void E0248LoopCallback(void *taskData);

// @verified - p
void E0248Init() {
}

void E0248Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0248MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0248_MOTION));
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0248");
    EV_AssignLightParams(1, 1);
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0248_L001);
    AssignLightProgram(0);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0248_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0248Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0248_A834, false);
    EV_SetCharacterAlphaEnabled(AREA_0248_A397, false);
    EV_SetCharacterAlphaEnabled(AREA_0248_A398, false);
    LoadBlurModelForCharacter(AREA_AUTHOR_RYO, "RYO_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_AUTHOR_REN, "REN_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_0248_TGY_, "TGY_B.MT7", 1.0f);
    for (const shen_name *character = AREA_0248_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0248Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0248_A834, true);
    EV_SetCharacterAlphaEnabled(AREA_0248_A397, true);
    EV_SetCharacterAlphaEnabled(AREA_0248_A398, true);
    for (const shen_name *character = AREA_0248_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0248(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0248_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0248_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0248Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0248LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetupE0248Characters();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0248MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0248Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0248(parent);
    }
}

// @verified - p
void E0248LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0248Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0248ScriptDefinition{.areaId = AREA_0248_AREA, .field04 = 0, .init = E0248Init, .main = E0248Main};

} // namespace shenmue::s2::areas
