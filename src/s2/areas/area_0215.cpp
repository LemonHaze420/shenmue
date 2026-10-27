#include "s2/areas/area_0215.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/weather.hpp"
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

void E0215MainCallback(void *taskData);
void E0215LoopCallback(void *taskData);

// @verified - p
void E0215Init() {
}

// @verified - p
void ConfigureE0215Environment() {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 22;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0215_L001);
    PushGlobalLightingProgram();
    EV_FreeAuthorPak(1, "0215");
}

// @verified - p
void E0215Main(std::uintptr_t) {
    EV_stepSetFlags(0, 0x13b);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0215MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0215_MOTION));
    parent->selection = 0;
    parent->state = 0;
    ConfigureE0215Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0215_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0215Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    EV_SetCharacterAlphaEnabled(AREA_0215_A067, false);
    EV_SetCharacterAlphaEnabled(AREA_0215_A413, false);
    EV_SetCharacterAlphaEnabled(AREA_0215_A530, false);
    const Vector3 scale{50.0f, 50.0f, 50.0f};
    mtwkSetMotionworkScale3(AREA_0215_A413, &scale);
}

// @verified - p
void RestoreE0215Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    EV_SetCharacterAlphaEnabled(AREA_0215_A067, true);
    EV_SetCharacterAlphaEnabled(AREA_0215_A413, true);
    EV_SetCharacterAlphaEnabled(AREA_0215_A530, true);
}

// @verified - p
void CleanupE0215(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0215_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0215_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0215Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0215LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_cuffsAttach(AREA_AUTHOR_RYO, 1, AREA_AUTHOR_REN, 0, 0.3f);
    SetupE0215Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0215MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0215Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0215(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128 && ++parent->selection > 0)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0215_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0215LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    EV_cuffsSetFlags();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0215Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0215ScriptDefinition{.areaId = AREA_0215_AREA, .field04 = 0, .init = E0215Init, .main = E0215Main};

} // namespace shenmue::s2::areas
