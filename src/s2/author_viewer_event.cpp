#include "s2/author_viewer_event.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>
#include <cstdarg>
#include <cstring>

namespace shenmue::s2 {

void debug_log_to_screen(const char *, std::va_list) {
        #ifdef SHENMUE_DEBUG

        #else
    (void)0;
        #endif
}

void set_debug_text_pos(std::int32_t, std::int32_t) {
        #ifdef SHENMUE_DEBUG

        #else
    (void)0;
        #endif
}

// @verified - p
void EV_set_debug_text_pos(std::int32_t column, std::int32_t row) {
    set_debug_text_pos(column, row);
}

// @verified - p
void EV_debug_log_to_screen(const char *szFormat, ...) {
    std::va_list arguments;
    va_start(arguments, szFormat);
    debug_log_to_screen(szFormat, arguments);
    va_end(arguments);
}

// @verified - p
void AuthorViewer332EInit() {
}

// @verified - p
void AuthorViewer332EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer332EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_332E_MOTION));
    params->selection = 0;
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_332E_CONVERSATION);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
}

// @verified - p
void AuthorViewer332EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 11;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 11)
                params->selection = 0;
        }
        switch (params->selection) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_332E_DEBUG_NAMES[params->selection]);
            break;
        default:
            break;
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer332ELoop(params);
        break;
    case 2:
        break;
    case 3:
        (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_332E_CONVERSATION);
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer332ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewerLoopTaskCallback, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
void AuthorViewer503EInit() {
}

// @verified - p
void AuthorViewer503EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer503EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_503E_MOTION));
    params->selection = 0;
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
}

// @verified - p
void AuthorViewer503EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 0;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 0)
                params->selection = 0;
        }
        if (params->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0503E020.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer503ELoop(params);
        break;
    case 2:
        break;
    case 3:
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer503ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewerLoopTaskCallback, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t AuthorViewerLoopCallback(void *taskData) {
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewerLoopTaskCallback(void *taskData) {
    (void)AuthorViewerLoopCallback(taskData);
}
// @verified - p
void AuthorViewer081EInit() {
}

// @verified - p
void AuthorViewer081EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer081EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_081E_MOTION));
    params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint));
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_081E_CONVERSATION);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    if (g_entryPoint == 99) {
        const std::uint32_t selectionAndState = 0;
        std::memcpy(&params->selection, &selectionAndState, sizeof(selectionAndState));
    } else {
        params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(g_entryPoint));
        params->state = 1;
        if (params->selection > 1) {
            (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_081E_CONVERSATION);
            (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
            EV_CleanupMapControlTask();
            (void)HLib::DestroyTask(params->self);
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        }
    }
}

// @verified - p
void AuthorViewer081EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 1;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 1)
                params->selection = 0;
        }
        if (params->selection == 0 || params->selection == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_081E_DEBUG_NAMES[params->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer081ELoop(params);
        break;
    case 2:
        break;
    case 3:
        (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_081E_CONVERSATION);
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer081ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer081ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer081ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer081ELoopTaskParameter *const params = static_cast<AuthorViewer081ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    params->currentFrame = 0;
    SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t AuthorViewer081ELoopCallback(void *taskData) {
    AuthorViewer081ELoopTaskParameter *const params = static_cast<AuthorViewer081ELoopTaskParameter *>(taskData);
    params->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (params->currentFrame == 0) {
        EV_EnableCharacterDisplay(AUTHOR_VIEWER_RYO);
        (void)EV_SetCharaControllerFlags(AUTHOR_VIEWER_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
        (void)EV_SetCharaControllerFlags(AUTHOR_VIEWER_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = g_entryPoint == 99 ? 0 : 3;
        EV_EnableCharacterDisplay(AUTHOR_VIEWER_RYO);
        (void)EV_SetCharaControllerFlags(AUTHOR_VIEWER_RYO, UpdateHeight, NpcCharacterControllerState::Enabled);
        (void)EV_SetCharaControllerFlags(AUTHOR_VIEWER_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        (void)EV_SetCharaControllerFlags(AUTHOR_VIEWER_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer081ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer081ELoopCallback(taskData);
}

// @verified - p
void AuthorViewer182EInit() {
}

// @verified - p
std::int64_t AuthorViewer182EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer182EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_182E_MOTION));
    params->selection = 0;
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_182E_CONVERSATION);
    return EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
}

void AuthorViewer182EAreaMain(std::uintptr_t argument) {
    (void)AuthorViewer182EMain(argument);
}

// @verified - p
void AuthorViewer182EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 0;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 0)
                params->selection = 0;
        }
        if (params->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0182E020.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer182ELoop(params);
        break;
    case 2:
        break;
    case 3:
        (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_182E_CONVERSATION);
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer182ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer182ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer182ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer182ELoopTaskParameter *const params = static_cast<AuthorViewer182ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_182E_SECOND_CHARACTER, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t AuthorViewer182ELoopCallback(void *taskData) {
    AuthorViewer182ELoopTaskParameter *const params = static_cast<AuthorViewer182ELoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_182E_SECOND_CHARACTER, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer182ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer182ELoopCallback(taskData);
}

// @verified - p
void AuthorViewer015EInit() {
}

// @verified - p
void AuthorViewer015EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer015EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_015E_MOTION));
    params->selection = 0;
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_015E_CONVERSATION);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
}

