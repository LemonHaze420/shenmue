#include "s2/areas/area_0271.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/handcuffs.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0271() {
}

// @verified - p
void Setup0271() {
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    SetLightRootPosition(AREA_0271_MAP_POSITION);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapModelSlotTransform(index, &AREA_0271_MAP_POSITION, 0);
    EV_SetObjectFlagByIndex(3, 0);
    EV_FreeAuthorPak(1, AREA_0271_CONVERSATION);
}

// @verified - p
void Start0271(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0271, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0271_MOTION));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0271_CHARACTERS, parent->self);
    Setup0271();
}

// @verified - p
void Update0271(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0271Author(parent);
    } else if (parent->state == 3) {
        Cleanup0271(parent);
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
            EV_debug_log_to_screen(AREA_0271_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0271(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0271_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0271_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0271Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0271AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_lightLoad(parent->selection == 0 ? LIGHT_DEFAULT_DEFINITION : AREA_0271_LIGHT);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0271Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0271Author(AuthorViewerLoopTaskParameter *child) {
    switch (static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame))) {
    case 581:
    case 1001:
    case 1311:
    case 1511:
    case 1711:
    case 2030:
        Handcuffs_Attach(AREA_0271_RYO_, 0, AREA_0271_YUA_, 1, AREA_0271_CUFF_DISTANCE);
        break;
    case 881:
    case 1050:
    case 1341:
    case 1631:
    case 1926:
    case 2221:
        cuffsSetFlags();
        break;
    default:
        break;
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        if (child->parent->selection != 0)
            cuffsSetFlags();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0271Characters();
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0271AuthorTask(void *taskData) {
    Update0271Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0271Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0271_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0271_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a073, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a392, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a399, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a414, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a512, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a516, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a026, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a026, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a554, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a030, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a323, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a148, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a220, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a069, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a070, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a017, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a018, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a058, false);
    EV_SetCharacterAlphaEnabled(AREA_0271_a712, false);
    for (const shen_name *character = AREA_0271_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0271Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0271_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0271_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a073, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a392, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a399, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a414, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a512, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a516, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a026, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a026, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a554, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a030, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a323, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a148, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a220, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a069, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a070, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a017, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a018, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a058, true);
    EV_SetCharacterAlphaEnabled(AREA_0271_a712, true);
    for (const shen_name *character = AREA_0271_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0271ScriptDefinition{.areaId = AREA_0271, .field04 = 0, .init = Initialize0271, .main = Start0271};

} // namespace shenmue::s2::areas
