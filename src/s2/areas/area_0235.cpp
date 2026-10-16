#include "s2/areas/area_0235.hpp"
#include "s2/character.hpp"
#include "s2/sound_program.hpp"

#include "s2/author.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0235MainCallback(void *taskData);
void E0235LoopCallback(void *taskData);

// @verified - p
void E0235Init() {
}

// @verified - p
void E0235Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0235MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0235_MOTION));
    parent->selection = 0;
    parent->state = 0;
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 17;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0235");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0235_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0235Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A371, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A368, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A367, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A369, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A370, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A516, false);
    EV_SetCharacterAlphaEnabled(AREA_0235_A512, false);
    for (const shen_name *character = AREA_0235_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0235Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A371, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A368, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A367, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A369, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A370, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A516, true);
    EV_SetCharacterAlphaEnabled(AREA_0235_A512, true);
    for (const shen_name *character = AREA_0235_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void LoadE0235Packages(std::int32_t selection) {
    const char *const oldPackage = selection == 2 ? "QA11" : "QAE6";
    const char *const newPackage = selection == 2 ? "QAE6" : "QA11";
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(oldPackage), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(oldPackage), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(newPackage));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

// @verified - p
void QueueE0235Loop(AreaAuthorTaskParameter *parent) {
    LoadE0235Packages(parent->selection);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0235LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection == 1) {
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0235_L101);
        PushGlobalLightingProgram();
    }
    EV_LoadCutsceneFromCurrentScene(parent->selection == 2 ? "0235A012" : "0235A000");
    BeginSecondarySampleExecution(0);
    SetupE0235Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0235MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0235Loop(parent);
    } else if (parent->state == 3) {
        EV_FreeAuthorPak(0, AREA_0235_CONVERSATION);
        EV_CleanupMapControlTask();
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 2;
        } else if (input == 128 && ++parent->selection > 2)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0235_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}

// @verified - p
void E0235LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0235Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0235ScriptDefinition{.areaId = AREA_0235_AREA, .field04 = 0, .init = E0235Init, .main = E0235Main};

} // namespace shenmue::s2::areas
