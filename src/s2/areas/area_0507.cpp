#include "s2/areas/area_0507.hpp"
#include "s2/author.hpp"
#include "s2/input.hpp"
#include "s2/character.hpp"
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
void Initialize0507() {
}

// @verified - p
void Setup0507() {
    SetPlayerControlEnabled(0);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 22;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    HLib::EnqueueTaskWithoutParameter(Wave0507Resources, 3, 2, static_cast<std::int32_t>(AREA_0507_WAVE_TASK.name));
    EV_FreeAuthorPak(1, AREA_0507_CONVERSATION);
}

// @verified - p
void Start0507(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0507, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0507_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0507();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0507_CHARACTERS, parent->self);
}

// @verified - p
void Update0507(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0507Author(parent);
    } else if (parent->state == 3) {
        Cleanup0507(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
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
        if (parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0507_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0507(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0507_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0507_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0507Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0507AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer519ELoopTaskParameter *const child = static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0507_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0507_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0507_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0507_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0507_a350, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0507Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0507_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0507_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0507_SIN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0507_SIN_, true);
        EV_SetCharacterAlphaEnabled(AREA_0507_a350, true);
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0507AuthorTask(void *taskData) {
    Update0507Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Wave0507Resources(void *) {
    SetTextureAnimationUV(0.0099999998f, 0.0f);
    ApplyTextureAnimationResource(6);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(7);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(8);
}

const ScriptAreaDefinition area0507ScriptDefinition{.areaId = AREA_0507, .field04 = 0, .init = Initialize0507, .main = Start0507};

} // namespace shenmue::s2::areas
