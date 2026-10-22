#include "s2/areas/area_063e.hpp"
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
void Initialize063E() {
}

// @verified - p
void Start063E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update063E, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 0;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, AREA_063E, AREA_063E_MOTION));
    parent->selection = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_063E_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_063E_CHARACTERS, parent->self);
}

// @verified - p
void Update063E(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue063EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup063E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 19;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 19)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 19) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_063E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup063E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_063E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_063E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue063EAuthor(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update063EAuthorTask, 4, 0x0b, sizeof(Area063ELoopParameter), AUTHOR_VIEWER_TASK);
    Area063ELoopParameter *const child = static_cast<Area063ELoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update063EAuthor(Area063ELoopParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        Prepare063ECharacters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore063ECharacters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update063EAuthorTask(void *taskData) {
    Update063EAuthor(static_cast<Area063ELoopParameter *>(taskData));
}

// @verified - p
void Prepare063ECharacters() {
    EV_EnableCharacterDisplay(AREA_063E_RYO_);
    EV_SetCharaControllerFlags(AREA_063E_RYO_, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_063E_RYO_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterRadius(AREA_063E_RYO_, 10.0f);
    EV_SetCharacterAlphaEnabled(AREA_063E_RYO_, false);
    EV_EnableCharacterDisplay(AREA_063E_REN_);
    EV_SetCharaControllerFlags(AREA_063E_REN_, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_063E_REN_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterRadius(AREA_063E_REN_, 10.0f);
    EV_SetCharacterAlphaEnabled(AREA_063E_REN_, false);
    EV_EnableCharacterDisplay(AREA_063E_a067);
    EV_SetCharaControllerFlags(AREA_063E_a067, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_063E_a067, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_063E_a067, false);
}

// @verified - p
void Restore063ECharacters() {
    EV_EnableCharacterDisplay(AREA_063E_RYO_);
    EV_SetCharaControllerFlags(AREA_063E_RYO_, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_RYO_, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_RYO_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_063E_RYO_, true);
    EV_EnableCharacterDisplay(AREA_063E_REN_);
    EV_SetCharaControllerFlags(AREA_063E_REN_, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_REN_, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_REN_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_063E_REN_, true);
    EV_EnableCharacterDisplay(AREA_063E_a067);
    EV_SetCharaControllerFlags(AREA_063E_a067, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_a067, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_063E_a067, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_063E_a067, true);
}

const ScriptAreaDefinition area063EScriptDefinition{.areaId = AREA_063E, .field04 = 0, .init = Initialize063E, .main = Start063E};

} // namespace shenmue::s2::areas
