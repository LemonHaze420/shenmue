#include "s2/areas/area_529e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize529E() {
}

// @verified - p
void Start529E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update529E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_529E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_529E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_529E_CHARACTERS, parent->self);
}

// @verified - p
void Update529E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue529EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup529E(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 0;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 0)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0529E010.c\n", parent->selection);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void Cleanup529E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_529E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_529E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue529EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update529EAuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_529E_SIN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_529E_SIN, false);
    EV_SetCharacterAlphaEnabled(AREA_529E_PROP, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update529EAuthor(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_529E_SIN), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_529E_SIN, true);
        EV_SetCharacterAlphaEnabled(AREA_529E_PROP, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update529EAuthorTask(void *taskData) {
    Update529EAuthor(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

const ScriptAreaDefinition area529EScriptDefinition{.areaId = AREA_529E, .field04 = 0, .init = Initialize529E, .main = Start529E};

} // namespace shenmue::s2::areas
