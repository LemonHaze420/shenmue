#include "s2/areas/area_0277.hpp"
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
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/date_time.hpp"
#include "s2/field_collision.hpp"
#include "s2/inventory.hpp"
#include "s2/map_assets.hpp"
#include "s2/world_render.hpp"
#include "s2/weather.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0277() {
}

// @verified - p
void Start0277(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0277, 4, 0x0b, sizeof(Area0277Parameter), AUTHOR_VIEWER_TASK);
    Area0277Parameter *const parent = static_cast<Area0277Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = g_entryPoint <= 2 ? static_cast<std::int16_t>(g_entryPoint) : 0;
    Setup0277Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0277_CHARACTERS, parent->self);
}

// @verified - p
void Update0277(void *taskData) {
    Area0277Parameter *const parent = static_cast<Area0277Parameter *>(taskData);
    if (parent->state == 1)
        Queue0277Author(parent);
    else if (parent->state == 3)
        Cleanup0277(parent);
}

// @verified - p
void Cleanup0277(Area0277Parameter *parent) {
    AddInventoryItem(11, 1, 0);
    AddInventoryItem(12, 1, 0);
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 22;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    for (const shen_name *character = AREA_0277_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0277_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, EV_GetShenmueFlag(0x434) == 0 ? AREA_0277_RETURN_CLEAR : AREA_0277_RETURN_FLAGGED, 0);
}

// @verified - p
std::int64_t Queue0277Author(Area0277Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0277AuthorTask, 4, 0x0b, sizeof(Area0277AuthorParameter), AUTHOR_VIEWER_TASK);
    Area0277AuthorParameter *const child = static_cast<Area0277AuthorParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_LoadAudio("0277A000", 0);
    if (parent->selection == 2) {
        EV_SetObjectFlagByIndex(0, 0);
        EV_SetObjectFlagByIndex(1, 1);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0277_LIGHT);
        PushGlobalLightingProgram();
    } else {
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 1);
        EV_SetModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0277Characters();
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0277Author(Area0277AuthorParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->parent->selection == 0) {
        Vector3 position;
        EV_GetCharaPosition(AREA_0277_RYO_, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y = 500.0f;
        position.y = shenFindIntersectionWithScene(position, 500.0f);
        EV_AT_SetPos(AREA_0277_RYO_, EventTransformFlags::AllAxes, position);
        if (frame == 2180)
            SetNewWindPhaseAndSpeed(60334, 1.8200000524520874f);
        else if (frame == 2290)
            SyncWindWithWDATWeatherState();
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        if (child->parent->selection == 0) {
            SyncWindWithWDATWeatherState();
            ++child->parent->selection;
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(child->parent->selection));
        } else if (child->parent->selection == 1) {
            ++child->parent->selection;
            child->parent->state = 1;
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            return HLib::DestroyCurrentTask();
        } else if (child->parent->selection == 2) {
            child->parent->state = 3;
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::DestroyCurrentTask();
            Restore0277Characters();
        }
    }
    return result;
}

void Update0277AuthorTask(void *taskData) {
    Update0277Author(static_cast<Area0277AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0277Characters() {
    SetScenarioCharacterRadius(AREA_0277_RYO_, 40.0f);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0277_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0277_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a209, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a207, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a208, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a270, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a324, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a325, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a047, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a342, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_a343, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_ab18, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_ab19, false);
    EV_SetCharacterAlphaEnabled(AREA_0277_aa23, false);
    for (const shen_name *character = AREA_0277_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        SetScenarioCharacterRadius(*character, 40.0f);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0277Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0277_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0277_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a209, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a207, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a208, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a270, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a324, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a325, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a047, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a342, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_a343, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_ab18, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_ab19, true);
    EV_SetCharacterAlphaEnabled(AREA_0277_aa23, true);
    for (const shen_name *character = AREA_0277_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void Setup0277Scene() {
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 17;
    date.minute = 25;
    SetCurrentDateTimeTo(date);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    SetIsSkySetup(0);
    SetPlayerControlEnabled(0);
    SetRenderClockAndCompassTo(1);
    EV_FreeAuthorPak(1, AREA_0277_CONVERSATION);
}

const ScriptAreaDefinition area0277ScriptDefinition{.areaId = AREA_0277, .field04 = 0, .init = Initialize0277, .main = Start0277};

} // namespace shenmue::s2::areas
