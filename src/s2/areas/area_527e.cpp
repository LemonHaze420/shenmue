#include "s2/areas/area_527e.hpp"
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
void Initialize527E() {
}

// @verified - p
void Start527E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update527E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_527E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_527E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_527E_CHARACTERS, parent->self);
}

// @verified - p
void Update527E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue527EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup527E(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 32;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 32)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 32) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_527E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        if (std::bit_cast<std::int32_t>(parent->selection) < 10) {
            EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
        } else {
            EV_debug_log_to_screen("seqdata%c.avb\n", parent->selection + 87U);
        }
    }
}

// @verified - p
void Cleanup527E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_527E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_527E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue527EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update527EAuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    const std::int32_t objectFlag = parent->selection == 13 ? 1 : 0;
    for (std::int32_t index = 0; index <= 5; ++index)
        EV_SetObjectFlagByIndex(index, objectFlag);
    switch (parent->selection) {
    case 2:
    case 9:
    case 11:
    case 12:
    case 14:
    case 15:
    case 23:
    case 24:
    case 25:
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        break;
    }
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare527ECharacters();
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update527EAuthor(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore527ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update527EAuthorTask(void *taskData) {
    Update527EAuthor(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare527ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_527E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_527E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_527E_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_527E_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_527E_a980, false);
    EV_SetCharacterAlphaEnabled(AREA_527E_a984, false);
    EV_SetCharacterAlphaEnabled(AREA_527E_a982, false);
    EV_SetCharacterAlphaEnabled(AREA_527E_a986, false);
    EV_SetCharacterAlphaEnabled(AREA_527E_a016, false);
    for (const shen_name *character = AREA_527E_PROPS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore527ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_527E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_527E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_527E_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_527E_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_527E_a980, true);
    EV_SetCharacterAlphaEnabled(AREA_527E_a984, true);
    EV_SetCharacterAlphaEnabled(AREA_527E_a982, true);
    EV_SetCharacterAlphaEnabled(AREA_527E_a986, true);
    EV_SetCharacterAlphaEnabled(AREA_527E_a016, true);
    for (const shen_name *character = AREA_527E_PROPS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area527EScriptDefinition{.areaId = AREA_527E, .field04 = 0, .init = Initialize527E, .main = Start527E};

} // namespace shenmue::s2::areas
