#include "s2/areas/area_0275.hpp"
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
#include "s2/world_render.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0275() {
}

// @verified - p
void Start0275(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0275, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0275_MOTION));
    parent->state = 1;
    parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint <= 1 ? g_entryPoint : 0));
    SetPlayerControlEnabled(0);
    SetIsSkySetup(0);
    EV_FreeAuthorPak(1, AREA_0275_CONVERSATION);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0275_CHARACTERS, parent->self);
}

// @verified - p
void Update0275(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1)
        Queue0275Author(parent);
    else if (parent->state == 3)
        Cleanup0275(parent);
}

// @verified - p
void Cleanup0275(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0275_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0275_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0275Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0275AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0275Characters();
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0275Author(AuthorViewerLoopTaskParameter *child) {
    if (child->parent->selection != 0) {
        const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        if (frame == 241) {
            EV_lightLoad(AREA_0275_LIGHT);
        } else if (frame == 510) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        }
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 3;
        Restore0275Characters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0275AuthorTask(void *taskData) {
    Update0275Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0275Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0275_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0275_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0275_a334, false);
    EV_SetCharacterAlphaEnabled(AREA_0275_a372, false);
    EV_SetCharacterAlphaEnabled(AREA_0275_a532, false);
    EV_SetCharacterAlphaEnabled(AREA_0275_aa72, false);
    for (const shen_name *character = AREA_0275_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0275Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0275_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0275_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0275_a334, true);
    EV_SetCharacterAlphaEnabled(AREA_0275_a372, true);
    EV_SetCharacterAlphaEnabled(AREA_0275_a532, true);
    EV_SetCharacterAlphaEnabled(AREA_0275_aa72, true);
    for (const shen_name *character = AREA_0275_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0275ScriptDefinition{.areaId = AREA_0275, .field04 = 0, .init = Initialize0275, .main = Start0275};

} // namespace shenmue::s2::areas
