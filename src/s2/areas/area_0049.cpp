#include "s2/areas/area_0049.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/character.hpp"
#include "s2/pak.hpp"
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
#include "s2/blur_figure_pose.hpp"
#include "s2/door.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
#define E0049_INLINE
#elif defined(_MSC_VER)
#define E0049_INLINE __forceinline
#else
#define E0049_INLINE inline __attribute__((always_inline))
#endif

void E0049MainCallback(void *taskData);
void E0049LoopCallback(void *taskData);
E0049_INLINE void ConfigureE0049Environment(void);
E0049_INLINE void InitializeE0049FallingLeaves(void);
E0049_INLINE void CleanupE0049(AreaImmediateAuthorTaskParameter *parent);
E0049_INLINE void SetupE0049Characters(void);

E0049_INLINE void QueueE0049Loop(AreaImmediateAuthorTaskParameter *parent);
E0049_INLINE void RestoreE0049Characters(void);
#if defined(SHENMUE_EVENT_OVERLAY)
typedef shen_name E0049CharacterName;
#else
typedef const shen_name &E0049CharacterName;
#endif
E0049_INLINE void PrepareE0049Character(E0049CharacterName name);
E0049_INLINE void RestoreE0049Character(E0049CharacterName name);
E0049_INLINE void HideE0049Prop(E0049CharacterName name);
E0049_INLINE void ShowE0049Prop(E0049CharacterName name);

#if !defined(SHENMUE_EVENT_ENTRY)
#if defined(SHENMUE_EVENT_OVERLAY)
shen_name AREA_0049_CHARACTERS[] = {AREA_0049_SYE_, AREA_AUTHOR_INVALID};
static unsigned int e0049Leaves;
#else
std::uint32_t e0049Leaves;
// @verified - p
void E0049Init() {
    e0049Leaves = 0;
}
#endif
#endif

#if defined(SHENMUE_EVENT_ENTRY) || !defined(SHENMUE_EVENT_OVERLAY)
#if defined(SHENMUE_EVENT_OVERLAY)
void E0049Main(void)
#else
// @verified - p
void E0049Main(std::uintptr_t)
#endif
{
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithParameter(E0049MainCallback, 4, 0x0b, sizeof(AreaImmediateAuthorTaskParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
#if defined(SHENMUE_EVENT_OVERLAY)
    parent->state = 1;
    parent->selection = 0;
    InitializeE0049FallingLeaves();
#else
    parent->selection = 0;
    parent->state = 1;
#endif
    ConfigureE0049Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0049_CHARACTERS, parent->self);
}
#endif

#if !defined(SHENMUE_EVENT_ENTRY)
// @verified - p
void E0049MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = (AreaImmediateAuthorTaskParameter *)taskData;
#if defined(SHENMUE_EVENT_OVERLAY)
    switch (parent->state) {
    case 1:
        QueueE0049Loop(parent);
        break;
    case 3:
        CleanupE0049(parent);
        break;
    }
#else
    if (parent->state == 1)
        QueueE0049Loop(parent);
    else if (parent->state == 3)
        CleanupE0049(parent);
#endif
}

