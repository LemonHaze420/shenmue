#include "s2/areas/area_0289.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0289() {
}

// @verified - p
void Setup0289() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0289_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    EV_SetObjectFlagByIndex(13, 0);
    SetLightRootPosition(AREA_0289_MAP_POSITION);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_0289_MAP_POSITION, 0);
}

// @verified - p
void Start0289(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0289, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0289_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Setup0289();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0289_CHARACTERS, parent->self);
}

// @verified - p
void Update0289(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0289Author(parent);
    } else if (parent->state == 3) {
        Cleanup0289(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, INPUT_PRESSED)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 2;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 2)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0289_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void Cleanup0289(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0289_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0289_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0289Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0289AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0289Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0289Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore0289Characters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0289AuthorTask(void *taskData) {
    Update0289Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0289Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0289_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_REN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0289_REN_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_TGY_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0289_TGY_, false);
    EV_SetCharacterAlphaEnabled(AREA_0289_a942, false);
}

// @verified - p
void Restore0289Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0289_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_REN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0289_REN_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0289_TGY_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0289_TGY_, true);
    EV_SetCharacterAlphaEnabled(AREA_0289_a942, true);
}

const ScriptAreaDefinition area0289ScriptDefinition{.areaId = AREA_0289, .field04 = 0, .init = Initialize0289, .main = Start0289};

} // namespace shenmue::s2::areas
