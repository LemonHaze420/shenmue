#include "s2/areas/area_280e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/areas/numeric_event.hpp"
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
void Initialize280E() {
}

// @verified - p
void Start280E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update280E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_280E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_280E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_280E_CHARACTERS, parent->self);
}

// @verified - p
void Update280E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue280EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup280E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 5;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 5)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_280E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup280E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_280E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_280E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue280EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update280EAuthorTask, 4, 0x0b, sizeof(Area280ELoopParameter), AUTHOR_VIEWER_TASK);
    Area280ELoopParameter *const child = static_cast<Area280ELoopParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare280ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update280EAuthor(Area280ELoopParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore280ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update280EAuthorTask(void *taskData) {
    Update280EAuthor(static_cast<Area280ELoopParameter *>(taskData));
}

// @verified - p
void Prepare280ECharacters() {
    ResetAreaAuthorCharacter(AREA_280E_RYO_, 0.0f);
    ResetAreaAuthorCharacter(AREA_280E_KT1_, 0.0f);
    const Vector3 position{};
    EV_AT_SetPos(AREA_280E_aa49, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_280E_aa49, false);
    EV_AT_SetPos(AREA_280E_a452, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_280E_a452, false);
    EV_AT_SetPos(AREA_280E_ab19, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_280E_ab19, false);
    EV_AT_SetPos(AREA_280E_ab18, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharacterAlphaEnabled(AREA_280E_ab18, false);
    for (const shen_name *character = AREA_280E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_AT_SetPos(*character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore280ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_280E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_280E_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_280E_aa49, true);
    EV_SetCharacterAlphaEnabled(AREA_280E_a452, true);
    EV_SetCharacterAlphaEnabled(AREA_280E_ab19, true);
    EV_SetCharacterAlphaEnabled(AREA_280E_ab18, true);
    for (const shen_name *character = AREA_280E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area280EScriptDefinition{.areaId = AREA_280E, .field04 = 0, .init = Initialize280E, .main = Start280E};

} // namespace shenmue::s2::areas
