#include "s2/areas/area_188e.hpp"
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
#include "s2/world_render.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize188E() {
}

// @verified - p
void Start188E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update188E, 4, 0x0b, sizeof(Area188EParameter), AUTHOR_VIEWER_TASK);
    Area188EParameter *const parent = static_cast<Area188EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_188E_FIRST_MOTION));
    parent->secondMotion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_188E_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_188E_FIRST_CONVERSATION);
    EV_FreeAuthorPak(1, AREA_188E_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_188E_CHARACTERS, parent->self);
}

// @verified - p
void Update188E(void *taskData) {
    Area188EParameter *const parent = static_cast<Area188EParameter *>(taskData);
    if (parent->state == 1) {
        Queue188EAuthor(parent);
    } else if (parent->state == 3) {
        Cleanup188E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 4;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 4)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 4) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_188E_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup188E(Area188EParameter *parent) {
    for (const shen_name *character = AREA_188E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_188E_FIRST_CONVERSATION);
    EV_FreeAuthorPak(0, AREA_188E_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    DeleteMotion(std::bit_cast<std::int32_t>(parent->secondMotion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue188EAuthor(Area188EParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update188EAuthorTask, 4, 0x0b, sizeof(Area188ELoopParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area188ELoopParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    if (parent->selection == 4)
        SetIsSkySetup(0);
    EV_LoadCutsceneFromCurrentScene(
        parent->selection >= 0 && (parent->selection <= 1 || parent->selection == 4) ? AREA_188E_SECOND_AUDIO : AREA_188E_FIRST_AUDIO);
    Move188EAuthorCharacter(AREA_188E_RYO_, 0.0f);
    Move188EAuthorCharacter(AREA_188E_XGH_, 0.0f);
    Move188EAuthorCharacter(AREA_188E_EDL_, 0.0f);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update188EAuthor(Area188ELoopParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetIsSkySetup(1);
        child->parent->state = 0;
        Restore188ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update188EAuthorTask(void *taskData) {
    Update188EAuthor(static_cast<Area188ELoopParameter *>(taskData));
}

// @verified - p
void Restore188ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_188E_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_188E_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_188E_XGH_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_188E_XGH_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_188E_EDL_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_188E_EDL_, true);
}

// @verified - p
void Move188EAuthorCharacter(shen_name character, float) {
    const Vector3 position{.x = 1000.0f, .y = 0.0f, .z = 0.0f};
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}

const ScriptAreaDefinition area188EScriptDefinition{.areaId = AREA_188E, .field04 = 0, .init = Initialize188E, .main = Start188E};

} // namespace shenmue::s2::areas
