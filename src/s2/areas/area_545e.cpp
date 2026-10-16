#include "s2/areas/area_545e.hpp"
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
void Initialize545E() {
}

// @verified - p
void Setup545E() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_545E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 20;
    SetCurrentDateTimeTo(time);
    EV_SetWeatherInstruction(15, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(3);
    EV_lightLoad(AREA_545E_SECOND_LIGHT);
    AssignLightProgram(0);
    AssignLightProgram(1);
    AssignLightProgram(5);
}

// @verified - p
void Start545E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update545E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_545E_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup545E();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_545E_CHARACTERS, parent->self);
}

// @verified - p
void Update545E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue545EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup545E(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
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
            EV_debug_log_to_screen(AREA_545E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup545E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_545E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_545E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue545EAuthor(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer519ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_545E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_545E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_545E_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_545E_SIN_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

const ScriptAreaDefinition area545EScriptDefinition{.areaId = AREA_545E, .field04 = 0, .init = Initialize545E, .main = Start545E};

} // namespace shenmue::s2::areas
