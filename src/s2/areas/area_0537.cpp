#include "s2/areas/area_0537.hpp"
#include "s2/author.hpp"
#include "s2/input.hpp"
#include "s2/character.hpp"
#include "s2/areas/numeric_event.hpp"
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
void Initialize0537() {
}

// @verified - p
void Setup0537() {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 20;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0537_CONVERSATION);
}

// @verified - p
void Start0537(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0537, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0537_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0537();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0537_CHARACTERS, parent->self);
}

// @verified - p
void Update0537(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0537Author(parent);
    } else if (parent->state == 3) {
        Cleanup0537(parent);
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
            EV_debug_log_to_screen(AREA_0537_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0537(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0537_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0537_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0537Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0537AuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    if (parent->selection != 0) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        AssignLightProgram(0);
        AssignLightProgram(1);
        AssignLightProgram(5);
        EV_lightLoad(AREA_0537_LIGHT1);
        AssignLightProgram(3);
    } else {
        EV_lightLoad(AREA_0537_LIGHT0);
        PushGlobalLightingProgram();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0537Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0537Author(AuthorViewer517ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0537Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0537AuthorTask(void *taskData) {
    Update0537Author(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0537Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0537_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0537_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0537_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0537_SIN_, false);
    const Vector3 position{};
    EV_AT_SetPos(AREA_0537_aa44, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa44, false);
    EV_AT_SetPos(AREA_0537_aa45, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa45, false);
    EV_AT_SetPos(AREA_0537_a726, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_a726, false);
    EV_AT_SetPos(AREA_0537_a728, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_a728, false);
    EV_AT_SetPos(AREA_0537_aa62, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa62, false);
    EV_AT_SetPos(AREA_0537_aa63, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa63, false);
}

// @verified - p
void Restore0537Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0537_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0537_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0537_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0537_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa44, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa45, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_a726, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_a728, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa62, true);
    EV_SetCharacterAlphaEnabled(AREA_0537_aa63, true);
}

const ScriptAreaDefinition area0537ScriptDefinition{.areaId = AREA_0537, .field04 = 0, .init = Initialize0537, .main = Start0537};

} // namespace shenmue::s2::areas
