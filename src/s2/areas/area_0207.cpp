#include "s2/areas/area_0207.hpp"
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

void E0207MainCallback(void *taskData);
void E0207LoopCallback(void *taskData);

// @verified - p
void E0207Init() {
}

// @verified - p
void E0207Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0207MainCallback, 4, 0x0b, 0x18, AREA_AUTHOR_TASK);
    E0207TaskParameter *const parent = static_cast<E0207TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0207_MOTION));
    parent->secondMotion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0207_SECOND_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0207_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0207_CONVERSATION);
    EV_FreeAuthorPak(1, AREA_0207_SECOND_CONVERSATION);
    EV_SetWeatherInstruction(0x0c, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0207_L001);
    PushGlobalLightingProgram();
}

// @verified - p
void SetupE0207Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_AB05, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A202, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A203, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A247, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A269, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A267, false);
    EV_SetCharacterAlphaEnabled(AREA_0207_A268, false);
    for (const shen_name *character = AREA_0207_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0207Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_AB05, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A202, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A203, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A247, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A269, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A267, true);
    EV_SetCharacterAlphaEnabled(AREA_0207_A268, true);
    for (const shen_name *character = AREA_0207_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0207(E0207TaskParameter *parent) {
    for (const shen_name *character = AREA_0207_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0207_CONVERSATION);
    EV_FreeAuthorPak(0, AREA_0207_SECOND_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    DeleteMotion(std::bit_cast<std::int32_t>(parent->secondMotion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0207Loop(E0207TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0207LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0207LoopTaskParameter *const child = static_cast<E0207LoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0207Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0207MainCallback(void *taskData) {
    E0207TaskParameter *const parent = static_cast<E0207TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0207Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0207(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0207_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0207LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    E0207TaskParameter *const parent = static_cast<E0207LoopTaskParameter *>(taskData)->parent;
    if (parent->selection != 0) {
        RestoreE0207Characters();
        parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    } else {
        parent->selection = 1;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    }
}

} // namespace

const ScriptAreaDefinition area0207ScriptDefinition{.areaId = AREA_0207_AREA, .field04 = 0, .init = E0207Init, .main = E0207Main};

} // namespace shenmue::s2::areas
