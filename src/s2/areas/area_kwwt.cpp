#include "s2/areas/area_kwwt.hpp"
#if !defined(SHENMUE_EVENT_OVERLAY)
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
#endif
#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#define KWWT_INLINE
#define KWWT_LOCAL
typedef shen_name KWWTCharacterName;
void FreeKWWTAuthors(void);
#else
namespace shenmue::s2::areas {
#define KWWT_LOCAL static
#if defined(_MSC_VER)
#define KWWT_INLINE __forceinline
#else
#define KWWT_INLINE inline __attribute__((always_inline))
#endif
typedef const shen_name &KWWTCharacterName;
#endif
KWWT_LOCAL KWWT_INLINE void PrepareKWWTCharacter(KWWTCharacterName character);
KWWT_LOCAL KWWT_INLINE void RestoreKWWTCharacter(KWWTCharacterName character);
KWWT_LOCAL KWWT_INLINE void PrepareKWWTProp(KWWTCharacterName character);
KWWT_LOCAL KWWT_INLINE void RestoreKWWTProp(KWWTCharacterName character);

#if !defined(SHENMUE_EVENT_ENTRY)
#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif
static unsigned int carriedFire;
static unsigned int stationaryFire;
#if !defined(SHENMUE_EVENT_OVERLAY)
void KWWTNativeNullsub() {
}
} // namespace

// @verified - p
void AreaKWWTInit() {
    carriedFire = 0;
    stationaryFire = 0;
}
#endif
#endif

#if !defined(SHENMUE_EVENT_OVERLAY) || defined(SHENMUE_EVENT_ENTRY)
void AreaKWWTMain(
#if defined(SHENMUE_EVENT_OVERLAY)
    int argument
#else
    std::uintptr_t argument
#endif
) {
    HLibTask *task;
    KWWTTaskParameter *parameter;
    FTBL.SetRenderClockAndCompassTo(1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetLoadingBusy();
#else
    KWWTNativeNullsub();
#endif
    FTBL.PTYCUtils(KWWT_SHENHUA, 3, 1);
    task = FTBL.HLib_EnqueueTaskWithParameter(KWWTAuthorMainCallback, 4, 0x0b, sizeof(KWWTTaskParameter), KWWT_TASK);
    parameter = (KWWTTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parameter->self = task;
    parameter->state = 1;
    parameter->selection = (int)argument;
#if defined(SHENMUE_EVENT_OVERLAY)
    Queue_EVT_UTL_LoadNPCChara_3(KWWT_CHARACTERS, parameter->self);
    FreeKWWTAuthors();
#else
    AssignGenericNpcLoadContext(KWWT_CHARACTERS, task);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(KWWT_LOAD_TASK.name));
    EV_FreeAuthorPak(1, "0541");
    EV_FreeAuthorPak(1, "0529");
#endif
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - p
void KWWTAuthorMainCallback(void *taskData) {
    KWWTTaskParameter *const parameter = (KWWTTaskParameter *)taskData;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parameter->state) {
    case 1:
        QueueKWWTAuthorLoop(parameter);
        break;
    case 3:
        KWWTFinishEvent(parameter);
        break;
    }
#else
    if (parameter->state == 1)
        QueueKWWTAuthorLoop(parameter);
    else if (parameter->state == 3)
        KWWTFinishEvent(parameter);
#endif
}

// @verified - p
void QueueKWWTAuthorLoop(KWWTTaskParameter *parameter) {
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithParameter(KWWTAuthorLoopCallback, 4, 0x0b, sizeof(KWWTLoopParameter), KWWT_TASK);
    KWWTLoopParameter *const loop = (KWWTLoopParameter *)FTBL.HLib_GetTaskParameter(task);
    loop->parent = parameter;
    loop->currentFrame = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parameter->selection) {
    case 1:
    case 2:
        break;
    case 3:
        FTBL.EV_LoadCutsceneFromCurrentScene("0529A000");
        FTBL.EV_LoadAudio("0529A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_authSceneCtrl(-7);
        break;
    }
#else
    if (parameter->selection == 3) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0529A000");
        FTBL.EV_LoadAudio("0529A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.EV_authSceneCtrl(-7);
    }
#endif
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    KWWTPrepareAuthorCharacters();
    FTBL.EV_authSceneCtrl(KWWT_AUTHORS[parameter->selection]);
    parameter->state = 2;
}

// @verified - p
void KWWTAuthorLoopCallback(void *taskData) {
    KWWTLoopParameter *const loop = (KWWTLoopParameter *)taskData;
#if defined(SHENMUE_EVENT_OVERLAY)
#define KWWT_PARENT (loop->parent)
#else
    KWWTTaskParameter *const parent = loop->parent;
#define KWWT_PARENT parent
#endif
    loop->currentFrame = (short)FTBL.EV_authSceneCtrl(-6);
    if (KWWT_PARENT->selection == 3) {
#if defined(SHENMUE_EVENT_OVERLAY)
        switch (loop->currentFrame) {
        case 540:
            FTBL.AttachFireEffect(carriedFire, KWWT_FIRE_SOURCE, 152, 0.05f);
            break;
        case 830:
            FTBL.AttachFireInstance(carriedFire, 1, KWWT_FIRE_TARGET, 152, 0.05f);
            break;
        }
#else
        if (loop->currentFrame == 540)
            FTBL.AttachFireEffect(carriedFire, KWWT_FIRE_SOURCE, 152, 0.05000000074505806f);
        else if (loop->currentFrame == 830)
            AttachFireInstance(carriedFire, 1, KWWT_FIRE_TARGET, 152, 0x3d4ccccd);
#endif
    }
    if (!KWWT_PARENT->selection && loop->currentFrame == FTBL.EV_authSceneCtrl(-5) - 30)
        FTBL.ProcessAicaCommand(0xaa0, 28);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        if (KWWT_PARENT->selection == 3) {
            KWWTRestoreAuthorCharacters();
            KWWT_PARENT->state = 3;
            FTBL.HLib_DestroyCurrentEventTask();
        } else if (!KWWT_PARENT->selection) {
            KWWT_PARENT->selection = 1;
            KWWT_PARENT->state = 1;
            FTBL.HLib_DestroyCurrentEventTask();
        } else {
            FTBL.HLib_SetCurrentTaskTo(KWWTAdvanceAuthorCallback);
            FTBL.HLib_SetupTaskDelay(30);
        }
    }
#undef KWWT_PARENT
}

