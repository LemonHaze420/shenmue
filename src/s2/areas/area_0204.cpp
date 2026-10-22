#include "s2/areas/area_0204.hpp"
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

void E0204MainCallback(void *taskData);
void E0204LoopCallback(void *taskData);

// @verified - p
void E0204Init() {
}

// @verified - p
void E0204Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0204MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0204_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0204_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_SetWeatherInstruction(0x0c, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0204_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0204Characters() {
    const Vector3 position{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0204_A358, false);
    EV_AT_SetPos(AREA_0204_A358, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    for (const shen_name *character = AREA_0204_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_AT_SetPos(*character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0204Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0204_A358, true);
    for (const shen_name *character = AREA_0204_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0204(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0204_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0204_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0204Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0204LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_LoadAudio(AREA_0204_AUDIO, 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0204Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0204MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0204Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0204(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 10;
        } else if (input == 128 && ++parent->selection > 10)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 10) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0204_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0204LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    RestoreE0204Characters();
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0204ScriptDefinition{.areaId = AREA_0204_AREA, .field04 = 0, .init = E0204Init, .main = E0204Main};

} // namespace shenmue::s2::areas
