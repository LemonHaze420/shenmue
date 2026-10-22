#include "s2/areas/area_0218.hpp"
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

void E0218MainCallback(void *taskData);
void E0218LoopCallback(void *taskData);

// @verified - p
void E0218Init() {
}

// @verified - p
void E0218Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0218MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0218_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0218");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0218_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0218Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0218_A274, false);
    EV_SetCharacterAlphaEnabled(AREA_0218_A275, false);
    for (const shen_name *character = AREA_0218_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0218Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0218_A274, true);
    EV_SetCharacterAlphaEnabled(AREA_0218_A275, true);
    for (const shen_name *character = AREA_0218_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0218(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0218_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0218_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void QueueE0218Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0218LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_cuffsAttach(AREA_AUTHOR_RYO, 1, AREA_AUTHOR_REN, 0, 0.3f);
    SetMapModelDataDefaultVertexShader();
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0218_L001);
    AssignLightProgram(0);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
    EV_AssignLightParams(1, 3);
    if (parent->selection == 1) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    SetupE0218Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0218MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0218Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0218(parent);
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
            EV_debug_log_to_screen(AREA_0218_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0218LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    EV_cuffsSetFlags();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0218Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0218ScriptDefinition{.areaId = AREA_0218_AREA, .field04 = 0, .init = E0218Init, .main = E0218Main};

} // namespace shenmue::s2::areas
