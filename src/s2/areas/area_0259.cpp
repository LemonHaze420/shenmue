#include "s2/areas/area_0259.hpp"
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0259MainCallback(void *taskData);
void E0259LoopCallback(void *taskData);

// @verified - p
void E0259Init() {
}

// @verified - p
void E0259Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0259MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0259_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0259_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0259_CONVERSATION);
}

// @verified - p
void SetupE0259Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0259_A438, false);
    for (const shen_name *character = AREA_0259_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0259Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0259_A438, true);
    for (const shen_name *character = AREA_0259_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0259(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0259_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0259_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0259Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0259LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetMapModelDataDefaultVertexShader();
    switch (parent->selection) {
    case 0:
        EV_lightLoad(AREA_0259_L001);
        break;
    case 1:
        EV_lightLoad(AREA_0259_L012);
        break;
    case 2:
        EV_lightLoad(AREA_0259_L023);
        break;
    case 3:
        EV_lightLoad(AREA_0259_L204);
        break;
    default:
        break;
    }
    PushGlobalLightingProgram();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0259Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0259MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0259Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0259(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 3;
        } else if (input == 128 && ++parent->selection > 3)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0259_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0259LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0259Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0259ScriptDefinition{.areaId = AREA_0259_AREA, .field04 = 0, .init = E0259Init, .main = E0259Main};

} // namespace shenmue::s2::areas
