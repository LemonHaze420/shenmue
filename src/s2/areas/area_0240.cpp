#include "s2/areas/area_0240.hpp"
#include "s2/character.hpp"
#include "s2/inventory.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/memo.hpp"
#include "s2/ui.hpp"
#include "s2/pause.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0240MainCallback(void *taskData);
void E0240LoopCallback(void *taskData);

// @verified - p
void E0240Init() {
}

// @verified - p
void E0240Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0240MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0240_MOTION));
    parent->selection = 0;
    parent->state = 4;
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0240_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0240_LIGHT);
    PushGlobalLightingProgram();
    EV_DisableCharacterDisplay(AREA_AUTHOR_RYO);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0240_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0240Characters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A207, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A209, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A014, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A326, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A045, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A208, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A270, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A806, false);
    EV_SetCharacterAlphaEnabled(AREA_0240_A341, false);
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0240Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A207, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A209, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A014, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A326, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A045, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A208, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A270, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A806, true);
    EV_SetCharacterAlphaEnabled(AREA_0240_A341, true);
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0240(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0240_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_stepSetFlags(0, 0x119);
    EV_FreeAuthorPak(0, AREA_0240_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0240_QR00, 99);
}

// @verified - p
void QueueE0240Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0240LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    RefreshWeatherForCurrentTime(0);
    EV_FreeWAIR();
    EV_UpdateWeather();
    SetupE0240Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0240ItemReadyNullsub() {
}

// @verified - p
void E0240MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    switch (parent->state) {
    case 1:
        SetIsSkySetup(1);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        QueueE0240Loop(parent);
        break;
    case 3:
        CleanupE0240(parent);
        break;
    case 4:
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        SetIsSkySetup(0);
        RefreshPadTask(2, 1);
        FinishLoading();
        parent->state = 5;
        break;
    case 5:
        if (!IsItemSelectable()) {
            shenSetClockAndCompassRenderTo(1);
            E0240ItemReadyNullsub();
            parent->state = 1;
            HLib::SetupTaskDelay(60);
        }
        break;
    }
}

// @verified - p
void E0240LoopCallback(void *taskData) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    AddInventoryItem(0x48, 1, 1);
    HLib::SetupTimedTask(static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->self, 130);
    RestoreE0240Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0240ScriptDefinition{.areaId = AREA_0240_AREA, .field04 = 0, .init = E0240Init, .main = E0240Main};

} // namespace shenmue::s2::areas
