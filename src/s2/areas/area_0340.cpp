#include "s2/areas/area_0340.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
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
void Initialize0340() {
}

// @verified - p
void Start0340(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0340, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0340_MOTION));
    parent->state = 0;
    parent->selection = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0340_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0340_CHARACTERS, parent->self);
}

// @verified - p
void Update0340(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0340Author(parent);
    } else if (parent->state == 3) {
        Cleanup0340(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 1;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 1)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0340_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0340(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0340_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0340_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0340Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0340AuthorTask, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer519ELoopTaskParameter *const child = static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0340Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0340Author(AuthorViewer519ELoopTaskParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        child->parent->state = 0;
        Restore0340Characters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0340AuthorTask(void *taskData) {
    Update0340Author(static_cast<AuthorViewer519ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0340Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0340_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0340_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0340_aa97, false);
    EV_SetCharacterAlphaEnabled(AREA_0340_aa98, false);
    for (const shen_name *character = AREA_0340_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
        const Vector3 position{};
        EV_AT_SetPos(*character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    }
}

// @verified - p
void Restore0340Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0340_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0340_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0340_aa97, true);
    EV_SetCharacterAlphaEnabled(AREA_0340_aa98, true);
    for (const shen_name *character = AREA_0340_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

const ScriptAreaDefinition area0340ScriptDefinition{.areaId = AREA_0340, .field04 = 0, .init = Initialize0340, .main = Start0340};

} // namespace shenmue::s2::areas
