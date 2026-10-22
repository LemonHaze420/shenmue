#include "s2/areas/area_0290.hpp"
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
void Initialize0290() {
}

// @verified - p
void Setup0290() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0290_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    SetLightRootPosition(AREA_0290_MAP_POSITION);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_0290_MAP_POSITION, 0);
}

// @verified - p
void Start0290(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0290, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0290_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Setup0290();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0290_CHARACTERS, parent->self);
}

// @verified - p
void Update0290(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0290AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
        static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        Prepare0290Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0290(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 1)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0290_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0290(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0290_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0290_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update0290Author(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0290Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0290AuthorTask(void *taskData) {
    Update0290Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0290Characters() {
    ResetAreaAuthorCharacter(AREA_0290_RYO_, 0.0f);
    for (const shen_name *character = AREA_0290_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        const Vector3 position{};
        EV_AT_SetPos(*character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0290Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0290_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0290_RYO_, true);
    for (const shen_name *character = AREA_0290_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0290ScriptDefinition{.areaId = AREA_0290, .field04 = 0, .init = Initialize0290, .main = Start0290};

} // namespace shenmue::s2::areas