// @verified - p
void KWWTAdvanceAuthorCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    KWWTLoopParameter *const loop = (KWWTLoopParameter *)taskData;
#define KWWT_PARENT (loop->parent)
#else
    KWWTTaskParameter *const parent = static_cast<KWWTLoopParameter *>(taskData)->parent;
#define KWWT_PARENT parent
#endif
    ++KWWT_PARENT->selection;
    KWWT_PARENT->state = 1;
    if (KWWT_PARENT->selection == 3)
        InitializeKWWTFires();
    FTBL.HLib_DestroyCurrentEventTask();
#undef KWWT_PARENT
}

#if defined(SHENMUE_EVENT_OVERLAY)
void FreeKWWTAuthors(void) {
    FTBL.EV_FreeAuthorPak(1, "0541");
    FTBL.EV_FreeAuthorPak(1, "0529");
}
#endif

// @verified - p
void InitializeKWWTFires(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {0, 0, 0};
#else
    Vector3 position{};
#endif
    carriedFire = (unsigned int)FTBL.CreateFireEffect(&position, 0.20000000298023224f, 0.5f, 0, 4);
    FTBL.SetFirePalette(carriedFire, KWWT_FIRE_PALETTE);
    FTBL.SetFireFalloff(carriedFire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    FTBL.SetFireFalloffScalar(carriedFire, -0.23529411852359772f);
    FTBL.SetParticleEffectSize(4, carriedFire, 0.30000001192092896f, 0.6000000238418579f);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.ResumeParticleEffect(4, carriedFire);
#else
    FTBL.SuspendParticleEffect(4, carriedFire);
#endif
    FTBL.SetFirePosition(carriedFire, 1, &position);
#if defined(SHENMUE_EVENT_OVERLAY)
    position.x = 66.2300033569336f;
    position.y = 101.5f;
    position.z = 139.13999938964844f;
#else
    position = {66.2300033569336f, 101.5f, 139.13999938964844f};
#endif
    stationaryFire = (unsigned int)FTBL.CreateFireEffect(&position, 0.20000000298023224f, 0.5f, 0, 0);
    FTBL.SetFirePalette(stationaryFire, KWWT_FIRE_PALETTE);
    FTBL.SetFireFalloff(stationaryFire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    FTBL.SetFireFalloffScalar(stationaryFire, -0.23529411852359772f);
    FTBL.SetParticleEffectSize(4, stationaryFire, 0.30000001192092896f, 0.5f);
    FTBL.SetParticleEffectPosition(4, stationaryFire, &position, &position);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.ResumeParticleEffect(4, stationaryFire);
#else
    FTBL.SuspendParticleEffect(4, stationaryFire);
#endif
}

// @verified - p
void KWWTFinishEvent(KWWTTaskParameter *parameter) {
    KWWTRestoreAuthorCharacters();
    FTBL.EV_FreeAuthorPak(0, "0529");
    FTBL.EV_FreeAuthorPak(0, "0541");
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SuspendParticleEffect(4, carriedFire);
    FTBL.SuspendParticleEffect(4, stationaryFire);
#else
    FTBL.ResumeParticleEffect(4, carriedFire);
    FTBL.ResumeParticleEffect(4, stationaryFire);
#endif
    FTBL.FreeParticleEffect(4, carriedFire);
    FTBL.FreeParticleEffect(4, stationaryFire);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(KWWT_CHARACTERS);
#else
    for (const shen_name *name = KWWT_CHARACTERS; name->name != 0xffffffffU; ++name) {
        if (DoesScenarioCharacterExist(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    }
#endif
    FTBL.HLib_DestroyTask(parameter->self);
    FTBL.EV_scenQueueLoad(g_actNumber, KWWT_RETURN_AREA, 53);
}

// @verified - p
void KWWTPrepareAuthorCharacters(void) {
    PrepareKWWTCharacter(KWWT_RYO);
    PrepareKWWTCharacter(KWWT_SHENHUA);
    PrepareKWWTProp(KWWT_PROP);
    PrepareKWWTProp(KWWT_FIRE_SOURCE);
    PrepareKWWTProp(KWWT_FIRE_TARGET);
}

// @verified - p
void KWWTRestoreAuthorCharacters(void) {
    RestoreKWWTCharacter(KWWT_RYO);
    RestoreKWWTCharacter(KWWT_SHENHUA);
    RestoreKWWTProp(KWWT_PROP);
    RestoreKWWTProp(KWWT_FIRE_SOURCE);
    RestoreKWWTProp(KWWT_FIRE_TARGET);
}

KWWT_LOCAL KWWT_INLINE void PrepareKWWTCharacter(KWWTCharacterName character) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}
KWWT_LOCAL KWWT_INLINE void RestoreKWWTCharacter(KWWTCharacterName character) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}
KWWT_LOCAL KWWT_INLINE void PrepareKWWTProp(KWWTCharacterName character) {
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    FTBL.EV_EnableCharacterDisplay(character);
}
KWWT_LOCAL KWWT_INLINE void RestoreKWWTProp(KWWTCharacterName character) {
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    FTBL.EV_DisableCharacterDisplay(character);
}

#if defined(SHENMUE_EVENT_OVERLAY)
const shen_name KWWT_CHARACTERS[] = {KWWT_SHENHUA, 0xffffffff};
const unsigned short KWWT_FIRE_PALETTE[] = {
#include "area_kwwt_palette.inc"
};
const shen_name KWWT_AUTHORS[] = {
    SHEN_NAME4('A', 'T', '0', '0'), SHEN_NAME4('A', 'T', '0', '1'), SHEN_NAME4('A', 'T', '0', '2'), SHEN_NAME4('A', 'T', '0', '3')};
#else
const ScriptAreaDefinition areaKWWTScriptDefinition{.areaId = AREA_KWWT, .field04 = 0, .init = AreaKWWTInit, .main = AreaKWWTMain};
#endif
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace shenmue::s2::areas
#endif
