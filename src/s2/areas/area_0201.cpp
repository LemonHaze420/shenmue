#include "s2/debug.hpp"
#include "s2/areas/area_0201.hpp"
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
namespace shenmue::s2::areas {
namespace {
void E0201MainCallback(void *data);
void E0201LoopCallback(void *data);
// @verified - p
void E0201Init() {
}
// @verified - p
void E0201Main(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0201MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0201TaskParameter *const parent = static_cast<E0201TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    if (g_currentMap.name == AREA_0201_AREA.name) {
        parent->state = 0;
        parent->selection = g_entryPoint != 0;
    } else {
        parent->state = 1;
        parent->selection = *reinterpret_cast<const std::int32_t *>(argument) <= 1 ? *reinterpret_cast<const std::int16_t *>(argument) : 0;
    }
    SetPlayerControlEnabled(0);
    SetRenderClockAndCompassTo(1);
    const Vector3 center{110.53f, 92.0f, 147.9f};
    if (npcSetAreaName(AREA_0201_ANP1, &center, 7.0f, 1) == 1)
        EV_debug_log("0201\\e0201_et_0201.c", 247, "NPC_AREA1 : TRUE\n");
    EV_FreeAuthorPak(1, "0201");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0201_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0201Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0201_CLJ_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0201_CLJ_, false);
    if (selection == 1) {
        EV_SetCharacterAlphaEnabled(AREA_0201_A209, false);
        EV_SetCharacterAlphaEnabled(AREA_0201_A207, false);
        EV_SetCharacterAlphaEnabled(AREA_0201_A208, false);
        EV_SetCharacterAlphaEnabled(AREA_0201_A270, false);
        EV_SetCharacterAlphaEnabled(AREA_0201_A326, false);
        EV_SetCharacterAlphaEnabled(AREA_0201_A014, false);
        if (g_currentMap.name == AREA_0201_QRR0.name) {
            EV_DisableCharacterDisplay(AREA_0201_DESK);
            EV_DisableCharacterDisplay(AREA_0201_LAMP);
        }
    } else if (selection == 0)
        EV_SetCharacterAlphaEnabled(AREA_0201_A358, false);
}
// @verified - p
void RestoreE0201Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0201_CLJ_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0201_CLJ_, true);
    if (selection == 1) {
        EV_SetCharacterAlphaEnabled(AREA_0201_A209, true);
        EV_SetCharacterAlphaEnabled(AREA_0201_A207, true);
        EV_SetCharacterAlphaEnabled(AREA_0201_A208, true);
        EV_SetCharacterAlphaEnabled(AREA_0201_A270, true);
        EV_SetCharacterAlphaEnabled(AREA_0201_A326, true);
        EV_SetCharacterAlphaEnabled(AREA_0201_A014, true);
    } else if (selection == 0)
        EV_SetCharacterAlphaEnabled(AREA_0201_A358, true);
}
// @verified - p
void CleanupE0201(E0201TaskParameter *parent) {
    EV_FreeAuthorPak(0, "0201");
    EV_debug_log("0201\\e0201_et_0201.c", 262, "NPC_AREA1 : FREE\n");
    npcSetAreaName(AREA_0201_ANP1, nullptr, 0.0f, 0);
    const char *const package = g_currentMap.name == AREA_0201_AREA.name ? "0201A" : parent->selection == 1 ? "0201A2" : "0201A1";
    if (g_currentMap.name == AREA_0201_AREA.name ||
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0201_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0201_AREA, g_entryPoint);
    else if (parent->selection == 1)
        EV_scenQueueLoad(g_actNumber, AREA_0201_QR00, 97);
    else
        EV_scenQueueLoad(g_actNumber, AREA_0201_QRR0, 1);
}
// @verified - p
void QueueE0201Loop(E0201TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0201LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0201LoopParameter *const child = static_cast<E0201LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (g_currentMap.name == AREA_0201_AREA.name) {
        for (std::int32_t index = 0; index < 4; ++index)
            EV_SetObjectFlagByIndex(index, parent->selection != 0 ? (index == 3) : (index != 3));
        if (parent->selection == 0) {
            EV_SetModelDataDefaultVertexShader();
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        }
    }
    if (parent->selection != 0) {
        EV_SetModelDataDefaultVertexShader();
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(AREA_0201_LIGHT);
        PushGlobalLightingProgram();
        EV_LoadCutsceneFromCurrentScene("0201A010");
    } else {
        EV_LoadCutsceneFromCurrentScene("0201A000");
        EV_NPC_NewLoadSw(1);
        EV_spmBeginSampleExecute();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0201Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0201_AUTHORS[parent->selection].name));
    parent->state = 2;
}
// @verified - p
void E0201MainCallback(void *data) {
    E0201TaskParameter *const parent = static_cast<E0201TaskParameter *>(data);
    if (parent->state == 1)
        QueueE0201Loop(parent);
    else if (parent->state == 3)
        CleanupE0201(parent);
    else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0201_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}
// @verified - p
void E0201LoopCallback(void *data) {
    E0201LoopParameter *const child = static_cast<E0201LoopParameter *>(data);
    E0201TaskParameter *const parent = child->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    parent->state = g_currentMap.name == AREA_0201_AREA.name ? 0 : 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0201Characters(parent->selection);
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0201ScriptDefinition{.areaId = AREA_0201_AREA, .field04 = 0, .init = E0201Init, .main = E0201Main};
} // namespace shenmue::s2::areas
