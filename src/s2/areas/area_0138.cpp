#include "s2/areas/area_0138.hpp"
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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/character.hpp"
#include "s2/weather.hpp"
#include "s2/sound_program.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0138MainCallback(void *taskData);
void E0138LoopCallback(void *taskData);

// @verified - p
void E0138Init() {
}

// @verified - p
void E0138Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0138MainCallback, 4, 0x0b, 0x18, AREA_AUTHOR_TASK);
    E0138TaskParameter *const parent = static_cast<E0138TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    for (std::int32_t index = 0; index < 3; ++index)
        parent->motions[index] = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0138_MOTIONS[index]));
    parent->selection = 0;
    parent->state = 0;
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 22;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0138_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0138_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0138Characters() {
    SetScenarioCharacterRadius(AREA_AUTHOR_RYO, 50.0f);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0138_A073, false);
    EV_SetCharacterAlphaEnabled(AREA_0138_A417, false);
    EV_SetCharacterAlphaEnabled(AREA_0138_AA50, false);
    EV_SetCharacterAlphaEnabled(AREA_0138_A014, false);
    for (const shen_name *character = AREA_0138_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        SetScenarioCharacterRadius(*character, 50.0f);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0138Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0138_A073, true);
    EV_SetCharacterAlphaEnabled(AREA_0138_A417, true);
    EV_SetCharacterAlphaEnabled(AREA_0138_AA50, true);
    EV_SetCharacterAlphaEnabled(AREA_0138_A014, true);
    for (const shen_name *character = AREA_0138_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0138(E0138TaskParameter *parent) {
    for (const shen_name *character = AREA_0138_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    for (const char *package : AREA_0138_PACKAGES)
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_FreeAuthorPak(0, AREA_0138_CONVERSATION);
    for (std::uint32_t motion : parent->motions)
        DeleteMotion(std::bit_cast<std::int32_t>(motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void ConfigureE0138Environment(std::int16_t selection) {
    const char *const oldPackage = selection != 0 ? "AKA3" : "AB00";
    const char *const package = selection != 0 ? "AB00" : "AKA3";
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(oldPackage), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(oldPackage), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetModelDataDefaultVertexShader();
    if (selection == 0 || selection == 1) {
        SetWeatherUpdateEnabledTo(1);
        EV_lightLoad(selection == 0 ? AREA_0138_LIGHT_001 : AREA_0138_LIGHT_101);
        PushGlobalLightingProgram();
    } else if (selection == 2) {
        SetWeatherUpdateEnabledTo(0);
        EV_lightLoad(AREA_0138_LIGHT_201);
        PushGlobalLightingProgram();
        EV_lightLoad(AREA_0138_LIGHT_202);
    }
}

// @verified - p
void QueueE0138Loop(E0138TaskParameter *parent) {
    ConfigureE0138Environment(parent->selection);
    EV_LoadCutsceneFromCurrentScene(AREA_0138_MEDIA[parent->selection]);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0138LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0138LoopParameter *const child = static_cast<E0138LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->state = 0;
    EV_LoadAudio(AREA_0138_MEDIA[parent->selection], 0);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0138Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0138MainCallback(void *taskData) {
    E0138TaskParameter *const parent = static_cast<E0138TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0138Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0138(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 2;
        } else if (input == 128 && ++parent->selection > 2)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0138_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0138LoopCallback(void *taskData) {
    E0138LoopParameter *const child = static_cast<E0138LoopParameter *>(taskData);
    if (child->parent->selection == 2 && EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 600)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 1);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0138Characters();
    child->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0138ScriptDefinition{.areaId = AREA_0138_AREA, .field04 = 0, .init = E0138Init, .main = E0138Main};

} // namespace shenmue::s2::areas
