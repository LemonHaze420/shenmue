#include "s2/areas/area_528e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
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
void Initialize528E() {
}

// @verified - p
void Setup528E() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_528E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_AssignLightParams(1, 0);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 3);
    EV_AssignLightParams(1, 4);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(2);
}

// @verified - p
void Start528E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update528E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_528E_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup528E();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_528E_CHARACTERS, parent->self);
}

// @verified - p
void Update528E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue528EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup528E(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 2;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 2)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_528E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup528E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_528E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_528E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue528EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update528EAuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare528ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update528EAuthor(AuthorViewer517ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore528ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update528EAuthorTask(void *taskData) {
    Update528EAuthor(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare528ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_528E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_528E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_528E_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_528E_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_528E_a349, false);
    EV_SetCharacterAlphaEnabled(AREA_528E_a636, false);
}

// @verified - p
void Restore528ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_528E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_528E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_528E_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_528E_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_528E_a349, true);
    EV_SetCharacterAlphaEnabled(AREA_528E_a636, true);
}

const ScriptAreaDefinition area528EScriptDefinition{.areaId = AREA_528E, .field04 = 0, .init = Initialize528E, .main = Start528E};

} // namespace shenmue::s2::areas
