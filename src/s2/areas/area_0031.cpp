#include "s2/areas/area_0031.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
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

std::array<std::uint32_t, 16> e0031Smoke;
void E0031MainCallback(void *taskData);
void E0031LoopCallback(void *taskData);

// @verified - p
void E0031Init() {
    e0031Smoke.fill(0);
}

// @verified - p
void InitializeE0031Smoke() {
    const Vector3 volume{0.1f, 0.0f, 0.1f};
    const Vector3 velocity{0.0f, 0.016f, 0.0f};
    const Vector3 variance{0.0f, 0.0f, 0.0f};
    e0031Smoke[0] = static_cast<std::uint32_t>(CreateSmokeEffect(AREA_0031_SMOKE_POSITIONS[0], volume, 70, 0.08f, shen_name{0}, 0));
    SetSmokeEffectFadeFactor(e0031Smoke[0], 21);
    SetSmokeEffectFalloff(e0031Smoke[0], 110);
    SetParticleEffectColour(0, e0031Smoke[0], 0, 144, 144, 144);
    SetParticleEffectInitialScale(0, e0031Smoke[0], 0.329f);
    SetParticleEffectSize(0, e0031Smoke[0], 1.56f, 1.41f);
    SetParticleEffectBirthRate(0, e0031Smoke[0], 0.0078f);
    SetParticleEffectVelocity(0, e0031Smoke[0], velocity, variance);
    ResumeParticleEffect(0, e0031Smoke[0]);
    for (std::size_t index = 1; index < 16; ++index) {
        const Vector3 localVolume{0.0f, 0.0f, 0.0f};
        const Vector3 localVelocity{0.0f, 0.0117647061124444f, 0.0f};
        const Vector3 localVariance{0.0f, 0.005098039284348488f, 0.0f};
        e0031Smoke[index] = static_cast<std::uint32_t>(CreateSmokeEffect(AREA_0031_SMOKE_POSITIONS[index], localVolume, 40, 0.06f, shen_name{0}, 0));
        SetSmokeEffectFadeFactor(e0031Smoke[index], 13);
        SetSmokeEffectFalloff(e0031Smoke[index], 188);
        SetParticleEffectColour(0, e0031Smoke[index], 0, 191, 194, 170);
        SetParticleEffectInitialScale(0, e0031Smoke[index], 0.027450980618596077f);
        SetParticleEffectSize(0, e0031Smoke[index], 0.6666666865348816f, 0.3921568691730499f);
        SetParticleEffectBirthRate(0, e0031Smoke[index], 0.0235294122248888f);
        SetParticleEffectVelocity(0, e0031Smoke[index], localVelocity, localVariance);
        ResumeParticleEffect(0, e0031Smoke[index]);
    }
}

// @verified - p
void E0031Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0031MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0031_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0031");
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0031_0000);
    PushGlobalLightingProgram();
    InitializeE0031Smoke();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0031_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0031Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0031_SYE_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0031_SYE_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0031_KNK_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0031_KNK_, false);
}

// @verified - p
void RestoreE0031Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0031_SYE_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0031_SYE_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0031_KNK_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0031_KNK_, true);
}

// @verified - p
void CleanupE0031(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0031_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0031_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(1, AREA_0031_BOSY, 0);
}

// @verified - p
void QueueE0031Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0031LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0031Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 3;
}

// @verified - p
void E0031MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 2) {
        QueueE0031Loop(parent);
    } else if (parent->state == 4) {
        CleanupE0031(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 2;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128 && ++parent->selection > 0)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0031_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0031LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0031Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0031ScriptDefinition{.areaId = AREA_0031_AREA, .field04 = 0, .init = E0031Init, .main = E0031Main};

} // namespace shenmue::s2::areas
