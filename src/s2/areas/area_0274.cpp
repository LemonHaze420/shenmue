#include "s2/areas/area_0274.hpp"
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
#include "s2/date_time.hpp"
#include "s2/world_render.hpp"
#include "s2/map_assets.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0274() {
}

// @verified - p
void Setup0274() {
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 17;
    date.minute = 30;
    SetCurrentDateTimeTo(date);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    SetPlayerControlEnabled(0);
    SetIsSkySetup(0);
    EV_FreeAuthorPak(1, AREA_0274_CONVERSATION);
}

// @verified - p
void Start0274(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0274, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0274_MOTION));
    parent->selection = 0;
    parent->state = 1;
    Setup0274();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0274_CHARACTERS, parent->self);
}

// @verified - p
void Update0274(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1)
        Queue0274Author(parent);
    else if (parent->state == 3)
        Cleanup0274(parent);
}

// @verified - p
void Cleanup0274(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0274_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0274_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0274Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0274AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0274Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0274Author(AuthorViewerLoopTaskParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 3511)
        SetAllMapFieldDepthSlotsEnabled(0);
    else if (frame == 3622)
        SetAllMapFieldDepthSlotsEnabled(1);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0274Characters();
        child->parent->state = 3;
        HLib::SetupTimedTask(child->parent->self, 30);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0274AuthorTask(void *taskData) {
    Update0274Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0274Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0274_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0274_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0274_a334, false);
    EV_SetCharacterAlphaEnabled(AREA_0274_a372, false);
    EV_SetCharacterAlphaEnabled(AREA_0274_a532, false);
    EV_SetCharacterAlphaEnabled(AREA_0274_aa72, false);
    for (const shen_name *character = AREA_0274_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0274Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0274_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0274_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0274_a334, true);
    EV_SetCharacterAlphaEnabled(AREA_0274_a372, true);
    EV_SetCharacterAlphaEnabled(AREA_0274_a532, true);
    EV_SetCharacterAlphaEnabled(AREA_0274_aa72, true);
    for (const shen_name *character = AREA_0274_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0274ScriptDefinition{.areaId = AREA_0274, .field04 = 0, .init = Initialize0274, .main = Start0274};

} // namespace shenmue::s2::areas
