#include "s2/areas/area_282e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize282E() {
}

// @verified - p
void Start282E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update282E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_282E_MOTION));
    parent->selection = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_282E_CONVERSATION);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_282E_CHARACTERS, parent->self);
}

// @verified - p
void Update282E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue282EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup282E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 5;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 5)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_282E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup282E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_282E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_282E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue282EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update282EAuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare282ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update282EAuthor(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore282ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update282EAuthorTask(void *taskData) {
    Update282EAuthor(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare282ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_282E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_282E_REN_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_TGY_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_282E_TGY_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_SOY_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_282E_SOY_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_SEE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_282E_SEE_, false);
    EV_SetCharacterAlphaEnabled(AREA_282E_a896, false);
    EV_SetCharacterAlphaEnabled(AREA_282E_YUKB, false);
}

// @verified - p
void Restore282ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_282E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_REN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_282E_REN_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_TGY_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_282E_TGY_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_SOY_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_282E_SOY_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_282E_SEE_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_282E_SEE_, true);
    EV_SetCharacterAlphaEnabled(AREA_282E_a896, true);
    EV_SetCharacterAlphaEnabled(AREA_282E_YUKB, true);
}

const ScriptAreaDefinition area282EScriptDefinition{.areaId = AREA_282E, .field04 = 0, .init = Initialize282E, .main = Start282E};

} // namespace shenmue::s2::areas
