#include "s2/areas/area_0033.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/load_title.hpp"
namespace shenmue::s2::areas {
namespace {
std::int32_t e0033LoadingFrame;
void E0033LoadingCallback(void *data);
void E0033AlternateLoopCallback(void *data);
void E0033MainCallback(void *data);
void E0033LoopCallback(void *data);
// @verified - p
void E0033Init() {
    e0033LoadingFrame = 0;
}
// @verified - p
void E0033Main(std::uintptr_t argument) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{});
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0033MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0033TaskParameter *const parent = static_cast<E0033TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->nextDay = 0;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, "0033");
    EV_FreeAuthorPak(1, "0192");
    if (g_currentMap.name == AREA_0033_WTA0.name)
        EV_DisableCharacterDisplay(AREA_0033_LIGHT_1);
    else {
        if (static_cast<std::uint32_t>(g_entryPoint - 4) > 4)
            EV_lightLoad(AREA_0033_LIGHT_1);
        else {
            EV_DisableCharacterDisplay(AREA_0033_FIRE);
            EV_SetObjectFlagByIndex(2, 0);
            EV_lightLoad(AREA_0033_LIGHT_0);
        }
        PushGlobalLightingProgram();
    }
    if (g_currentMap.name == AREA_0033_WTA0.name) {
        parent->state = 1;
        parent->selection = *reinterpret_cast<const std::int16_t *>(argument);
        parent->mode = 0;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_0033_SELECTED_CHARACTERS[0], parent->self);
    } else {
        parent->selection = 0;
        parent->state = 0;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_0033_CHARACTERS, parent->self);
    }
}
// @verified - p
void SetupE0033Characters(std::int8_t mode) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0033_A047, false);
    EV_SetCharacterAlphaEnabled(AREA_0033_A044, false);
    EV_SetCharacterAlphaEnabled(AREA_0033_A045, false);
    EV_SetCharacterAlphaEnabled(AREA_0033_A046, false);
    EV_SetCharacterAlphaEnabled(AREA_0033_A353, false);
    if (g_currentMap.name == AREA_0033_AREA.name)
        EV_SetCharacterAlphaEnabled(AREA_0033_A048, false);
    const shen_name *const table = g_currentMap.name == AREA_0033_AREA.name ? AREA_0033_CHARACTERS : AREA_0033_SELECTED_CHARACTERS[mode];
    for (const shen_name *character = table; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0033Characters(std::int8_t mode) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0033_A047, true);
    EV_SetCharacterAlphaEnabled(AREA_0033_A044, true);
    EV_SetCharacterAlphaEnabled(AREA_0033_A045, true);
    EV_SetCharacterAlphaEnabled(AREA_0033_A046, true);
    EV_SetCharacterAlphaEnabled(AREA_0033_A353, true);
    if (g_currentMap.name == AREA_0033_AREA.name)
        EV_SetCharacterAlphaEnabled(AREA_0033_A048, false);
    const shen_name *const table = g_currentMap.name == AREA_0033_AREA.name ? AREA_0033_CHARACTERS : AREA_0033_SELECTED_CHARACTERS[mode];
    for (const shen_name *character = table; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
// @verified - p
void CleanupE0033(E0033TaskParameter *parent) {
    for (const shen_name *character = AREA_0033_SELECTED_CHARACTERS[parent->mode]; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    shenUpdateCurrentDateTime();
    EV_stepSetFlags(0, 39);
    EV_FreeAuthorPak(0, "0033");
    EV_FreeAuthorPak(0, "0192");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0033A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0033_WTA0.name) {
        SetSceneInfos(2, AREA_0033_WTA0, 99);
        StartShowScreenOfId(4);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueE0033Loop(E0033TaskParameter *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(parent->selection > 1 ? E0033AlternateLoopCallback : E0033LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0033LoopParameter *const child = static_cast<E0033LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetupE0033Characters(parent->mode);
    AuthSceneCtrl author;
    if (g_currentMap.name == AREA_0033_WTA0.name) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        if (parent->selection != 0) {
            parent->nextDay = 1;
            author = static_cast<AuthSceneCtrl>(AREA_0033_AUTHOR_192B.name);
        } else
            author = static_cast<AuthSceneCtrl>(AREA_0033_AUTHOR_192A.name);
    } else if (parent->selection != 0) {
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadCutsceneFromCurrentScene("0033A010");
        author = static_cast<AuthSceneCtrl>(parent->selection);
    } else {
        EV_LoadCutsceneFromCurrentScene("0033A000");
        author = static_cast<AuthSceneCtrl>(AREA_0033_AREA.name);
    }
    EV_authSceneCtrl(author);
    parent->state = 2;
}
// @verified - p
void E0033MainCallback(void *data) {
    E0033TaskParameter *const parent = static_cast<E0033TaskParameter *>(data);
    if (parent->state == 1)
        QueueE0033Loop(parent);
    else if (parent->state == 3)
        CleanupE0033(parent);
    else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 8;
        } else if (input == 128 && ++parent->selection > 8)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 8) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0033_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
    }
}
// @verified - p
void E0033LoopCallback(void *data) {
    E0033LoopParameter *const child = static_cast<E0033LoopParameter *>(data);
    E0033TaskParameter *const parent = child->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 2 && g_currentMap.name == AREA_0033_WTA0.name) {
        if (frame == 971) {
            if (parent->nextDay == 0) {
                DateTime time;
                GetCurrentDateTime(time);
                time.hour = 23;
                time.minute = 30;
                SetCurrentDateTimeTo(time);
            }
            Queue_ST_LoadTitleFadeIn(AREA_0033_WTSH);
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            HLib::SetCurrentTaskTo(E0033LoadingCallback);
        } else if (frame == 1100)
            SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    if (parent->selection > 1) {
        if (g_currentMap.name == AREA_0033_WTA0.name) {
            RestoreE0033Characters(parent->mode);
            parent->state = 3;
            HLib::SetupTimedTask(parent->self, 60);
            HLib::DestroyCurrentTask();
            return;
        }
    } else if (g_currentMap.name == AREA_0033_WTA0.name) {
        parent->selection = 2;
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0033_AREA.name));
        EV_LoadCutsceneFromCurrentScene("0033A000");
        return;
    }
    parent->state = 0;
    HLib::DestroyCurrentTask();
}
// @verified - p
void E0033LoadingCallback(void *) {
    if (e0033LoadingFrame == 120) {
        ST_LoadTitleFade();
        ++e0033LoadingFrame;
    } else if (e0033LoadingFrame == 150) {
        EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
        HLib::SetCurrentTaskTo(E0033LoopCallback);
        e0033LoadingFrame = 1;
    } else
        ++e0033LoadingFrame;
}
// @verified - p
void E0033AlternateLoopCallback(void *data) {
    E0033TaskParameter *const parent = static_cast<E0033LoopParameter *>(data)->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0033Characters(parent->mode);
    parent->state = 0;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0033ScriptDefinition{.areaId = AREA_0033_AREA, .field04 = 0, .init = E0033Init, .main = E0033Main};
} // namespace shenmue::s2::areas
