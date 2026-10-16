#include "s2/author_viewer_pair_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

#include <bit>

namespace shenmue::s2 {

// @verified - p
void AuthorViewer517EInit() {
}

// @verified - p
void AuthorViewer517EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer517EMainCallback, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const params = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_517E_MOTION));
    params->selection = 0;
    params->state = 0;
    SetupAuthorViewer517E();
    (void)Queue_EVT_UTL_LoadNPCChara_3(AUTHOR_VIEWER_517E_CHARACTERS, params->self);
}

// @verified - p
void AuthorViewer517EMainCallback(void *taskData) {
    AuthorViewerPairTaskParameter *const params = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        switch (input) {
        case 1024:
            params->state = 1;
            break;
        case 64:
            --params->selection;
            if (std::bit_cast<std::int32_t>(params->selection) < 0)
                params->selection = 0;
            break;
        case 128:
            ++params->selection;
            if (std::bit_cast<std::int32_t>(params->selection) > 0)
                params->selection = 0;
            break;
        default:
            break;
        }
        if (params->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_517E_DEBUG_NAME);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer517ELoop(params);
        break;
    case 2:
        break;
    case 3:
        CleanupAuthorViewer517E(params);
        break;
    default:
        return;
    }
}

// @verified - p
void SetupAuthorViewer517E() {
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_517E_CONVERSATION);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    dateTime.hour = 20;
    SetCurrentDateTimeTo(dateTime);
    EV_SetWeatherInstruction(AUTHOR_VIEWER_517E_WEATHER_LIGHTING_MASK, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    EV_lightLoad(AUTHOR_VIEWER_517E_LIGHT);
    AssignLightProgram(3);
    AssignLightProgram(4);
}

// @verified - p
void CleanupAuthorViewer517E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AUTHOR_VIEWER_517E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_517E_CONVERSATION);
    (void)DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    (void)HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t QueueAuthorViewer517ELoop(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer517ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const params = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    params->currentFrame = 0;
    (void)EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(std::bit_cast<std::int32_t>(parent->selection))));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t AuthorViewer517ELoopCallback(void *taskData) {
    AuthorViewer517ELoopTaskParameter *const params = static_cast<AuthorViewer517ELoopTaskParameter *>(taskData);
    params->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (params->currentFrame == 0) {
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_SIN), 6, 2);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_SIN, false);
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_SIN), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_SIN, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer517ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer517ELoopCallback(taskData);
}

// @verified - p
void AuthorViewer519EInit() {
}

// @verified - p
void AuthorViewer519EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer519EMainCallback, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const params = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_519E_MOTION));
    params->state = 0;
    params->selection = 0;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_519E_CONVERSATION);
    (void)Queue_EVT_UTL_LoadNPCChara_3(AUTHOR_VIEWER_519E_CHARACTERS, params->self);
}

// @verified - p
void AuthorViewer519EMainCallback(void *taskData) {
    AuthorViewerPairTaskParameter *const params = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        switch (input) {
        case 1024:
            params->state = 1;
            (void)SelectAuthorViewer519EPackage(std::bit_cast<std::int32_t>(params->selection));
            break;
        case 64:
            --params->selection;
            if (std::bit_cast<std::int32_t>(params->selection) < 0)
                params->selection = 3;
            break;
        case 128:
            ++params->selection;
            if (std::bit_cast<std::int32_t>(params->selection) > 3)
                params->selection = 0;
            break;
        default:
            break;
        }
        switch (params->selection) {
        case 0:
        case 1:
        case 2:
        case 3:
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_519E_DEBUG_NAMES[params->selection]);
            break;
        default:
            break;
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", std::bit_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer519ELoop(params);
        break;
    case 2:
        break;
    case 3:
        CleanupAuthorViewer519E(params);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t SelectAuthorViewer519EPackage(std::int32_t selection) {
    const char *directory;
    const char *package;
    if (selection < 2) {
        if (EV_ControlPackageResources(
                PakCategory::Package, AUTHOR_VIEWER_PACKAGE_IS_LOADED, reinterpret_cast<std::uintptr_t>(AUTHOR_VIEWER_519E_SECOND_PACKAGE), 0)) {
            (void)EV_ControlPackageResources(
                PakCategory::Package, AUTHOR_VIEWER_PACKAGE_FREE_IF_LOADED, reinterpret_cast<std::uintptr_t>(AUTHOR_VIEWER_519E_SECOND_PACKAGE), 0);
        }
        directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
        package = AUTHOR_VIEWER_519E_FIRST_PACKAGE;
    } else {
        if (EV_ControlPackageResources(
                PakCategory::Package, AUTHOR_VIEWER_PACKAGE_IS_LOADED, reinterpret_cast<std::uintptr_t>(AUTHOR_VIEWER_519E_FIRST_PACKAGE), 0)) {
            (void)EV_ControlPackageResources(
                PakCategory::Package, AUTHOR_VIEWER_PACKAGE_FREE_IF_LOADED, reinterpret_cast<std::uintptr_t>(AUTHOR_VIEWER_519E_FIRST_PACKAGE), 0);
        }
        directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
        package = AUTHOR_VIEWER_519E_SECOND_PACKAGE;
    }
    (void)EV_ControlPackageResources(
        PakCategory::Package, AUTHOR_VIEWER_PACKAGE_LOAD, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    return EV_ControlPackageResources(PakCategory::Package, AUTHOR_VIEWER_PACKAGE_WAIT, 0, 0);
}

// @verified - p
std::int64_t QueueAuthorViewer519ELoop(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer519ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer519ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer519ELoopTaskParameter *const params = static_cast<AuthorViewer519ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_SIN), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_SIN, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(std::bit_cast<std::int32_t>(parent->selection))));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t AuthorViewer519ELoopCallback(void *taskData) {
    AuthorViewer519ELoopTaskParameter *const params = static_cast<AuthorViewer519ELoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_SIN), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_SIN, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer519ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer519ELoopCallback(taskData);
}

// @verified - p
void CleanupAuthorViewer519E(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AUTHOR_VIEWER_519E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_519E_CONVERSATION);
    (void)DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    (void)HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

const ScriptAreaDefinition authorViewer517EScriptDefinition{
    .areaId = AUTHOR_VIEWER_517E_AREA, .field04 = 0, .init = &AuthorViewer517EInit, .main = &AuthorViewer517EMain};
const ScriptAreaDefinition authorViewer519EScriptDefinition{
    .areaId = AUTHOR_VIEWER_519E_AREA, .field04 = 0, .init = &AuthorViewer519EInit, .main = &AuthorViewer519EMain};

} // namespace shenmue::s2
