#include "s2/areas/area_0146.hpp"
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

void E0146MainCallback(void *taskData);
void E0146LoopCallback(void *taskData);

// @verified - p
void E0146Init() {
}

// @verified - p
void E0146Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0146MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0146_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0146_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0146_CONVERSATION);
}

// @verified - p
void SetupE0146Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A308, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A309, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A311, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A312, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A302, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A303, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A295, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A296, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A305, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A304, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A301, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A310, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A306, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A307, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A331, false);
    EV_SetCharacterAlphaEnabled(AREA_0146_A332, false);
    for (const shen_name *character = AREA_0146_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0146Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A308, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A309, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A311, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A312, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A302, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A303, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A295, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A296, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A305, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A304, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A301, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A310, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A306, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A307, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A331, true);
    EV_SetCharacterAlphaEnabled(AREA_0146_A332, true);
    for (const shen_name *character = AREA_0146_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0146(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0146_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0146_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0146Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0146LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    const char *media;
    if (parent->selection != 0) {
        SetMapModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        media = AREA_0146_MEDIA_OTHER;
    } else {
        EV_SetWeatherInstruction(0x0c, 0);
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0146_L002);
        PushGlobalLightingProgram();
        media = AREA_0146_MEDIA_ZERO;
    }
    EV_LoadCutsceneFromCurrentScene(media);
    EV_spmBeginSampleExecute();
    SetupE0146Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0146MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0146Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0146(parent);
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
            EV_debug_log_to_screen(AREA_0146_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0146LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0146Characters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0146ScriptDefinition{.areaId = AREA_0146_AREA, .field04 = 0, .init = E0146Init, .main = E0146Main};

} // namespace shenmue::s2::areas
