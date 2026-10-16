#include "s2/areas/area_0541.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/sound_program.hpp"
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
void Initialize0541() {
}

// @verified - p
void Start0541(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0541, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0541_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0541_CONVERSATION);
    EV_SetWeatherInstruction(0x780, 0);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0541_CHARACTERS, parent->self);
}

// @verified - p
void Update0541(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0541Author(parent);
    } else if (parent->state == 3) {
        Cleanup0541(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
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
            EV_debug_log_to_screen(AREA_0541_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0541(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0541_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0541_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0541Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0541AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_LoadCutsceneFromCurrentScene(parent->selection != 0 ? AREA_0541_AUDIO1 : AREA_0541_AUDIO0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0541Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0541Author(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0541_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0541_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0541_SIN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0541_SIN_, true);
        EV_SetCharacterAlphaEnabled(AREA_0541_a636, true);
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0541AuthorTask(void *taskData) {
    Update0541Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0541Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0541_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0541_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0541_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0541_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0541_a636, false);
    const Vector3 position{};
    EV_AT_SetPos(AREA_0541_RYO_, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetPos(AREA_0541_SIN_, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetPos(AREA_0541_a636, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}

const ScriptAreaDefinition area0541ScriptDefinition{.areaId = AREA_0541, .field04 = 0, .init = Initialize0541, .main = Start0541};

} // namespace shenmue::s2::areas
