#include "s2/areas/area_284e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize284E() {
}

// @verified - p
void Setup284E() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_284E_CONVERSATION);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    SetLightRootPosition(AREA_284E_MAP_POSITION);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_284E_MAP_POSITION, 0);
    EV_SetObjectFlagByIndex(2, 0);
}

// @verified - p
void Start284E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update284E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_284E_MOTION));
    parent->selection = 0;
    Setup284E();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_284E_CHARACTERS, parent->self);
}

// @verified - p
void Update284E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue284EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup284E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 7;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 7)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 7) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_284E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup284E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_284E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_284E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue284EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update284EAuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    const std::uint32_t selection = static_cast<std::uint32_t>(parent->selection);
    if (selection - 1U <= 3U || selection == 6U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare284ECharacters(selection, 0.0f);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update284EAuthor(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore284ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update284EAuthorTask(void *taskData) {
    Update284EAuthor(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare284ECharacters(std::uint32_t, float) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_284E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_284E_RYO_, false);
    for (const shen_name *character = AREA_284E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
        const Vector3 position{};
        EV_AT_SetPos(*character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
}

// @verified - p
void Restore284ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_284E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_284E_RYO_, true);
    for (const shen_name *character = AREA_284E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area284EScriptDefinition{.areaId = AREA_284E, .field04 = 0, .init = Initialize284E, .main = Start284E};

} // namespace shenmue::s2::areas
