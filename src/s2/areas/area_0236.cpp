#include "s2/areas/area_0236.hpp"
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

void E0236MainCallback(void *taskData);
void E0236LoopCallback(void *taskData);

// @verified - p
void E0236Init() {
}

// @verified - p
void E0236Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0236MainCallback, 4, 0x0b, 0x18, AREA_AUTHOR_TASK);
    E0236TaskParameter *const parent = static_cast<E0236TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0236_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    EV_FreeAuthorPak(1, AREA_0236_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0236_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0236Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0236_YUA_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0236_YUA_, false);
    EV_SetCharacterAlphaEnabled(AREA_0236_A502, false);
    EV_SetCharacterAlphaEnabled(AREA_0236_A336, false);
    EV_SetCharacterAlphaEnabled(AREA_0236_AB19, false);
    EV_SetCharacterAlphaEnabled(AREA_0236_AB18, false);
}

// @verified - p
void RestoreE0236Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0236_YUA_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0236_YUA_, true);
    EV_SetCharacterAlphaEnabled(AREA_0236_A502, true);
    EV_SetCharacterAlphaEnabled(AREA_0236_AB19, true);
    EV_SetCharacterAlphaEnabled(AREA_0236_AB18, true);
}

// @verified - p
void CleanupE0236(E0236TaskParameter *parent) {
    for (const shen_name *character = AREA_0236_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0236_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0236Loop(E0236TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0236LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0236FrameTaskParameter *const child = static_cast<E0236FrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0236Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0236MainCallback(void *taskData) {
    E0236TaskParameter *const parent = static_cast<E0236TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0236Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0236(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 7;
        } else if (input == 128 && ++parent->selection > 7)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 7) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0236_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0236LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0236Characters();
    static_cast<E0236FrameTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0236ScriptDefinition{.areaId = AREA_0236_AREA, .field04 = 0, .init = E0236Init, .main = E0236Main};

} // namespace shenmue::s2::areas
