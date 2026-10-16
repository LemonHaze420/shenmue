#include "s2/areas/area_048e.hpp"
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
void Initialize048E() {
}

// @verified - p
void Start048E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update048E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_048E_MOTION));
    parent->selection = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_048E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_048E_CHARACTERS, parent->self);
}

// @verified - p
void Update048E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue048EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup048E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 0)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_048E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup048E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_048E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_048E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue048EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update048EAuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare048ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update048EAuthor(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore048ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update048EAuthorTask(void *taskData) {
    Update048EAuthor(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare048ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_048E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_IKO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_048E_IKO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_KIC_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_048E_KIC_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_SBK_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_048E_SBK_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_VCN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_048E_VCN_, false);
}

// @verified - p
void Restore048ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_048E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_IKO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_048E_IKO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_KIC_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_048E_KIC_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_SBK_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_048E_SBK_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_048E_VCN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_048E_VCN_, true);
}

const ScriptAreaDefinition area048EScriptDefinition{.areaId = AREA_048E, .field04 = 0, .init = Initialize048E, .main = Start048E};

} // namespace shenmue::s2::areas
