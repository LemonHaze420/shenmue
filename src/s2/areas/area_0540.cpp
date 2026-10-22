#include "s2/areas/area_0540.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/blur_figure_pose.hpp"
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
void Initialize0540() {
}

// @verified - p
void Start0540(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0540, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0540_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0540_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0540_CHARACTERS, parent->self);
}

// @verified - p
void Update0540(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0540Author(parent);
    } else if (parent->state == 3) {
        Cleanup0540(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 3;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 3)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0540_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0540(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0540_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0540_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0540Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0540AuthorTask, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    Prepare0540Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0540Author(AuthorViewerLoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0540_RYO_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0540_RYO_, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0540_SIN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0540_SIN_, true);
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0540AuthorTask(void *taskData) {
    Update0540Author(static_cast<AuthorViewerLoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0540Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0540_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0540_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0540_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0540_SIN_, false);
    LoadBlurModelForCharacter(AREA_0540_RYO_, AREA_0540_BLUR_MODEL, 1.0f);
}

const ScriptAreaDefinition area0540ScriptDefinition{.areaId = AREA_0540, .field04 = 0, .init = Initialize0540, .main = Start0540};

} // namespace shenmue::s2::areas
