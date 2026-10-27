#include "s2/areas/area_kwwt.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
std::uint32_t carriedFire;
std::uint32_t stationaryFire;
void KWWTNativeNullsub() {
}
} // namespace

// @verified - p
void AreaKWWTInit() {
    carriedFire = 0;
    stationaryFire = 0;
}

void AreaKWWTMain(std::uintptr_t argument) {
    shenSetClockAndCompassRenderTo(1);
    KWWTNativeNullsub();
    EV_PTYCUtils(KWWT_SHENHUA, 3, 1);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KWWTAuthorMainCallback, 4, 0x0b, sizeof(KWWTTaskParameter), KWWT_TASK);
    KWWTTaskParameter *const parameter = static_cast<KWWTTaskParameter *>(HLib::GetTaskParameter(task));
    parameter->self = task;
    parameter->state = 1;
    parameter->selection = static_cast<std::int32_t>(argument);
    AssignGenericNpcLoadContext(KWWT_CHARACTERS, task);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(KWWT_LOAD_TASK.name));
    EV_FreeAuthorPak(1, "0541");
    EV_FreeAuthorPak(1, "0529");
}

// @verified - p
void KWWTAuthorMainCallback(void *taskData) {
    KWWTTaskParameter *const parameter = static_cast<KWWTTaskParameter *>(taskData);
    if (parameter->state == 1)
        QueueKWWTAuthorLoop(parameter);
    else if (parameter->state == 3)
        KWWTFinishEvent(parameter);
}

// @verified - p
void QueueKWWTAuthorLoop(KWWTTaskParameter *parameter) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KWWTAuthorLoopCallback, 4, 0x0b, sizeof(KWWTLoopParameter), KWWT_TASK);
    KWWTLoopParameter *const loop = static_cast<KWWTLoopParameter *>(HLib::GetTaskParameter(task));
    loop->parent = parameter;
    loop->currentFrame = 0;
    if (parameter->selection == 3) {
        EV_LoadCutsceneFromCurrentScene("0529A000");
        EV_LoadAudio("0529A000", 0);
        EV_spmBeginSampleExecute();
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    KWWTPrepareAuthorCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(KWWT_AUTHORS[parameter->selection].name));
    parameter->state = 2;
}

// @verified - p
void KWWTAuthorLoopCallback(void *taskData) {
    KWWTLoopParameter *const loop = static_cast<KWWTLoopParameter *>(taskData);
    KWWTTaskParameter *const parent = loop->parent;
    loop->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 3) {
        if (loop->currentFrame == 540)
            AttachFireEffect(carriedFire, KWWT_FIRE_SOURCE, 152, 0.05000000074505806f);
        else if (loop->currentFrame == 830)
            AttachFireInstance(carriedFire, 1, KWWT_FIRE_TARGET, 152, 0x3d4ccccd);
    }
    if (!parent->selection && loop->currentFrame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
        ProcessAicaCommand(0xaa0, 28);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        if (parent->selection == 3) {
            KWWTRestoreAuthorCharacters();
            parent->state = 3;
            HLib::DestroyCurrentTask();
        } else if (!parent->selection) {
            parent->selection = 1;
            parent->state = 1;
            HLib::DestroyCurrentTask();
        } else {
            HLib::SetCurrentTaskTo(KWWTAdvanceAuthorCallback);
            HLib::SetupTaskDelay(30);
        }
    }
}

// @verified - p
void KWWTAdvanceAuthorCallback(void *taskData) {
    KWWTTaskParameter *const parent = static_cast<KWWTLoopParameter *>(taskData)->parent;
    ++parent->selection;
    parent->state = 1;
    if (parent->selection == 3)
        InitializeKWWTFires();
    HLib::DestroyCurrentTask();
}

// @verified - p
void KWWTPrepareAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(KWWT_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(KWWT_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(KWWT_SHENHUA), 6, 2);
    EV_SetCharacterAlphaEnabled(KWWT_SHENHUA, false);
    EV_SetCharacterAlphaEnabled(KWWT_PROP, false);
    EV_EnableCharacterDisplay(KWWT_PROP);
    EV_SetCharacterAlphaEnabled(KWWT_FIRE_SOURCE, false);
    EV_EnableCharacterDisplay(KWWT_FIRE_SOURCE);
    EV_SetCharacterAlphaEnabled(KWWT_FIRE_TARGET, false);
    EV_EnableCharacterDisplay(KWWT_FIRE_TARGET);
}

// @verified - p
void KWWTRestoreAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(KWWT_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(KWWT_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(KWWT_SHENHUA), 6, 0);
    EV_SetCharacterAlphaEnabled(KWWT_SHENHUA, true);
    EV_SetCharacterAlphaEnabled(KWWT_PROP, true);
    EV_DisableCharacterDisplay(KWWT_PROP);
    EV_SetCharacterAlphaEnabled(KWWT_FIRE_SOURCE, true);
    EV_DisableCharacterDisplay(KWWT_FIRE_SOURCE);
    EV_SetCharacterAlphaEnabled(KWWT_FIRE_TARGET, true);
    EV_DisableCharacterDisplay(KWWT_FIRE_TARGET);
}

// @verified - p
void InitializeKWWTFires() {
    Vector3 position{};
    carriedFire = static_cast<std::uint32_t>(CreateFireEffect(position, 0.20000000298023224f, 0.5f, 0, 4));
    SetFirePalette(carriedFire, KWWT_FIRE_PALETTE);
    SetFireFalloff(carriedFire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(carriedFire, -0.23529411852359772f);
    SetParticleEffectSize(4, carriedFire, 0.30000001192092896f, 0.6000000238418579f);
    SuspendParticleEffect(4, carriedFire);
    SetFirePosition(carriedFire, 1, &position);
    position = {66.2300033569336f, 101.5f, 139.13999938964844f};
    stationaryFire = static_cast<std::uint32_t>(CreateFireEffect(position, 0.20000000298023224f, 0.5f, 0, 0));
    SetFirePalette(stationaryFire, KWWT_FIRE_PALETTE);
    SetFireFalloff(stationaryFire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(stationaryFire, -0.23529411852359772f);
    SetParticleEffectSize(4, stationaryFire, 0.30000001192092896f, 0.5f);
    SetParticleEffectPosition(4, stationaryFire, &position, &position);
    SuspendParticleEffect(4, stationaryFire);
}

// @verified - p
void KWWTFinishEvent(KWWTTaskParameter *parameter) {
    KWWTRestoreAuthorCharacters();
    EV_FreeAuthorPak(0, "0529");
    EV_FreeAuthorPak(0, "0541");
    ResumeParticleEffect(4, carriedFire);
    ResumeParticleEffect(4, stationaryFire);
    FreeParticleEffect(4, carriedFire);
    FreeParticleEffect(4, stationaryFire);
    for (const shen_name *name = KWWT_CHARACTERS; name->name != 0xffffffffU; ++name) {
        if (DoesScenarioCharacterExist(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    }
    HLib::DestroyTask(parameter->self);
    EV_scenQueueLoad(g_actNumber, KWWT_RETURN_AREA, 53);
}

const ScriptAreaDefinition areaKWWTScriptDefinition{.areaId = AREA_KWWT, .field04 = 0, .init = AreaKWWTInit, .main = AreaKWWTMain};

} // namespace shenmue::s2::areas
