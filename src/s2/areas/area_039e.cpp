#include "s2/areas/area_039e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
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
void Initialize039E() {
}

// @verified - p
void Start039E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update039E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_039E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_039E_CONVERSATION);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 20;
    SetCurrentDateTimeTo(time);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_039E_CHARACTERS, parent->self);
}

// @verified - p
void Update039E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue039EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup039E(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 1;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 1)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_039E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup039E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_039E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_039E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue039EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update039EAuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare039ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update039EAuthor(AuthorViewer517ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore039ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update039EAuthorTask(void *taskData) {
    Update039EAuthor(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare039ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_039E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_039E_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_039E_a128, false);
    for (const shen_name *character = AREA_039E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore039ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_039E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_039E_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_039E_a128, true);
    for (const shen_name *character = AREA_039E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area039EScriptDefinition{.areaId = AREA_039E, .field04 = 0, .init = Initialize039E, .main = Start039E};

} // namespace shenmue::s2::areas
