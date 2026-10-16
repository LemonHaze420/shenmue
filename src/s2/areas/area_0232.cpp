#include "s2/areas/area_0232.hpp"
#include "s2/character.hpp"

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
#include "s2/world_render.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0232MainCallback(void *taskData);
void E0232LoopCallback(void *taskData);

// @verified - p
void E0232Init() {
}

// @verified - p
void E0232Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0232MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0232_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0232_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0232_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0232Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0232_GAI2), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0232_GAI2, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A333, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A339, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A340, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A558, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A210, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A211, false);
    EV_SetCharacterAlphaEnabled(AREA_0232_A409, false);
    const Vector3 rootPosition{0.0f, 0.0f, 0.0f};
    EV_AT_SetPos(AREA_0232_A333, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A339, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A340, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A558, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A210, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A211, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
    EV_AT_SetPos(AREA_0232_A409, AREA_AUTHOR_ROOT_POSITION_FLAGS, rootPosition);
}

// @verified - p
void RestoreE0232Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0232_GAI2), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0232_GAI2, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A333, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A339, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A340, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A558, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A210, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A211, true);
    EV_SetCharacterAlphaEnabled(AREA_0232_A409, true);
}

// @verified - p
void CleanupE0232(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0232_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0232_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0232Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0232LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    if (parent->selection == 1)
        EV_lightLoad(AREA_0232_L701);
    else if (parent->selection == 0 || parent->selection == 2)
        EV_lightLoad(AREA_0232_L304);
    PushGlobalLightingProgram();
    if (parent->selection == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0232Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0232MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0232Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0232(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 2;
        } else if (input == 128 && ++parent->selection > 2)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0232_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0232LoopCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 0 && frame == 1355)
        SetIsSkySetup(0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (parent->selection == 0)
        SetIsSkySetup(1);
    parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0232Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0232ScriptDefinition{.areaId = AREA_0232_AREA, .field04 = 0, .init = E0232Init, .main = E0232Main};

} // namespace shenmue::s2::areas
