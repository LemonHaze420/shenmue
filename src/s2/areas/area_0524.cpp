#include "s2/areas/area_0524.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/areas/area_0507.hpp"
#include "s2/date_time.hpp"
#include "s2/world_render.hpp"
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
void Initialize0524() {
}

// @verified - p
void Setup0524() {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 0;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0524_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0524_LIGHT);
    AssignLightProgram(0);
    AssignLightProgram(6);
    AssignLightProgram(7);
    AssignLightProgram(8);
    AssignLightProgram(3);
    AssignLightProgram(5);
    HLib::EnqueueTaskWithoutParameter(Wave0507Resources, 3, 2, static_cast<std::int32_t>(AREA_0507_WAVE_TASK.name));
}

// @verified - p
void Start0524(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0524, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0524_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0524();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0524_CHARACTERS, parent->self);
}

// @verified - p
void Update0524(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0524Author(parent);
    } else if (parent->state == 3) {
        Cleanup0524(parent);
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
            EV_debug_log_to_screen(AREA_0524_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0524(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0524_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0524_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0524Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0524AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer519ELoopTaskParameter *const child = static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_LoadAudio(AREA_0524_AUDIO, 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0524_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0524_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0524_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0524_SIN_, false);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0524Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0524_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0524_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0524_SIN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0524_SIN_, true);
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0524AuthorTask(void *taskData) {
    Update0524Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

const ScriptAreaDefinition area0524ScriptDefinition{.areaId = AREA_0524, .field04 = 0, .init = Initialize0524, .main = Start0524};

} // namespace shenmue::s2::areas
