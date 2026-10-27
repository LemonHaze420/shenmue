#include "s2/areas/area_552e.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/pause.hpp"
#include "s2/flag.hpp"
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
void Initialize552E() {
}

// @verified - p
void Start552E(std::uintptr_t) {
    shenSetClockAndCompassRenderTo(1);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update552E, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = 0;
    EV_UpdateWeatherForMinute(0, 0, 0, 1.0f);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_552E_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_552E_CHARACTERS, parent->self);
}

// @verified - p
void Update552E(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update552EAuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
        static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        Prepare552ECharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup552E();
    }
}

// @verified - p
void Cleanup552E() {
    for (const shen_name *character = AREA_552E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_552E_CONVERSATION);
    EV_EventSetUpHelpIcon_0(0x40, 0);
    Queue_ST_LoadTitleFadeIn(AREA_552E_TITLE);
    FinishLoading();
    HLib::SetCurrentTaskTo(Exit552EB);
    HLib::SetupTaskDelay(120);
}

// @verified - p
std::int64_t Update552EAuthor(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 3;
        Restore552ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update552EAuthorTask(void *taskData) {
    Update552EAuthor(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare552ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_552E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_552E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_552E_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_552E_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_552E_ab02, false);
    EV_SetCharacterAlphaEnabled(AREA_552E_ab03, false);
}

// @verified - p
void Restore552ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_552E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_552E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_552E_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_552E_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_552E_ab02, true);
    EV_SetCharacterAlphaEnabled(AREA_552E_ab03, true);
}

// @verified - p
void Exit552EB(void *) {
    ST_LoadTitleFade();
    Initialize552E();
    HLib::SetCurrentTaskTo(Exit552EC);
    HLib::SetupTaskDelay(30);
}

// @verified - p
void Exit552EC(void *taskData) {
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(static_cast<AuthorViewerPairTaskParameter *>(taskData)->self);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 902, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 901, 1);
    EV_PTYCUtils(AREA_552E_SIN_, 3, 0);
    std::int32_t flag = 0x459;
    shen_name map = AREA_552E_FOREST1;
    std::uint32_t entry = 99;
    std::int8_t hour = 9, minute = 0;
    if (!EV_GetShenmueFlag(0x459)) {
        if (EV_GetShenmueFlag(0x454)) {
            flag = 0x454;
            map = AREA_552E_FOREST2;
            entry = 0;
            minute = 30;
        } else if (EV_GetShenmueFlag(0x455)) {
            flag = 0x455;
            map = AREA_552E_FOREST3;
            entry = 0;
            hour = 10;
        } else if (EV_GetShenmueFlag(0x456)) {
            flag = 0x456;
            map = AREA_552E_FOREST3;
            hour = 11;
            minute = 30;
        } else if (EV_GetShenmueFlag(0x457)) {
            flag = 0x457;
            map = AREA_552E_FOREST4;
            entry = 0;
            hour = 15;
        }
    }
    EV_stepSetFlags(0, flag);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = hour;
    time.minute = minute;
    SetCurrentDateTimeTo(time);
    scenSetTaskFlags(8);
    EV_scenQueueLoad(g_actNumber, map, entry);
}

const ScriptAreaDefinition area552EScriptDefinition{.areaId = AREA_552E, .field04 = 0, .init = Initialize552E, .main = Start552E};

} // namespace shenmue::s2::areas
