#include "s2/areas/area_0037.hpp"
#include "s2/character.hpp"
#include "s2/inventory.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_smoke.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/particle_effects.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0037MainCallback(void *taskData);
void E0037LoopCallback(void *taskData);

// @verified - p
std::int16_t e0037SavedWeather;
void E0037Init() {
    e0037SavedWeather = 0;
}

// @verified - p
void E0037Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0037MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0037_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_FreeAuthorPak(1, AREA_0037_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0037_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0037Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0037_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0037_SYE_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0037_SRY_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0037_SRY_, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A115, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A054, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A055, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A092, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A536, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A538, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A057, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A404, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A405, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A403, false);
    EV_SetCharacterAlphaEnabled(AREA_0037_A406, false);
    EV_DisableCharacterDisplay(AREA_0037_KUN_);
}

// @verified - p
void RestoreE0037Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0037_SYE_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0037_SYE_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0037_SRY_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0037_SRY_, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A115, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A054, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A055, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A092, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A536, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A538, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A057, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A404, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A405, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A403, true);
    EV_SetCharacterAlphaEnabled(AREA_0037_A406, true);
    EV_EnableCharacterDisplay(AREA_0037_KUN_);
}

// @verified - p
void CleanupE0037(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0037_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_FreeAuthorPak(0, AREA_0037_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void InitializeE0037FallingLeaves(E0037LoopParameter *child) {
    InitializeWarehouseFallingLeaves(&child->leaves);
}

// @verified - p
void QueueE0037Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0037LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0037LoopParameter *const child = static_cast<E0037LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection != 2)
        EV_LoadAudio("0037A000", 0);
    if (parent->selection == 4)
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    if (parent->selection >= 0 && (parent->selection <= 1 || static_cast<std::uint32_t>(parent->selection - 3) <= 1)) {
        InitializeE0037FallingLeaves(child);
        SetMapModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    SetupE0037Characters();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0037MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0037Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0037(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 4;
        } else if (input == 128 && ++parent->selection > 4)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 4) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0037_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0037LoopCallback(void *taskData) {
    E0037LoopParameter *const child = static_cast<E0037LoopParameter *>(taskData);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->parent->selection == 2) {
        if (frame == 1762) {
            e0037SavedWeather = static_cast<std::int16_t>(GetCurrentWeather());
            if (e0037SavedWeather == 2)
                EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
            SetIsSkySetup(0);
        } else if (frame == 1928) {
            if (e0037SavedWeather == 2) {
                e0037SavedWeather = 0;
                ResetWeatherOverrides();
                EV_FreeWAIR();
                EV_UpdateWeather();
            }
            SetIsSkySetup(1);
        }
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (child->leaves != 0xffffffffU) {
        FreeParticleEffect(3, child->leaves);
        child->leaves = 0xffffffffU;
    }
    if (child->parent->selection == 2) {
        SetIsSkySetup(1);
        HLib::SetupTimedTask(child->parent->self, 100);
        AddInventoryItem(0x0e, 1, 1);
    }
    RestoreE0037Characters();
    static_cast<E0037LoopParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0037ScriptDefinition{.areaId = AREA_0037_AREA, .field04 = 0, .init = E0037Init, .main = E0037Main};

} // namespace shenmue::s2::areas
