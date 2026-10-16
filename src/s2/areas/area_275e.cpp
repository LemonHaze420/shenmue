#include "s2/areas/area_275e.hpp"
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

#include "s2/blur_figure_pose.hpp"
#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize275E() {
}

// @verified - p
void Start275E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update275E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_275E_MOTION));
    parent->selection = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_275E_CONVERSATION);
    SetIsSkySetup(0);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_275E_CHARACTERS, parent->self);
}

// @verified - p
void Update275E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue275EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup275E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
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
            EV_debug_log_to_screen(AREA_275E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup275E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_275E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_275E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue275EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update275EAuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_authSceneCtrl(
        static_cast<std::uint32_t>(static_cast<std::int32_t>(parent->selection) - 3) <= 2U ? AuthSceneCtrl::HideFilmBars : AuthSceneCtrl::ShowFilmBars);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare275ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update275EAuthor(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore275ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update275EAuthorTask(void *taskData) {
    Update275EAuthor(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare275ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_275E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_275E_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_275E_a532, false);
    LoadBlurModelForCharacter(AREA_275E_RYO_, "RYO_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_275E_TGY_, "TGY_B.MT7", 1.0f);
    for (const shen_name *character = AREA_275E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore275ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_275E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_275E_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_275E_a532, true);
    for (const shen_name *character = AREA_275E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area275EScriptDefinition{.areaId = AREA_275E, .field04 = 0, .init = Initialize275E, .main = Start275E};

} // namespace shenmue::s2::areas