// @verified - p
void AuthorViewer015EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 1;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 1)
                params->selection = 0;
        }
        if (params->selection == 0 || params->selection == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_015E_DEBUG_NAMES[params->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer015ELoop(params);
        break;
    case 2:
        break;
    case 3:
        (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_015E_CONVERSATION);
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer015ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer015ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer015ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer015ELoopTaskParameter *const params = static_cast<AuthorViewer015ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    params->currentFrame = 0;
    if (parent->selection == 0)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t AuthorViewer015ELoopCallback(void *taskData) {
    AuthorViewer015ELoopTaskParameter *const params = static_cast<AuthorViewer015ELoopTaskParameter *>(taskData);
    params->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (params->currentFrame == 0)
        DisableAuthorViewer015ECharacters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        RestoreAuthorViewer015ECharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer015ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer015ELoopCallback(taskData);
}

// @verified - p
void DisableAuthorViewer015ECharacters() {
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    for (const shen_name character : AUTHOR_VIEWER_015E_CHARACTERS)
        EV_SetCharacterAlphaEnabled(character, false);
}

// @verified - p
void RestoreAuthorViewer015ECharacters() {
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
    for (const shen_name character : AUTHOR_VIEWER_015E_CHARACTERS)
        EV_SetCharacterAlphaEnabled(character, true);
}

// @verified - p
void AuthorViewer295EInit() {
}

// @verified - p
HLib::Task *AuthorViewer295EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer295EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    params->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_295E_MOTION));
    params->selection = 0;
    SetupAuthorViewer295E();
    return Queue_EVT_UTL_LoadNPCChara_3(AUTHOR_VIEWER_295E_CHARACTERS, params->self);
}

void AuthorViewer295EAreaMain(std::uintptr_t argument) {
    (void)AuthorViewer295EMain(argument);
}

// @verified - p
void AuthorViewer295EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 0;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 0)
                params->selection = 0;
        }
        if (params->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_295E_DEBUG_NAME);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer295ELoop(params);
        break;
    case 2:
        break;
    case 3:
        CleanupAuthorViewer295E(params);
        break;
    default:
        return;
    }
}

// @verified - p
void SetupAuthorViewer295E() {
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_295E_CONVERSATION);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    const Vector3 position = AUTHOR_VIEWER_295E_ROOT_POSITION;
    (void)SetLightRootPosition(position);
    for (std::int32_t index = 0; index < 9; ++index)
        (void)SetMapModelSlotTransform(index, &position, 0);
}

// @verified - p
void CleanupAuthorViewer295E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AUTHOR_VIEWER_295E_CHARACTERS; character->name != AUTHOR_VIEWER_295E_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_295E_CONVERSATION);
    (void)DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    (void)HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t QueueAuthorViewer295ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer295ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewer295ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer295ELoopTaskParameter *const params = static_cast<AuthorViewer295ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_295E_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_295E_REN, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t AuthorViewer295ELoopCallback(void *taskData) {
    AuthorViewer295ELoopTaskParameter *const params = static_cast<AuthorViewer295ELoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_295E_REN), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_295E_REN, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer295ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer295ELoopCallback(taskData);
}

