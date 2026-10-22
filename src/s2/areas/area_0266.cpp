#include "s2/areas/area_0266.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0266() {
}

// @verified - p
void Setup0266() {
    SetPlayerControlEnabled(0);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_0266_MAP_POSITION, 0);
    SetMapModelSlotTransform(7, &AREA_0266_SEVENTH_MAP_POSITION, 0);
    EV_FreeAuthorPak(1, AREA_0266_CONVERSATION);
    EV_SetObjectFlagByIndex(2, 0);
}

// @verified - p
void Start0266(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0266, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0266_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Setup0266();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0266_CHARACTERS, parent->self);
}

// @verified - p
void Update0266(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0266Author(parent);
    } else if (parent->state == 3) {
        Cleanup0266(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 3;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 3)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0266_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0266(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0266_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0266_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0266Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0266AuthorTask, 4, 0x0b, sizeof(AuthorViewer081ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer081ELoopTaskParameter *const child = static_cast<AuthorViewer081ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_LoadAudio(AREA_0266_AUDIO, 0);
    if (parent->selection == 0) {
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        SetLightRootPosition(AREA_0266_MAP_POSITION);
        PushGlobalLightingProgram();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0266Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0266Author(AuthorViewer081ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0266Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0266AuthorTask(void *taskData) {
    Update0266Author(static_cast<AuthorViewer081ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0266Characters() {
    ResetAreaAuthorCharacter(AREA_0266_RYO_, 0.0f);
    const Vector3 position{};
    EV_AT_SetPos(AREA_0266_aa99, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0266_aa99, false);
    EV_AT_SetPos(AREA_0266_a020, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    EV_SetCharacterAlphaEnabled(AREA_0266_a020, false);
    for (const shen_name *character = AREA_0266_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_AT_SetPos(*character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0266Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0266_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0266_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0266_aa99, true);
    EV_SetCharacterAlphaEnabled(AREA_0266_a020, true);
    for (const shen_name *character = AREA_0266_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0266ScriptDefinition{.areaId = AREA_0266, .field04 = 0, .init = Initialize0266, .main = Start0266};

} // namespace shenmue::s2::areas
