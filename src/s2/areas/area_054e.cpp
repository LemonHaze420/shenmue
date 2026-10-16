#include "s2/areas/area_054e.hpp"
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
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize054E() {
}

// @verified - p
void Start054E(std::uintptr_t argument) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update054E, 4, 0x0b, sizeof(Area054EParameter), AUTHOR_VIEWER_TASK);
    Area054EParameter *const parent = static_cast<Area054EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    if (g_currentMap.name == AREA_054E.name) {
        parent->selection = 0;
        parent->state = 0;
    } else {
        const std::int32_t selection = *reinterpret_cast<const std::int32_t *>(argument);
        parent->selection = selection <= 1 ? std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(selection)) : 0;
        parent->state = 1;
    }
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_054E_CONVERSATION);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_LoadCutsceneFromCurrentScene(AREA_054E_AUDIO);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_054E_CHARACTERS, parent->self);
}

// @verified - p
void Update054E(void *taskData) {
    Area054EParameter *const parent = static_cast<Area054EParameter *>(taskData);
    if (parent->state == 1) {
        Queue054EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup054E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == PAD_BUTTON_X) {
            parent->state = 1;
        } else if (input == PAD_TRIGGER_LEFT) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 1;
        } else if (input == PAD_TRIGGER_RIGHT) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 1)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_054E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup054E(Area054EParameter *parent) {
    for (const shen_name *character = AREA_054E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_FreeAuthorPak(0, AREA_054E_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_054E_ARCHIVE), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_054E.name)
        EV_scenQueueLoad(g_actNumber, AREA_054E, g_entryPoint);
    else
        EV_scenQueueLoad(g_actNumber, AREA_054E_RETURN, 1);
}

// @verified - p
std::int64_t Queue054EAuthor(Area054EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update054EAuthorTask, 4, 0x0b, sizeof(Area054EAuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area054EAuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    Prepare054ECharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_054E_AUTHORS[parent->selection].name));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update054EAuthor(Area054EAuthorParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = g_currentMap.name == AREA_054E.name ? 0 : 3;
        Restore054ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update054EAuthorTask(void *taskData) {
    Update054EAuthor(static_cast<Area054EAuthorParameter *>(taskData));
}

// @verified - p
void Prepare054ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_054E_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_054E_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_054E_AGO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_054E_AGO_, false);
    EV_SetCharacterAlphaEnabled(AREA_054E_a417, false);
    EV_SetCharacterAlphaEnabled(AREA_054E_aa50, false);
    EV_SetCharacterAlphaEnabled(AREA_054E_a073, false);
}

// @verified - p
void Restore054ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_054E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_054E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_054E_AGO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_054E_AGO_, true);
    EV_SetCharacterAlphaEnabled(AREA_054E_a417, true);
    EV_SetCharacterAlphaEnabled(AREA_054E_aa50, true);
    EV_SetCharacterAlphaEnabled(AREA_054E_a073, true);
}

const ScriptAreaDefinition area054EScriptDefinition{.areaId = AREA_054E, .field04 = 0, .init = Initialize054E, .main = Start054E};

} // namespace shenmue::s2::areas
