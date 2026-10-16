#include "s2/areas/area_0525.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/areas/area_0507.hpp"
#include "s2/areas/area_0524.hpp"
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
void Initialize0525() {
}

// @verified - p
void Setup0525() {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 2;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    HLib::EnqueueTaskWithoutParameter(Wave0507Resources, 3, 2, static_cast<std::int32_t>(AREA_0507_WAVE_TASK.name));
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0525_LIGHT);
    AssignLightProgram(0);
    AssignLightProgram(6);
    AssignLightProgram(7);
    AssignLightProgram(8);
    AssignLightProgram(3);
    AssignLightProgram(5);
    EV_FreeAuthorPak(1, AREA_0525_CONVERSATION);
}

// @verified - p
void Start0525(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0525, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0525_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0525();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0525_CHARACTERS, parent->self);
}

// @verified - p
void Update0525(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0525Author(parent);
    } else if (parent->state == 3) {
        Cleanup0525(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 3;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 3)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0525_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0525(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0525_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0525_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0525Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0524AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer519ELoopTaskParameter *const child = static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio(AREA_0525_AUDIO, std::bit_cast<std::int32_t>(parent->selection));
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0525_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0525_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0525_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0525_SIN_, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

const ScriptAreaDefinition area0525ScriptDefinition{.areaId = AREA_0525, .field04 = 0, .init = Initialize0525, .main = Start0525};

} // namespace shenmue::s2::areas