// @verified - p
void AuthorViewer035EInit() {
}

// @verified - p
HLib::Task *AuthorViewer035EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer035EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_035E_MOTION));
    const std::uint32_t zero = 0;
    std::memcpy(reinterpret_cast<unsigned char *>(params) + offsetof(AuthorViewerTaskParameter, selection), &zero, sizeof(zero));
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_035E_CONVERSATION);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    return Queue_EVT_UTL_LoadNPCChara_3(AUTHOR_VIEWER_035E_CHARACTERS, params->self);
}

void AuthorViewer035EAreaMain(std::uintptr_t argument) {
    (void)AuthorViewer035EMain(argument);
}

// @verified - p
void AuthorViewer035EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 1;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 1)
                params->selection = 0;
        }
        if (params->selection == 0 || params->selection == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_035E_DEBUG_NAMES[params->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer035ELoop(params);
        break;
    case 2:
        break;
    case 3:
        CleanupAuthorViewer035E(params);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer035ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer035ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_035E_SAI, false);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t AuthorViewer035ELoopCallback(void *taskData) {
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_035E_SAI, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer035ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer035ELoopCallback(taskData);
}

// @verified - p
void CleanupAuthorViewer035E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AUTHOR_VIEWER_035E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_035E_CONVERSATION);
    (void)DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    (void)HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void AuthorViewer061EInit() {
}

// @verified - p
HLib::Task *AuthorViewer061EMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer061EMainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->state = 0;
    const std::uint32_t motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_061E_MOTION));
    params->selection = 0;
    params->motion = motion;
    SetPlayerControlEnabled(0);
    (void)EV_FreeAuthorPak(1, AUTHOR_VIEWER_061E_CONVERSATION);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    return Queue_EVT_UTL_LoadNPCChara_3(AUTHOR_VIEWER_061E_CHARACTERS, params->self);
}

void AuthorViewer061EAreaMain(std::uintptr_t argument) {
    (void)AuthorViewer061EMain(argument);
}

// @verified - p
void AuthorViewer061EMainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 0;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 0)
                params->selection = 0;
        }
        if (params->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_061E_DEBUG_NAME);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        (void)QueueAuthorViewer061ELoop(params);
        break;
    case 2:
        break;
    case 3:
        CleanupAuthorViewer061E(params);
        break;
    default:
        return;
    }
}

// @verified - p
std::int64_t QueueAuthorViewer061ELoop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AuthorViewer061ELoopTaskCallback, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_061E_SAM), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_061E_SAM, false);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_061E_KEIB, false);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t AuthorViewer061ELoopCallback(void *taskData) {
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(taskData);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        params->parent->state = 0;
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
        (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_061E_SAM), 6, 0);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_061E_SAM, true);
        EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_061E_KEIB, true);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void AuthorViewer061ELoopTaskCallback(void *taskData) {
    (void)AuthorViewer061ELoopCallback(taskData);
}

// @verified - p
void CleanupAuthorViewer061E(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AUTHOR_VIEWER_061E_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            (void)EV_npcLockUnlock(*character);
            (void)EV_npcFree(*character);
        }
    }
    (void)EV_FreeAuthorPak(0, AUTHOR_VIEWER_061E_CONVERSATION);
    (void)DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    (void)HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void AuthorViewer0006Init() {
}

// @verified - p
void AuthorViewer0006Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer0006MainCallback, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    params->self = task;
    params->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AUTHOR_VIEWER_0006_MOTION));
    params->selection = 0;
    params->state = 0;
    SetPlayerControlEnabled(0);
    (void)EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
}

// @verified - p
void AuthorViewer0006MainCallback(void *taskData) {
    AuthorViewerTaskParameter *const params = static_cast<AuthorViewerTaskParameter *>(taskData);
    switch (params->state) {
    case 0: {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            params->state = 1;
        } else if (input == 64) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) - 1U));
            if (params->selection < 0)
                params->selection = 5;
        } else if (input == 128) {
            params->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(params->selection) + 1U));
            if (params->selection > 5)
                params->selection = 0;
        }
        if (params->selection >= 0 && params->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AUTHOR_VIEWER_0006_DEBUG_NAMES[params->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(params->selection));
        break;
    }
    case 1:
        QueueAuthorViewer0006Loop(params);
        break;
    case 3:
        (void)DeleteMotion(std::bit_cast<std::int32_t>(params->motion));
        EV_CleanupMapControlTask();
        (void)HLib::DestroyTask(params->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        break;
    default:
        break;
    }
}