E0049_INLINE void QueueE0049Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLibTask *const task = FTBL.HLib_EnqueueTaskWithParameter(E0049LoopCallback, 4, 0x0b, sizeof(AreaImmediateAuthorFrameParameter), AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *const child = (AreaImmediateAuthorFrameParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->state = 0;
    FTBL.EV_spmBeginSampleExecute(0);
    SetupE0049Characters();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_authSceneCtrl(AREA_0049_AREA);
#else
    FTBL.EV_authSceneCtrl(static_cast<std::int32_t>(AREA_0049_AREA.name));
#endif
    parent->state = 2;
}

// @verified - p
void E0049LoopCallback(void *taskData) {
    if ((unsigned int)FTBL.EV_authSceneCtrl(-1) != 0)
        return;
    FTBL.HLib_SetupTimedTask(((AreaImmediateAuthorFrameParameter *)taskData)->parent->self, 5);
    RestoreE0049Characters();
    ((AreaImmediateAuthorFrameParameter *)taskData)->parent->state = 3;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
}

// @verified - p
E0049_INLINE void InitializeE0049FallingLeaves(void) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {332.8f, 130.34f, 231.26f}, volume = {1.3f, 0.02f, 1.3f};
    Vector3 velocity = {0.0f, -0.03f, 0.0f}, variance = {0.002f, -0.005f, 0.002f};
    e0049Leaves = FTBL.CreateFallingParticleEffect(&position, &volume, 50, 0.5f, 0x400, "misc", "REDLEAF.pvr", 126.4f);
    FTBL.SetParticleEffectVelocity(3, e0049Leaves, &velocity, &variance);
    FTBL.SetParticleEffectSize(3, e0049Leaves, 0.03f, 0.09f);
    FTBL.ResumeParticleEffect(3, e0049Leaves);
#else
    InitializeWarehouseFallingLeaves(&e0049Leaves);
#endif
}

// @verified - p
E0049_INLINE void ConfigureE0049Environment(void) {
#if !defined(SHENMUE_EVENT_OVERLAY)
    InitializeE0049FallingLeaves();
#endif
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0049A000");
    FTBL.EV_FreeAuthorPak(1, AREA_0049_CONVERSATION);
}

// @verified - p
E0049_INLINE void CleanupE0049(AreaImmediateAuthorTaskParameter *parent) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {335.6f, 127.0f, 280.1f};
    Vector3i rotation = {0, 17021, 0};
#endif
    FTBL.FreeParticleEffect(3, e0049Leaves);
#if defined(SHENMUE_EVENT_OVERLAY)
    EVT_UTL_FreeNPCChara(AREA_0049_CHARACTERS);
#else
    for (const shen_name *character = AREA_0049_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (ctrlGetScenarioChara(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
#endif
    FTBL.EV_FreeAuthorPak(0, AREA_0049_CONVERSATION);
    FTBL.EV_ControlPackageResources(0, 6, "0049A", 0, 0);
    FTBL.EventFunction857(0);
#if !defined(SHENMUE_EVENT_OVERLAY)
    EV_CleanupMapControlTask();
#endif
    FTBL.HLib_DestroyTask(parent->self);
#if defined(SHENMUE_EVENT_OVERLAY)
    if (g_currentMap == AREA_0049_WB00) {
#else
    if (g_currentMap.name == AREA_0049_WB00.name) {
#endif
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.EV_AT_SetPos(AREA_AUTHOR_RYO, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(AREA_AUTHOR_RYO, 0x78000000, &rotation, 0);
#else
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {335.6f, 127.0f, 280.1f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 17021, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
#endif
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
E0049_INLINE void SetupE0049Characters(void) {
    PrepareE0049Character(AREA_AUTHOR_RYO);
    PrepareE0049Character(AREA_0049_SYE_);
    HideE0049Prop(AREA_0049_A092);
    FTBL.LoadBlurModelForCharacter_0(AREA_0049_SYE_, "SYE_B.MT7", 1.0f);
}

E0049_INLINE void RestoreE0049Characters(void) {
    RestoreE0049Character(AREA_AUTHOR_RYO);
    RestoreE0049Character(AREA_0049_SYE_);
    ShowE0049Prop(AREA_0049_A092);
}

E0049_INLINE void PrepareE0049Character(E0049CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
E0049_INLINE void RestoreE0049Character(E0049CharacterName name) {
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
#else
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
#endif
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
E0049_INLINE void HideE0049Prop(E0049CharacterName name) { FTBL.EV_SetCharacterAlphaEnabled(name, 0); }
E0049_INLINE void ShowE0049Prop(E0049CharacterName name) { FTBL.EV_SetCharacterAlphaEnabled(name, 1); }
#undef E0049_INLINE
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace

const ScriptAreaDefinition area0049ScriptDefinition{.areaId = AREA_0049_AREA, .field04 = 0, .init = E0049Init, .main = E0049Main};

} // namespace shenmue::s2::areas
#endif
