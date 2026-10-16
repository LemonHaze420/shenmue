#include "s2/areas/area_0309.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/elevator.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0309() {
}

// @verified - p
void Setup0309() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0309_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    SetLightRootPosition(AREA_0309_MAP_POSITION);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_0309_MAP_POSITION, 0);
}

// @verified - p
void Start0309(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0309, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0309_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Setup0309();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0309_CHARACTERS, parent->self);
}

// @verified - p
void Update0309(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0309AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
        static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        Prepare0309Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0309(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 0)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0309_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0309(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0309_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0309_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update0309Author(AuthorViewerLoopTaskParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    for (const Area0309ElevatorFrame *row = AREA_0309_ELEVATOR_FRAMES; row->elevator.name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++row) {
        if (row->frame == frame)
            SetCurrentElevatorFloor(row->elevator, static_cast<std::int16_t>(row->floor));
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetCurrentElevatorFloor(AREA_0309_ELEVATOR, 18);
        Restore0309Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0309AuthorTask(void *taskData) {
    Update0309Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0309Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0309_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0309_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0309_a512, false);
    EV_SetCharacterAlphaEnabled(AREA_0309_a516, false);
    for (const shen_name *character = AREA_0309_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0309Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0309_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0309_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0309_a512, true);
    EV_SetCharacterAlphaEnabled(AREA_0309_a516, true);
    for (const shen_name *character = AREA_0309_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0309ScriptDefinition{.areaId = AREA_0309, .field04 = 0, .init = Initialize0309, .main = Start0309};

} // namespace shenmue::s2::areas
