#include "s2/areas/area_011e.hpp"

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
#include "s2/character.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E011EMainCallback(void *taskData);
void E011ELoopCallback(void *taskData);

// @verified - p
void E011EInit() {
}

// @verified - p
void E011EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E011EMainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_011E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0011");
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_011E_CHARACTERS, parent->self);
}

void SetupE011ECharacters() {
    const Vector3 zero{0, 0, 0};
    EV_AT_SetPos(AREA_AUTHOR_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    SetScenarioCharacterRadius(AREA_AUTHOR_RYO, 10.0f);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_AT_SetPos(AREA_011E_A317, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_SetCharacterAlphaEnabled(AREA_011E_A317, false);
    EV_AT_SetPos(AREA_011E_A318, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_SetCharacterAlphaEnabled(AREA_011E_A318, false);
    EV_AT_SetPos(AREA_011E_A319, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_SetCharacterAlphaEnabled(AREA_011E_A319, false);
    EV_AT_SetPos(AREA_011E_KBOU, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_SetCharacterAlphaEnabled(AREA_011E_KBOU, false);
    EV_AT_SetPos(AREA_011E_SALA, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    EV_SetCharacterAlphaEnabled(AREA_011E_SALA, false);
    for (const shen_name *character = AREA_011E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        const Vector3 position{0, 0, 0};
        EV_AT_SetPos(*character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        SetScenarioCharacterRadius(*character, 10.0f);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE011ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_011E_A317, true);
    EV_SetCharacterAlphaEnabled(AREA_011E_A318, true);
    EV_SetCharacterAlphaEnabled(AREA_011E_A319, true);
    EV_SetCharacterAlphaEnabled(AREA_011E_KBOU, true);
    EV_SetCharacterAlphaEnabled(AREA_011E_SALA, true);
    for (const shen_name *character = AREA_011E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE011E(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_011E_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_011E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE011ELoop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E011ELoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    switch (parent->selection) {
    case 3:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
    case 22:
    case 23:
    case 24:
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        break;
    }
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(parent->selection) - 22) > 1)
        SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE011ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E011EMainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE011ELoop(parent);
    } else if (parent->state == 3) {
        CleanupE011E(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 24;
        } else if (input == 128 && ++parent->selection > 24)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 24) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_011E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void E011ELoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    RestoreE011ECharacters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area011EScriptDefinition{.areaId = AREA_011E_AREA, .field04 = 0, .init = E011EInit, .main = E011EMain};

} // namespace shenmue::s2::areas
