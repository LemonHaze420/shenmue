#include "s2/areas/area_0012.hpp"
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
#include "s2/sound_program.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0012MainCallback(void *taskData);
void E0012LoopCallback(void *taskData);

// @verified - p
void SetupE0012Environment() {
    SetPlayerControlEnabled(0);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    if (dateTime.hour < 19) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        EV_lightLoad(AREA_0012_L082);
    }
    EV_FreeAuthorPak(1, AREA_0012_CONVERSATION);
}

// @verified - p
void E0012Init() {
}

// @verified - p
void E0012Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0012MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0012_MOTION));
    parent->state = 0;
    parent->selection = g_entryPoint <= 5 ? static_cast<std::int16_t>(g_entryPoint) : 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0012_CHARACTERS, parent->self);
    SetupE0012Environment();
}

// @verified - p
void SetupE0012Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_SAM_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0012_SAM_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_LLY_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0012_LLY_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_CLJ_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0012_CLJ_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_WON_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0012_WON_, false);
    EV_SetCharacterAlphaEnabled(AREA_0012_A014, false);
    EV_SetCharacterAlphaEnabled(AREA_0012_A100, false);
    EV_SetCharacterAlphaEnabled(AREA_0012_A101, false);
}

// @verified - p
void RestoreE0012Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_SAM_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0012_SAM_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_LLY_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0012_LLY_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_CLJ_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0012_CLJ_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0012_WON_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0012_WON_, true);
    EV_SetCharacterAlphaEnabled(AREA_0012_A014, true);
    EV_SetCharacterAlphaEnabled(AREA_0012_A100, true);
    EV_SetCharacterAlphaEnabled(AREA_0012_A101, true);
}

// @verified - p
void CleanupE0012(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0012_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_FreeAuthorPak(0, AREA_0012_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0012Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0012LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_LoadCutsceneFromCurrentScene(parent->selection == 0 ? AREA_0012_MEDIA_ZERO : AREA_0012_MEDIA_OTHER);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0012Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0012MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0012Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0012(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 5;
        } else if (input == 128 && ++parent->selection > 5)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0012_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0012LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0012Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0012ScriptDefinition{.areaId = AREA_0012_AREA, .field04 = 0, .init = E0012Init, .main = E0012Main};

} // namespace shenmue::s2::areas
