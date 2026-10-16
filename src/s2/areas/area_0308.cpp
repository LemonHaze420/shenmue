#include "s2/areas/area_0308.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/date_time.hpp"
#include "s2/load_title.hpp"
#include "s2/sound_program.hpp"
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
void Initialize0308() {
}

// @verified - p
void Start0308(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0308, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0308_MOTION));
    parent->selection = 0;
    parent->state = 1;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0308_CHARACTERS, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0308_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0308_LIGHT);
    PushGlobalLightingProgram();
}

// @verified - p
void Update0308(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0308AuthorTask, 4, 0x0b, sizeof(Area0308LoopParameter), AUTHOR_VIEWER_TASK);
        Area0308LoopParameter *const child = static_cast<Area0308LoopParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->currentFrame = 0;
        Prepare0308Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0308(parent);
    }
}

// @verified - p
void Cleanup0308(AuthorViewerTaskParameter *parent) {
    shenUpdateCurrentDateTime();
    for (const shen_name *character = AREA_0308_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0308_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, AREA_0308_RETURN, 70);
}

// @verified - p
std::int64_t Update0308Author(Area0308LoopParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        if (static_cast<std::uint32_t>(GetAuthorFlags()) != 0) {
            spmFadeOut();
            HLib::SetupTimedTask(child->parent->self, 40);
        }
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 3;
        Restore0308Characters();
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0308AuthorTask(void *taskData) {
    Update0308Author(static_cast<Area0308LoopParameter *>(taskData));
}

// @verified - p
void Prepare0308Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0308_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0308_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a209, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a207, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a208, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a270, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a047, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a520, false);
    EV_SetCharacterAlphaEnabled(AREA_0308_a014, false);
    for (const shen_name *character = AREA_0308_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0308Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0308_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0308_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a209, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a207, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a208, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a270, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a047, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a520, true);
    EV_SetCharacterAlphaEnabled(AREA_0308_a014, true);
    for (const shen_name *character = AREA_0308_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0308ScriptDefinition{.areaId = AREA_0308, .field04 = 0, .init = Initialize0308, .main = Start0308};

} // namespace shenmue::s2::areas
