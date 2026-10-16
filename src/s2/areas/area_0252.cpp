#include "s2/areas/area_0252.hpp"
#include "s2/character.hpp"
#include "s2/sound_program.hpp"

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

void E0252MainCallback(void *taskData);
void E0252LoopCallback(void *taskData);

// @verified - p
void E0252Init() {
}

// @verified - p
void ConfigureE0252Environment() {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 11;
    time.minute = 30;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0252_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0252_L001);
    PushGlobalLightingProgram();
}

// @verified - p
void E0252Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0252MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0252_MOTION));
    parent->selection = 0;
    parent->state = 1;
    ConfigureE0252Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0252_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0252Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A209, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A207, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A208, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A270, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A326, false);
    EV_SetCharacterAlphaEnabled(AREA_0252_A300, false);
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0252Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A209, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A207, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A208, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A270, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A326, true);
    EV_SetCharacterAlphaEnabled(AREA_0252_A300, true);
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0252(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0252_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0252_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0252_QR00, 97);
}

// @verified - p
void E0252MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0252LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
        AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        BeginSecondarySampleExecution(0);
        SetupE0252Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0252(parent);
}

// @verified - p
void E0252LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0252Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0252ScriptDefinition{.areaId = AREA_0252_AREA, .field04 = 0, .init = E0252Init, .main = E0252Main};

} // namespace shenmue::s2::areas
