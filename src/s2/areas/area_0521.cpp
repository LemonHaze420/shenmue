#include "s2/areas/area_0521.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
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
void Initialize0521() {
}

// @verified - p
void Setup0521() {
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    SetupLightConfig();
    AssignLightProgram(3);
    EV_AssignLightParams(1, 0);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 5);
    EV_FreeAuthorPak(1, AREA_0521_CONVERSATION);
}

// @verified - p
void Start0521(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0521, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0521_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0521();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0521_CHARACTERS, parent->self);
}

// @verified - p
void Update0521(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0521AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
        static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        Prepare0521Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0521(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 5;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 5)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0521_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0521(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0521_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0521_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update0521Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 0;
        Restore0521Characters();
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0521AuthorTask(void *taskData) {
    Update0521Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0521Characters() {
    ResetAreaAuthorCharacter(AREA_0521_RYO_, 0.0f);
    ResetAreaAuthorCharacter(AREA_0521_SIN_, 0.0f);
    const Vector3 position{};
    EV_AT_SetPos(AREA_0521_aa84, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0521_aa84, false);
    EV_AT_SetPos(AREA_0521_aa86, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_0521_aa86, false);
}

// @verified - p
void Restore0521Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0521_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0521_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0521_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0521_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0521_aa84, true);
    EV_SetCharacterAlphaEnabled(AREA_0521_aa86, true);
}

const ScriptAreaDefinition area0521ScriptDefinition{.areaId = AREA_0521, .field04 = 0, .init = Initialize0521, .main = Start0521};

} // namespace shenmue::s2::areas
