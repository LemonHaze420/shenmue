#include "s2/areas/area_0064.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

#include "s2/author.hpp"
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
#include "s2/blur_figure_pose.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0064MainCallback(void *taskData);
void E0064LoopCallback(void *taskData);

// @verified - p
void E0064Init() {
}

// @verified - p
void E0064Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0064MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0064_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0064_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0064_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0064Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    LoadBlurModelForCharacter(AREA_0064_SYE_, "SYE_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_0064_KNK_, "KNK_B.MT7", 1.0f);
    for (const shen_name *character = AREA_0064_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0064Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    for (const shen_name *character = AREA_0064_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0064(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0064_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0064_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void LoadE0064ScenePackages(std::int32_t selection) {
    const char *const oldPackage = selection == 6 ? "WB00" : "WBK1";
    const char *const newPackage = selection == 6 ? "WBK1" : "WB00";
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(oldPackage), 0) != 0)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(oldPackage), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(newPackage));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(newPackage), 0);
    if (selection != 6)
        EV_SetObjectFlagByIndex(2, 0);
}

// @verified - p
void QueueE0064Loop(AreaAuthorTaskParameter *parent) {
    LoadE0064ScenePackages(parent->selection);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0064LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    EV_LoadAudio("0064A000", 0);
    if (parent->selection != 6)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0064Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0064MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0064Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0064(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 6;
        } else if (input == 128 && ++parent->selection > 6)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 6) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0064_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0064LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0064Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0064ScriptDefinition{.areaId = AREA_0064_AREA, .field04 = 0, .init = E0064Init, .main = E0064Main};

} // namespace shenmue::s2::areas
