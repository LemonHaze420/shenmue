#include "s2/areas/area_0032.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/inventory.hpp"
#include "s2/particle_effects.hpp"
#include <array>
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

std::array<std::uint32_t, 16> e0032Smoke;
void E0032MainCallback(void *taskData);
void E0032LoopCallback(void *taskData);

// @verified - p
void E0032Init() {
    e0032Smoke.fill(0);
}

// @verified - p
void InitializeE0032Smoke() {
    for (std::size_t index = 0; index < 2; ++index) {
        const Vector3 volume{0.1f, 0.0f, 0.1f};
        const Vector3 velocity{0.0f, 0.016f, 0.0f};
        const Vector3 variance{0.0f, 0.0f, 0.0f};
        e0032Smoke[index] = static_cast<std::uint32_t>(CreateSmokeEffect(AREA_0032_SMOKE_POSITIONS[index], volume, 70, 0.08f, shen_name{0}, 0));
        SetSmokeEffectFadeFactor(e0032Smoke[index], 21);
        SetSmokeEffectFalloff(e0032Smoke[index], 110);
        SetParticleEffectColour(0, e0032Smoke[index], 0, 144, 144, 144);
        SetParticleEffectInitialScale(0, e0032Smoke[index], 0.329f);
        SetParticleEffectSize(0, e0032Smoke[index], 1.56f, 1.41f);
        SetParticleEffectBirthRate(0, e0032Smoke[index], 0.0078f);
        SetParticleEffectVelocity(0, e0032Smoke[index], velocity, variance);
        ResumeParticleEffect(0, e0032Smoke[index]);
    }
}

// @verified - p
void E0032Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0032MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0032_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0032");
    InitializeE0032Smoke();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0032_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0032Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0032_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0032_SYE_, false);
    EV_SetCharacterAlphaEnabled(AREA_0032_A043, false);
    LoadBlurModelForCharacter(AREA_AUTHOR_RYO, "RYO_B.MT7", 1.0f);
    LoadBlurModelForCharacter(AREA_0032_SYE_, "SYE_B.MT7", 1.0f);
}

// @verified - p
void RestoreE0032Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0032_SYE_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0032_SYE_, true);
    EV_SetCharacterAlphaEnabled(AREA_0032_A043, true);
    figpAssignModeFor(AREA_AUTHOR_RYO, 0);
    figpAssignModeFor(AREA_0032_SYE_, 0);
}

// @verified - p
void CleanupE0032(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0032_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0032_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0032Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0032LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    if (parent->selection <= 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0032Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0032MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0032Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0032(parent);
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
            EV_debug_log_to_screen(AREA_0032_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0032LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0032Characters();
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent;
    if (parent->selection == 2) {
        AddInventoryItem(0x1782, 1, 1);
        HLib::SetupTimedTask(parent->self, 130);
    }
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0032ScriptDefinition{.areaId = AREA_0032_AREA, .field04 = 0, .init = E0032Init, .main = E0032Main};

} // namespace shenmue::s2::areas
