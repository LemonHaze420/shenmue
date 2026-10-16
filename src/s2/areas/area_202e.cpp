#include "s2/areas/area_202e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize202E() {
}

// @verified - p
void Start202E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update202E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_202E_MOTION));
    parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint));
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_202E_CONVERSATION);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    if (g_entryPoint == 99) {
        parent->selection = 0;
        parent->state = 0;
    } else {
        parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint));
        parent->state = 1;
        if (std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint)) > 0)
            Cleanup202E(parent);
    }
    Queue_EVT_UTL_LoadNPCChara_3(AREA_202E_CHARACTERS, parent->self);
}

// @verified - p
void Update202E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update202EAuthorTask, 4, 0x0b, sizeof(Area202ELoopParameter), AUTHOR_VIEWER_TASK);
        Area202ELoopParameter *const child = static_cast<Area202ELoopParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->currentFrame = 0;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup202E(parent);
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
            EV_debug_log_to_screen(AREA_202E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup202E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_202E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_202E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Update202EAuthor(Area202ELoopParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        Prepare202ECharacters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_entryPoint == 99 ? 0 : 3;
        Restore202ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update202EAuthorTask(void *taskData) {
    Update202EAuthor(static_cast<Area202ELoopParameter *>(taskData));
}

// @verified - p
void Prepare202ECharacters() {
    EV_EnableCharacterDisplay(AREA_202E_RYO_);
    EV_SetCharaControllerFlags(AREA_202E_RYO_, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_202E_RYO_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterRadius(AREA_202E_RYO_, 10.0f);
    EV_SetCharacterAlphaEnabled(AREA_202E_RYO_, false);
    EV_EnableCharacterDisplay(AREA_202E_a052);
    EV_SetCharaControllerFlags(AREA_202E_a052, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_202E_a052, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a052, false);
    EV_EnableCharacterDisplay(AREA_202E_a053);
    EV_SetCharaControllerFlags(AREA_202E_a053, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_202E_a053, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a053, false);
    EV_EnableCharacterDisplay(AREA_202E_a212);
    EV_SetCharaControllerFlags(AREA_202E_a212, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_202E_a212, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a212, false);
    EV_EnableCharacterDisplay(AREA_202E_aa22);
    EV_SetCharaControllerFlags(AREA_202E_aa22, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_202E_aa22, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_aa22, false);
    for (const shen_name *character = AREA_202E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        SetScenarioCharacterRadius(*character, 10.0f);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore202ECharacters() {
    EV_EnableCharacterDisplay(AREA_202E_RYO_);
    EV_SetCharaControllerFlags(AREA_202E_RYO_, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_RYO_, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_RYO_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_RYO_, true);
    EV_EnableCharacterDisplay(AREA_202E_a052);
    EV_SetCharaControllerFlags(AREA_202E_a052, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a052, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a052, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a052, true);
    EV_EnableCharacterDisplay(AREA_202E_a053);
    EV_SetCharaControllerFlags(AREA_202E_a053, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a053, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a053, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a053, true);
    EV_EnableCharacterDisplay(AREA_202E_a212);
    EV_SetCharaControllerFlags(AREA_202E_a212, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a212, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_a212, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_a212, true);
    EV_EnableCharacterDisplay(AREA_202E_aa22);
    EV_SetCharaControllerFlags(AREA_202E_aa22, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_aa22, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_202E_aa22, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_202E_aa22, true);
    for (const shen_name *character = AREA_202E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area202EScriptDefinition{.areaId = AREA_202E, .field04 = 0, .init = Initialize202E, .main = Start202E};

} // namespace shenmue::s2::areas