// @verified - p
void QueueAuthorViewer0006Loop(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorViewer0006LoopCallback, 4, 0x0b, sizeof(AuthorViewerLoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(HLib::GetTaskParameter(task));
    params->parent = parent;
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    EV_SetModelDataDefaultVertexShader();
    if (dateTime.hour < 19) {
        (void)EV_SetObjectFlagByIndex(5, 0);
        (void)EV_SetObjectFlagByIndex(6, 1);
        EV_lightLoad(AUTHOR_VIEWER_0006_DAY_LIGHT);
        AssignLightProgram(3);
        for (const std::int32_t slot : AUTHOR_VIEWER_0006_LIGHT_SLOTS)
            EV_AssignLightParams(1, slot);
    } else {
        (void)EV_SetObjectFlagByIndex(5, 1);
        (void)EV_SetObjectFlagByIndex(6, 0);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
    }
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, false);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_015E_A016, false);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_0006_A298, false);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int64_t>(parent->selection)));
    parent->state = 2;
}

// @verified - p
void AuthorViewer0006LoopCallback(void *taskData) {
    AuthorViewerLoopTaskParameter *const params = static_cast<AuthorViewerLoopTaskParameter *>(taskData);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    params->parent->state = 0;
    (void)mtwkSetExtraFlag(scenFindMotionwork(AUTHOR_VIEWER_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_RYO, true);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_015E_A016, true);
    EV_SetCharacterAlphaEnabled(AUTHOR_VIEWER_0006_A298, true);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    (void)HLib::DestroyCurrentTask();
}

const ScriptAreaDefinition authorViewer0006ScriptDefinition{
    .areaId = AUTHOR_VIEWER_0006_AREA, .field04 = 0, .init = &AuthorViewer0006Init, .main = &AuthorViewer0006Main};

const ScriptAreaDefinition authorViewer332EScriptDefinition{
    .areaId = AUTHOR_VIEWER_332E_AREA, .field04 = 0, .init = &AuthorViewer332EInit, .main = &AuthorViewer332EMain};
const ScriptAreaDefinition authorViewer503EScriptDefinition{
    .areaId = AUTHOR_VIEWER_503E_AREA, .field04 = 0, .init = &AuthorViewer503EInit, .main = &AuthorViewer503EMain};
const ScriptAreaDefinition authorViewer081EScriptDefinition{
    .areaId = AUTHOR_VIEWER_081E_AREA, .field04 = 0, .init = &AuthorViewer081EInit, .main = &AuthorViewer081EMain};
const ScriptAreaDefinition authorViewer182EScriptDefinition{
    .areaId = AUTHOR_VIEWER_182E_AREA, .field04 = 0, .init = &AuthorViewer182EInit, .main = &AuthorViewer182EAreaMain};
const ScriptAreaDefinition authorViewer015EScriptDefinition{
    .areaId = AUTHOR_VIEWER_015E_AREA, .field04 = 0, .init = &AuthorViewer015EInit, .main = &AuthorViewer015EMain};
const ScriptAreaDefinition authorViewer295EScriptDefinition{
    .areaId = AUTHOR_VIEWER_295E_AREA, .field04 = 0, .init = &AuthorViewer295EInit, .main = &AuthorViewer295EAreaMain};
const ScriptAreaDefinition authorViewer035EScriptDefinition{
    .areaId = AUTHOR_VIEWER_035E_AREA, .field04 = 0, .init = &AuthorViewer035EInit, .main = &AuthorViewer035EAreaMain};
const ScriptAreaDefinition authorViewer061EScriptDefinition{
    .areaId = AUTHOR_VIEWER_061E_AREA, .field04 = 0, .init = &AuthorViewer061EInit, .main = &AuthorViewer061EAreaMain};

} // namespace shenmue::s2
